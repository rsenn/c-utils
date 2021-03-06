#!/bin/bash
ME=`basename "$0" .sh`
THISDIR=`dirname "$0"`
BASEDIR=`cd "$THISDIR"/.. && pwd`
NL="
"

check_support_arg() {
  (CMD="cproto -h 2>&1 | grep -q '^\\s*$1\\s'"
  eval "$CMD") }

get_preprocessor() {
  (set -- $(ls -d /usr/bin/cpp{,-[0-9]*} |sort -fuV )
   eval "echo \$${#}") 2>/dev/null
}

check_exec() {
  (CMD="\"\$@\" 2>&1 1>/dev/null <<<\"\" || echo \"ERROR: \$?\""
  eval "ERROR=\$($CMD)"
  test -z "$ERROR")
}

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

get_name() {
  (NAME=${1##*[!A-Za-z0-9_]}
  if [ "$NAME"  = "$1" ]; then
    NAME=
  fi
  #echo "NAME='$NAME'" 1>&2
  echo $NAME)
}

remove_name() {
  (NAME=`get_name "$1"`
   ARG=${ARG%"$NAME"}
   ARG=${ARG%" "}
   echo "$ARG")
}

adjust_length() {
  eval "[  \${${1}_MAXLEN} -ge \${#$1} ] 2>/dev/null || ${1}_MAXLEN=\${#$1}"
}

clean_args() {
  old_IFS="$IFS"; IFS=","
  : ${DEPTH:=0}
  ARGS=${1#"("}
  set -- ${ARGS%")"*}
  echo "[$DEPTH] clean_args $*" 1>&7
  [ "${OUT+set}" != set ] && OUT=
  ARG=
  I=0
  while [ $# -gt 0 ]; do 
    ARG="${ARG:+$ARG, }${1# }"; shift

    : $((++I))

   #[ "$REMOVE_NAMES" = "$I" ] && REMOVE_NAMES=true

    P1=${ARG%%")("*}
    P2=${ARG#"$P1"}
    case "$P2" in
      ")("*")"*) echo "[$DEPTH] Ok: $ARG" 1>&7 ;;
      ")("*) echo "[$DEPTH] Again" 1>&7; continue ;;
      *) ;;
    esac
         
      echo "[$DEPTH] ARG='$ARG'" 1>&7

    ARG2=
    case "$ARG" in
      *")("*")"*) 
        ARG1=${ARG%%")("*}
        ARG2=${ARG#$ARG1}
         
        ARG2=$(DEPTH=$((DEPTH+1)) OUT= clean_args "${ARG2#")"}")
        ARG="$ARG1)"
        ;;
    esac
    ARG=${ARG#"("}
    ARG=${ARG#" "}
    ARG=${ARG//" ***"/"*** "}
    ARG=${ARG//" **"/"** "}
    ARG=${ARG//" *"/"* "}
   
    if [ "$REMOVE_NAMES" = true ] || [ -n "$REMOVE_NAMES" -a "$REMOVE_NAMES" -ge "$I" ] 2>/dev/null; then 
  ARG=`remove_name "$ARG"`
    fi

    [ "$EMPTY" = true -a -n "$ARG2" ] && ARG2="()"
    ARG=$ARG$ARG2
    #ARG=${ARG//' )'/')'}
     OUT="${OUT:+$OUT, }$ARG"
    ARG=
  done
  echo "[$DEPTH] OUT='$OUT'" 1>&7
  echo "($OUT)"
}

get_prototypes() {
  : ${PAD_ARGS=false}
  unset INCLUDES

  while :; do
    case "$1" in
      -[dx] | --debug) DEBUG=true; shift ;;
      -D | --define) DEFS=${DEFS:+$DEFS }-D$2; shift 2 ;;
      -D*) DEFS=${DEFS:+$DEFS }$1; shift ;;
      -A | --no-pad-args* | -*no*args*) PAD_ARGS=false; shift ;;
      -a | --pad-args* | -*args*) PAD_ARGS=true; shift ;;
      -r=* | --remove*=* | -R=*) REMOVE_NAMES=${1#*=}; shift ;;
      -I* ) CPROTO_ARGS="$CPROTO_ARGS$NL$1" ; shift ;; 
      -include  ) INCLUDES="${INCLUDES:+$INCLUDES }-include $2" ; shift 2 ;; 
      -r | --remove* | -R) REMOVE_NAMES=true; shift ;;
      -c | --copy* | --xclip*) XCLIP=true; shift ;;
      -S | --no-sort) NO_SORT=true; shift ;;
      -E | --ellips* | --empty*) EMPTY=true; shift ;;
      -q | --quiet) QUIET=true; shift ;;
      -e | --expr) EXPR="${EXPR:+$EXPR ;; }$2"; shift 2 ;;
      -e=* | --expr=*) EXPR="${EXPR:+$EXPR ;; }$2"; shift ;;
      -e* ) EXPR="${1#-e}"; shift ;;
      *) break ;;
    esac
  done
  #: ${CPROTO_CMD:="cproto${INCLUDES:+ -E 'cpp $INCLUDES'}"}
 IFS="$NL"
  add_arg() {
    CPROTO_ARGS="$CPROTO_ARGS$NL$*"
   }
   add_arg $DEFS
  if [ "$DEBUG" = true ]; then
    exec 7>&2
  else
    exec 7>/dev/null
  fi
  if check_support_arg -N; then
    add_arg -N
  fi
  if check_support_arg -n; then
    add_arg -n
  fi
  PP=$(get_preprocessor)
  if [ -x "$PP" ]; then
    PPARGS=$INCLUDES
    check_exec "$PP" -std=c2x &&   PPARGS=${PPARGS:+$PPARGS }-std=c2x
    if [ -n "$PPARGS" ]; then
      PPTMP=$(mktemp ./cpp-XXXXXX)
      trap 'rm -f "$PPTMP"' EXIT
      cat >$PPTMP <<EOF
