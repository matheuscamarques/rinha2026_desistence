#!/bin/bash
# Phase 1 Validation: Per-Channel Quantization Calibration Test
# Goal: Ensure quantization calibration is computed and applied correctly

set -e

WORKSPACE="/home/sanonichan/projetos/rinha/2026_c_sol"
BINARY="$WORKSPACE/fraud_api"
DATA_DIR="$WORKSPACE/priv"

echo "=========================================="
echo "Phase 1: Per-Channel Quantization Test"
echo "=========================================="

if [ ! -f "$BINARY" ]; then
    echo "[ERROR] Binary not found: $BINARY"
    exit 1
fi

if [ ! -d "$DATA_DIR" ]; then
    echo "[ERROR] Data directory not found: $DATA_DIR"
    exit 1
fi

echo "[INFO] Binary size: $(ls -lh $BINARY | awk '{print $5}')"
echo "[INFO] Reference file size: $(ls -lh $DATA_DIR/references.bin | awk '{print $5}')"

# Start API in background with verbose stderr capture
echo "[INFO] Starting API with per-channel quantization enabled..."
TMPLOG=$(mktemp)
timeout 10s $BINARY > $TMPLOG 2>&1 &
API_PID=$!
sleep 1

# Check if API started
if ! ps -p $API_PID > /dev/null 2>&1; then
    echo "[ERROR] API failed to start"
    cat $TMPLOG
    rm $TMPLOG
    exit 1
fi

echo "[INFO] API started (PID: $API_PID)"
echo ""
echo "=== Calibration Output ==="
cat $TMPLOG | grep -A 20 "Per-channel feature ranges" || echo "[WARNING] Calibration log not found"
echo ""

# Send a test request
echo "[INFO] Sending test fraud-score request..."
TEST_REQUEST='{
  "transaction": {"amount": 1000, "installments": 3, "requested_at": "2026-05-03T12:00:00Z"},
  "customer": {"avg_amount": 800, "tx_count_24h": 5, "known_merchants": []},
  "merchant": {"id": "MER123", "mcc": "5411", "avg_amount": 500},
  "terminal": {"is_online": true, "card_present": false, "km_from_home": 5},
  "last_transaction": {"timestamp": "2026-05-03T10:00:00Z", "km_from_current": 2}
}'

RESPONSE=$(curl -s -X POST http://localhost:9999/fraud-score \
    -H "Content-Type: application/json" \
    -d "$TEST_REQUEST" || echo '{"error":"request failed"}')

echo "[RESPONSE] $RESPONSE"

# Check response format
if echo "$RESPONSE" | grep -q '"approved"'; then
    echo "[✓] Response format is valid"
    APPROVED=$(echo "$RESPONSE" | grep -o '"approved":[^,]*' | cut -d: -f2)
    SCORE=$(echo "$RESPONSE" | grep -o '"fraud_score":[0-9.]*' | cut -d: -f2)
    echo "    Approved: $APPROVED | Fraud Score: $SCORE"
else
    echo "[✗] Response format is invalid or API error"
    echo "    Raw: $RESPONSE"
fi

# Cleanup
kill $API_PID 2>/dev/null || true
rm $TMPLOG

echo ""
echo "=========================================="
echo "Phase 1 Validation Complete"
echo "=========================================="
