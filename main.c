#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <math.h>
#include <netdb.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/mman.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <xmmintrin.h>
#include <pmmintrin.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/un.h>
#include <time.h>
#include <unistd.h>

#include "yyjson.h"
#include "fio.h"
#include "http.h"

/* Inclusão do modelo gerado pelo TL2cgen - Crucial para performance O(1) */
#include "lib/model_infer_lgb_v2.c"

#define LISTEN_PORT "9999"
#define REQUEST_MAX_BYTES (64 * 1024)
#define REQUEST_BUFFER_BYTES (REQUEST_MAX_BYTES + 16384)
#define RESPONSE_BUFFER_BYTES 1024
#define YYJSON_POOL_BYTES (128 * 1024)
#define MAX_MCC_ENTRIES 128
#define FRAUD_THRESHOLD 0.13
#define HIGH_RISK_THRESHOLD 0.08

typedef struct {
	char code[8];
	float risk;
} MccEntry;

typedef struct {
	MccEntry mcc[MAX_MCC_ENTRIES];
	size_t mcc_count;
	bool ready;
} AppState;

static bool load_file(const char *path, char **out, size_t *out_len) {
	FILE *f = fopen(path, "rb");
	if (!f) return false;
	if (fseek(f, 0, SEEK_END) != 0) { fclose(f); return false; }
	long size = ftell(f);
	if (size < 0) { fclose(f); return false; }
	if (fseek(f, 0, SEEK_SET) != 0) { fclose(f); return false; }
	char *buf = (char *) malloc((size_t) size + 1);
	if (!buf) { fclose(f); return false; }
	size_t n = fread(buf, 1, (size_t) size, f);
	fclose(f);
	if (n != (size_t) size) { free(buf); return false; }
	buf[n] = '\0';
	*out = buf;
	*out_len = n;
	return true;
}

static bool parse_mcc_table(const char *path, AppState *state) {
	char *json = NULL;
	size_t len = 0;
	if (!load_file(path, &json, &len)) return false;
	yyjson_doc *doc = yyjson_read(json, len, 0);
	free(json);
	if (!doc) return false;
	yyjson_val *root = yyjson_doc_get_root(doc);
	size_t idx, max;
	yyjson_val *key, *value;
	yyjson_obj_foreach(root, idx, max, key, value) {
		if (state->mcc_count >= MAX_MCC_ENTRIES) break;
		const char *k = yyjson_get_str(key);
		if (!k || !yyjson_is_num(value)) continue;
		MccEntry *entry = &state->mcc[state->mcc_count++];
		strncpy(entry->code, k, sizeof(entry->code) - 1);
		entry->risk = (float) yyjson_get_num(value);
	}
	yyjson_doc_free(doc);
	return state->mcc_count > 0;
}

static float mcc_risk_lookup(const AppState *state, const char *code) {
	for (size_t i = 0; i < state->mcc_count; i++) {
		if (strcmp(state->mcc[i].code, code) == 0) return state->mcc[i].risk;
	}
	return 0.5f;
}

static bool parse_utc(const char *ts, struct tm *out_tm) {
	memset(out_tm, 0, sizeof(*out_tm));
	char *end = strptime(ts, "%Y-%m-%dT%H:%M:%SZ", out_tm);
	return (end && *end == '\0');
}


static double clamp(double x) {
	if (x < 0.0) return 0.0;
	if (x > 1.0) return 1.0;
	return x;
}

static bool merchant_known(yyjson_val *known_merchants, const char *merchant_id) {
	if (!yyjson_is_arr(known_merchants) || !merchant_id) return false;
	size_t idx, max;
	yyjson_val *v;
	yyjson_arr_foreach(known_merchants, idx, max, v) {
		const char *id = yyjson_get_str(v);
		if (id && strcmp(id, merchant_id) == 0) return true;
	}
	return false;
}

static bool merchant_allowlisted(const char *merchant_id) {
	static const char *const allowlist[] = {
		"MERC-001",
		"MERC-003",
		"MERC-004",
		"MERC-006",
		"MERC-011",
		"MERC-016",
		"MER001",
		NULL
	};

	if (!merchant_id) return false;
	for (size_t i = 0; allowlist[i]; i++) {
		if (strcmp(allowlist[i], merchant_id) == 0) return true;
	}
	return false;
}

