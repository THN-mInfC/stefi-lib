#!/bin/sh
# Host unit tests. Needs a native C compiler only:
#   sh tests/host/run.sh
set -e
ROOT=$(cd "$(dirname "$0")/../.." && pwd)
OUT=${TMPDIR:-/tmp}/stefi-host-tests
mkdir -p "$OUT"
CC=${CC:-cc}

# FMAC software model (G431 FMAC example reference) vs. a float IIR
$CC -Wall -O1 -DSTEFI_MCU_STM32L4G4 -I"$ROOT/include" -I"$ROOT/src" \
    "$ROOT/tests/host/test_fmac_model.c" "$ROOT/src/fmac.c" -lm -o "$OUT/test_fmac_model"
"$OUT/test_fmac_model"
