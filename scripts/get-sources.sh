#!/bin/sh
ME=`basename "$0" .sh`
THISDIR=`dirname "$0"`
BASEDIR=`cd "$THISDIR"/.. && pwd`
filter() { 
 (PATTERN="$1"; shift; OUT=; for ARG; do case "$ARG" in 
   $PATTERN) OUT="${OUT:+$OUT${IFS:0:1}}$ARG" ;;
  esac; done; echo "$OUT")
}
get_sources() {
  while :; do
    case "$1" in
      -p | --pro | --qmake) QMAKE=true; shift ;;
      -u=* | --update=* | --*upd*=*) UPDATE=${1#*=}; shift ;;
      -u | --update | --*upd*) UPDATE=$2; shift 2 ;;
      -u*) UPDATE=${1#-?}; shift ;;
      *) break ;;
    esac
  done
  echo QMAKE=$QMAKE 1>&2
  SOURCE_FILES=`readelf -a "$1" | sed -n 's|.*ABS\s\([^ ]\+\)|\1|p' | sort -u`
  [ -n "$SOURCE_FILES" ] || SOURCE_FILES=`ls -d $(strings "$1" | grep '\.c$' ) 2>/dev/null  |sort -u`
  isin() {
    A=$1; shift
    for ITEM; do [ "$A" = "$ITEM" ] && return 0; done; return 1
  }
  set -- 
  for FILE in $SOURCE_FILES; do
    NAME=${FILE##*/}
    MASK=$NAME 
    case "$FILE" in
    */*/*_*.c | *_*.c)
      isin "${MASK%%_*}.h" "$@" || set -- "$@" ${@:+-or} -name "${MASK%%_*}.h"
      isin "${MASK%%.*}.c" "$@" || set -- "$@" ${@:+-or} -name "${MASK%%.*}.c"
      #isin "${MASK%%_*}_*.c" "$@" || set -- "$@" ${@:+-or} -name "${MASK%%_*}_*.c"
     ;;
    *)
      isin "${MASK}" "$@" || set -- "$@" ${@:+-or} -name "${MASK}"
      ;;
    esac
  done 
  FILES=`find "$BASEDIR/" "(" "$@" ")" -and -not -wholename "*build/*" | sort -u | sed "s|$PWD/||"`

  if [ -n "$UPDATE" -a -e "$UPDATE" ]; then
    ( sed -i \
     -e "/^SOURCES =/ { /\\$/ { :lp1; N; /\\$/ b lp1; s,\\\n\s*,,g }; s|=.*|= $(filter "*.c" $FILES)|; }" \
     -e "/^HEADERS =/ { /\\$/ { :lp2; N; /\\$/ b lp2; s,\\\n\s*,,g }; s|=.*|= $(filter "*.h" $FILES)|; }" \
     $UPDATE)

  elif [ "${QMAKE:-false}" = true ]; then
    echo SOURCES = $(filter "*.c" $FILES)
    echo HEADERS = $(filter "*.h" $FILES)
  else 
    echo $FILES
  fi
}
get_sources "$@"
