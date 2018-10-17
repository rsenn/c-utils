#!/bin/sh

ME=`basename "$0" .sh`
THISDIR=`dirname "$0"`
BASEDIR=`cd "$THISDIR"/.. && pwd`
IFS="
"

RELDIR=$(realpath --relative-to "$PWD" "$BASEDIR")

for FILE;  do
 (set -- $(gcc -I"${RELDIR:-$BASEDIR}" -MM "$FILE" |
  sed \
    -e '/\\$/ { :lp; N; /\\$/ { b lp }; s,\\\n\s*,,g }' \
    -e "s|$PWD/||g" \
    -e 's|\s\+|\n|g')
  shift
  [ "$1" = "$FILE" ] && shift
  set -- $(echo "$*" |grep -v "_internal" | sort -u )
  echo "$FILE:" "$@")
done
