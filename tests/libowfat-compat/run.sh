#!/bin/sh
# Differential test of c-utils' fmt/scan/str/byte/case/uintN functions
# against the real libowfat.
#
#   run.sh LIBOWFAT_SRC_DIR LIBOWFAT_A BUILD_LIB_DIR [ITERATIONS]
#
#   LIBOWFAT_SRC_DIR  unpacked libowfat source (headers), e.g. from
#                     `apt-get source libowfat`
#   LIBOWFAT_A        its libowfat.a (e.g. /usr/lib/libowfat.a)
#   BUILD_LIB_DIR     c-utils build directory holding the module archives
#                     (build/x86_64-linux-gnu/lib)
#
# libowfat's symbols are renamed owf_* so both libraries can be linked into
# one program; random inputs run through both and the outputs are compared.
# Known false positives with the Debian libowfat.a: fmt_ulong0 and
# fmt_netstring (libowfat declares fmt_* with __attribute__((pure)) and its
# own build then drops the stores of the digits).
set -e
OWSRC=$1; OWA=$2; LIB=$3; N=${4:-20000}
HERE=$(cd "$(dirname "$0")" && pwd)
W=$(mktemp -d)
cp "$HERE/cases.inc" "$W/h.inc"
python3 - "$W" "$HERE" <<'PY'
import re,sys
w=sys.argv[1]; t=open(w+'/h.inc').read()
names=sorted(set(re.findall(r'\b((?:fmt|scan|str|byte|case|uint16|uint32|uint64)_\w+)\(',t)))
names=[n for n in names if not n.startswith('t_')]
tests=re.findall(r'^T\((\w+)\)',t,re.M)+re.findall(r'^SC\((\w+),',t,re.M)
open(w+'/names','w').write(' '.join('-D%s=owf_%s'%(n,n) for n in names))
pre='#include <stdio.h>\n#include <stdlib.h>\n#include <string.h>\n#include <stdint.h>\n#include <time.h>\n#include <math.h>\n#define __attribute__(x)\n'
inc='#include <fmt.h>\n#include <scan.h>\n#include <str.h>\n#include <byte.h>\n#include <case.h>\n#include <uint16.h>\n#include <uint32.h>\n#include <uint64.h>\n'
for ns in 'ab':
    open(w+'/%s.c'%ns,'w').write(pre+'#define PFX(x) %s_##x\n'%ns+inc+'#include "h.inc"\n')
g='#include <stdio.h>\n#include <stdlib.h>\n#include <string.h>\ntypedef void (*fn)(const unsigned char*,size_t,unsigned long long,char*,size_t);\n'
for x in tests: g+='void a_t_%s(const unsigned char*,size_t,unsigned long long,char*,size_t);void b_t_%s(const unsigned char*,size_t,unsigned long long,char*,size_t);\n'%(x,x)
g+='struct {const char*n;fn a,b;} T[]={'+','.join('{"%s",a_t_%s,b_t_%s}'%(x,x,x) for x in tests)+'};\n'
g+=open(sys.argv[2]+'/main.inc').read()
open(w+'/main.c','w').write(g)
PY
nm --defined-only -g "$OWA" | awk 'NF==3 && $2 ~ /[TDBRW]/ {print $3" owf_"$3}' | sort -u > "$W/redef"
objcopy --redefine-syms="$W/redef" "$OWA" "$W/libowf.a"
gcc -O1 -w $(cat "$W/names") -I"$OWSRC" -c "$W/a.c" -o "$W/a.o"
gcc -O1 -w -I"$HERE/../../lib" -c "$W/b.c" -o "$W/b.o"
gcc -O1 -w -c "$W/main.c" -o "$W/main.o"
gcc "$W/main.o" "$W/a.o" "$W/b.o" -o "$W/diff" "$W/libowf.a" -Wl,--start-group "$LIB"/*.a -Wl,--end-group -lm
"$W/diff" "$N"
