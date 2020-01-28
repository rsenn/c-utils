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
      BUILD_TOOLS="nmake pomake gmake ninja"
      EXTENSIONS="jom mk gnu ninja"
      FILENAMES="\${C} \${C} Makefile \${C}"
    ;;
  *) 
    : ${COMPILERS="gcc clang tcc zapcc"}
    BUILD_TOOLS="nmake pomake make gmake ninja"
    EXTENSIONS="jom pomake make gmake ninja"
    FILENAMES="NMakefile Makefile Makefile GNUmakefile build"
   
  ;;
esac
      OUTPUT_FILE="\${C}.\${EXT}"

set -f

if [ $# -eq 0 ]; then
  set -- lib {ar-wrap,binfmttest,bsdiffcat,buffertest,ccat,cc-wrap,cmake-run,cofflist,compiler-wrapper,count-depth,crc,decode-ls-lR,dnsip,dnsname,dnstest,eagle-gen-cmds,eagle-init-brd,eagle-to-circuit,eagle-to-svg,elf64list,elflist,elfwrsec,genmakefile,hexedit,httpproxy,httptest,impgen,jsonpp,jsontest,list-r,macho32list,mediathek-list,mediathek-parser,msys-shell,ntldd,omflist,opensearch-dump,parse,pathtool,pelist,piccfg,pkgcfg,plsconv,rdir-test,reg2cmd,regfilter,sln,strarraytest,tcping,testihex,torrent-progress,xml2json,xml2moon,xmlpp,xmltest2,xmltest3,xmltest4,xmltest,ziptest}.c 3rdparty #tests
  set -- "$@" -DHAVE_{ZLIB,LIBBZ2,LIBLZMA}=1 -DHAVE_CONFIG_H=1
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

  echo Compiler: $C Build tool: $MAKE Output file: $OUTFILE 1>&2

   GENMK="$CMD $* -t $C -m $MAKE -o $OUTFILE"
   #echo "GENMK='$GENMK'" 1>&2
   ([ -n "$CMD" ] && set -f
   IFS=" "
   eval "$GENMK 2>&1 ")
  done
done





