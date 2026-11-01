#!/bin/sh
# --pair-headers: "#include "x.h"" adds x.c next to it (the library convention of the PIC code)
# usage: pair-headers.sh /path/to/genmakefile
G=${1:-genmakefile}
d=$(mktemp -d) || exit 1
trap 'rm -rf "$d"' EXIT
cd "$d" || exit 1
mkdir lib
printf '#include "adc.h"\n#include <xc.h>\nint main(void){return 0;}\n' >main.c
printf 'void adc_init(void);\n' >lib/adc.h
printf '#include "adc.h"\n#include "util.h"\nvoid adc_init(void){}\n' >lib/adc.c
printf 'int util(void);\n' >lib/util.h
printf 'int util(void){return 1;}\n' >lib/util.c
printf 'int other(void){return 2;}\n' >lib/other.c
"$G" -t xc8 -m gmake --chip 18f2550 --pair-headers -Ilib -o Makefile main.c >/dev/null 2>&1 || { echo "FAIL: genmakefile"; exit 1; }
for o in adc util; do grep -q "$o.p1" Makefile || { echo "FAIL: $o.c not paired"; exit 1; }; done
grep -q "other.p1" Makefile && { echo "FAIL: other.c has no header and must not be added"; exit 1; }
"$G" -t xc8 -m gmake --chip 18f2550 -Ilib -o Makefile2 main.c >/dev/null 2>&1
grep -q "adc.p1" Makefile2 && { echo "FAIL: pairing without the option"; exit 1; }
echo "pair-headers: ok"
