#!/bin/bash

: ${BUILDDIR='$(BUILDDIR)'}
: ${EXEEXT='$(EXEEXT)'}
: ${OBJEXT=.o}

: ${A_CMD='$(AR) rcs $@ $^'}
: ${COMPILE_CMD='$(CC) $(CFLAGS) $(CPPFLAGS) $(DEFS) -c -o $@ $^'}
: ${LINK_CMD='$(CCLD) $(CFLAGS) $(LDFLAGS) -o $@ $^ $(LIBS) $(EXTRA_LIBS)'}

DEFVAR="DEFINES += HAVE_"
DEFVAR="DEFS += -DHAVE_"

TAB="	"
NL="
"
IFS="
 "

match_expr() {  (EXPR="^(|\.[/\\\\]|.*[\\\\/])$1\$";  shift; IFS="$NL"; echo "$*" |  grep -E "$EXPR" ); }
 list() {
   (IFS=" $IFS"; set -- $*; IFS="$NL"; echo "$*"); }

   dirnames() {
     (IFS="$NL"; echo "$*" |sed "\\|[/\\\\]|! s|.*|.|; s|/[^/]*$||")
   }
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
  (IFS="$NL$IFS"; echo "$*" | sort -fu)

}
sort_var() {
  (VARS="$*"; IFS=\"\$NL \"; set --; for VAR in $VARS; do eval "set -- \"\$@\" \$$VAR"; done; echo "$*"|sort -fu)
}
get_objs() {
  [ $# -gt 0 ] && {
  strings "$@" |
  sed -n  "\\|^\([[:alnum:]_]\+\)\\.[co]| { s|^\([[:alnum:]_]\+\)\\.[oc].*|\1.o|; \\|lib/|! s|^|lib/|; p }"

  } ||
    echo
}
count() {
  echo $#
}
get_srcs() {
  (list $SOURCES | grep -E "$X_SRC")
}

