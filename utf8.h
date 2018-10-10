#!/bin/bash

: ${BUILDDIR='$(BUILDDIR)'}
: ${EXEEXT='$(EXEEXT)'}
: ${A_CMD='$(AR) rcs $@ $^'}
: ${COMPILE_CMD='$(CC) $(CFLAGS) $(CPPFLAGS) -c -o $@ $^'}
: ${LINK_CMD='$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^'}
TAB="	"
NL="
"
IFS="
 "
match_expr() {  (EXPR="^(|\.[/\\\\]|.*[\\\\/])$1\$";  shift; IFS="$NL"; echo "$*" |  grep -E "$EXPR" ); }
 list() {
   (IFS=" $IFS"; set -- $*; IFS="$NL"; echo "$*"); }

debug() {
  [ "$DEBUG" = true ] && echo "$@" 1>&2
}
implode() 
{ 
(if [ "${#1}" = 1 ]; then
    IFS=$1;
    shift;
    echo "$*";
  else
    sep=$1 out=;
    if shift && [ "$#" -gt 0 ]; then
      out=$1;
      while shift && [ "$#" -gt 0 ]; do
        out="$out$sep$1"
        shift
      done
      echo "$out"
    fi
  fi)
}

grep_e_expr() { 
  (IFS="|$IFS";  echo "($@)")
}
#A_LIBS=$(echo "${@##*/}"|sort -u)
count() {
  echo $#
}
sort_args() {
  (IFS="
"; echo "$*" | sort -fu)

}
sort_var() {
  (VARS="$*"; IFS=\"\$NL \"; set --; for VAR in $VARS; do eval "set -- \"\$@\" \$$VAR"; done; echo "$*"|sort -fu)
}
get_objs() {
  [ $# -gt 0 ] && {
  strings "$@" |
  sed -n  "\\|^\([[:alnum:]_]\+\)\.o| { s|^\([[:alnum:]_]\+\)\.o.*|\1.o|; \\|lib/|! s|^|lib/|; p }"

  } ||
    echo
}
count() {
  echo $#
}
get_srcs() {
  ( match_expr "($X_SRC)" $SOURCES)
}
# c_o_entry <.a-file> <objects>
c_o_entry() {
  (A=$1; S="$2"
  SRCDIR=${S%/%*}
  shift 2
  pushv O "$A:"
  pushv O "${BUILDDIR}%.o:" "$S"
  [ -n "$COMPILE_CMD" ] && O="$O$NL$TAB${COMPILE_CMD}"
  echo "$O$NL")
}
# a_entry <.a-file> <objects>
a_entry() {
  (A=$1; O="${BUILDDIR}$A:"
  shift
  for OBJ; do O="${O:+$O }${BUILDDIR}$OBJ"; done
  [ -n "$A_CMD" ] && O="$O$NL$TAB${A_CMD}"
  echo "$NL$O")
}
dump() {
  eval "(IFS=\" \$NL\$IFS\"; debug \"\$1:\" \$$1 )"
}
gen_a_deps() {
  IFS="
  "
  [ $# -gt 0 ] || set -- build/*/*/*.a
  A_LIBS="$*"
  set -- $(find . lib/ lib/*/ -follow -maxdepth 1 -name "*.c" | sed 's|^\./||'|sort -fu)
  SOURCES="$*"
  dump SOURCES
  SRCDIRS=
  for SRC ; do 
    case "$SRC" in
      */*/*) SRCDIRS="${SRCDIRS:+$SRCDIRS$NL}${SRC%/*.c}" ;;
      ./*.c) SRCDIRS="${SRCDIRS:+$SRCDIRS$NL}." ;;
      *.c) SRCDIRS="${SRCDIRS:+$SRCDIRS$NL}." ;;
    esac
  done
  set -- $SRCDIRS
  IFS="
  "
  LIB_DIRS=$(list $A_LIBS|sed 's|/[^/]*$||' |sort -fu)

  LIB_NAMES=$(sort_args $A_LIBS | sed 's|.*/||' |sort -u)
  LIBS=$(sort_args $A_LIBS | sed 's|.*/||' |sort -u| sed "s|^|${BUILDDIR}|" ) 


  dump A_LIBS
  dump PROGRAMS
  dump LIB_DIRS
  per_program_rules() {

    for P in $PROGRAMS; do 
    (O=
    EXE=$(ls -d $(list $LIB_DIRS | sed "s|\$|/$P|") 2>/dev/null)
    dump EXE
    IFS=" ""
    "
      #SRCS=$(strings $EXE | sed ' s|\.[co]||p' -n|sort -fu)
      #OBJS=$(echo "$SRCS" | sed -n "/\.[co]/ { s|\\.[co]|| ; s|.*/||; s|^|${BUILDDIR}|; s|\$|.o|; p; }"|sort -fu)
      O="${BUILDDIR}$P:"
      set -- $(list ${LIBS} | sed "s|.*/||; s|^|${BUILDDIR}| ; s|\$|.a|" | sort -fu)

    #  O="$O $*"
      [ "$LINK_CMD" ] && O="$O$NL$TAB${LINK_CMD}"
      echo "$NL$O")
    done

    O=
    pushv O $(list $PROGRAMS|sed "|^|${BUILDDIR}|")

    O="$O:"
    pushv LIBS
  }
all_program_rules() {
O=
IFS="$NL "

  set -- $(echo  "$PROGRAMS"|  sed "s|^|${BUILDDIR}| ; s|\$|${EXEEXT}|")
  IFS=" $NL"
O="$*: ${BUILDDIR}%: %.c"
IFS="$NL "
[ -n "$LINK_CMD" ] && O="$O$NL$TAB${LINK_CMD}"
echo "$NL$O"

  O=
  pushv O "$@:" "\\${NL}${TAB}"  $(sort_args $LIBS)
  echo "$NL$O"


}
  SRCDIRS=`echo  "$*" | sort -u`

  dump SRCDIRS  
    per_srcdir_rules() {
      for SRCDIR in $SRCDIRS; do
        
          ( F=$(find $SRCDIR -maxdepth 1 -name "*.c")
          IFS=" ""
      "
          OBJS=$(echo "$F" |  sed "s|\.c$|.o|; s|^\.[/\\\\]|| ; s|.*[/\\\\]||; s|^|${BUILDDIR}|" )
          set -- $OBJS
      O=
        pushv O "$@: \\$NL$TAB"
        pushv O "${BUILDDIR}%.o":
        SRC=$SRCDIR/%.c
        SRC=${SRC#./}
        pushv O "$SRC"
          [ -n "$COMPILE_CMD" ] && O="$O$NL$TAB${COMPILE_CMD}"
          echo "$NL$O")
      done
    }
  a_lib_rules() {

    dump LIB_NAMES 
    dump LIBS 
    for A_LIB in $LIB_NAMES; do
      (
      NAME=${A_LIB##*/}
      NAME=${NAME%.a}
      LIBEXPR="${NAME}\.a"
      FILES=$( list $A_LIBS | grep -E "(^|.*/)$NAME\.a\$")
      dump FILES
    IFS=" ""
    "
      OBJS=$(get_objs $FILES |sort -u)
      X=$(set -- $OBJS; IFS="|"; echo "($*)") 
      dump OBJS
      dump X
        N_OBJ=$(count $OBJS)
        debug "$A_LIB:" $N_OBJ objects  
      if [ "${N_OBJ:-0}" -gt 0 ] ; then
        X_SRC=$(echo "$X" |sed "s|(\(.*\))|\1|; s,[^ |]\+/,,g; s|\\.o|.c|g; s|\.c|\\.c|g")
        dump X_SRC

        SRCS=$(match_expr "($X_SRC)" $SOURCES)
        
        N_SRCS=$(count $SRCS)

        debug "$A_LIB:" $N_SRCS "sources." 1>&2
        [ "$N_SRCS" -gt 0 ] &&
        dump SRCS

      #c_o_entry "%.o" "$SRCDIR/%.c"
      [ "$N_OBJ" -ge 0  ] &&
        a_entry  ${A_LIB##*/} $(list $SRCS| sed "s|lib/[^ /]*/||g ; s|lib/||g ; s|\\.c|.o|")
      fi
      )
    done
  }
IFS=" $NL"
 O=
 pushv O "all:" 
 #pushv O $(list $LIBS |sed "s|.*/|| "|sort -u) 
 pushv O $(list $PROGRAMS |sed "s|.*/|| ; s|^|${BUILDDIR}| ; s|\$|${EXEEXT}|") 
 echo "$O$NL"
  
 PROGRAMS=$(grep 'main\s*(.*)' *.c -l |xargs grep -E '#include "(lib/|)' -l | sed 's|\.c$||')
  dump PROGRAMS

  a_lib_rules 
 per_srcdir_rules
  all_programs_rules
}
pushv_unique() { 
 v=$1 s IFS=${IFS%${IFS#?}};
  shift;
  for s in "$@"; do
    if eval "! isin \$s \${$v}"; then
      pushv "$v" "$s";
    else
      return 1;
    fi
  done
}

pushv() { 
  eval "shift;IFS=\" \$NL\$IFS\";$1=\"\${$1:+\$$1 }\$*\""
}

isin() { 
 (needle="$1";
  while [ "$#" -gt 1 ]; do
    shift;
    test "$needle" = "$1" && exit 0;
  done;
  exit 1 )
}
gen_a_deps "$@"
