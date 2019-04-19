#!/bin/sh

THISDIR=`dirname "$0"`

cd "$THISDIR/.."

PROJECTDIR=$PWD
PROJECTNAME=${PROJECTDIR##*/}

get_num() {
 (IFS=" "; N=$1; shift; set -- $@
  eval 'echo "${'$N'}"')
}

BUILD_TOOLS="nmake make gmake batch ninja"
EXTENSIONS="nmake mk gmake bat ninja"
FILENAMES="NMakefile Makefile GNUmakefile build.bat build.ninja"

: ${BUILDDIR="build/\$C"}

: ${COMPILERS="gcc clang tcc zapcc"}
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





