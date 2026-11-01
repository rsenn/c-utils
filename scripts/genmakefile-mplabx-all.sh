#!/bin/sh
# One makefile for every configuration of a MPLAB X project.
# usage: genmakefile-mplabx-all.sh PROJECT.X OUTDIR [genmakefile options]
# writes OUTDIR/Makefile.<configuration> for each <conf name="..."> of
# PROJECT.X/nbproject/configurations.xml
[ $# -ge 2 ] || { echo "usage: $0 PROJECT.X OUTDIR [genmakefile options]" >&2; exit 1; }
proj=$1 out=$2
shift 2
G=${GENMAKEFILE:-genmakefile}
mkdir -p "$out" || exit 1
cd "$proj" || exit 1
status=0
for conf in $(sed -n 's/^ *<conf name="\([^"]*\)".*/\1/p' nbproject/configurations.xml); do
  "$G" --import-mplabx nbproject/configurations.xml --mplabx-conf "$conf" "$@" -o "$out/Makefile.$conf" || status=1
done
exit $status
