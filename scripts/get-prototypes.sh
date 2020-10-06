#!/bin/sh
ME=`basename "$0" .sh`
THISDIR=`dirname "$0"`
BASEDIR=`cd "$THISDIR"/.. && pwd`
nl="
"
filter() { 
 (PATTERN="$1"; shift; OUT=; for ARG; do case "$ARG" in 
   $PATTERN) OUT="${OUT:+$OUT${IFS:0:1}}$ARG" ;;
  esac; done; echo "$OUT")
}
str_escape () 
{ 
  local s=$*;
  case $s in 
    *[$cr$lf$ht$vt'�']*)
      s=${s//'\'/'\\'};
'/'\r'};${s//'
      s=${s//'
'/'\n'};
      s=${s//'  '/'\t'};
      s=${s//'
              '/'\v'};
      s=${s//\'/'\047'};
      s=${s//''/'\001'};
      s=${s//'�'/'\200'}
    ;;
    *$sq*)
      s=${s//"\\"/'\\'};
      s=${s//"\""/'\"'};
      s=${s//"\$"/'\$'};
      s=${s//"\`"/'\`'}
    ;;
  esac;
  echo "$s"
}
str_quote () 
{ 
  case "$**" in 
    *["$cr$lf$ht$vt"]*)
      echo "\$'`str_escape "$*"`'"
    ;;
    *"$squote"*)
      echo "'`str_escape "$*"`'"
    ;;
    *)
      echo "'$*'"
    ;;
  esac
}

# var_dump <name>
# -------------------------------------------------------------------------
var_dump()
{
 (for N; do
    N=${N%%=*}
    O=${O:+$O${var_s-${IFS%${IFS#?}}}}$N=`eval 'str_quote "${'$N'}"'`
  done
  echo "$O")
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
      #echo "ARG='$ARG'" 1>&2
      BRACKET=${ARG%"["*}
      if [ "$ARG" != "$BRACKET" ]; then
        BRACKET=${ARG#"$BRACKET"}
      else
        BRACKET=
      fi
      ARG=${ARG%" "[[:alpha:]]*}
      ARG=${ARG%" "}
      ARG=${ARG}${BRACKET}

      #ARG=$(echo "$ARG" | sed 's,\s*[[:alpha:]_][[:alnum:]_]*,,') 
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

output_line() {
  O="$1"
  if [ ! -z "$O" ]; then
    case "$O:$PREV_OUTPUT" in
      *stralloc*:*stralloc*) ;;
      *stralloc*:*) PREPEND="${PREPEND:+$PREPEND$nl}#ifdef HAVE_STRALLOC_H" ;;
      *:*stralloc* ) PREPEND="#endif /* HAVE_STRALLOC_H */${PREPEND:+$nl$PREPEND}" ;;
    esac
    case "$O:$PREV_OUTPUT" in
      *buffer*:*buffer*) ;;
      *buffer*:*) PREPEND="${PREPEND:+$PREPEND$nl}#ifdef HAVE_BUFFER_H" ;;
      *:*buffer* )  PREPEND="#endif /* HAVE_BUFFER_H */${PREPEND:+$nl$PREPEND}" ;;
    esac
    PREV_OUTPUT=$O
  fi
 #var_dump PREPEND O APPEND 1>&2
  LINES="${PREPEND:+$nl$PREPEND$nl}$O${APPEND:+$APPEND}"
 (IFS="$nl"
  I=0
   while read -r LINE; do 
   
  #   [  "$LINE" -o "$I" -gt 0 ] &&
     echo "$LINE"
    
      I=$((I+1))
  done) <<<"$LINES"
}

get_prototypes() {
  : ${PAD_ARGS=false}
  while :; do
    case "$1" in
      -[dx] | --debug) DEBUG=true; shift ;;
      -A | --no-pad-args* | -*no*args*) PAD_ARGS=false; shift ;;
      -a | --pad-args* | -*args*) PAD_ARGS=true; shift ;;
      -r=* | --remove*=* | -R=*) REMOVE_NAMES=${1#*=}; shift ;;
      -r | --remove* | -R) REMOVE_NAMES=true; shift ;;
      -s | --sort) SORT='sort -t"|" -k2 |' ; shift ;;
      -S | --no-sort) SORT=''; shift ;;
      -m | --main) MAIN=true; shift ;;
      -M | --no-main) MAIN=false; shift ;;
      -c | --copy* | --xclip*) XCLIP=true; shift ;;
      -E | --ellips* | --empty*) EMPTY=true; shift ;;
      -D | --defin* | --ifdef*) OUTPUT_FN=echo ; shift ;;
      -e | --expr) EXPR="${EXPR:+$EXPR ;; }$2"; shift 2 ;;
      -e=* | --expr=*) EXPR="${EXPR:+$EXPR ;; }$2"; shift ;;
      -e* ) EXPR="${1#-e}"; shift ;;
      *) break ;;
    esac
  done

  if [ "$DEBUG" = true ]; then
    exec 7>&2
  else
    exec 7>/dev/null
  fi

  CPROTO_OUT=`cproto -D_Noreturn= -D__{value,x,y}= -p "$@"  | sed "\\|^/|d ;; $EXPR"`
 
  IFS=" "
  while read_proto; do
    adjust_length TYPE
    adjust_length FNAME
    adjust_length ARGS
  done <<<"$CPROTO_OUT"
FILTER="$SORT sed 's:|::g'"
 (TEMP=`mktemp`
  trap 'rm -f "$TEMP"' EXIT 
  [ "$PAD_ARGS" = true ] && PAD_A2="-$((FNAME_MAXLEN))"
  while read_proto; do
    if [ "$MAIN" != true -a "$FNAME" = "main" ]; then
      continue
    fi
    APPEND="" PREPEND=""
    if [ -n "$PREV_FNAME" -a "${FNAME%%_*}" != "${PREV_FNAME%%_*}" ]; then
      output_line  ""

    fi
    set -- "$TYPE" "$FNAME" "$ARGS"
  OUTPUT=$(printf "%-$((TYPE_MAXLEN))s |%${PAD_A2}s|%s\n" "$1" "$2" "$(clean_args "$3");")


${OUTPUT_FN:-output_line}  "$OUTPUT"
  
    PREV_FNAME=$FNAME
 done <<<"$CPROTO_OUT" | 
      eval "$FILTER" >"$TEMP"

  [ "$XCLIP" = true ] && xclip -selection clipboard -in <"$TEMP"
  cat "$TEMP"
  )
}

get_prototypes "$@"
