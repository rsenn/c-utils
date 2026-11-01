#!/bin/sh
# --project FILE: the [project] section stands for command line arguments
# usage: project-file.sh /path/to/genmakefile
G=${1:-genmakefile}
d=$(mktemp -d) || exit 1
trap 'rm -rf "$d"' EXIT
cd "$d" || exit 1
mkdir lib
printf '#include "adc.h"\nint main(void){return 0;}\n' >main.c
printf 'void adc_init(void);\n' >lib/adc.h
printf 'void adc_init(void){}\n' >lib/adc.c
cat >genmakefile.ini <<'INI'
; the firmware of the test
[project]
toolchain = xc8
make = gmake
chip = 18f2550
xtal = 20000000
baud = 38400
build = minsizerel
define = USE_ADC=1 DEBUG_UART
include = lib
pair-headers = yes
sources = main.c
INI
"$G" --project genmakefile.ini -o Makefile >/dev/null 2>&1 || { echo "FAIL: genmakefile"; exit 1; }
grep -q '^CHIP = 18f2550' Makefile || { echo "FAIL: chip"; exit 1; }
grep -q 'UART_BAUD=38400' Makefile && grep -q '_XTAL_FREQ=20000000' Makefile || { echo "FAIL: xtal or baud"; exit 1; }
grep -q 'USE_ADC=1' Makefile && grep -q 'DEBUG_UART' Makefile || { echo "FAIL: defines"; exit 1; }
grep -q 'minsizerel_20mhz_38kbps' Makefile || { echo "FAIL: build type"; exit 1; }
grep -q '/adc\.p1' Makefile || { echo "FAIL: pair-headers via the project file"; exit 1; }
# the command line comes last and wins
"$G" --project genmakefile.ini --chip 18f25k50 -o M2 >/dev/null 2>&1
grep -q '^CHIP = 18f25k50' M2 || { echo "FAIL: command line does not win"; exit 1; }
"$G" --project genmakefile.ini --dump-project >dump.txt 2>&1
grep -qx -- '--chip' dump.txt && grep -qx '18f2550' dump.txt && grep -qx -- '-DUSE_ADC=1' dump.txt || { echo "FAIL: --dump-project"; exit 1; }
echo "project-file: ok"
