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
      -c | --c-only ) C_ONLY=true; shift ;;
      -p | --pro | --qmake) QMAKE=true; shift ;;
      -u=* | --update=* | --*upd*=*) UPDATE=${1#*=}; shift ;;
      -u | --update | --*upd*) UPDATE=$2; shift 2 ;;
      -u*) UPDATE=${1#-?}; shift ;;
      -x | --debug) DEBUG=true; shift ;;
      *) break ;;
    esac
  done
  echo QMAKE=$QMAKE 1>&2
  if [ ! -e "$1" ]; then
    set -- $(ls -td build/*/Debug/"$1".exe | head -n1)
  fi
  [ "$DEBUG" = true ] && echo "ARG:" $@ 1>&2

  case "$1" in
    *.exe)  SOURCE_FILES=$(IFS="
" 
#set -- $(sed 's,[^[:print:]],\n,g' "$1" | sed -n "/\.c\$/ { s|.*/||; p }" | sort -fu); IFS="|"; grep -E "(^|/)$*" files.list)
sed 's,[^[:print:]],\n,g' "$1" | sed -n "/\.c\$/ { s|.*/||; p }" | sort -fu)
    ;;
    *) SOURCE_FILES=$(readelf -a "$1" | sed -n 's|.*ABS\s\([^ ]\+\)|\1|p' | sort -u)  ;;
  esac
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
  EXPR="s|$PWD/||"
  if [ "$C_ONLY" = true ]; then
    EXPR="$EXPR; \\|\.c$|! d"
  fi
  FILES=`find "$BASEDIR/" "(" "$@" ")" -and -not -wholename "*build/*" | sort -u | sed "$EXPR"`

  if [ -n "$UPDATE" -a -e "$UPDATE" ]; then
    ( sed -i \
     -e "/^SOURCES =/ { /\\\\\$/ { :lp1; N; /\\\\\$/ b lp1; s,\\\\\\n\\s*,,g }; s|=.*|= $(filter "*.c" $FILES)|; }" \
     -e "/^HEADERS =/ { /\\\\\$/ { :lp2; N; /\\\\\$/ b lp2; s,\\\\\\n\\s*,,g }; s|=.*|= $(filter "*.h" $FILES)|; }" \
     $UPDATE)

  elif [ "${QMAKE:-false}" = true ]; then
    echo SOURCES = $(filter "*.c" $FILES)
    echo HEADERS = $(filter "*.h" $FILES)
  else 
    echo "$FILES"
  fi
}
get_sources "$@"