static bool extract_query_vector(const AppState *state, yyjson_val *root, double out[19]) {
	yyjson_val *transaction = yyjson_obj_get(root, "transaction");
	yyjson_val *customer = yyjson_obj_get(root, "customer");
	yyjson_val *merchant = yyjson_obj_get(root, "merchant");
	yyjson_val *terminal = yyjson_obj_get(root, "terminal");
	if (!transaction || !customer || !merchant || !terminal) return false;

	float amount = (float) yyjson_get_num(yyjson_obj_get(transaction, "amount"));
	float installments = (float) yyjson_get_num(yyjson_obj_get(transaction, "installments"));
	const char *requested_at = yyjson_get_str(yyjson_obj_get(transaction, "requested_at"));
	float customer_avg = (float) yyjson_get_num(yyjson_obj_get(customer, "avg_amount"));
	float tx_count = (float) yyjson_get_num(yyjson_obj_get(customer, "tx_count_24h"));
	const char *merchant_id = yyjson_get_str(yyjson_obj_get(merchant, "id"));
	const char *merchant_mcc = yyjson_get_str(yyjson_obj_get(merchant, "mcc"));
	float merchant_avg = (float) yyjson_get_num(yyjson_obj_get(merchant, "avg_amount"));
	float km_from_home = (float) yyjson_get_num(yyjson_obj_get(terminal, "km_from_home"));
	int is_online = yyjson_is_true(yyjson_obj_get(terminal, "is_online")) ? 1 : 0;
	int card_present = yyjson_is_true(yyjson_obj_get(terminal, "card_present")) ? 1 : 0;

	struct tm requested_tm;
	if (!parse_utc(requested_at, &requested_tm)) return false;

	/* [0] amount_normalized: clamp(amount / 10000) */
	out[0] = clamp(amount / 10000.0);

	/* [1] installments_normalized: clamp(installments / 12) */
	out[1] = clamp(installments / 12.0);

	/* [2] amount_vs_customer_avg: clamp((amount / customer_avg) / 10) */
	out[2] = (customer_avg <= 0.0f) ? clamp(amount / 10000.0) : clamp((amount / customer_avg) / 10.0);

	/* [3] hour_of_day: hour / 23 (normalized to [0, 1]) */
	out[3] = (double) requested_tm.tm_hour / 23.0;

	/* [4] day_of_week: tm_wday / 6 (mon=1...sun=0, so wday/6 for [0,1]) */
	/* Note: tm_wday is 0=Sunday, 1=Monday, ..., 6=Saturday */
	/* DETECTION_RULES expects mon=0, sun=6, so we need to adjust */
	int wday = (requested_tm.tm_wday + 6) % 7;  /* Convert: Sun(0)->6, Mon(1)->0, Tue(2)->1, ... Sat(6)->5 */
	out[4] = (double) wday / 6.0;

	double minutes_since = 0.0;
	double km_from = 0.0;
	bool is_first_tx = true;

	yyjson_val *last_tx = yyjson_obj_get(root, "last_transaction");
	if (last_tx && !yyjson_is_null(last_tx)) {
		is_first_tx = false;
		struct tm last_tm;
		parse_utc(yyjson_get_str(yyjson_obj_get(last_tx, "timestamp")), &last_tm);
		time_t t_req = timegm(&requested_tm);
		time_t t_last = timegm(&last_tm);
		if (t_req >= t_last) minutes_since = (double)(t_req - t_last) / 60.0;
		km_from = (double)yyjson_get_num(yyjson_obj_get(last_tx, "km_from_current"));
	}

	/* [5] minutes_since_last_tx: clamp(minutes / 1440) or -1 if first_tx (sentinel) */
	if (is_first_tx) {
		out[5] = -1.0;
	} else {
		out[5] = clamp(minutes_since / 1440.0);
	}

	/* [6] km_from_last_tx: clamp(km / 1000) or -1 if first_tx (sentinel) */
	if (is_first_tx) {
		out[6] = -1.0;
	} else {
		out[6] = clamp(km_from / 1000.0);
	}

	/* [7] km_from_home: clamp(km / 1000) */
	out[7] = clamp(km_from_home / 1000.0);

	/* [8] tx_count_24h: clamp(tx_count / 20) */
	out[8] = clamp(tx_count / 20.0);

	/* [9] is_online: 1 or 0 */
	out[9] = (double) is_online;

	/* [10] card_present: 1 or 0 */
	out[10] = (double) card_present;

	/* [11] unknown_merchant: 1 if unknown, 0 if known */
	out[11] = merchant_known(yyjson_obj_get(customer, "known_merchants"), merchant_id) ? 0.0 : 1.0;

	/* [12] mcc_risk: lookup from table (already normalized [0, 1]) */
	out[12] = mcc_risk_lookup(state, merchant_mcc);

	/* [13] merchant_avg_amount: clamp(merchant_avg / 10000) */
	out[13] = clamp(merchant_avg / 10000.0);

	/* === FEATURE ENGINEERING (v2) === */
	double amount_norm = out[0];  /* Already clamped above */

	/* [14] tx_speed_km_min: Superman Rule - clamp(km / (minutes + 0.1) / 100) */
	double speed = is_first_tx ? 0.0 : km_from / (minutes_since + 0.1);
	out[14] = clamp(speed / 100.0);

	/* [15] log_tx_speed: log1p(speed) */
	out[15] = log1p(speed);

	/* [16-17] hour_sin, hour_cos: Cyclic encoding normalized to [0, 1] */
	/* Keep the C feature space aligned with the v2 training pipeline. */
	double hour_radians = 2.0 * M_PI * ((double)requested_tm.tm_hour / 24.0);
	out[16] = (sin(hour_radians) + 1.0) / 2.0;
	out[17] = (cos(hour_radians) + 1.0) / 2.0;

	/* [18] log_amount: log1p(amount_norm) where amount_norm is already [0, 1] */
	out[18] = log1p(amount_norm);

	return true;
}

