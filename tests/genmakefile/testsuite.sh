#!/bin/sh
# genmakefile shell testsuite.
#
# Builds a small mock source tree, runs genmakefile over it for a matrix
# of {compiler, build tool, directory layout} combinations, then actually
# invokes the resulting build tool and checks it succeeds.
#
# Compilers are not assumed to be on PATH: gm_discover_compilers globs
# every sdcc/xc8/xc8-cc install found under /opt (plus gcc, if already
# on PATH), and each one found becomes its own row in the matrix, with
# its bin directory prepended to PATH only for that row's build-tool run.
#
# Usage: tests/genmakefile/testsuite.sh
# Env:   GENMAKEFILE=path/to/genmakefile   GM_KEEP=1 (keep scratch dir)

set -u

THISDIR=$(CDPATH= cd -- "$(dirname -- "$0")" && pwd)
# shellcheck source=./common.sh
. "$THISDIR/common.sh"

GM_MAKE_TYPES="make gmake ninja shell"
GM_PIC_ARGS="-a pic18 -p 18f25k50 --debug"

while IFS=: read -r label compiler kind bindir; do
  [ -z "$label" ] && continue

  extra_args=""
  [ "$kind" = "pic" ] && extra_args=$GM_PIC_ARGS

  for make_type in $GM_MAKE_TYPES; do
    for partition in $GM_PARTITIONS; do
      name="$label-$make_type-$partition"
      # shellcheck disable=SC2086
      gm_run_case "$name" "$label" "$kind" "$compiler" "$bindir" "$make_type" "$partition" $extra_args
    done
  done
done <<EOF
$(gm_discover_compilers)
EOF

gm_summary
