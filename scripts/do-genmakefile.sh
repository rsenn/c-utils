#!/bin/sh

THISDIR=`dirname "$0"`

cd "$THISDIR/.."

PROJECTDIR=$PWD
PROJECTNAME=${PROJECTDIR##*/}

count() {
  (IFS=" "; eval "set -- $@"; echo $#)
}
get_num() {
 (IFS=" "; N=$1; shift; eval "set -- $@"
  eval 'echo "${'$N'}"')
}

CMD=
while [ $# -gt 0 ]; do
  case "$1" in
    -x | -d | --debug) CMD="echo" ;;
    *) break ;;
  esac
done

: ${BUILDDIR="build/\$C"}

: ${FILENAME="\$FN\${EXT:+.\$EXT}"}
OUTPUT_FILE="${BUILDDIR:+$BUILDDIR/}$FILENAME"

: ${SYSTEM=`uname -s`}
case $SYSTEM in
  *_NT-* | NT) 
      : ${COMPILERS="bcc55 bcc32 dmc32 pocc32 pocc64 tcc32 tcc64 lcc32 lcc64 occ32"}
      BUILD_TOOLS="pomake batch ninja"
      EXTENSIONS="mk bat ninja"
      FILENAMES="\${C} \${C} \${C}"
      OUTPUT_FILE="$FILENAME"
    ;;
  *) 
    : ${COMPILERS="gcc clang tcc zapcc"}
    BUILD_TOOLS="nmake pomake make gmake batch ninja"
    EXTENSIONS="'' '' '' '' bat ninja"
    FILENAMES="NMakefile Makefile Makefile GNUmakefile build build"
  ;;
esac

set -f

if [ $# -eq 0 ]; then
  set -- lib *.c 3rdparty tests
  set -- "$@" -DHAVE_{ZLIB,LIBBZ2,LIBLZMA}=1
  set -- "$@" -I3rdparty/{zlib,bzip2,xz/liblzma/api}
elif [ -d "$1" ]; then
  SRCDIRS=$(find "$1" -name "*.c" -exec dirname {} \; |sort -u)
  INCDIRS=$(find "$1" -name "*.h" -exec dirname {} \; |sort -u)
  OUTPUT_FILE="${1%/}/$OUTPUT_FILE"
  set -- --create-libs
  for INCDIR in $INCDIRS; do
    set -- "$@" -I "$INCDIR"
  done
  for SRCDIR in $SRCDIRS; do
    set -- "$@" "$SRCDIR/*.c"
  done
fi

#set -- genmakefile --minsizerel "$@" -lzlib -lbz2 -llzma 
set -- genmakefile --minsizerel "$@" 

echo Count: $(count "$BUILD_TOOLS") 1>&2
for C in $COMPILERS; do 
  for N in $(seq 1 $(count "$BUILD_TOOLS")); do

    MAKE=`get_num $N $BUILD_TOOLS`
    EXT=`get_num $N $EXTENSIONS`
    FN=`get_num $N $FILENAMES`
    set -f 
eval "OUTFILE=\"$OUTPUT_FILE\""
set +f
OUTDIR=`dirname "$OUTFILE"`
mkdir -p "$OUTDIR"
   GENMK="$CMD $* -t $C -m $MAKE -o $OUTFILE"
   #echo "GENMK='$GENMK'" 1>&2
   ([ -n "$CMD" ] && set -f
   IFS=" "
   eval "$GENMK >&/dev/null")
  done
done





