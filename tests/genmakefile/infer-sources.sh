#!/bin/sh
# --infer-sources: a called function that is not defined is looked for in the .c files nearby
# usage: infer-sources.sh /path/to/genmakefile
G=${1:-genmakefile}
d=$(mktemp -d) || exit 1
trap 'rm -rf "$d"' EXIT
cd "$d" || exit 1
mkdir lib
printf 'int main(void){ adc_init(); return util(); }\n' >main.c
printf 'void\nadc_init(void) {\n  hidden();\n}\n' >lib/adc.c
printf 'int util(void){return 1;}\nint hidden(void){return 0;}\n' >lib/util.c
printf 'int other(void){return 2;}\n' >lib/other.c
printf '#ifdef WITH_X\nint x_fn(void){return 3;}\n#endif\n' >lib/x.c
printf 'int main2(void){ x_fn(); return 0; }\n' >main2.c
"$G" -t xc8 -m gmake --chip 18f2550 --infer-sources -Ilib -o Makefile main.c >/dev/null 2>&1 || { echo "FAIL: genmakefile"; exit 1; }
for o in adc util; do grep -q "[/)]$o\.p1" Makefile || { echo "FAIL: $o.c not inferred"; exit 1; }; done
grep -q "[/)]other\.p1" Makefile && { echo "FAIL: other.c defines nothing that is called"; exit 1; }
# the symbols of the files that were added count too (adc.c calls hidden, util.c has it)
"$G" -t xc8 -m gmake --chip 18f2550 --infer-sources -Ilib -o M2 main2.c >/dev/null 2>&1
grep -q "[/)]x\.p1" M2 && { echo "FAIL: x_fn is defined only with WITH_X"; exit 1; }
"$G" -t xc8 -m gmake --chip 18f2550 --infer-sources -DWITH_X=1 -Ilib -o M3 main2.c >/dev/null 2>&1
grep -q "[/)]x\.p1" M3 || { echo "FAIL: x_fn with WITH_X"; exit 1; }
echo "infer-sources: ok"
