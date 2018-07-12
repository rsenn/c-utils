#!/bin/sh

ME=`basename "$0" .sh`
THISDIR=`dirname "$0"`
BASEDIR=`cd "$THISDIR"/.. && pwd`

SOURCE_FILES=$(
 (readelf -a "$1" | sed -n "s|.*ABS\\s\([^ ]\+\)|\1|p"

 ls -d $(strings "$1" | grep '\.c$' ) 2>/dev/null ) |sort -u 
)

for FILE in $SOURCE_FILES; do
  find "$BASEDIR" -name "${FILE##*/}"
done |sort -u| 
  sed "s|$PWD/||" #|   sed "1! s|^|  | ;; \$! s|.*|& \\\\|"
