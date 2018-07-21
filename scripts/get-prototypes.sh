#!/bin/sh
ME=`basename "$0" .sh`
THISDIR=`dirname "$0"`
BASEDIR=`cd "$THISDIR"/.. && pwd`

filter() { 
 (PATTERN="$1"; shift; OUT=; for ARG; do case "$ARG" in 
   $PATTERN) OUT="${OUT:+$OUT${IFS:0:1}}$ARG" ;;
  esac; done; echo "$OUT")
}

read_proto() {
  read -r LINE || return $?
   FN=${LINE%%"("*}
   ARGS=${LINE#$FN}
   FNAME=${FN##*" "}
   TYPE=${FN%"$FNAME"}
   TYPE=${TYPE%" "}
   case "$FNAME" in
     \**)  TYPE="$TYPE*"; FNAME=${FNAME#"*"} ;;
   esac
  #echo "TYPE=$TYPE FNAME=$FNAME ARGS=$ARGS"   1>&7
}

adjust_length() {
  eval "[  \${${1}_MAXLEN} -ge \${#$1} ] 2>/dev/null || ${1}_MAXLEN=\${#$1}"
}

clean_args() {
  old_IFS="$IFS"; IFS=","
  : ${DEPTH:=0}
  ARGS=${1#"("}
  set -- ${ARGS%")"*}
  echo "clean_args($DEPTH) $*" 1>&7
  [ "${OUT+set}" != set ] && OUT=
  ARG=
  while [ $# -gt 0 ]; do 
    ARG="${ARG:+$ARG, }${1# }"; shift

    P1=${ARG%%")("*}
    P2=${ARG#"$P1"}
    case "$P2" in
      ")("*")"*) echo "Ok: $ARG" 1>&7 ;;
      ")("*) echo Again 1>&7; continue ;;
      *) ;;
    esac
         
    echo "ARG='$ARG'" 1>&7

    ARG2=
    case "$ARG" in
      *")("*")"*) 
        ARG1=${ARG%%")("*}
        ARG2=${ARG#$ARG1}
         
        ARG2=$(DEPTH=$((DEPTH+1)) clean_args "${ARG2#")"}")
        ARG="$ARG1)"
        ;;
    esac
    ARG=${ARG#"("}
    ARG=${ARG#" "}
    ARG=${ARG//" *"/"* "}
    ARG=${ARG%" "[[:alpha:]]*}
    ARG=${ARG%" "}
    ARG=$ARG$ARG2
    #ARG=${ARG//' )'/')'}
     OUT="${OUT:+$OUT, }$ARG"
    ARG=
  done
  echo "($OUT)"
}

get_prototypes() {
  while :; do
    case "$1" in
      -[dx] | --debug) DEBUG=true; shift ;;
      *) break ;;
    esac
  done

  if [ "$DEBUG" = true ]; then
    exec 7>&2
  else
    exec 7>/dev/null
  fi

  CPROTO_OUT=`cproto -p "$@"  | grep -v '^/'`
 
 
  IFS=" "
  while read_proto; do
    adjust_length TYPE
    adjust_length FNAME
    adjust_length ARGS
  done <<<"$CPROTO_OUT"

  while read_proto; do
    set -- "$TYPE" "$FNAME" "$ARGS"
    printf "%-$((TYPE_MAXLEN))s |%-$((FNAME_MAXLEN))s|%s\n" "$1" "$2" "$(clean_args "$3")"
  done <<<"$CPROTO_OUT" | 
      sort -t'|' -k2 -f |
      sed "s,|,,g"
}

get_prototypes "$@"
