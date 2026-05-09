#!/usr/bin/env bash
# Orchestrates local validation: build, readiness, monitoring, k6 smoke + main test, teardown.

set -euo pipefail

# Configuration (override via env)
COMPOSE="docker compose"
SMOKE_SCRIPT="documentations/rinha-de-backend-2026/test/smoke.js"
MAIN_TEST_SCRIPT="documentations/rinha-de-backend-2026/test/test.js"
RESULTS_JSON="results.json"
RESOURCE_LOG="resource_log.txt"
POLL_TIMEOUT=${POLL_TIMEOUT:-30}
POLL_INTERVAL=${POLL_INTERVAL:-1}
MONITOR_INTERVAL=${MONITOR_INTERVAL:-2}
MEMORY_LIMIT_MB=${MEMORY_LIMIT_MB:-350}

# Containers to monitor (as discovered in repo)
CONTAINERS=(fraud-lb fraud-api-01 fraud-api-02)

# ANSI colors
RED="\033[0;31m"
GREEN="\033[0;32m"
YELLOW="\033[0;33m"
NC="\033[0m"

info(){ printf "%b\n" "${GREEN}[INFO]${NC} $*"; }
warn(){ printf "%b\n" "${YELLOW}[WARN]${NC} $*"; }
err(){ printf "%b\n" "${RED}[ERROR]${NC} $*"; }

pid_monitor=0

cleanup(){
  rc=$?
  if [ "$pid_monitor" -ne 0 ]; then
    kill "$pid_monitor" 2>/dev/null || true
    wait "$pid_monitor" 2>/dev/null || true
  fi
  ${COMPOSE} down --remove-orphans || true
  return $rc
}
trap cleanup EXIT INT TERM

wait_for_ready(){
  info "Waiting up to ${POLL_TIMEOUT}s for API readiness..."
  local start=$(date +%s)
  while true; do
    # Prefer light readiness probe
    if curl -s -o /dev/null -w '%{http_code}' http://localhost:9999/ready 2>/dev/null | grep -q "200"; then
      info "/ready responded 200"
      return 0
    fi

    # Fallback: try POST /fraud-score with tiny payload
    local code
    code=$(curl -s -o /dev/null -w '%{http_code}' -X POST http://localhost:9999/fraud-score \
      -H 'Content-Type: application/json' -d '{"id":"__ready__"}' --max-time 5 2>/dev/null || echo 0)
    if [ "$code" = "200" ]; then
      info "/fraud-score accepted dummy payload"
      return 0
    fi

    now=$(date +%s)
    if [ $((now - start)) -ge ${POLL_TIMEOUT} ]; then
      err "Timeout waiting for API readiness after ${POLL_TIMEOUT}s"
      return 1
    fi
    sleep ${POLL_INTERVAL}
  done
}

start_monitor(){
  info "Starting background resource monitor (every ${MONITOR_INTERVAL}s) -> ${RESOURCE_LOG}"
  echo "timestamp,name,cpu_perc,mem_usage,mem_limit" > "${RESOURCE_LOG}"
  (
    while true; do
      ts=$(date --iso-8601=seconds)
      ${COMPOSE} ps --services --filter "status=running" >/dev/null 2>&1 || true
      for c in "${CONTAINERS[@]}"; do
        # docker stats --no-stream with custom format
        ${COMPOSE} exec -T "$c" true >/dev/null 2>&1 || true
      done
      # Use docker stats directly to avoid compose formatting differences
      docker stats --no-stream --format '"{{.Name}}","{{.CPUPerc}}","{{.MemUsage}}"' ${CONTAINERS[*]} 2>/dev/null | \
        while IFS= read -r line; do
          name=$(echo "$line" | awk -F"," '{gsub(/"/,"",$1); print $1}')
          cpu=$(echo "$line" | awk -F"," '{gsub(/"/,"",$2); print $2}')
          mem=$(echo "$line" | awk -F"," '{gsub(/"/,"",$3); print $3}')
          # mem looks like: "24.58MiB / 1.952GiB"
          mem_usage=$(echo "$mem" | awk -F"/" '{gsub(/^[ \t]+|[ \t]+$/,"",$1); print $1}')
          mem_limit=$(echo "$mem" | awk -F"/" '{gsub(/^[ \t]+|[ \t]+$/,"",$2); print $2}')
          echo "${ts},${name},${cpu},${mem_usage},${mem_limit}" >> "${RESOURCE_LOG}"
        done || true
      sleep ${MONITOR_INTERVAL}
    done
  ) &
  pid_monitor=$!
  info "Monitor PID=${pid_monitor}"
}

