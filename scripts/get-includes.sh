#!/bin/sh
IFS="
"
TS='  '

SOURCES=$(find "$@" -name "*.[ch]" )
find_source() {
  echo "$SOURCES" | grep -E "(/|^)$1\$"
}
isin() {
 (__NEEDLE="$1"
  while [ "$#" -gt 1 ]; do
    shift
    [ "$__NEEDLE" = "$1" ] && exit 0
  done
  exit 1)
}
pushv() {
    eval 'shift;'$1'="${'$1':+$'$1' }$*"'
}
pushv_unique() {
  local __VAR=$1 __ARG IFS=${IFS%${IFS#?}}
  shift
  for __ARG; do
    eval "! isin \$__ARG \${$__VAR}" && pushv "$__VAR" "$__ARG" || return 1
  done
}

output_target() {
 (SOURCE=$1
  SUFFIX=.o
  shift
  IFS=" "
  DEPS="$SOURCE $*"
OBJECT='$(BUILDDIR)'$(basename "$SOURCE" .c)${SUFFIX}

  cat <<EOF
${CPPFLAGS:+$OBJECT: CPPFLAGS += $CPPFLAGS
}$OBJECT: ${DEPS}
$TS\$(CROSS_COMPILE)\$(CC) \$(CFLAGS) \$(EXTRA_CFLAGS) \$(CPPFLAGS) \$(INCLUDES) \$(DEFS) -c -o \$@ \$<

EOF
)
}

ALL_INCLUDES=

for SOURCE in $SOURCES; do 
  SOURCE="${SOURCE#./}"
  SOURCE_DIR=$(dirname "${SOURCE}")
case "$SOURCE_DIR" in
  3rdparty/*) 
  SOURCE_ROOT=${SOURCE_DIR#3rdparty/}
  SOURCE_ROOT=3rdparty/${SOURCE_ROOT%%/*}
  ;;
  *)
SOURCE_ROOT=.
;;
esac
INCLUDES=
INCLUDE_FILES=

CMDS=
 trap 'rm -f "$TMP"' EXIT
 TMP=`mktemp $PWD/tmpXXXXX`
 grep -H -n '^\s*#\s*include' $SOURCE >$TMP 

  {
  IFS=":"; while read -r FILE LINE INCLUDE_CMD; do
    IFS="
  "; 
    INCLUDE=${INCLUDE_CMD#*include}
    INCLUDE=${INCLUDE#[ \r\t]}
    INCLUDE=${INCLUDE%[ \r\t]}
    case "$INCLUDE" in
      *"<"*">"*) SYSINCLUDE=true ;;
      *) SYSINCLUDE=false ;;
    esac
    INCLUDE_FILE=${INCLUDE#*[\"<]}
    INCLUDE_FILE=${INCLUDE_FILE%[\">]*}
    "$SYSINCLUDE" && continue
       [ -z "$INCLUDE_FILE" ] && continue
    CMDS="${CMDS:+$CMDS }$INCLUDE_CMD"
    case "$INCLUDE_FILE" in
      ../*) RELATIVE=true ;;
      *) RELATIVE=false ;;
    esac
    INCLUDE_NAME=$(basename "$INCLUDE_FILE")
    if $RELATIVE; then
      INCLUDE_PATH="$SOURCE_DIR/${INCLUDE_FILE}"
    else
      DIR=$SOURCE_DIR
      while :; do
        INCLUDE_PATH=$( (find "${DIR}" -name "$INCLUDE_NAME" -not -wholename "*build/*") | head -n1)
        test -n "$INCLUDE_PATH" && break
        [ "$DIR" = "." ] && break
        DIR=`dirname "$DIR"`
      done
    fi
    if $RELATIVE ; then
      if [ ! -e "$INCLUDE_PATH" ]; then
          echo "No such file relative: ${INCLUDE_PATH}" 1>&2 
      else
        continue
      fi
    fi
    [ ! -e "$INCLUDE_PATH" ] && echo "No such file:" ${INCLUDE_PATH} "($FILE $LINE $INCLUDE_CMD)" 1>&2
    [ -z "$INCLUDE_PATH" ] && continue
    INCLUDE_REL=$( realpath --relative-to="$(dirname "$SOURCE")" $INCLUDE_PATH)
    INCLUDE_DIR=`dirname $INCLUDE_PATH`
    INCLUDE_DIR=${INCLUDE_DIR#./}
    if pushv_unique ALL_INCLUDES -I${INCLUDE_DIR}; then
       pushv_unique INCLUDES -I${INCLUDE_DIR}
       pushv_unique INCLUDE_FILES ${INCLUDE_PATH}
  
fi
  done
  ([ -n "$INCLUDES" ] &&
           echo $SOURCE CPPFLAGS=\"${INCLUDES}\" $INCLUDE_FILES ||
           echo $SOURCE $INCLUDE_FILES) 1>&2

  CPPFLAGS="${INCLUDES}"
  output_target "$SOURCE" $INCLUDE_FILES 

 } <"$TMP"
done
 if [ -n "$ALL_INCLUDES" ]; then
    echo $ALL_INCLUDES
  fi
