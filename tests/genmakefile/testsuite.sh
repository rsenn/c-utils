#!/bin/sh
# genmakefile shell testsuite.
#
# Builds a small mock source tree, runs genmakefile over it for a matrix
# of {compiler, build tool, directory layout} combinations, then actually
# invokes the resulting build tool and checks it succeeds.
#
# Usage: tests/genmakefile/testsuite.sh
# Env:   GENMAKEFILE=path/to/genmakefile   GM_KEEP=1 (keep scratch dir)

set -u

THISDIR=$(CDPATH= cd -- "$(dirname -- "$0")" && pwd)
# shellcheck source=./common.sh
. "$THISDIR/common.sh"

# compiler:kind:extra-genmakefile-args, one per line ('extra args' may
# itself contain spaces, so this is read line-by-line rather than
# word-split)
GM_COMPILERS="gcc:host:
sdcc:pic:-a pic18 -p 18f25k50 --debug
xc8:pic:-a pic18 -p 18f25k50 --debug"

GM_MAKE_TYPES="make gmake ninja shell"

while IFS=: read -r compiler kind extra_args; do
  [ -z "$compiler" ] && continue

  for make_type in $GM_MAKE_TYPES; do
    for partition in $GM_PARTITIONS; do
      name="$compiler-$make_type-$partition"
      # shellcheck disable=SC2086
      gm_run_case "$name" "$kind" "$compiler" "$make_type" "$partition" $extra_args
    done
  done
done <<EOF
$GM_COMPILERS
EOF

gm_summary
