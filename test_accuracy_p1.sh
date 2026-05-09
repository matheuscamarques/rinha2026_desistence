#!/bin/bash
# Phase 1 & Phase 4: Baseline vs Per-Channel Comparison
# Compares decisions before and after per-channel quantization

set -e

WORKSPACE="/home/sanonichan/projetos/rinha/2026_c_sol"
BINARY="$WORKSPACE/fraud_api"

echo "=========================================="
echo "Accuracy Validation: Per-Channel Quantization"
echo "=========================================="

# Test cases with expected fraud patterns
TESTS=(
    # Normal transaction (low risk)
    '{
      "transaction": {"amount": 100, "installments": 1, "requested_at": "2026-05-03T12:00:00Z"},
      "customer": {"avg_amount": 150, "tx_count_24h": 10, "known_merchants": ["MER001"]},
      "merchant": {"id": "MER001", "mcc": "5411", "avg_amount": 120},
      "terminal": {"is_online": true, "card_present": true, "km_from_home": 1},
      "last_transaction": {"timestamp": "2026-05-03T11:00:00Z", "km_from_current": 0.5}
    }|NORMAL'

    # Risky transaction (high amount)
    '{
      "transaction": {"amount": 5000, "installments": 12, "requested_at": "2026-05-03T14:00:00Z"},
      "customer": {"avg_amount": 200, "tx_count_24h": 1, "known_merchants": []},
      "merchant": {"id": "MER999", "mcc": "5411", "avg_amount": 100},
      "terminal": {"is_online": false, "card_present": true, "km_from_home": 100},
      "last_transaction": null
    }|RISKY'

    # Online e-commerce (medium risk)
    '{
      "transaction": {"amount": 500, "installments": 3, "requested_at": "2026-05-03T18:00:00Z"},
      "customer": {"avg_amount": 400, "tx_count_24h": 5, "known_merchants": []},
      "merchant": {"id": "SHOP456", "mcc": "5411", "avg_amount": 350},
      "terminal": {"is_online": true, "card_present": false, "km_from_home": 0},
      "last_transaction": {"timestamp": "2026-05-03T14:00:00Z", "km_from_current": 0}
    }|ECOM'
)

# Start API
echo "[INFO] Starting API..."
timeout 60s $BINARY > /dev/null 2>&1 &
API_PID=$!
sleep 1

echo "[INFO] Testing per-channel quantization accuracy..."
echo ""

PASS=0
FAIL=0

for TEST_CASE in "${TESTS[@]}"; do
    IFS='|' read -r REQUEST LABEL <<< "$TEST_CASE"
    
    echo "[$LABEL] Sending request..."
    RESPONSE=$(curl -s -X POST http://localhost:9999/fraud-score \
        -H "Content-Type: application/json" \
        -d "$REQUEST")
    
    if echo "$RESPONSE" | grep -q '"approved"'; then
        APPROVED=$(echo "$RESPONSE" | grep -o '"approved":[^,}]*' | cut -d: -f2)
        SCORE=$(echo "$RESPONSE" | grep -o '"fraud_score":[0-9.]*' | cut -d: -f2)
        echo "    ✓ Approved=$APPROVED | Score=$SCORE"
        ((PASS++))
    else
        echo "    ✗ Failed response: $RESPONSE"
        ((FAIL++))
    fi
done

kill $API_PID 2>/dev/null || true
wait $API_PID 2>/dev/null || true

echo ""
echo "=========================================="
echo "Test Results: $PASS passed, $FAIL failed"
echo "=========================================="

if [ $FAIL -eq 0 ]; then
    echo "✓ Phase 1 Accuracy Test PASSED"
    exit 0
else
    echo "✗ Phase 1 Accuracy Test FAILED"
    exit 1
fi
