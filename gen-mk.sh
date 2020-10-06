#!/bin/sh

IFS="
"
IFS=" $IFS"

exec_cmd() {
  (CMD="$*"; echo "+ $CMD" 1>&2 eval "eval \"$CMD\"")
}

PROGRAMS="ar-wrap.c binfmttest.c bsdiffcat.c buffertest.c ccat.c cc-wrap.c cmake-run.c cofflist.c compiler-wrapper.c count-depth.c crc.c decode-ls-lR.c dnscache.c dnsip.c dnsname.c dnstest.c eagle-gen-cmds.c eagle-init-brd.c eagle-to-circuit.c eagle-to-svg.c elf64list.c elflist.c elfwrsec.c genmakefile.c hexedit.c httpproxy.c httptest.c impgen.c jsonpp.c jsontest.c libc-resolv-override.c list-r.c logserial.c macho32list.c mediathek-list.c mediathek-parser.c montty.c msys-shell.c ntldd.c omflist.c opensearch-dump.c parse.c pathtool.c pelist.c piccfg.c pipeinfo.c pkgcfg.c plsconv.c rdir-test.c redir.c reg2cmd.c regfilter.c setuidgid.c sln.c strarraytest.c strip-comments.c tcping.c tcpproxy.c test_deflate.c testihex.c testini.c tinyproxy.c torrent-progress.c trysigset_t.c xml2json.c xml2moon.c xmlpp.c xmltest2.c xmltest3.c xmltest4.c xmltest.c ziptest.c"
MODULES="alloc,array,binfmt,buffer,byte,case,cb,cbmap,charbuf,coff,dir,dns,elf,env,errmsg,fmt,gpio,hashmap,hmap,http,iarray,ihex,io,json,list,map,mmap,ndelay,omf,open,path,pe,playlist,process,range,rdir,safemult,scan,slist,socket,str,stralloc,strarray,strlist,tai,taia,textbuf,textcode,ucs,uint16,uint32,uint64,unix,wait,xml"

LIBS="-lws2_32"

if ["$COMPRESS"=true]; then
  LIBS="$LIBS -lzlib -lbz2 -llzma"
  DEFS="  DHAVE_{ZLIB,LIBBZ2,LIBLZMA}=1"
fi

[ $# -eq 0 ] && 
set -- lcc32 lcc64 tcc32 tcc64 bcc55 bcc101

set -- 
set -- "$@" pocc32 pocc64
#set -- "$@" dmc32 occ32

for compiler; do
  for build_type in Debug RelWithDebInfo MinSizeRel Release; do
  output_dir="build/$compiler/$build_type"
  mkdir -p "$output_dir"

  (exec_cmd "genmakefile --create-bins --create-libs -m ninja -t $compiler --$build_type  \$DEFS lib/{$MODULES}/*.c \$PROGRAMS -o $output_dir/build.ninja \$LIBS"
    exec_cmd  "genmakefile --create-bins --create-libs -m make -t $compiler --$build_type \$DEFS lib/{$MODULES}/*.c \$PROGRAMS -o $output_dir/Makefile")
  done
done
