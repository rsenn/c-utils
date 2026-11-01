#!/bin/sh
# Every public header must compile on its own (no other include before it).
# usage: tests/check-headers.sh [builddir-with-config.h]
cd "$(dirname "$0")/.." || exit 1
bd=${1:-build/x86_64-linux-gnu}
fails=0
for h in lib/*.h; do
  b=$(basename "$h")
  case $b in windoze.h) continue ;; esac
  echo "#include \"$b\"" >/tmp/check-hdr.$$.c
  if ! ${CC:-gcc} -fsyntax-only -w -Ilib -I"$bd" -DHAVE_CONFIG_H /tmp/check-hdr.$$.c 2>/dev/null; then
    echo "FAIL $b"
    fails=$((fails + 1))
  fi
done
rm -f /tmp/check-hdr.$$.c
echo "headers that do not compile alone: $fails"
[ "$fails" = 0 ]