mem_to_mb(){
  # Accepts values like: 24.58MiB, 1.952GiB, 150KiB
  local v="$1"
  # separate number and unit
  local num=$(echo "$v" | sed -E 's/([0-9.]+).*/\1/')
  local unit=$(echo "$v" | sed -E 's/[0-9.]+\s*([A-Za-z]+).*/\1/')
  if [ -z "$num" ]; then echo 0; return; fi
  case "$unit" in
    B) echo "$(awk "BEGIN {printf \"%.3f\", ${num}/1024/1024}")" ;; 
    KiB) echo "$(awk "BEGIN {printf \"%.3f\", ${num}/1024}")" ;; 
    MiB) echo "$(awk "BEGIN {printf \"%.3f\", ${num}}")" ;; 
    GiB) echo "$(awk "BEGIN {printf \"%.3f\", ${num}*1024}")" ;; 
    *) echo "$(awk "BEGIN {printf \"%.3f\", ${num}}")" ;; 
  esac
}

parse_peaks(){
  info "Parsing ${RESOURCE_LOG} for peak memory usage"
  declare -A peaks
  while IFS=, read -r ts name cpu mem_usage mem_limit; do
    # skip header
    if [ "$name" = "name" ] || [ -z "$name" ]; then continue; fi
    mb=$(mem_to_mb "$mem_usage")
    # compare
    prev=${peaks[$name]:-0}
    # Use awk comparison
    is_gt=$(awk -v a="$mb" -v b="$prev" 'BEGIN{print (a>b)?1:0}')
    if [ "$is_gt" -eq 1 ]; then
      peaks[$name]="$mb"
    fi
  done < <(tail -n +2 "${RESOURCE_LOG}" 2>/dev/null || true)

  combined=0
  printf "%s\n" "Peak memory usage (MB):"
  for c in "${CONTAINERS[@]}"; do
    v=${peaks[$c]:-0}
    # print with 3 decimals
    printf "  %s: %.3f MB\n" "$c" "$v"
    combined=$(awk -v a="$combined" -v b="$v" 'BEGIN{printf "%.3f", a+b}')
  done
  printf "  Combined: %.3f MB\n" "$combined"
  # warn if exceeded
  exceed=$(awk -v a="$combined" -v lim="$MEMORY_LIMIT_MB" 'BEGIN{print (a>lim)?1:0}')
  if [ "$exceed" -eq 1 ]; then
    printf "%b\n" "${RED}PEAK MEMORY EXCEEDED: combined ${combined} MB > ${MEMORY_LIMIT_MB} MB${NC}"
    return 2
  else
    printf "%b\n" "${GREEN}Peak memory within limits: ${combined} MB <= ${MEMORY_LIMIT_MB} MB${NC}"
    return 0
  fi
}

main(){
  info "Bringing up docker compose..."
  ${COMPOSE} up -d --build

  if ! wait_for_ready; then
    err "Service did not become ready"
    exit 1
  fi

  start_monitor

  info "Running smoke test: ${SMOKE_SCRIPT}"
  if ! command -v k6 >/dev/null 2>&1; then
    err "k6 not found in PATH"
    exit 1
  fi

  if ! k6 run "${SMOKE_SCRIPT}"; then
    err "Smoke test failed — aborting"
    exit 1
  fi

  info "Running main k6 test: ${MAIN_TEST_SCRIPT} (output -> ${RESULTS_JSON})"
  if ! k6 run --out json=${RESULTS_JSON} "${MAIN_TEST_SCRIPT}"; then
    warn "k6 reported failures"
  fi

  # stop monitor then parse
  if [ "$pid_monitor" -ne 0 ]; then
    kill "$pid_monitor" 2>/dev/null || true
    wait "$pid_monitor" 2>/dev/null || true
    pid_monitor=0
  fi

  if [ -f "${RESOURCE_LOG}" ]; then
    parse_peaks || exit 2
  else
    warn "No ${RESOURCE_LOG} found; skipping peak parsing"
  fi

  info "Validation completed. Results: ${RESULTS_JSON} (if produced)"
}

main "$@"
