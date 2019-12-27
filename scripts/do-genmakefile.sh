#!/bin/sh

THISDIR=`dirname "$0"`

cd "$THISDIR/.."

PROJECTDIR=$PWD
PROJECTNAME=${PROJECTDIR##*/}

get_num() {
 (IFS=" "; N=$1; shift; set -- $@
  eval 'echo "${'$N'}"')
}

BUILD_TOOLS="nmake pomake make gmake batch ninja"
EXTENSIONS="nmake pomk mk gmake bat ninja"
FILENAMES="NMakefile Makefile Makefile GNUmakefile build.bat build.ninja"

case `uname -s` in
  *_NT-*) : ${COMPILERS="bcc55 bcc32 dmc32 pocc32 pocc64 tcc32 tcc64 lcc32 lcc64 occ32"} ;;
  *) : ${COMPILERS="gcc clang tcc zapcc"} ;;
esac

: ${BUILDDIR="build/\$C"}

: ${FILENAME="\$FN"}

set -f
set -- lib *.c 3rdparty tests

#set -- genmakefile --minsizerel "$@" -lzlib -lbz2 -llzma 
set -- "$@" -DHAVE_{ZLIB,LIBBZ2,LIBLZMA}=1
set -- genmakefile --minsizerel "$@" 

: ${CMD=echo}

for C in $COMPILERS; do 
  for N in $(seq 1 5); do

    MAKE=`get_num $N $BUILD_TOOLS`
    EXT=`get_num $N $EXTENSIONS`
    FN=`get_num $N $FILENAMES`

   GENMK="$CMD $* -t $C -m $MAKE -o ${BUILDDIR:+$BUILDDIR/}$FILENAME"
   #echo "GENMK='$GENMK'" 1>&2
   (set -f
   IFS=" "
   eval "$GENMK")
  done
done





