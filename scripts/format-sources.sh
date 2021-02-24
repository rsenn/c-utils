#!/bin/bash
TS=$'\t'
CR=$'\r'

NL="
"
fs_time() {
  (while :; do
    case "$1" in
      -t | --time-style | -s)
        TIME_STYLE="$2"
        shift 2
        ;;
      -[ts]=* | --time-style=*)
        TIME_STYLE="${1#*=}"
        shift
        ;;
      -[ts]*)
        TIME_STYLE="${1#-[ts]}"
        shift
        ;;
      *) break ;;
      esac
   done
   fs_lscol 6 "$@")
}
dump() { (OUT=;  for VAR; do eval "OUT=\${OUT:+\$OUT }\$VAR=\$$VAR"; done; echo "$OUT") 1>&2; }

str_index(){
( STR="$2"
  eval "echo \"\${STR:$(($1 + 1)):1}\"")
}

 bin2dec () {
  while [ $# -gt 0 ]; do
    eval 'echo "$((2#'${1#0b}'))"';
    shift;
  done
}

parse_mode() {
  (STR=$1;  I=0; N=${#STR}; V=0
  while [ "$I" -lt "$N" ]; do
   C=`str_index "$I" "$STR"`; V=$(($V * 2))
   case "$C" in
    -) ;;
    *) V=$(($V | 1)) ;;
esac
I=$(($I + 1))
done
printf "%04o" "$V")
}

list_cmd() {
(
  set --  $( ( [ "${DEBUG:-0}" -gt 1 ] && set -x; find "${@:-.}" -mindepth 0  -maxdepth 3  -name "*.[ch]" -and -not -wholename "*3rdparty/*" -and -not -wholename "*build/*"   ) |sed '\|/|! s|^|./|')

#(echo @:   "${@}" 1>&2 )
(echo Dirs:   $(echo "${*%/*}" | sort -u) 1>&2 )
 #set -- "${@#./}"
 




ls -l -n -d --time-style="+%s" -- "$@") | (IFS=" $TS$CR$NL"; while read -r MODE NLINKS FUID FGID FSIZE FTIME FILE; do
 #dump MODE NLINKS FUID FGID FSIZE FTIME  FILE 1>&2
  CRC=`  crc32 "$FILE"`
  echo "$CRC" "$FSIZE" "$FTIME" "$FILE"
  echo "$CRC" "$MODE"   "$FSIZE" "$FTIME"   "$FILE" 1>&2
done)
  #  set -- -f'!d' -i'*.'{h,hpp,c,cpp} "$@"
#  exec list-r -c -n -l "$@")
}

temp_file() {
  FILENAME="${2:-${0#-}}-$(($$ + ${RANDOM:-0})).tmp"
  TEMPFILES="${TEMPFILES:+$TEMPFILES$NL}$FILENAME"
  eval "$1=\$FILENAME"
}
main() {
  (IFS="$NL"
    if [ $# -le 0 ]; then
      set -- ./src ./quickjs
    fi
    while [ $# -gt 0 ]; do
      case "$1" in
      --width | -w)
        WIDTH=$2
        shift 2
        ;;
      --width=* | -w=*)
        WIDTH=${1#*=}
        shift
        ;;
      -w*)
        WIDTH=${1#-w}
        shift
        ;; 
           -x*)
        DEBUG=$((${DEBUG:-0}+1))
        shift
        ;;  
           --expr=* | -e=*)
        EXPR=${1#*=}
        shift
        ;;
   --expr | -e)
        EXPR=${2}
        shift 2
        ;;
      *) break ;;
      esac
    done
    ARGS="$*"
    add_expr() {
EXPR="${EXPR:+$EXPR ;; }$1"
}

add_expr 's|\s\s\+\\$| \\|'
add_expr '\|(\s\\$|  { N; s|\\\n\s\+|| }'

    trap 'IFS=$NL; : ls -la  -- $TEMPFILES; rm -f -- $TEMPFILES 1>&2' EXIT

    LIST=$(list_cmd "$@")
    FILES=$(set -- $LIST; echo "${*##* }")
    (set -- $LIST; echo "Got $# files." 1>&2 )
    ([ "$DEBUG" -gt 0 ] && set -x; clang-format -style=file ${WIDTH:+-style="{ColumnLimit:$WIDTH}"} -i $FILES) 
    [ -n "$EXPR" ] && ([ "$DEBUG" -gt 0 ] && set -x; sed -i -e "$EXPR" -i $FILES )
    temp_file A
    MATCH=""
    echo "$LIST" >"$A"
    { IFS=" "
      while read -r CRC32 MODE N USERID GROUPID SIZE TIME FILE; do
        MATCH="${MATCH:+$MATCH\\|}^$CRC32 .* $FILE\$"
      done; } <"$A"
    IFS="$NL"
    temp_file B
    list_cmd "$@" >"$B"
    { IFS=" "
      while read -r CRC32 MODE N USERID GROUPID SIZE TIME FILE; do
        (temp_file C
          grep " $FILE\$" "$A" >"$C"
          read -r OTHER_{CRC32,MODE,N,USERID,GROUPID,SIZE,TIME,FILE} <"$C"
          if [ "$CRC32" != "$OTHER_CRC32" -o "$TIME" != "$OTHER_TIME" ]; then
      [ "${DEBUG:-0}" -gt 1 ] &&   dump CRC32 MODE N USERID GROUPID SIZE TIME FILE
            #dump OTHER_{CRC32,MODE,N,USERID,GROUPID,SIZE,TIME,FILE}
         #    echo 1>&2
          fi)
      done; } <"$B"
    # (diff -U0 "$A" "$B" | grep "^[-+][^-+]" | sort -t' ' -k8)
  )
}
main "$@"
