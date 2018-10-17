#!/bin/sh

ME=`basename "$0" .sh`
THISDIR=`dirname "$0"`
BASEDIR=`cd "$THISDIR"/.. && pwd`
IFS="
"
OUTEXPR="/_internal/d"

RELDIR=$(realpath --relative-to "$PWD" "$BASEDIR")

case "$1" in
  --lib* | -l*)
    OUTEXPR="$OUTEXPR ;; s|.*/\([^/]*\)|\1|"
    GETICMD='sed "s,.*#\s*include\s*\"\(.*\)\.h\",\1,p" -n "$FILE" '
    shift
    ;;
esac

for FILE;  do
  ([ "$GETICMD" ] && set -- $(ls -d $(eval "$GETICMD") 2>/dev/null) || { set -- $( gcc -I"${RELDIR:-$BASEDIR}" -MM "$FILE" |
  sed \
    -e '/\\$/ { :lp; N; /\\$/ { b lp }; s,\\\n\s*,,g }' \
    -e "s|$PWD/||g" \
    -e 's|\s\+|\n|g'
  }); shift; }
  [ "$1" = "$FILE" ] && shift
  set -- $(echo "$*" |sed -e "$OUTEXPR" | sort -u )
  [ "$GETICMD" ] && echo "add_program(${FILE%.*} $@)" ||  echo "$FILE:" "$@")
done
