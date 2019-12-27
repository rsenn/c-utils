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
  *) : ${COMPILERS="gcc clang tcc zapcc"}
BUILD_TOOLS="nmake pomake make gmake batch ninja"
EXTENSIONS="'' '' '' '' bat ninja"
FILENAMES="NMakefile Makefile Makefile GNUmakefile build build"
;;
esac

set -f
set -- lib *.c 3rdparty tests

#set -- genmakefile --minsizerel "$@" -lzlib -lbz2 -llzma 
set -- "$@" -DHAVE_{ZLIB,LIBBZ2,LIBLZMA}=1
set -- genmakefile --minsizerel "$@" 

: ${CMD=echo}
echo Count: $(count "$BUILD_TOOLS") 1>&2
for C in $COMPILERS; do 
  for N in $(seq 1 $(count "$BUILD_TOOLS")); do

    MAKE=`get_num $N $BUILD_TOOLS`
    EXT=`get_num $N $EXTENSIONS`
    FN=`get_num $N $FILENAMES`

   GENMK="$CMD $* -t $C -m $MAKE -o $OUTPUT_FILE"
   #echo "GENMK='$GENMK'" 1>&2
   (set -f
   IFS=" "
   eval "$GENMK")
  done
done