all_program_rules() {
  IFS="$NL "

    set -- $(echo  "$PROGRAMS"|  sed "s|^|${BUILDDIR}| ; s|\$|${EXEEXT}|")
    IFS=" $NL"
    O=
    pushv O "$@: ${BUILDDIR}%${EXEEXT}: ${BUILDDIR}%.o${LIBS:+ $(implode ' ' $LIBS)}"
   #[ -n "$A_LIBS"] && pushv O "\\${NL}$TAB"$(sort_args $A_LIBS)

  [ -n "$LINK_CMD" ] && O="$O$NL$TAB${LINK_CMD}"

  echo "$NL$O$NL"


}
# c_o_entry <$LIBEXT-file> <objects>
c_o_entry() {
  (A=$1; S="$2"
  SRCDIR=${S%/%*}
  shift 2
  O=
  pushv O "$A:"
  pushv O "${BUILDDIR}%$OBJEXT:" "$S"
  [ -n "$COMPILE_CMD" ] && O="$O$NL$TAB${COMPILE_CMD}"
  echo "$O$NL")
}
# a_entry <$LIBEXT-file> <objects>
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
  [ $# -gt 0 ] || set -- build/*/*/*$LIBEXT
  A_LIBS="$*"
set -- ALIGNED_ALLOC ALLOCA BSDSENDFILE C11_GENERICS CYGWIN_CONV_PATH DEVPOLL DYNSTACK EAGAIN_READWRITE EPOLL FNMATCHGETDELIM GETOPT GETOPT_LONG GLOB IO_FD_FLAGS IO_QUEUEFORREAD KQUEUE LIBBZ2 LIBLZMA LIBZ LOCALTIME_R LSTAT N2I PIPE2 POPEN POSIX_MEMALIGN PREAD ROUND SENDFILE SIGIO SOCKET_FASTOPEN UINT128 WORDEXP ZLIB STDINT_H \
  ALLOCA_H CONFIG_H ERRNO_H INTTYPES_H LINUX_LIMITS_H
COND_DEFS="$*"
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


  dump LIBS
  dump LIB_NAMES
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
      #OBJS=$(echo "$SRCS" | sed -n "/\.[co]/ { s|\\.[co]|| ; s|.*/||; s|^|${BUILDDIR}|; s|\$|$OBJEXT|; p; }"|sort -fu)
      O="${BUILDDIR}$P:"
      set -- $(list ${LIBS} | sed "s|.*/||; s|^|${BUILDDIR}| ; s|\$|$LIBEXT|" | sort -fu)

    #  O="$O $*"
      [ "$LINK_CMD" ] && O="$O$NL$TAB${LINK_CMD}"
      echo "$NL$O")
    done

    O=
    pushv O $(list $PROGRAMS|sed "|^|${BUILDDIR}|")

    O="$O:"
    pushv LIBS
  }
  SRCDIRS=`dirnames $SOURCES | sort -u`

  dump SRCDIRS
    per_srcdir_rules() {
      for SRCDIR in $SRCDIRS; do

          ( F=$(find $SRCDIR -maxdepth 1 -name "*.c")
          IFS=" ""
"
          OBJS=$(echo "$F" |  sed "s|\.c$|$OBJEXT|; s|^\.[/\\\\]|| ; s|.*[/\\\\]||; s|^|${BUILDDIR}|" )
          set -- $OBJS
      O=
        pushv O "$@: \\$NL$TAB"
        SRC=$SRCDIR/%.c
        SRC=${SRC#./}
        pushv O "${BUILDDIR}%$OBJEXT:${SRC:+ $SRC}"
          [ -n "$COMPILE_CMD" ] && O="$O$NL$TAB${COMPILE_CMD}"
          echo "$NL$O")
      done
    }
  a_lib_rules() {

    dump LIB_NAMES
    dump LIBS
    for A_LIB in $LIB_NAMES; do
    ( NAME=${A_LIB##*/}
      NAME=${NAME%.a}
      LIBEXPR="${NAME}\.a"
      FILES=$( match_expr "$NAME\.a" $A_LIBS)
      dump FILES
    IFS=" ""
 "
      OBJS=$(IFS="$NL "; get_objs $FILES |sort -u)
      X=$(set -- $OBJS; IFS="|"; echo "($*)")
      dump OBJS
#      dump X
        N_OBJ=$(count $OBJS)
        debug "$A_LIB:" $N_OBJ objects
      if [ "${N_OBJ:-0}" -gt 0 ] ; then
        X_SRC=$(echo "$X" |sed "s|(\(.*\))|\1|; s,[^ |]\+/,,g; s|\.o|.c|g")
        #dump X_SRC


        SRCS=$(ls -d -- $(match_expr "($X_SRC)" $SOURCES ) 2>/dev/null| sort -u)

        N_SRCS=$(count $SRCS)

        #debug "$A_LIB:" $N_SRCS "sources." 1>&2
        [ "$N_SRCS" -gt 0 ] &&
        dump SRCS


      #c_o_entry "%$OBJEXT" "$SRCDIR/%.c"
      [ "$N_OBJ" -ge 0  ] &&
        a_entry  ${A_LIB##*/} $(list $SRCS| sed "s|lib/[^ /]*/||g ; s|lib/||g ; s|\\.c|$OBJEXT|")
      fi
      )
    done
  }
IFS=" $NL"
 PROGRAMS=$(grep 'main\s*(.*)' *.c -l |xargs grep -E '#include "(lib/|)' -l | sed 's|\.c$||')
  dump PROGRAMS

O=
SEP=$NL
  [ -n "$objext" ] && OBJEXT=$objext
  [ -n "$libext" ] && LIBEXT=$libext
  pushv O "CC = ${CC:-gcc}"
  if [  -n "$link" ]; then
     pushv O "LINK = ${link}"
     pushv O "CCLD = \$(LINK)"
   else
  pushv O "CCLD ?= ${CCLD:-\$(CC)}"
  fi
  pushv O "AR = ${AR:-ar}"
  pushv O "CFLAGS = ${CFLAGS:--Os}"
 pushv O "CPPFLAGS = ${CPPFLAGS}"
  [ -n "$exeext" ] && pushv O "EXEEXT = ${exeext}"
  [ -n "$builddir" ] && pushv O "BUILDDIR = ${builddir%/}/"
SEP=$NL
 if [ "$COND_DEFS" ]; then
   var_get() {   eval "echo \"\${$1}\"";   }
   var_isset() {   eval "test \"\${$1+set}\" = set";   }
   for COND in $COND_DEFS; do
   if var_isset "$COND"; then
     pushv COND_VARS "$COND = 1"
    else
     pushv COND_VARS "#$COND ="
   fi
    pushv COND_IFDEFS "ifneq (\$($COND),)${NL}$DEFVAR${COND}=\"\$($COND)\"${NL}endif"
   done

 fi
SEP=$NL$NL
pushv O "$COND_VARS${NL}${NL}$COND_IFDEFS"

pushv O "all:"  ${BUILDDIR} $(list $PROGRAMS |sed "s|.*/|| ; s|^|${BUILDDIR}| ; s|\$|${EXEEXT}|")
pushv O "${BUILDDIR}:$NL$TAB@mkdir -p \$@ || mkdir \$@"
 echo "$O"
  unset SEP

 all_program_rules
 per_srcdir_rules
 a_lib_rules
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
  eval "shift;IFS=\" \$NL\$IFS\";$1=\"\${$1:+\$$1\${SEP-" "}}\$*\""
}

isin() {
 (needle="$1";
  while [ "$#" -gt 1 ]; do
    shift;
    test "$needle" = "$1" && exit 0;
  done;
  exit 1 )
}
gen_bcc32_makefile() { 
( : A_CMD='LIB -out:$@ $^' 
A_CMD='(cd $(BUILDDIR); for OBJ in $(notdir $^); do echo tlib /u /A /C $(notdir $@) $$OBJ 1>&2;  tlib /u /A /C $(notdir $@) $$OBJ; done)'
unset A_CMD
  CC="bcc32c" \
    CFLAGS="-G -O" \
    link="ilink32"  \
    builddir=build/bcc/Debug/ gen_a_deps "$@" |tee ${OUT:-Makefile.bcc32})
  }
case $1 in
  -bcc)  shift; gen_bcc32_makefile "$@" ;;
  *) gen_a_deps "$@" ;;
esac