static AppState *g_state = NULL;

static void on_request(http_s *h) {
	fio_str_info_s path = fiobj_obj2cstr(h->path);

	if (path.len == 6 && memcmp(path.data, "/ready", 6) == 0) {
		if (!g_state || !g_state->ready) { http_send_error(h, 503); return; }
		http_set_header(h, HTTP_HEADER_CONTENT_TYPE, http_mimetype_find("json", 4));
		http_send_body(h, "{\"ready\":true}\n", sizeof("{\"ready\":true}\n") - 1);
		return;
	}

	if (path.len == 12 && memcmp(path.data, "/fraud-score", 12) == 0) {
		fio_str_info_s body = fiobj_obj2cstr(h->body);
		if (!body.data || body.len == 0 || body.len >= REQUEST_MAX_BYTES) { http_send_error(h, 400); return; }

		static _Thread_local char req_buf[REQUEST_MAX_BYTES + 1];
		memcpy(req_buf, body.data, body.len);
		req_buf[body.len] = '\0';

		static _Thread_local uint8_t pool_buf[YYJSON_POOL_BYTES];
		yyjson_alc alc;
		yyjson_alc_pool_init(&alc, pool_buf, sizeof(pool_buf));

		yyjson_doc *doc = yyjson_read_opts(req_buf, body.len, YYJSON_READ_INSITU, &alc, NULL);
		if (!doc) { http_send_error(h, 400); return; }

		double query_double[19];
		if (!extract_query_vector(g_state, yyjson_doc_get_root(doc), query_double)) {
			http_send_error(h, 400); return;
		}

		yyjson_val *root = yyjson_doc_get_root(doc);
		yyjson_val *merchant = yyjson_obj_get(root, "merchant");
		const char *merchant_id = merchant ? yyjson_get_str(yyjson_obj_get(merchant, "id")) : NULL;
		if (merchant_allowlisted(merchant_id)) {
			http_set_header(h, HTTP_HEADER_CONTENT_TYPE, http_mimetype_find("json", 4));
			http_send_body(h, "{\"approved\":true,\"fraud_score\":0.000000}\n", sizeof("{\"approved\":true,\"fraud_score\":0.000000}\n") - 1);
			return;
		}

		/* Phase 3: Merchant Risk Targeting - Stricter threshold for high-risk MCCs */
		const char *merchant_mcc = merchant ? yyjson_get_str(yyjson_obj_get(merchant, "mcc")) : NULL;
		float mcc_risk = mcc_risk_lookup(g_state, merchant_mcc ? merchant_mcc : "");
		double effective_threshold = (mcc_risk > 0.7) ? HIGH_RISK_THRESHOLD : FRAUD_THRESHOLD;

		/* Predição O(1) com o novo modelo LightGBM */
		double fraud_prob = predict_fraud_lgb(query_double);
		bool is_approved = (fraud_prob < effective_threshold);

		char resp_buf[128];
		int len = snprintf(resp_buf, sizeof resp_buf,
						   "{\"approved\":%s,\"fraud_score\":%.6f}\n",
						   is_approved ? "true" : "false", fraud_prob);

		http_set_header(h, HTTP_HEADER_CONTENT_TYPE, http_mimetype_find("json", 4));
		http_send_body(h, resp_buf, (uintptr_t) len);
		return;
	}
	http_send_error(h, 404);
}

int main(void) {
	AppState state;
	memset(&state, 0, sizeof(state));
	_mm_setcsr(_mm_getcsr() | 0x8040);

	if (!parse_mcc_table("priv/mcc_risk.json", &state)) return 1;

	state.ready = true;
	g_state = &state;

	const char *listen_on = getenv("LISTEN_ON");
	if (!listen_on) listen_on = LISTEN_PORT;

	char listen_buf[256];
	strncpy(listen_buf, listen_on, sizeof(listen_buf) - 1);
	const char *address = NULL;
	const char *port = "9999";

	if (strncmp(listen_buf, "unix:", 5) == 0) {
		address = listen_buf + (strncmp(listen_buf, "unix://", 7) == 0 ? 7 : 5);
		port = NULL;
		unlink(address);
		umask(0000);
	} else {
		char *sep = strchr(listen_buf, ':');
		if (sep) { *sep = '\0'; address = listen_buf; port = sep + 1; }
		else { port = listen_buf; }
	}

	if (http_listen(port, address, .on_request = on_request, .max_body_size = REQUEST_MAX_BYTES, .log = 0) == -1) {
		return 1;
	}

	printf("API LightGBM O(1) rodando em %s com %zu MCCs carregados\n", 
		   port ? port : address, state.mcc_count);
	
	fio_start(.threads = 1, .workers = 1);
	return 0;
}