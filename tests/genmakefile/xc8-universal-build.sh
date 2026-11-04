#!/bin/sh
# build the universal hex of USB-Stack CDC_Serial_UART_Example.X with XC8 2.31 through ninja and
# the shell backend; skipped when the project, XC8 or ninja is missing
# usage: xc8-universal-build.sh /path/to/genmakefile /path/to/ihextool [PROJECT.X]
G=${1:-genmakefile}
T=${2:-ihextool}
P=${3:-$HOME/Sources/USB-Stack/USB_Stack/Examples/CDC_Examples/CDC_Serial_UART_Example.X}
XC8=${XC8_BIN:-/mnt/data/opt/microchip/xc8/v2.31/bin}
[ -f "$P/nbproject/configurations.xml" ] && [ -x "$XC8/xc8-cc" ] && command -v ninja >/dev/null || { echo "xc8-universal-build: skipped"; exit 0; }
d=$(mktemp -d) || exit 1
trap 'rm -rf "$d"' EXIT
# a copy of the tree the project refers to (../../../USB, ../Shared_Files), so that nothing is written into it
R=$(cd "$P/../../.." && pwd)
cp -r "$R" "$d/USB_Stack" || exit 1
P2=$d/USB_Stack/Examples/CDC_Examples/$(basename "$P")
cd "$P2" || exit 1
rm -rf build
IHEXTOOL=$(command -v "$T" || echo "$PWD/$T"); export IHEXTOOL
gen() { m=$1; o=$2; "$G" -t xc8-cc -m "$m" --import-mplabx nbproject/configurations.xml --mplabx-conf PIC18F25K50 --create-bins -d build -o "$o" >/dev/null 2>&1; }
run() { "$@" >log 2>&1 || { echo "FAIL: $*"; grep -i "error\|fail" log | grep -v "warning" | head -5; exit 1; }; }
gen ninja build.ninja && run ninja
cp build/*-universal.hex ninja.hex || { echo "FAIL: ninja did not write the universal hex"; exit 1; }
rm -rf build
gen shell build.sh && run sh build.sh
cp build/*-universal.hex shell.hex || { echo "FAIL: shell did not write the universal hex"; exit 1; }
cmp ninja.hex shell.hex || { echo "FAIL: ninja and shell hex differ"; exit 1; }
"$T" verify ninja.hex >/dev/null || { echo "FAIL: verify"; exit 1; }
for a in 0x0000 0x0008 0x0018; do
  "$T" dump -a $a -l 4 ninja.hex | grep -q 'EF 1. F0' || { echo "FAIL: no stub at $a"; exit 1; }
done
rm -rf build
gen gmake Makefile && run make
[ "$("$T" bin -a 0 -l 0x2000 build/*-universal.hex | md5sum)" = "$("$T" bin -a 0 -l 0x2000 ninja.hex | md5sum)" ] || { echo "FAIL: stubs differ from make"; exit 1; }
echo "xc8-universal-build: ok"