#!/bin/sh
exec $PP $PPARGS "\$@"
EOF
      chmod +x "$PPTMP"
      PP="$PPTMP"
    fi
      add_arg "-E" "$PP"
  fi
  if [ "$QUIET" = true ]; then
    add_arg -q
    CPROTO_REDIR="2>/dev/null"
  fi
  CPROTO_CMD="${CPROTO_CMD:-cproto} \$CPROTO_ARGS -D_Noreturn= -D__{value,x,y}= -p \"\$@\" $CPROTO_REDIR | sed \"\\|^/|d ;; $EXPR\""
  if [ "$DEBUG" = true ]; then
    eval "echo \"Command:\" $CPROTO_CMD 1>&2"
  fi
  CPROTO_OUT=`eval "$CPROTO_CMD"`
 
  IFS=" "
  while read_proto; do
    adjust_length TYPE
    adjust_length FNAME
    adjust_length ARGS
  done <<<"$CPROTO_OUT"
  
  FILTER="sed 's,|,,g'"
[ "$NO_SORT" = true ] || FILTER="sort -t'|' -k2 -f | $FILTER"
 (TEMP=`mktemp`
  trap 'rm -f "$TEMP"' EXIT 
  [ "$PAD_ARGS" = true ] && PAD_A2="-$((FNAME_MAXLEN))"
  while read_proto; do
    set -- "$TYPE" "$FNAME" "$ARGS"
    printf "%-$((TYPE_MAXLEN))s |%${PAD_A2}s|%s\n" "$1" "$2" "$(clean_args "$3");"
  done <<<"$CPROTO_OUT" | eval "$FILTER"  >"$TEMP"

  [ "$XCLIP" = true ] && xclip -selection clipboard -in <"$TEMP"
  cat "$TEMP"
  )
}

get_prototypes "$@"
