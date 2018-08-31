#!/bin/bash

isin() {
 (needle="$1";
  while [ "$#" -gt 1 ]; do
    shift;
    test "$needle" = "$1" && exit 0;
  done;
  exit 1)
}

get_a_deps() {

  DIR="$1"
    IFS=" ""
:"


  set -- $(find "$DIR" -mindepth 1 -maxdepth 2 -name "*.[ao]")

  NMFILE=`mktemp`
  trap 'rm -f "$NMFILE"' EXIT

  parse-nm() {
    while read -r LIB OBJ SYM; do
  
      NAME=${SYM##*" "}
      SYM=${SYM%" $NAME"}
      ADDR=${SYM%?}
      TYPE=${SYM#"$ADDR"}
      ADDR=${ADDR%[ \t\r]}

      case "$TYPE:$NAME" in
        *:_GLOBAL_OFFSET_TABLE_ | *:__stack_chk_fail) ;;
        T:* | U:*) echo "${LIB##*/} $TYPE $NAME" ;;
      esac
    done
  }

  
  ${NM-nm} -A "$@"   |
    sed -n "/ _/! s|.*/\([^ ]*\.a\):.* \([UT]\) \(.*\)|\1 \2 \3|p" >"$NMFILE"


  get_dep() {
    EXCLUDE_EXPR=$(sed -n  "/^$(basename "$1") T / { s|.* \(.*\)|/ \1$/d|; p }" "$NMFILE" )
    MATCH_EXPR=$(sed -n  "$EXCLUDE_EXPR;  /^$(basename "$1") U / { s|.* \(.*\)|/\.a T \1/ { s/ .*//p } |; p }" "$NMFILE")

    set -- "$(basename "$1")" $(sed -n "$MATCH_EXPR" "$NMFILE" |sort -u )


    echo "${INDENT}$1"
    shift
    for DEP; do 
      echo "${INDENT}  $DEP"
    done

#    for DEP; do 
#      if ! isin "$DEP" $DEPS; then
#        DEPS=${DEPS:+$DEPS }$*
#      fi
#    done
  }
  
 
}

case "${0##*/}" in
  -* | sh | bash) ;;
  *) get_a_deps "$@" || exit $? ;;
esac

