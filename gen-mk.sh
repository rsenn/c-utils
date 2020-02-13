#!/bin/sh -e 

IFS=" ""
"

exec_cmd() {
 (CMD="$*"
  echo "+ $CMD" 1>&2
  eval "eval \"$CMD\"")
}

PROGRAMS="ar-wrap.c binfmttest.c bsdiffcat.c buffertest.c ccat.c cc-wrap.c cmake-run.c cofflist.c compiler-wrapper.c count-depth.c crc.c decode-ls-lR.c dnsip.c dnsname.c dnstest.c eagle-gen-cmds.c eagle-init-brd.c eagle-to-circuit.c eagle-to-svg.c elf64list.c elflist.c elfwrsec.c genmakefile.c hexedit.c httpproxy.c httptest.c impgen.c jsonpp.c jsontest.c list-r.c macho32list.c mediathek-list.c mediathek-parser.c msys-shell.c ntldd.c omflist.c opensearch-dump.c parse.c pathtool.c pelist.c piccfg.c pkgcfg.c plsconv.c rdir-test.c reg2cmd.c regfilter.c sln.c strarraytest.c tcping.c testihex.c torrent-progress.c xml2json.c xml2moon.c xmlpp.c xmltest.c xmltest2.c xmltest3.c xmltest4.c ziptest.c"

MODULES="alloc,array,binfmt,buffer,byte,case,cb,cbmap,charbuf,coff,dir,dns,elf,env,errmsg,expand,fmt,gpio,hashmap,hmap,http,iarray,ihex,io,json,list,map,mmap,ndelay,omf,open,path,pe,playlist,process,range,rdir,safemult,scan,slist,socket,str,stralloc,strarray,strlist,tai,taia,textbuf,textcode,ucs,uint16,uint32,uint64,unix,var,vartab,wait,xml"

LIBS="-lws2_32"

if [ "$COMPRESS" = true ]; then
  LIBS="$LIBS -lzlib -lbz2 -llzma"
  DEFS="-DHAVE_{ZLIB,LIBBZ2,LIBLZMA}=1"
fi

[ $# -eq 0 ] && 
set -- lcc32 lcc64 tcc32 tcc64 bcc55 bcc101
set -- "$@" pocc32 pocc64
set -- "$@" dmc32 occ32

for compiler; do 
  for build_type in Debug RelWithDebInfo MinSizeRel Release; do
	output_dir=build/$compiler/$build_type
	mkdir -p $output_dir
 (exec_cmd "genmakefile --create-bins --create-libs -m ninja -t $compiler --$build_type \$DEFS lib/{\$MODULES}/*.c \$PROGRAMS -o $output_dir/build.ninja \$LIBS"
  exec_cmd "genmakefile --create-bins --create-libs -m make -t $compiler --$build_type \$DEFS lib/{\$MODULES}/*.c \$PROGRAMS -o $output_dir/Makefile")
  done
done
