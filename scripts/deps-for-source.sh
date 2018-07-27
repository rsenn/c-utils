#!/bin/sh

ME=`basename "$0" .sh`
THISDIR=`dirname "$0"`
BASEDIR=`cd "$THISDIR"/.. && pwd`
IFS="
"

for FILE;  do
 (set -- $(gcc -I"$BASEDIR"{,/lib} -MM "$FILE" |
  sed \
    -e '/\\$/ { :lp; N; /\\$/ { b lp }; s,\\\n\s*,,g }' \
    -e "s|$PWD/||g" \
    -e 's|\s\+|\n|g')
  shift
  [ "$1" = "$FILE" ] && shift
  set -- $(echo "$*" | sort -u )
  echo "$FILE:" "$@")
done
