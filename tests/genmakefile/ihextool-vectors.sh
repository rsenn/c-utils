#!/bin/sh
# ihextool vectors: GOTO stubs at the PIC18 vectors for a hex built with a code offset
# usage: ihextool-vectors.sh /path/to/ihextool
T=${1:-ihextool}
d=$(mktemp -d) || exit 1
trap 'rm -rf "$d"' EXIT
cd "$d" || exit 1
# an application at 0x2000: reset, then some code, and a config record far away
printf '\357\020\360\000\000\001\002\003\004\005\006\007' >app.bin
"$T" frombin -a 0x2000 app.bin -o app.hex || { echo "FAIL: frombin"; exit 1; }
"$T" vectors -a 0x2000 -o uni.hex app.hex || { echo "FAIL: vectors"; exit 1; }
"$T" verify uni.hex >/dev/null || { echo "FAIL: verify"; exit 1; }
"$T" records uni.hex | grep -q '0x00000000  | 00 EF 10 F0 |' || { echo "FAIL: stub at 0x00"; exit 1; }
"$T" records uni.hex | grep -q '0x00000008  | 04 EF 10 F0 |' || { echo "FAIL: stub at 0x08"; exit 1; }
"$T" records uni.hex | grep -q '0x00000018  | 0C EF 10 F0 |' || { echo "FAIL: stub at 0x18"; exit 1; }
grep -q '^:0400000000EF10F0' uni.hex || { echo "FAIL: record text"; exit 1; }
[ "$("$T" bin -a 0x2000 app.hex | md5sum)" = "$("$T" bin -a 0x2000 uni.hex | md5sum)" ] || { echo "FAIL: application changed"; exit 1; }
"$T" vectors -a 0x2000 --family pic16 app.hex >/dev/null 2>&1 && { echo "FAIL: pic16 accepted"; exit 1; }
"$T" vectors -a 0x2000 --vectors 0x09 app.hex >/dev/null 2>&1 && { echo "FAIL: odd vector accepted"; exit 1; }
"$T" vectors -a 0x2000 --vectors 0x2000 app.hex >/dev/null 2>&1 && { echo "FAIL: vector in application accepted"; exit 1; }
"$T" vectors app.hex >/dev/null 2>&1 && { echo "FAIL: missing -a accepted"; exit 1; }
# data at a vector is replaced, with a warning
"$T" frombin -a 0 app.bin -o low.hex && "$T" merge low.hex app.hex -o both.hex
"$T" vectors -a 0x2000 -o uni2.hex both.hex 2>w.txt || { echo "FAIL: replace"; exit 1; }
grep -q replaced w.txt || { echo "FAIL: no warning on replace"; exit 1; }
echo "ihextool-vectors: ok"
