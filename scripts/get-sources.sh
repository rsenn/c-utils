#!/bin/sh

ME=`basename "$0" .sh`
THISDIR=`dirname "$0"`
BASEDIR=`cd "$THISDIR"/.. && pwd`

SOURCE_FILES=$(
 (readelf -a "$1" | sed -n "s|.*ABS\\s\([^ ]\+\)|\1|p"

 ls -d $(strings "$1" | grep '\.c$' ) 2>/dev/null ) |sort -u 
)

for FILE in $SOURCE_FILES; do
  NAME=${FILE##*/}
  MASK=$NAME 
#  case "$FILE" in
#    lib/*/*_*.c) MASK=${NAME%%_*}_*.c ;;
#  esac

  find "$BASEDIR/" -name "$MASK"
done |sort -u| 
  sed "s|$PWD/||" 
  
#|   sed "1! s|^|  | ;; \$! s|.*|& \\\\|"
