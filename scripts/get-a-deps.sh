#!/bin/bash


pushv() {
    eval "shift;$1=\"\${$1+\"\$$1\${IFS%\"\${IFS#?}\"}\"}\$*\""
}
pushv_unique() {
  eval "shift; while [ \$# -gt 0 ]; do
    isin \$1 \${$1} || pushv $1 \$1; done"
}

isin() {
 (needle="$1";
  while [ "$#" -gt 1 ]; do
    shift;
    test "$needle" = "$1" && exit 0;
  done;
  exit 1)
}
  get_undefined() {
   (EXPR=; for ARG; do
   pushv EXPR "s|^$(basename "$1") U ||p" 
 done;  sed -n "$EXPR" "$NMFILE")
  }

get_providers() {
 (IFS="|"; X="\\.a T ($*)\$"
 command grep -E "$X" "$NMFILE"  | sed 's, [^ ] .*,,' | sort -u)
}

get_deps() {
  (IFS="
"; N=$#; for ARG; do
(undef=$(get_undefined "$ARG")
 [ "$N" -gt 1 ] && OUT="$ARG:"
(set -- $(get_providers $undef)
 for PROVIDER; do
   [ "$PROVIDER" != "$ARG" ] && OUT="${OUT:+$OUT${S:- }}$PROVIDER"
 done
 echo "$OUT"))
done)
}
get_deps_r() {
  eval "while [ \$# -gt 1 ]; do 
  pushv_unique $1 \$(S=' ' get_deps \$1)
done"
}



get_a_deps() {

  while :; do
    case "$1" in
      --cmake | -c) CMAKE=true; shift ;;
      --direct | -d) DIRECT=true; shift ;;
      --debug | -x) DEBUG=true; shift ;;
      *) break ;;
    esac
  done

  DIR="$1"
  shift
  OBJ="$*"
    IFS=" ""
:"


  set -- $(find "$DIR" -mindepth 1 -maxdepth 2 -name "*.[ao]")

  NMFILE=`mktemp`

  [ "$DEBUG" != true ] && 
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
    sed -n " s|:[^ :]*\.o||;  s|.*/\([^ ]*\.[ao]\):.* \([UT]\) \(.*\)|\1 \2 \3|p" >"$NMFILE"


  unset DEPS

  get_dep() {
    EXCLUDE_EXPR=$(sed -n  "/^$(basename "$1") T / { s|.* \(.*\)|/ \1$/d|; p }" "$NMFILE" )
    MATCH_EXPR=$(sed -n  "$EXCLUDE_EXPR;  /^$(basename "$1") U / { s|.* \(.*\)|/\.a T \1/ { s/ .*//p } |; p }" "$NMFILE")

    if ! isin "$1" $DEPS; then
      echo "$INDENT$1" 1>&2 
      pushv DEPS "$1"
    fi

    set -- $(sed -n "$MATCH_EXPR" "$NMFILE" |sort -u )

      for DEP; do 
        if ! isin "$DEP" $DEPS; then
          pushv DEPS "$DEP"
      #    echo "${INDENT}  $DEP"
           if [ "$DIRECT" != true ]; then
            INDENT="$INDENT  " get_dep "$DEP"
          fi
      fi
      done

#    for DEP; do 
#      if ! isin "$DEP" $DEPS; then
#        DEPS=${DEPS:+$DEPS }$*
#      fi
#    done
  }

  if [ -n "$OBJ" ]; then
    for O in $OBJ; do 
      unset DEPS
      get_dep "$O"
      set -- $DEPS
      [ "$1" = "$O" ] && shift
      if [ "$CMAKE" = true ]; then
        [ $# -gt 0 ] && echo "target_link_libraries($(basename $O .o) ${@%.a})"
      else
        echo $OBJ: ${@%.a}
      fi
    done
  fi
  
 
}

case "${0##*/}" in
  -* | sh | bash) ;;
  *) get_a_deps "$@" || exit $? ;;
esac
