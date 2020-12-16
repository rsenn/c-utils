#!/bin/sh
IFS="
"
dump() {
  [ "$DEBUG" = true ] && while [ $# -gt 0 ]; do 
    eval "echo \"$1='\${$1}'\""
    shift
  done 1>&2
}
multiline_list() {
  (: ${INDENT='  '}
  while :; do case "$1" in
      -i) INDENT=$2 && shift 2 ;;
      -i*) INDENT=${2#-i} && shift ;;
      *) break ;;
    esac; done
  CMD='echo -n " \\$IFS$INDENT$LINE"'
  [ $# -ge 1 ] && CMD="for LINE; do $CMD; done" || CMD="while read -r LINE; do $CMD; done"
  eval "${CMD}")
}  
  
main() {

  if [ $# -eq 0 ]; then
    set -- $(ls -td -- build/*/CMakeCache.txt)
    set -- "$1"
  fi
  FILES=
  PATTERNS=
  while [ $# -gt 0 ]; do
    case "$1" in
      -x* | --debug*) DEBUG=true ;;
      -d* | --def*) DEFINE=true ;;
      -e* | --empty*) EMPTY=true ;;
      -t* | --type*) TYPE=true ;;
      -q* | --q*) QUOTE=true ;;
      -Q* | --force*q*) FORCE_QUOTE=true ;;
      -i* | --int*) INTERNAL=true ;;

      -C | --no-cmake*) NO_CMAKE=true ;;
      -n | --escape*n*) ESCAPE_NEWLINES=true ;;
      -V | --no-v*) NO_VALUE=true ;;
      \\* | /*/*) PATTERNS="${PATTERNS:+$PATTERNS$IFS}$1" ;;
      *) 
    if [ -f "$1" ]; then
      FILES="${FILES:+$FILES$IFS}$1"
    else
      PATTERNS="${PATTERNS:+$PATTERNS$IFS}\\|$1|i"
    fi
    ;;
esac
    shift
  done

  if [ -z "$PATTERNS" ] ; then
    PATTERNS="/^[-A-Z0-9_]\+:[A-Z]\+=/"
  fi
  MATCH=
 if [ "$INTERNAL" != true ]; then
    MATCH="${MATCH:+$MATCH; }"'/:INTERNAL/d'
  fi
   if [ "$TYPE" != true ]; then
    MATCH="${MATCH:+$MATCH; }"'s|:\([A-Z]\+\)=|=|'
  fi

if [ "$NO_VALUE" = true ]; then
    MATCH="${MATCH:+$MATCH; }"'s|=.*||'
  elif [ "$QUOTE" = true ]; then
    MATCH="${MATCH:+$MATCH; }"'s|=\(.*\s.*\)|="\1"|'
  elif [ "$FORCE_QUOTE" = true ]; then
    MATCH="${MATCH:+$MATCH; }"'s|=\(.*\)|="\1"|'
  fi
   if [ "$EMPTY" != true ]; then
    MATCH="${MATCH:+$MATCH; }"'/=\s*$/d'
  fi
  if [ "$NO_CMAKE" = true ]; then
    MATCH="${MATCH:+$MATCH; }"'/^CMAKE/d'
  fi
  if [ "$DEFINE" = true ]; then
    MATCH="${MATCH:+$MATCH; }"'/=/ s|^|-D|'
  fi

  MATCH="${MATCH:+$MATCH; }p"
  dump TYPE DEFINE PATTERNS
  set -f
  for PATTERN in $PATTERNS; do 
    MATCH="$PATTERN { $MATCH }"
  done
 EXPR="\\|^\s*//|d; /^\s*\$/d; $MATCH" 
  dump MATCH EXPR
  CMD='sed -n -e "$EXPR" $FILES'
  if [ "$ESCAPE_NEWLINES" = true ]; then
	  CMD="$CMD | sed 's|^|  | ;; \$! s|\$| \\\\|'"
	fi
  : set -x
eval "$CMD"
}

main "$@"
