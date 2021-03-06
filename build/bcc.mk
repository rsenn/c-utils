CC = bcc32c
CCLD = $(CC)
LIB = tlib
CFLAGS = -v -O -O1 -d -dc
DEFS = -DHAVE_ERRNO_H=1 -DHAVE_INTTYPES_H=1 -DHAVE_STDBOOL_H=1 -DHAVE_STDINT_H=1 -DHAVE_SYS_FCNTL_H=1 -DHAVE_ZLIB=1 -DHAVE_LIBLZMA=1 -DHAVE_LIBBZ2=1
CPPFLAGS = 
COMPILE = $(CC) $(CFLAGS) $(DEFS) $(CPPFLAGS) -c
LIBS = array.lib binfmt.lib buffer.lib byte.lib case.lib cb.lib cbmap.lib charbuf.lib dir.lib dns.lib elf.lib env.lib errmsg.lib expand.lib fmt.lib gpio.lib hmap.lib http.lib iarray.lib io.lib json.lib list.lib map.lib mmap.lib ndelay.lib open.lib path.lib pe.lib playlist.lib rdir.lib scan.lib sig.lib slist.lib socket.lib str.lib stralloc.lib strarray.lib strlist.lib tai.lib taia.lib textbuf.lib uint16.lib uint32.lib uint64.lib var.lib vartab.lib xml.lib
EXTRA_LIBS = bz2.lib lzma.lib zlib.lib lzma.lib zlib.lib

all: build\bcc32\Debug build\bcc32\Debug\array.lib build\bcc32\Debug\binfmt.lib build\bcc32\Debug\buffer.lib build\bcc32\Debug\byte.lib build\bcc32\Debug\case.lib build\bcc32\Debug\cb.lib build\bcc32\Debug\cbmap.lib build\bcc32\Debug\charbuf.lib build\bcc32\Debug\dir.lib build\bcc32\Debug\dns.lib build\bcc32\Debug\elf.lib build\bcc32\Debug\env.lib build\bcc32\Debug\errmsg.lib build\bcc32\Debug\expand.lib build\bcc32\Debug\fmt.lib build\bcc32\Debug\gpio.lib build\bcc32\Debug\hmap.lib build\bcc32\Debug\http.lib build\bcc32\Debug\iarray.lib build\bcc32\Debug\io.lib build\bcc32\Debug\json.lib build\bcc32\Debug\list.lib build\bcc32\Debug\map.lib build\bcc32\Debug\mmap.lib build\bcc32\Debug\ndelay.lib build\bcc32\Debug\open.lib build\bcc32\Debug\path.lib build\bcc32\Debug\pe.lib build\bcc32\Debug\playlist.lib build\bcc32\Debug\rdir.lib build\bcc32\Debug\scan.lib build\bcc32\Debug\sig.lib build\bcc32\Debug\slist.lib build\bcc32\Debug\socket.lib build\bcc32\Debug\str.lib build\bcc32\Debug\stralloc.lib build\bcc32\Debug\strarray.lib build\bcc32\Debug\strlist.lib build\bcc32\Debug\tai.lib build\bcc32\Debug\taia.lib build\bcc32\Debug\textbuf.lib build\bcc32\Debug\uint16.lib build\bcc32\Debug\uint32.lib build\bcc32\Debug\uint64.lib build\bcc32\Debug\var.lib build\bcc32\Debug\vartab.lib build\bcc32\Debug\binfmttest.exe build\bcc32\Debug\bsdiffcat.exe build\bcc32\Debug\buffertest.exe build\bcc32\Debug\ccat.exe build\bcc32\Debug\compiler-wrapper.exe build\bcc32\Debug\count-depth.exe build\bcc32\Debug\decode-ls-lR.exe build\bcc32\Debug\dnsip.exe build\bcc32\Debug\dnsname.exe build\bcc32\Debug\dnstest.exe build\bcc32\Debug\eagle-gen-cmds.exe build\bcc32\Debug\eagle-init-brd.exe build\bcc32\Debug\eagle-to-circuit.exe build\bcc32\Debug\elf64list.exe build\bcc32\Debug\elflist.exe build\bcc32\Debug\elfwrsec.exe build\bcc32\Debug\genmakefile.exe build\bcc32\Debug\hexedit.exe build\bcc32\Debug\httptest.exe build\bcc32\Debug\impgen.exe build\bcc32\Debug\jsontest.exe build\bcc32\Debug\list-r.exe build\bcc32\Debug\macho32list.exe build\bcc32\Debug\mediathek-list.exe build\bcc32\Debug\mediathek-parser.exe build\bcc32\Debug\ntldd.exe build\bcc32\Debug\opensearch-dump.exe build\bcc32\Debug\pathtool.exe build\bcc32\Debug\pelist.exe build\bcc32\Debug\pkgcfg.exe build\bcc32\Debug\plsconv.exe build\bcc32\Debug\rdir-test.exe build\bcc32\Debug\reg2cmd.exe build\bcc32\Debug\regfilter.exe build\bcc32\Debug\sln.exe build\bcc32\Debug\strarraytest.exe build\bcc32\Debug\torrent-progress.exe build\bcc32\Debug\xmlpp.exe build\bcc32\Debug\xmltest.exe build\bcc32\Debug\xmltest2.exe build\bcc32\Debug\xmltest3.exe build\bcc32\Debug\xmltest4.exe build\bcc32\Debug\ziptest.exe

build\bcc32\Debug:
	-mkdir "build\bcc32\Debug"
	
build\bcc32\Debug\binfmttest.exe: build\bcc32\Debug\binfmttest.obj
	$(CC) $(CFLAGS) $(LDFLAGS) -e$@ build\bcc32\Debug\binfmttest.obj -tWC -L build\bcc32\Debug @&&|
 $(LIBS) $(EXTRA_LIBS)
|

build\bcc32\Debug\bsdiffcat.exe: build\bcc32\Debug\bsdiffcat.obj
	$(CC) $(CFLAGS) $(LDFLAGS) -e$@ build\bcc32\Debug\bsdiffcat.obj -tWC -L build\bcc32\Debug @&&|
 $(LIBS) $(EXTRA_LIBS)
|

build\bcc32\Debug\buffertest.exe: build\bcc32\Debug\buffertest.obj
	$(CC) $(CFLAGS) $(LDFLAGS) -e$@ build\bcc32\Debug\buffertest.obj -tWC -L build\bcc32\Debug @&&|
 $(LIBS) $(EXTRA_LIBS)
|

build\bcc32\Debug\ccat.exe: build\bcc32\Debug\ccat.obj build\bcc32\Debug\getopt.obj
	$(CC) $(CFLAGS) $(LDFLAGS) -e$@ build\bcc32\Debug\ccat.obj build\bcc32\Debug\getopt.obj -tWC -L build\bcc32\Debug @&&|
 $(LIBS) $(EXTRA_LIBS)
|

build\bcc32\Debug\compiler-wrapper.exe: build\bcc32\Debug\compiler-wrapper.obj
	$(CC) $(CFLAGS) $(LDFLAGS) -e$@ build\bcc32\Debug\compiler-wrapper.obj -tWC -L build\bcc32\Debug @&&|
 $(LIBS) $(EXTRA_LIBS)
|

build\bcc32\Debug\count-depth.exe: build\bcc32\Debug\count-depth.obj
	$(CC) $(CFLAGS) $(LDFLAGS) -e$@ build\bcc32\Debug\count-depth.obj -tWC -L build\bcc32\Debug @&&|
 $(LIBS) $(EXTRA_LIBS)
|

build\bcc32\Debug\decode-ls-lR.exe: build\bcc32\Debug\decode-ls-lR.obj
	$(CC) $(CFLAGS) $(LDFLAGS) -e$@ build\bcc32\Debug\decode-ls-lR.obj -tWC -L build\bcc32\Debug @&&|
 $(LIBS) $(EXTRA_LIBS)
|

build\bcc32\Debug\dnsip.exe: build\bcc32\Debug\dnsip.obj
	$(CC) $(CFLAGS) $(LDFLAGS) -e$@ build\bcc32\Debug\dnsip.obj -tWC -L build\bcc32\Debug @&&|
 $(LIBS) $(EXTRA_LIBS)
|

build\bcc32\Debug\dnsname.exe: build\bcc32\Debug\dnsname.obj
	$(CC) $(CFLAGS) $(LDFLAGS) -e$@ build\bcc32\Debug\dnsname.obj -tWC -L build\bcc32\Debug @&&|
 $(LIBS) $(EXTRA_LIBS)
|

build\bcc32\Debug\dnstest.exe: build\bcc32\Debug\dnstest.obj
	$(CC) $(CFLAGS) $(LDFLAGS) -e$@ build\bcc32\Debug\dnstest.obj -tWC -L build\bcc32\Debug @&&|
 $(LIBS) $(EXTRA_LIBS)
|

build\bcc32\Debug\eagle-gen-cmds.exe: build\bcc32\Debug\eagle-gen-cmds.obj build\bcc32\Debug\getopt.obj
	$(CC) $(CFLAGS) $(LDFLAGS) -e$@ build\bcc32\Debug\eagle-gen-cmds.obj build\bcc32\Debug\getopt.obj -tWC -L build\bcc32\Debug $(LIBS) $(EXTRA_LIBS)

build\bcc32\Debug\eagle-init-brd.exe: build\bcc32\Debug\eagle-init-brd.obj
	$(CC) $(CFLAGS) $(LDFLAGS) -e$@ build\bcc32\Debug\eagle-init-brd.obj -tWC -L build\bcc32\Debug @&&|
 $(LIBS) $(EXTRA_LIBS)
|

build\bcc32\Debug\eagle-to-circuit.exe: build\bcc32\Debug\eagle-to-circuit.obj
	$(CC) $(CFLAGS) $(LDFLAGS) -e$@ build\bcc32\Debug\eagle-to-circuit.obj -tWC -L build\bcc32\Debug @&&|
 $(LIBS) $(EXTRA_LIBS)
|

build\bcc32\Debug\elf64list.exe: build\bcc32\Debug\elf64list.obj
	$(CC) $(CFLAGS) $(LDFLAGS) -e$@ build\bcc32\Debug\elf64list.obj -tWC -L build\bcc32\Debug @&&|
 $(LIBS) $(EXTRA_LIBS)
|

build\bcc32\Debug\elflist.exe: build\bcc32\Debug\elflist.obj
	$(CC) $(CFLAGS) $(LDFLAGS) -e$@ build\bcc32\Debug\elflist.obj -tWC -L build\bcc32\Debug @&&|
 $(LIBS) $(EXTRA_LIBS)
|

build\bcc32\Debug\elfwrsec.exe: build\bcc32\Debug\elfwrsec.obj
	$(CC) $(CFLAGS) $(LDFLAGS) -e$@ build\bcc32\Debug\elfwrsec.obj -tWC -L build\bcc32\Debug @&&|
 $(LIBS) $(EXTRA_LIBS)
|

build\bcc32\Debug\genmakefile.exe: build\bcc32\Debug\genmakefile.obj build\bcc32\Debug\getopt.obj
	$(CC) $(CFLAGS) $(LDFLAGS) -e$@ build\bcc32\Debug\genmakefile.obj build\bcc32\Debug\getopt.obj -tWC -L build\bcc32\Debug @&&|
 $(LIBS) $(EXTRA_LIBS)
|

build\bcc32\Debug\hexedit.exe: build\bcc32\Debug\hexedit.obj
	$(CC) $(CFLAGS) $(LDFLAGS) -e$@ build\bcc32\Debug\hexedit.obj -tWC -L build\bcc32\Debug @&&|
 $(LIBS) $(EXTRA_LIBS)
|

build\bcc32\Debug\httptest.exe: build\bcc32\Debug\httptest.obj
	$(CC) $(CFLAGS) $(LDFLAGS) -e$@ build\bcc32\Debug\httptest.obj -tWC -L build\bcc32\Debug @&&|
 $(LIBS) $(EXTRA_LIBS)
|

build\bcc32\Debug\impgen.exe: build\bcc32\Debug\impgen.obj
	$(CC) $(CFLAGS) $(LDFLAGS) -e$@ build\bcc32\Debug\impgen.obj -tWC -L build\bcc32\Debug @&&|
 $(LIBS) $(EXTRA_LIBS)
|

build\bcc32\Debug\jsontest.exe: build\bcc32\Debug\jsontest.obj
	$(CC) $(CFLAGS) $(LDFLAGS) -e$@ build\bcc32\Debug\jsontest.obj -tWC -L build\bcc32\Debug @&&|
 $(LIBS) $(EXTRA_LIBS)
|

build\bcc32\Debug\list-r.exe: build\bcc32\Debug\list-r.obj build\bcc32\Debug\getopt.obj
	$(CC) $(CFLAGS) $(LDFLAGS) -e$@ build\bcc32\Debug\list-r.obj build\bcc32\Debug\getopt.obj -tWC -L build\bcc32\Debug @&&|
 $(LIBS) $(EXTRA_LIBS)
|

build\bcc32\Debug\macho32list.exe: build\bcc32\Debug\macho32list.obj
	$(CC) $(CFLAGS) $(LDFLAGS) -e$@ build\bcc32\Debug\macho32list.obj -tWC -L build\bcc32\Debug @&&|
 $(LIBS) $(EXTRA_LIBS)
|

build\bcc32\Debug\mediathek-list.exe: build\bcc32\Debug\mediathek-list.obj build\bcc32\Debug\getopt.obj
	$(CC) $(CFLAGS) $(LDFLAGS) -e$@ build\bcc32\Debug\mediathek-list.obj build\bcc32\Debug\getopt.obj -tWC -L build\bcc32\Debug @&&|
 $(LIBS) $(EXTRA_LIBS)
|

build\bcc32\Debug\mediathek-parser.exe: build\bcc32\Debug\mediathek-parser.obj build\bcc32\Debug\getopt.obj 
	$(CC) $(CFLAGS) $(LDFLAGS) -e$@ build\bcc32\Debug\mediathek-parser.obj build\bcc32\Debug\getopt.obj -tWC -L build\bcc32\Debug @&&|
 $(LIBS) $(EXTRA_LIBS)
|

build\bcc32\Debug\ntldd.exe: build\bcc32\Debug\ntldd.obj build\bcc32\Debug\getopt.obj build\bcc32\Debug\libntldd.obj
	$(CC) $(CFLAGS) $(LDFLAGS) -e$@ build\bcc32\Debug\ntldd.obj build\bcc32\Debug\getopt.obj build\bcc32\Debug\libntldd.obj -tWC -L build\bcc32\Debug @&&|
 $(LIBS) $(EXTRA_LIBS)
|

build\bcc32\Debug\opensearch-dump.exe: build\bcc32\Debug\opensearch-dump.obj
	$(CC) $(CFLAGS) $(LDFLAGS) -e$@ build\bcc32\Debug\opensearch-dump.obj -tWC -L build\bcc32\Debug @&&|
 $(LIBS) $(EXTRA_LIBS)
|

build\bcc32\Debug\pathtool.exe: build\bcc32\Debug\pathtool.obj build\bcc32\Debug\getopt.obj
	$(CC) $(CFLAGS) $(LDFLAGS) -e$@ build\bcc32\Debug\pathtool.obj build\bcc32\Debug\getopt.obj -tWC -L build\bcc32\Debug @&&|
 $(LIBS) $(EXTRA_LIBS)
|

build\bcc32\Debug\pelist.exe: build\bcc32\Debug\pelist.obj build\bcc32\Debug\getopt.obj
	$(CC) $(CFLAGS) $(LDFLAGS) -e$@ build\bcc32\Debug\pelist.obj build\bcc32\Debug\getopt.obj -tWC -L build\bcc32\Debug @&&|
 $(LIBS) $(EXTRA_LIBS)
|

build\bcc32\Debug\pkgcfg.exe: build\bcc32\Debug\pkgcfg.obj build\bcc32\Debug\getopt.obj
	$(CC) $(CFLAGS) $(LDFLAGS) -e$@ build\bcc32\Debug\pkgcfg.obj build\bcc32\Debug\getopt.obj -tWC -L build\bcc32\Debug @&&|
 $(LIBS) $(EXTRA_LIBS)
|

build\bcc32\Debug\plsconv.exe: build\bcc32\Debug\plsconv.obj build\bcc32\Debug\getopt.obj
	$(CC) $(CFLAGS) $(LDFLAGS) -e$@ build\bcc32\Debug\plsconv.obj build\bcc32\Debug\getopt.obj -tWC -L build\bcc32\Debug @&&|
 $(LIBS) $(EXTRA_LIBS)
|

build\bcc32\Debug\rdir-test.exe: build\bcc32\Debug\rdir-test.obj
	$(CC) $(CFLAGS) $(LDFLAGS) -e$@ build\bcc32\Debug\rdir-test.obj -tWC -L build\bcc32\Debug @&&|
 $(LIBS) $(EXTRA_LIBS)
|

build\bcc32\Debug\reg2cmd.exe: build\bcc32\Debug\reg2cmd.obj
	$(CC) $(CFLAGS) $(LDFLAGS) -e$@ build\bcc32\Debug\reg2cmd.obj -tWC -L build\bcc32\Debug @&&|
 $(LIBS) $(EXTRA_LIBS)
|

build\bcc32\Debug\regfilter.exe: build\bcc32\Debug\regfilter.obj
	$(CC) $(CFLAGS) $(LDFLAGS) -e$@ build\bcc32\Debug\regfilter.obj -tWC -L build\bcc32\Debug @&&|
 $(LIBS) $(EXTRA_LIBS)
|

build\bcc32\Debug\sln.exe: build\bcc32\Debug\sln.obj
	$(CC) $(CFLAGS) $(LDFLAGS) -e$@ build\bcc32\Debug\sln.obj -tWC -L build\bcc32\Debug @&&|
 $(LIBS) $(EXTRA_LIBS)
|

build\bcc32\Debug\strarraytest.exe: build\bcc32\Debug\strarraytest.obj
	$(CC) $(CFLAGS) $(LDFLAGS) -e$@ build\bcc32\Debug\strarraytest.obj -tWC -L build\bcc32\Debug @&&|
 $(LIBS) $(EXTRA_LIBS)
|

build\bcc32\Debug\torrent-progress.exe: build\bcc32\Debug\torrent-progress.obj
	$(CC) $(CFLAGS) $(LDFLAGS) -e$@ build\bcc32\Debug\torrent-progress.obj -tWC -L build\bcc32\Debug @&&|
 $(LIBS) $(EXTRA_LIBS)
|

build\bcc32\Debug\xmlpp.exe: build\bcc32\Debug\xmlpp.obj
	$(CC) $(CFLAGS) $(LDFLAGS) -e$@ build\bcc32\Debug\xmlpp.obj -tWC -L build\bcc32\Debug @&&|
 $(LIBS) $(EXTRA_LIBS)
|

build\bcc32\Debug\xmltest.exe: build\bcc32\Debug\xmltest.obj
	$(CC) $(CFLAGS) $(LDFLAGS) -e$@ build\bcc32\Debug\xmltest.obj -tWC -L build\bcc32\Debug @&&|
 $(LIBS) $(EXTRA_LIBS)
|

build\bcc32\Debug\xmltest2.exe: build\bcc32\Debug\xmltest2.obj
	$(CC) $(CFLAGS) $(LDFLAGS) -e$@ build\bcc32\Debug\xmltest2.obj -tWC -L build\bcc32\Debug @&&|
 $(LIBS) $(EXTRA_LIBS)
|

build\bcc32\Debug\xmltest3.exe: build\bcc32\Debug\xmltest3.obj
	$(CC) $(CFLAGS) $(LDFLAGS) -e$@ build\bcc32\Debug\xmltest3.obj -tWC -L build\bcc32\Debug @&&|
 $(LIBS) $(EXTRA_LIBS)
|

build\bcc32\Debug\xmltest4.exe: build\bcc32\Debug\xmltest4.obj
	$(CC) $(CFLAGS) $(LDFLAGS) -e$@ build\bcc32\Debug\xmltest4.obj -tWC -L build\bcc32\Debug @&&|
 $(LIBS) $(EXTRA_LIBS)
|

build\bcc32\Debug\ziptest.exe: build\bcc32\Debug\ziptest.obj build\bcc32\Debug\getopt.obj
	$(CC) $(CFLAGS) $(LDFLAGS) -e$@ build\bcc32\Debug\ziptest.obj build\bcc32\Debug\getopt.obj -tWC -L build\bcc32\Debug @&&|
 $(LIBS) $(EXTRA_LIBS)
|

build\bcc32\Debug\access.obj: access.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ access.c
build\bcc32\Debug\binfmttest.obj: binfmttest.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ binfmttest.c
build\bcc32\Debug\bsdiffcat.obj: bsdiffcat.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ bsdiffcat.c
build\bcc32\Debug\buffertest.obj: buffertest.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ buffertest.c
build\bcc32\Debug\ccat.obj: ccat.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ ccat.c
build\bcc32\Debug\compiler-wrapper.obj: compiler-wrapper.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ compiler-wrapper.c
build\bcc32\Debug\count-depth.obj: count-depth.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ count-depth.c
build\bcc32\Debug\decode-ls-lR.obj: decode-ls-lR.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ decode-ls-lR.c
build\bcc32\Debug\dnsip.obj: dnsip.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ dnsip.c
build\bcc32\Debug\dnsname.obj: dnsname.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ dnsname.c
build\bcc32\Debug\dnstest.obj: dnstest.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ dnstest.c
build\bcc32\Debug\eagle-gen-cmds.obj: eagle-gen-cmds.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ eagle-gen-cmds.c
build\bcc32\Debug\eagle-init-brd.obj: eagle-init-brd.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ eagle-init-brd.c
build\bcc32\Debug\eagle-to-circuit.obj: eagle-to-circuit.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ eagle-to-circuit.c
build\bcc32\Debug\elf64list.obj: elf64list.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ elf64list.c
build\bcc32\Debug\elflist.obj: elflist.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ elflist.c
build\bcc32\Debug\elfwrsec.obj: elfwrsec.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ elfwrsec.c
build\bcc32\Debug\genmakefile.obj: genmakefile.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ genmakefile.c
build\bcc32\Debug\hexedit.obj: hexedit.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ hexedit.c
build\bcc32\Debug\httptest.obj: httptest.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ httptest.c
build\bcc32\Debug\impgen.obj: impgen.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ impgen.c
build\bcc32\Debug\jsontest.obj: jsontest.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ jsontest.c
build\bcc32\Debug\array_allocate.obj: lib\array\array_allocate.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\array\array_allocate.c
build\bcc32\Debug\array_bytes.obj: lib\array\array_bytes.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\array\array_bytes.c
build\bcc32\Debug\array_cat.obj: lib\array\array_cat.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\array\array_cat.c
build\bcc32\Debug\array_cat0.obj: lib\array\array_cat0.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\array\array_cat0.c
build\bcc32\Debug\array_catb.obj: lib\array\array_catb.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\array\array_catb.c
build\bcc32\Debug\array_cate.obj: lib\array\array_cate.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\array\array_cate.c
build\bcc32\Debug\array_cats.obj: lib\array\array_cats.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\array\array_cats.c
build\bcc32\Debug\array_cats0.obj: lib\array\array_cats0.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\array\array_cats0.c
build\bcc32\Debug\array_end.obj: lib\array\array_end.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\array\array_end.c
build\bcc32\Debug\array_equal.obj: lib\array\array_equal.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\array\array_equal.c
build\bcc32\Debug\array_fail.obj: lib\array\array_fail.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\array\array_fail.c
build\bcc32\Debug\array_get.obj: lib\array\array_get.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\array\array_get.c
build\bcc32\Debug\array_length.obj: lib\array\array_length.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\array\array_length.c
build\bcc32\Debug\array_reset.obj: lib\array\array_reset.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\array\array_reset.c
build\bcc32\Debug\array_start.obj: lib\array\array_start.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\array\array_start.c
build\bcc32\Debug\array_trunc.obj: lib\array\array_trunc.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\array\array_trunc.c
build\bcc32\Debug\array_truncate.obj: lib\array\array_truncate.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\array\array_truncate.c
build\bcc32\Debug\binfmt_open.obj: lib\binfmt\binfmt_open.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\binfmt\binfmt_open.c
build\bcc32\Debug\buffer_0.obj: lib\buffer\buffer_0.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\buffer\buffer_0.c
build\bcc32\Debug\buffer_0small.obj: lib\buffer\buffer_0small.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\buffer\buffer_0small.c
build\bcc32\Debug\buffer_1.obj: lib\buffer\buffer_1.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\buffer\buffer_1.c
build\bcc32\Debug\buffer_1small.obj: lib\buffer\buffer_1small.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\buffer\buffer_1small.c
build\bcc32\Debug\buffer_2.obj: lib\buffer\buffer_2.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\buffer\buffer_2.c
build\bcc32\Debug\buffer_bz2.obj: lib\buffer\buffer_bz2.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\buffer\buffer_bz2.c
build\bcc32\Debug\buffer_bzip.obj: lib\buffer\buffer_bzip.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\buffer\buffer_bzip.c
build\bcc32\Debug\buffer_close.obj: lib\buffer\buffer_close.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\buffer\buffer_close.c
build\bcc32\Debug\buffer_copy.obj: lib\buffer\buffer_copy.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\buffer\buffer_copy.c
build\bcc32\Debug\buffer_default.obj: lib\buffer\buffer_default.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\buffer\buffer_default.c
build\bcc32\Debug\buffer_deflate.obj: lib\buffer\buffer_deflate.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\buffer\buffer_deflate.c
build\bcc32\Debug\buffer_dummyread.obj: lib\buffer\buffer_dummyread.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\buffer\buffer_dummyread.c
build\bcc32\Debug\buffer_dummyreadmmap.obj: lib\buffer\buffer_dummyreadmmap.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\buffer\buffer_dummyreadmmap.c
build\bcc32\Debug\buffer_dump.obj: lib\buffer\buffer_dump.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\buffer\buffer_dump.c
build\bcc32\Debug\buffer_feed.obj: lib\buffer\buffer_feed.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\buffer\buffer_feed.c
build\bcc32\Debug\buffer_flush.obj: lib\buffer\buffer_flush.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\buffer\buffer_flush.c
build\bcc32\Debug\buffer_free.obj: lib\buffer\buffer_free.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\buffer\buffer_free.c
build\bcc32\Debug\buffer_freshen.obj: lib\buffer\buffer_freshen.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\buffer\buffer_freshen.c
build\bcc32\Debug\buffer_fromarray.obj: lib\buffer\buffer_fromarray.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\buffer\buffer_fromarray.c
build\bcc32\Debug\buffer_frombuf.obj: lib\buffer\buffer_frombuf.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\buffer\buffer_frombuf.c
build\bcc32\Debug\buffer_fromsa.obj: lib\buffer\buffer_fromsa.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\buffer\buffer_fromsa.c
build\bcc32\Debug\buffer_fromstr.obj: lib\buffer\buffer_fromstr.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\buffer\buffer_fromstr.c
build\bcc32\Debug\buffer_get.obj: lib\buffer\buffer_get.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\buffer\buffer_get.c
build\bcc32\Debug\buffer_getc.obj: lib\buffer\buffer_getc.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\buffer\buffer_getc.c
build\bcc32\Debug\buffer_getline.obj: lib\buffer\buffer_getline.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\buffer\buffer_getline.c
build\bcc32\Debug\buffer_getline_sa.obj: lib\buffer\buffer_getline_sa.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\buffer\buffer_getline_sa.c
build\bcc32\Debug\buffer_getn.obj: lib\buffer\buffer_getn.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\buffer\buffer_getn.c
build\bcc32\Debug\buffer_getnewline_sa.obj: lib\buffer\buffer_getnewline_sa.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\buffer\buffer_getnewline_sa.c
build\bcc32\Debug\buffer_gettok_sa.obj: lib\buffer\buffer_gettok_sa.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\buffer\buffer_gettok_sa.c
build\bcc32\Debug\buffer_get_new_token_sa.obj: lib\buffer\buffer_get_new_token_sa.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\buffer\buffer_get_new_token_sa.c
build\bcc32\Debug\buffer_get_new_token_sa_pred.obj: lib\buffer\buffer_get_new_token_sa_pred.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\buffer\buffer_get_new_token_sa_pred.c
build\bcc32\Debug\buffer_get_token.obj: lib\buffer\buffer_get_token.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\buffer\buffer_get_token.c
build\bcc32\Debug\buffer_get_token_pred.obj: lib\buffer\buffer_get_token_pred.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\buffer\buffer_get_token_pred.c
build\bcc32\Debug\buffer_get_token_sa.obj: lib\buffer\buffer_get_token_sa.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\buffer\buffer_get_token_sa.c
build\bcc32\Debug\buffer_get_token_sa_pred.obj: lib\buffer\buffer_get_token_sa_pred.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\buffer\buffer_get_token_sa_pred.c
build\bcc32\Debug\buffer_get_until.obj: lib\buffer\buffer_get_until.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\buffer\buffer_get_until.c
build\bcc32\Debug\buffer_gzip.obj: lib\buffer\buffer_gzip.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\buffer\buffer_gzip.c
build\bcc32\Debug\buffer_inflate.obj: lib\buffer\buffer_inflate.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\buffer\buffer_inflate.c
build\bcc32\Debug\buffer_init.obj: lib\buffer\buffer_init.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\buffer\buffer_init.c
build\bcc32\Debug\buffer_init_free.obj: lib\buffer\buffer_init_free.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\buffer\buffer_init_free.c
build\bcc32\Debug\buffer_lzma.obj: lib\buffer\buffer_lzma.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\buffer\buffer_lzma.c
build\bcc32\Debug\buffer_mmapprivate.obj: lib\buffer\buffer_mmapprivate.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\buffer\buffer_mmapprivate.c
build\bcc32\Debug\buffer_mmapprivate_fd.obj: lib\buffer\buffer_mmapprivate_fd.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\buffer\buffer_mmapprivate_fd.c
build\bcc32\Debug\buffer_mmapread.obj: lib\buffer\buffer_mmapread.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\buffer\buffer_mmapread.c
build\bcc32\Debug\buffer_mmapread_fd.obj: lib\buffer\buffer_mmapread_fd.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\buffer\buffer_mmapread_fd.c
build\bcc32\Debug\buffer_mmapshared_fd.obj: lib\buffer\buffer_mmapshared_fd.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\buffer\buffer_mmapshared_fd.c
build\bcc32\Debug\buffer_munmap.obj: lib\buffer\buffer_munmap.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\buffer\buffer_munmap.c
build\bcc32\Debug\buffer_peek.obj: lib\buffer\buffer_peek.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\buffer\buffer_peek.c
build\bcc32\Debug\buffer_peekc.obj: lib\buffer\buffer_peekc.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\buffer\buffer_peekc.c
build\bcc32\Debug\buffer_prefetch.obj: lib\buffer\buffer_prefetch.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\buffer\buffer_prefetch.c
build\bcc32\Debug\buffer_put.obj: lib\buffer\buffer_put.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\buffer\buffer_put.c
build\bcc32\Debug\buffer_put8long.obj: lib\buffer\buffer_put8long.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\buffer\buffer_put8long.c
build\bcc32\Debug\buffer_putalign.obj: lib\buffer\buffer_putalign.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\buffer\buffer_putalign.c
build\bcc32\Debug\buffer_putc.obj: lib\buffer\buffer_putc.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\buffer\buffer_putc.c
build\bcc32\Debug\buffer_putdouble.obj: lib\buffer\buffer_putdouble.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\buffer\buffer_putdouble.c
build\bcc32\Debug\buffer_puterror.obj: lib\buffer\buffer_puterror.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\buffer\buffer_puterror.c
build\bcc32\Debug\buffer_puterror2.obj: lib\buffer\buffer_puterror2.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\buffer\buffer_puterror2.c
build\bcc32\Debug\buffer_putflush.obj: lib\buffer\buffer_putflush.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\buffer\buffer_putflush.c
build\bcc32\Debug\buffer_putlong.obj: lib\buffer\buffer_putlong.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\buffer\buffer_putlong.c
build\bcc32\Debug\buffer_putlonglong.obj: lib\buffer\buffer_putlonglong.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\buffer\buffer_putlonglong.c
build\bcc32\Debug\buffer_putm_internal.obj: lib\buffer\buffer_putm_internal.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\buffer\buffer_putm_internal.c
build\bcc32\Debug\buffer_putm_internal_flush.obj: lib\buffer\buffer_putm_internal_flush.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\buffer\buffer_putm_internal_flush.c
build\bcc32\Debug\buffer_putnc.obj: lib\buffer\buffer_putnc.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\buffer\buffer_putnc.c
build\bcc32\Debug\buffer_putnlflush.obj: lib\buffer\buffer_putnlflush.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\buffer\buffer_putnlflush.c
build\bcc32\Debug\buffer_putnspace.obj: lib\buffer\buffer_putnspace.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\buffer\buffer_putnspace.c
build\bcc32\Debug\buffer_putptr.obj: lib\buffer\buffer_putptr.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\buffer\buffer_putptr.c
build\bcc32\Debug\buffer_puts.obj: lib\buffer\buffer_puts.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\buffer\buffer_puts.c
build\bcc32\Debug\buffer_putsa.obj: lib\buffer\buffer_putsa.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\buffer\buffer_putsa.c
build\bcc32\Debug\buffer_putsaflush.obj: lib\buffer\buffer_putsaflush.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\buffer\buffer_putsaflush.c
build\bcc32\Debug\buffer_putsalign.obj: lib\buffer\buffer_putsalign.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\buffer\buffer_putsalign.c
build\bcc32\Debug\buffer_putsflush.obj: lib\buffer\buffer_putsflush.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\buffer\buffer_putsflush.c
build\bcc32\Debug\buffer_putspace.obj: lib\buffer\buffer_putspace.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\buffer\buffer_putspace.c
build\bcc32\Debug\buffer_putspad.obj: lib\buffer\buffer_putspad.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\buffer\buffer_putspad.c
build\bcc32\Debug\buffer_puts_escaped.obj: lib\buffer\buffer_puts_escaped.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\buffer\buffer_puts_escaped.c
build\bcc32\Debug\buffer_putulonglong.obj: lib\buffer\buffer_putulonglong.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\buffer\buffer_putulonglong.c
build\bcc32\Debug\buffer_putulong.obj: lib\buffer\buffer_putulong.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\buffer\buffer_putulong.c
build\bcc32\Debug\buffer_putulong0.obj: lib\buffer\buffer_putulong0.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\buffer\buffer_putulong0.c
build\bcc32\Debug\buffer_putxlong.obj: lib\buffer\buffer_putxlong.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\buffer\buffer_putxlong.c
build\bcc32\Debug\buffer_putxlong0.obj: lib\buffer\buffer_putxlong0.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\buffer\buffer_putxlong0.c
build\bcc32\Debug\buffer_putxlonglong.obj: lib\buffer\buffer_putxlonglong.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\buffer\buffer_putxlonglong.c
build\bcc32\Debug\buffer_putxlonglong0.obj: lib\buffer\buffer_putxlonglong0.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\buffer\buffer_putxlonglong0.c
build\bcc32\Debug\buffer_put_escaped.obj: lib\buffer\buffer_put_escaped.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\buffer\buffer_put_escaped.c
build\bcc32\Debug\buffer_read.obj: lib\buffer\buffer_read.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\buffer\buffer_read.c
build\bcc32\Debug\buffer_read_fd.obj: lib\buffer\buffer_read_fd.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\buffer\buffer_read_fd.c
build\bcc32\Debug\buffer_seek.obj: lib\buffer\buffer_seek.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\buffer\buffer_seek.c
build\bcc32\Debug\buffer_skipc.obj: lib\buffer\buffer_skipc.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\buffer\buffer_skipc.c
build\bcc32\Debug\buffer_skipn.obj: lib\buffer\buffer_skipn.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\buffer\buffer_skipn.c
build\bcc32\Debug\buffer_skipspace.obj: lib\buffer\buffer_skipspace.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\buffer\buffer_skipspace.c
build\bcc32\Debug\buffer_skip_pred.obj: lib\buffer\buffer_skip_pred.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\buffer\buffer_skip_pred.c
build\bcc32\Debug\buffer_skip_until.obj: lib\buffer\buffer_skip_until.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\buffer\buffer_skip_until.c
build\bcc32\Debug\buffer_stubborn.obj: lib\buffer\buffer_stubborn.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\buffer\buffer_stubborn.c
build\bcc32\Debug\buffer_stubborn2.obj: lib\buffer\buffer_stubborn2.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\buffer\buffer_stubborn2.c
build\bcc32\Debug\buffer_tosa.obj: lib\buffer\buffer_tosa.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\buffer\buffer_tosa.c
build\bcc32\Debug\buffer_truncfile.obj: lib\buffer\buffer_truncfile.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\buffer\buffer_truncfile.c
build\bcc32\Debug\buffer_write.obj: lib\buffer\buffer_write.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\buffer\buffer_write.c
build\bcc32\Debug\buffer_write_fd.obj: lib\buffer\buffer_write_fd.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\buffer\buffer_write_fd.c
build\bcc32\Debug\byte_case_diff.obj: lib\byte\byte_case_diff.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\byte\byte_case_diff.c
build\bcc32\Debug\byte_case_equal.obj: lib\byte\byte_case_equal.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\byte\byte_case_equal.c
build\bcc32\Debug\byte_case_start.obj: lib\byte\byte_case_start.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\byte\byte_case_start.c
build\bcc32\Debug\byte_chr.obj: lib\byte\byte_chr.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\byte\byte_chr.c
build\bcc32\Debug\byte_copy.obj: lib\byte\byte_copy.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\byte\byte_copy.c
build\bcc32\Debug\byte_copyr.obj: lib\byte\byte_copyr.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\byte\byte_copyr.c
build\bcc32\Debug\byte_count.obj: lib\byte\byte_count.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\byte\byte_count.c
build\bcc32\Debug\byte_diff.obj: lib\byte\byte_diff.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\byte\byte_diff.c
build\bcc32\Debug\byte_equal.obj: lib\byte\byte_equal.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\byte\byte_equal.c
build\bcc32\Debug\byte_fill.obj: lib\byte\byte_fill.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\byte\byte_fill.c
build\bcc32\Debug\byte_fmt.obj: lib\byte\byte_fmt.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\byte\byte_fmt.c
build\bcc32\Debug\byte_lower.obj: lib\byte\byte_lower.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\byte\byte_lower.c
build\bcc32\Debug\byte_rchr.obj: lib\byte\byte_rchr.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\byte\byte_rchr.c
build\bcc32\Debug\byte_scan.obj: lib\byte\byte_scan.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\byte\byte_scan.c
build\bcc32\Debug\byte_upper.obj: lib\byte\byte_upper.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\byte\byte_upper.c
build\bcc32\Debug\byte_zero.obj: lib\byte\byte_zero.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\byte\byte_zero.c
build\bcc32\Debug\case_diffb.obj: lib\case\case_diffb.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\case\case_diffb.c
build\bcc32\Debug\case_diffs.obj: lib\case\case_diffs.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\case\case_diffs.c
build\bcc32\Debug\case_lowerb.obj: lib\case\case_lowerb.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\case\case_lowerb.c
build\bcc32\Debug\case_lowers.obj: lib\case\case_lowers.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\case\case_lowers.c
build\bcc32\Debug\case_starts.obj: lib\case\case_starts.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\case\case_starts.c
build\bcc32\Debug\cb_clear.obj: lib\cb\cb_clear.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\cb\cb_clear.c
build\bcc32\Debug\cb_erase.obj: lib\cb\cb_erase.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\cb\cb_erase.c
build\bcc32\Debug\cb_find.obj: lib\cb\cb_find.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\cb\cb_find.c
build\bcc32\Debug\cb_find_prefix.obj: lib\cb\cb_find_prefix.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\cb\cb_find_prefix.c
build\bcc32\Debug\cb_foreach.obj: lib\cb\cb_foreach.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\cb\cb_foreach.c
build\bcc32\Debug\cb_get_kv.obj: lib\cb\cb_get_kv.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\cb\cb_get_kv.c
build\bcc32\Debug\cb_get_kv_ex.obj: lib\cb\cb_get_kv_ex.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\cb\cb_get_kv_ex.c
build\bcc32\Debug\cb_insert.obj: lib\cb\cb_insert.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\cb\cb_insert.c
build\bcc32\Debug\cb_new_kv.obj: lib\cb\cb_new_kv.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\cb\cb_new_kv.c
build\bcc32\Debug\alloc.obj: lib\cbmap\alloc.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\cbmap\alloc.c
build\bcc32\Debug\cbmap_count.obj: lib\cbmap\cbmap_count.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\cbmap\cbmap_count.c
build\bcc32\Debug\cbmap_data_node_destroy.obj: lib\cbmap\cbmap_data_node_destroy.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\cbmap\cbmap_data_node_destroy.c
build\bcc32\Debug\cbmap_delete.obj: lib\cbmap\cbmap_delete.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\cbmap\cbmap_delete.c
build\bcc32\Debug\cbmap_destroy.obj: lib\cbmap\cbmap_destroy.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\cbmap\cbmap_destroy.c
build\bcc32\Debug\cbmap_get.obj: lib\cbmap\cbmap_get.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\cbmap\cbmap_get.c
build\bcc32\Debug\cbmap_insert.obj: lib\cbmap\cbmap_insert.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\cbmap\cbmap_insert.c
build\bcc32\Debug\cbmap_internal_node.obj: lib\cbmap\cbmap_internal_node.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\cbmap\cbmap_internal_node.c
build\bcc32\Debug\cbmap_new.obj: lib\cbmap\cbmap_new.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\cbmap\cbmap_new.c
build\bcc32\Debug\cbmap_visit.obj: lib\cbmap\cbmap_visit.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\cbmap\cbmap_visit.c
build\bcc32\Debug\cbmap_visit_all.obj: lib\cbmap\cbmap_visit_all.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\cbmap\cbmap_visit_all.c
build\bcc32\Debug\charbuf_close.obj: lib\charbuf\charbuf_close.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\charbuf\charbuf_close.c
build\bcc32\Debug\charbuf_get.obj: lib\charbuf\charbuf_get.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\charbuf\charbuf_get.c
build\bcc32\Debug\charbuf_getc.obj: lib\charbuf\charbuf_getc.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\charbuf\charbuf_getc.c
build\bcc32\Debug\charbuf_init.obj: lib\charbuf\charbuf_init.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\charbuf\charbuf_init.c
build\bcc32\Debug\charbuf_nextc.obj: lib\charbuf\charbuf_nextc.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\charbuf\charbuf_nextc.c
build\bcc32\Debug\charbuf_peek.obj: lib\charbuf\charbuf_peek.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\charbuf\charbuf_peek.c
build\bcc32\Debug\charbuf_peekc.obj: lib\charbuf\charbuf_peekc.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\charbuf\charbuf_peekc.c
build\bcc32\Debug\charbuf_skip.obj: lib\charbuf\charbuf_skip.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\charbuf\charbuf_skip.c
build\bcc32\Debug\charbuf_skip_pred.obj: lib\charbuf\charbuf_skip_pred.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\charbuf\charbuf_skip_pred.c
build\bcc32\Debug\charbuf_skip_until.obj: lib\charbuf\charbuf_skip_until.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\charbuf\charbuf_skip_until.c
build\bcc32\Debug\dir_close.obj: lib\dir\dir_close.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\dir\dir_close.c
build\bcc32\Debug\dir_name.obj: lib\dir\dir_name.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\dir\dir_name.c
build\bcc32\Debug\dir_open.obj: lib\dir\dir_open.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\dir\dir_open.c
build\bcc32\Debug\dir_read.obj: lib\dir\dir_read.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\dir\dir_read.c
build\bcc32\Debug\dir_time.obj: lib\dir\dir_time.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\dir\dir_time.c
build\bcc32\Debug\dir_type.obj: lib\dir\dir_type.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\dir\dir_type.c
build\bcc32\Debug\dns_dfd.obj: lib\dns\dns_dfd.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\dns\dns_dfd.c
build\bcc32\Debug\dns_domain.obj: lib\dns\dns_domain.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\dns\dns_domain.c
build\bcc32\Debug\dns_dtda.obj: lib\dns\dns_dtda.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\dns\dns_dtda.c
build\bcc32\Debug\dns_ip4.obj: lib\dns\dns_ip4.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\dns\dns_ip4.c
build\bcc32\Debug\dns_ip6.obj: lib\dns\dns_ip6.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\dns\dns_ip6.c
build\bcc32\Debug\dns_ipq.obj: lib\dns\dns_ipq.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\dns\dns_ipq.c
build\bcc32\Debug\dns_ipq6.obj: lib\dns\dns_ipq6.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\dns\dns_ipq6.c
build\bcc32\Debug\dns_mx.obj: lib\dns\dns_mx.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\dns\dns_mx.c
build\bcc32\Debug\dns_name.obj: lib\dns\dns_name.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\dns\dns_name.c
build\bcc32\Debug\dns_nd.obj: lib\dns\dns_nd.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\dns\dns_nd.c
build\bcc32\Debug\dns_nd6.obj: lib\dns\dns_nd6.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\dns\dns_nd6.c
build\bcc32\Debug\dns_packet.obj: lib\dns\dns_packet.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\dns\dns_packet.c
build\bcc32\Debug\dns_random.obj: lib\dns\dns_random.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\dns\dns_random.c
build\bcc32\Debug\dns_rcip.obj: lib\dns\dns_rcip.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\dns\dns_rcip.c
build\bcc32\Debug\dns_rcrw.obj: lib\dns\dns_rcrw.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\dns\dns_rcrw.c
build\bcc32\Debug\dns_resolve.obj: lib\dns\dns_resolve.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\dns\dns_resolve.c
build\bcc32\Debug\dns_sortip.obj: lib\dns\dns_sortip.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\dns\dns_sortip.c
build\bcc32\Debug\dns_sortip6.obj: lib\dns\dns_sortip6.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\dns\dns_sortip6.c
build\bcc32\Debug\dns_transmit.obj: lib\dns\dns_transmit.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\dns\dns_transmit.c
build\bcc32\Debug\dns_txt.obj: lib\dns\dns_txt.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\dns\dns_txt.c
build\bcc32\Debug\elf_dynamic_section.obj: lib\elf\elf_dynamic_section.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\elf\elf_dynamic_section.c
build\bcc32\Debug\elf_get_section.obj: lib\elf\elf_get_section.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\elf\elf_get_section.c
build\bcc32\Debug\elf_get_value.obj: lib\elf\elf_get_value.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\elf\elf_get_value.c
build\bcc32\Debug\elf_header.obj: lib\elf\elf_header.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\elf\elf_header.c
build\bcc32\Debug\elf_program_headers.obj: lib\elf\elf_program_headers.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\elf\elf_program_headers.c
build\bcc32\Debug\elf_section_headers.obj: lib\elf\elf_section_headers.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\elf\elf_section_headers.c
build\bcc32\Debug\elf_section_index.obj: lib\elf\elf_section_index.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\elf\elf_section_index.c
build\bcc32\Debug\elf_section_offset.obj: lib\elf\elf_section_offset.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\elf\elf_section_offset.c
build\bcc32\Debug\elf_section_size.obj: lib\elf\elf_section_size.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\elf\elf_section_size.c
build\bcc32\Debug\elf_section_type.obj: lib\elf\elf_section_type.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\elf\elf_section_type.c
build\bcc32\Debug\elf_shstrtab.obj: lib\elf\elf_shstrtab.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\elf\elf_shstrtab.c
build\bcc32\Debug\env_addmodif.obj: lib\env\env_addmodif.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\env\env_addmodif.c
build\bcc32\Debug\env_get.obj: lib\env\env_get.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\env\env_get.c
build\bcc32\Debug\env_get2.obj: lib\env\env_get2.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\env\env_get2.c
build\bcc32\Debug\env_len.obj: lib\env\env_len.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\env\env_len.c
build\bcc32\Debug\env_make.obj: lib\env\env_make.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\env\env_make.c
build\bcc32\Debug\env_merge.obj: lib\env\env_merge.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\env\env_merge.c
build\bcc32\Debug\env_pick.obj: lib\env\env_pick.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\env\env_pick.c
build\bcc32\Debug\env_put.obj: lib\env\env_put.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\env\env_put.c
build\bcc32\Debug\env_set.obj: lib\env\env_set.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\env\env_set.c
build\bcc32\Debug\env_string.obj: lib\env\env_string.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\env\env_string.c
build\bcc32\Debug\env_unset.obj: lib\env\env_unset.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\env\env_unset.c
build\bcc32\Debug\errmsg_iam.obj: lib\errmsg\errmsg_iam.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\errmsg\errmsg_iam.c
build\bcc32\Debug\errmsg_info.obj: lib\errmsg\errmsg_info.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\errmsg\errmsg_info.c
build\bcc32\Debug\errmsg_infosys.obj: lib\errmsg\errmsg_infosys.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\errmsg\errmsg_infosys.c
build\bcc32\Debug\errmsg_puts.obj: lib\errmsg\errmsg_puts.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\errmsg\errmsg_puts.c
build\bcc32\Debug\errmsg_warn.obj: lib\errmsg\errmsg_warn.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\errmsg\errmsg_warn.c
build\bcc32\Debug\errmsg_warnsys.obj: lib\errmsg\errmsg_warnsys.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\errmsg\errmsg_warnsys.c
build\bcc32\Debug\errmsg_write.obj: lib\errmsg\errmsg_write.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\errmsg\errmsg_write.c
build\bcc32\Debug\expand_arg.obj: lib\expand\expand_arg.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\expand\expand_arg.c
build\bcc32\Debug\expand_args.obj: lib\expand\expand_args.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\expand\expand_args.c
build\bcc32\Debug\expand_argv.obj: lib\expand\expand_argv.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\expand\expand_argv.c
build\bcc32\Debug\expand_arith.obj: lib\expand\expand_arith.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\expand\expand_arith.c
build\bcc32\Debug\expand_cat.obj: lib\expand\expand_cat.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\expand\expand_cat.c
build\bcc32\Debug\expand_catsa.obj: lib\expand\expand_catsa.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\expand\expand_catsa.c
build\bcc32\Debug\expand_command.obj: lib\expand\expand_command.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\expand\expand_command.c
build\bcc32\Debug\expand_copysa.obj: lib\expand\expand_copysa.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\expand\expand_copysa.c
build\bcc32\Debug\expand_escape.obj: lib\expand\expand_escape.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\expand\expand_escape.c
build\bcc32\Debug\expand_glob.obj: lib\expand\expand_glob.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\expand\expand_glob.c
build\bcc32\Debug\expand_param.obj: lib\expand\expand_param.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\expand\expand_param.c
build\bcc32\Debug\expand_unescape.obj: lib\expand\expand_unescape.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\expand\expand_unescape.c
build\bcc32\Debug\expand_vars.obj: lib\expand\expand_vars.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\expand\expand_vars.c
build\bcc32\Debug\fmt_8long.obj: lib\fmt\fmt_8long.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\fmt\fmt_8long.c
build\bcc32\Debug\fmt_octal.obj: lib\fmt\fmt_octal.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\fmt\fmt_octal.c
build\bcc32\Debug\fmt_asn1derlength.obj: lib\fmt\fmt_asn1derlength.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\fmt\fmt_asn1derlength.c
build\bcc32\Debug\fmt_asn1dertag.obj: lib\fmt\fmt_asn1dertag.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\fmt\fmt_asn1dertag.c
build\bcc32\Debug\fmt_double.obj: lib\fmt\fmt_double.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\fmt\fmt_double.c
build\bcc32\Debug\fmt_escapecharc.obj: lib\fmt\fmt_escapecharc.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\fmt\fmt_escapecharc.c
build\bcc32\Debug\fmt_escapecharhtml.obj: lib\fmt\fmt_escapecharhtml.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\fmt\fmt_escapecharhtml.c
build\bcc32\Debug\fmt_escapecharquotedprintable.obj: lib\fmt\fmt_escapecharquotedprintable.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\fmt\fmt_escapecharquotedprintable.c
build\bcc32\Debug\fmt_escapecharquotedprintableutf8.obj: lib\fmt\fmt_escapecharquotedprintableutf8.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\fmt\fmt_escapecharquotedprintableutf8.c
build\bcc32\Debug\fmt_escapecharshell.obj: lib\fmt\fmt_escapecharshell.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\fmt\fmt_escapecharshell.c
build\bcc32\Debug\fmt_escapecharxml.obj: lib\fmt\fmt_escapecharxml.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\fmt\fmt_escapecharxml.c
build\bcc32\Debug\fmt_fill.obj: lib\fmt\fmt_fill.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\fmt\fmt_fill.c
build\bcc32\Debug\fmt_hexb.obj: lib\fmt\fmt_hexb.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\fmt\fmt_hexb.c
build\bcc32\Debug\fmt_httpdate.obj: lib\fmt\fmt_httpdate.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\fmt\fmt_httpdate.c
build\bcc32\Debug\fmt_human.obj: lib\fmt\fmt_human.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\fmt\fmt_human.c
build\bcc32\Debug\fmt_humank.obj: lib\fmt\fmt_humank.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\fmt\fmt_humank.c
build\bcc32\Debug\fmt_iso8601.obj: lib\fmt\fmt_iso8601.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\fmt\fmt_iso8601.c
build\bcc32\Debug\fmt_long.obj: lib\fmt\fmt_long.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\fmt\fmt_long.c
build\bcc32\Debug\fmt_longlong.obj: lib\fmt\fmt_longlong.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\fmt\fmt_longlong.c
build\bcc32\Debug\fmt_minus.obj: lib\fmt\fmt_minus.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\fmt\fmt_minus.c
build\bcc32\Debug\fmt_pad.obj: lib\fmt\fmt_pad.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\fmt\fmt_pad.c
build\bcc32\Debug\fmt_plusminus.obj: lib\fmt\fmt_plusminus.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\fmt\fmt_plusminus.c
build\bcc32\Debug\fmt_repeat.obj: lib\fmt\fmt_repeat.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\fmt\fmt_repeat.c
build\bcc32\Debug\fmt_str.obj: lib\fmt\fmt_str.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\fmt\fmt_str.c
build\bcc32\Debug\fmt_strm_internal.obj: lib\fmt\fmt_strm_internal.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\fmt\fmt_strm_internal.c
build\bcc32\Debug\fmt_strn.obj: lib\fmt\fmt_strn.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\fmt\fmt_strn.c
build\bcc32\Debug\fmt_tohex.obj: lib\fmt\fmt_tohex.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\fmt\fmt_tohex.c
build\bcc32\Debug\fmt_ulonglong.obj: lib\fmt\fmt_ulonglong.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\fmt\fmt_ulonglong.c
build\bcc32\Debug\fmt_ulong.obj: lib\fmt\fmt_ulong.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\fmt\fmt_ulong.c
build\bcc32\Debug\fmt_ulong0.obj: lib\fmt\fmt_ulong0.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\fmt\fmt_ulong0.c
build\bcc32\Debug\fmt_utf8.obj: lib\fmt\fmt_utf8.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\fmt\fmt_utf8.c
build\bcc32\Debug\fmt_xlong.obj: lib\fmt\fmt_xlong.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\fmt\fmt_xlong.c
build\bcc32\Debug\fmt_xlonglong.obj: lib\fmt\fmt_xlonglong.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\fmt\fmt_xlonglong.c
build\bcc32\Debug\fmt_xmlescape.obj: lib\fmt\fmt_xmlescape.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\fmt\fmt_xmlescape.c
build\bcc32\Debug\fnmatch.obj: lib\fnmatch.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\fnmatch.c
build\bcc32\Debug\fork.obj: lib\fork.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\fork.c
build\bcc32\Debug\getdelim.obj: lib\getdelim.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\getdelim.c
build\bcc32\Debug\getenv.obj: lib\getenv.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\getenv.c
build\bcc32\Debug\getopt.obj: lib\getopt.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\getopt.c
build\bcc32\Debug\glob.obj: lib\glob.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\glob.c
build\bcc32\Debug\gpio_init.obj: lib\gpio\gpio_init.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\gpio\gpio_init.c
build\bcc32\Debug\gpio_read.obj: lib\gpio\gpio_read.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\gpio\gpio_read.c
build\bcc32\Debug\gpio_set_input_pull.obj: lib\gpio\gpio_set_input_pull.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\gpio\gpio_set_input_pull.c
build\bcc32\Debug\gpio_set_input_pull_down.obj: lib\gpio\gpio_set_input_pull_down.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\gpio\gpio_set_input_pull_down.c
build\bcc32\Debug\gpio_set_input_pull_up.obj: lib\gpio\gpio_set_input_pull_up.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\gpio\gpio_set_input_pull_up.c
build\bcc32\Debug\gpio_set_input_resistor_pull.obj: lib\gpio\gpio_set_input_resistor_pull.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\gpio\gpio_set_input_resistor_pull.c
build\bcc32\Debug\gpio_set_output.obj: lib\gpio\gpio_set_output.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\gpio\gpio_set_output.c
build\bcc32\Debug\gpio_set_pin_mode.obj: lib\gpio\gpio_set_pin_mode.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\gpio\gpio_set_pin_mode.c
build\bcc32\Debug\gpio_short_wait.obj: lib\gpio\gpio_short_wait.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\gpio\gpio_short_wait.c
build\bcc32\Debug\gpio_write.obj: lib\gpio\gpio_write.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\gpio\gpio_write.c
build\bcc32\Debug\hashmap.obj: lib\hashmap.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\hashmap.c
build\bcc32\Debug\hmap_add.obj: lib\hmap\hmap_add.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\hmap\hmap_add.c
build\bcc32\Debug\hmap_add_tuple_with_data.obj: lib\hmap\hmap_add_tuple_with_data.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\hmap\hmap_add_tuple_with_data.c
build\bcc32\Debug\hmap_delete.obj: lib\hmap\hmap_delete.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\hmap\hmap_delete.c
build\bcc32\Debug\hmap_destroy.obj: lib\hmap\hmap_destroy.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\hmap\hmap_destroy.c
build\bcc32\Debug\hmap_dump.obj: lib\hmap\hmap_dump.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\hmap\hmap_dump.c
build\bcc32\Debug\hmap_free_data.obj: lib\hmap\hmap_free_data.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\hmap\hmap_free_data.c
build\bcc32\Debug\hmap_init.obj: lib\hmap\hmap_init.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\hmap\hmap_init.c
build\bcc32\Debug\hmap_internal.obj: lib\hmap\hmap_internal.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\hmap\hmap_internal.c
build\bcc32\Debug\hmap_is_locate.obj: lib\hmap\hmap_is_locate.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\hmap\hmap_is_locate.c
build\bcc32\Debug\hmap_print_list.obj: lib\hmap\hmap_print_list.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\hmap\hmap_print_list.c
build\bcc32\Debug\hmap_print_table.obj: lib\hmap\hmap_print_table.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\hmap\hmap_print_table.c
build\bcc32\Debug\hmap_print_tree.obj: lib\hmap\hmap_print_tree.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\hmap\hmap_print_tree.c
build\bcc32\Debug\hmap_search.obj: lib\hmap\hmap_search.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\hmap\hmap_search.c
build\bcc32\Debug\hmap_set.obj: lib\hmap\hmap_set.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\hmap\hmap_set.c
build\bcc32\Debug\hmap_set_chars.obj: lib\hmap\hmap_set_chars.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\hmap\hmap_set_chars.c
build\bcc32\Debug\hmap_set_stralloc.obj: lib\hmap\hmap_set_stralloc.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\hmap\hmap_set_stralloc.c
build\bcc32\Debug\hmap_size.obj: lib\hmap\hmap_size.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\hmap\hmap_size.c
build\bcc32\Debug\hmap_truncate.obj: lib\hmap\hmap_truncate.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\hmap\hmap_truncate.c
build\bcc32\Debug\http_get.obj: lib\http\http_get.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\http\http_get.c
build\bcc32\Debug\http_init.obj: lib\http\http_init.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\http\http_init.c
build\bcc32\Debug\http_readable.obj: lib\http\http_readable.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\http\http_readable.c
build\bcc32\Debug\http_sendreq.obj: lib\http\http_sendreq.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\http\http_sendreq.c
build\bcc32\Debug\http_socket.obj: lib\http\http_socket.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\http\http_socket.c
build\bcc32\Debug\http_writeable.obj: lib\http\http_writeable.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\http\http_writeable.c
build\bcc32\Debug\iarray_allocate.obj: lib\iarray\iarray_allocate.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\iarray\iarray_allocate.c
build\bcc32\Debug\iarray_free.obj: lib\iarray\iarray_free.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\iarray\iarray_free.c
build\bcc32\Debug\iarray_get.obj: lib\iarray\iarray_get.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\iarray\iarray_get.c
build\bcc32\Debug\iarray_init.obj: lib\iarray\iarray_init.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\iarray\iarray_init.c
build\bcc32\Debug\iarray_length.obj: lib\iarray\iarray_length.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\iarray\iarray_length.c
build\bcc32\Debug\io_appendfile.obj: lib\io\io_appendfile.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\io\io_appendfile.c
build\bcc32\Debug\io_block.obj: lib\io\io_block.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\io\io_block.c
build\bcc32\Debug\io_canread.obj: lib\io\io_canread.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\io\io_canread.c
build\bcc32\Debug\io_canwrite.obj: lib\io\io_canwrite.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\io\io_canwrite.c
build\bcc32\Debug\io_check.obj: lib\io\io_check.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\io\io_check.c
build\bcc32\Debug\io_close.obj: lib\io\io_close.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\io\io_close.c
build\bcc32\Debug\io_closeonexec.obj: lib\io\io_closeonexec.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\io\io_closeonexec.c
build\bcc32\Debug\io_createfile.obj: lib\io\io_createfile.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\io\io_createfile.c
build\bcc32\Debug\io_debugstring.obj: lib\io\io_debugstring.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\io\io_debugstring.c
build\bcc32\Debug\io_dontwantread.obj: lib\io\io_dontwantread.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\io\io_dontwantread.c
build\bcc32\Debug\io_dontwantwrite.obj: lib\io\io_dontwantwrite.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\io\io_dontwantwrite.c
build\bcc32\Debug\io_dup.obj: lib\io\io_dup.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\io\io_dup.c
build\bcc32\Debug\io_eagain.obj: lib\io\io_eagain.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\io\io_eagain.c
build\bcc32\Debug\io_eagain_read.obj: lib\io\io_eagain_read.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\io\io_eagain_read.c
build\bcc32\Debug\io_eagain_write.obj: lib\io\io_eagain_write.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\io\io_eagain_write.c
build\bcc32\Debug\io_fd.obj: lib\io\io_fd.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\io\io_fd.c
build\bcc32\Debug\io_finishandshutdown.obj: lib\io\io_finishandshutdown.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\io\io_finishandshutdown.c
build\bcc32\Debug\io_getcookie.obj: lib\io\io_getcookie.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\io\io_getcookie.c
build\bcc32\Debug\io_mmapwritefile.obj: lib\io\io_mmapwritefile.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\io\io_mmapwritefile.c
build\bcc32\Debug\io_nonblock.obj: lib\io\io_nonblock.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\io\io_nonblock.c
build\bcc32\Debug\io_pipe.obj: lib\io\io_pipe.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\io\io_pipe.c
build\bcc32\Debug\io_readfile.obj: lib\io\io_readfile.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\io\io_readfile.c
build\bcc32\Debug\io_readwritefile.obj: lib\io\io_readwritefile.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\io\io_readwritefile.c
build\bcc32\Debug\io_sendfile.obj: lib\io\io_sendfile.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\io\io_sendfile.c
build\bcc32\Debug\io_setcookie.obj: lib\io\io_setcookie.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\io\io_setcookie.c
build\bcc32\Debug\io_sigpipe.obj: lib\io\io_sigpipe.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\io\io_sigpipe.c
build\bcc32\Debug\io_socketpair.obj: lib\io\io_socketpair.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\io\io_socketpair.c
build\bcc32\Debug\io_timedout.obj: lib\io\io_timedout.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\io\io_timedout.c
build\bcc32\Debug\io_timeout.obj: lib\io\io_timeout.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\io\io_timeout.c
build\bcc32\Debug\io_timeouted.obj: lib\io\io_timeouted.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\io\io_timeouted.c
build\bcc32\Debug\io_tryread.obj: lib\io\io_tryread.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\io\io_tryread.c
build\bcc32\Debug\io_tryreadtimeout.obj: lib\io\io_tryreadtimeout.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\io\io_tryreadtimeout.c
build\bcc32\Debug\io_trywrite.obj: lib\io\io_trywrite.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\io\io_trywrite.c
build\bcc32\Debug\io_trywritetimeout.obj: lib\io\io_trywritetimeout.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\io\io_trywritetimeout.c
build\bcc32\Debug\io_wait.obj: lib\io\io_wait.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\io\io_wait.c
build\bcc32\Debug\io_waitread.obj: lib\io\io_waitread.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\io\io_waitread.c
build\bcc32\Debug\io_waituntil.obj: lib\io\io_waituntil.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\io\io_waituntil.c
build\bcc32\Debug\io_waituntil2.obj: lib\io\io_waituntil2.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\io\io_waituntil2.c
build\bcc32\Debug\io_waitwrite.obj: lib\io\io_waitwrite.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\io\io_waitwrite.c
build\bcc32\Debug\io_wantread.obj: lib\io\io_wantread.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\io\io_wantread.c
build\bcc32\Debug\io_wantwrite.obj: lib\io\io_wantwrite.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\io\io_wantwrite.c
build\bcc32\Debug\iopause.obj: lib\iopause.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\iopause.c
build\bcc32\Debug\io_passfd.obj: lib\io_passfd.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\io_passfd.c
build\bcc32\Debug\isleap.obj: lib\isleap.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\isleap.c
build\bcc32\Debug\json_free.obj: lib\json\json_free.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\json\json_free.c
build\bcc32\Debug\json_newnode.obj: lib\json\json_newnode.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\json\json_newnode.c
build\bcc32\Debug\json_parse.obj: lib\json\json_parse.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\json\json_parse.c
build\bcc32\Debug\json_print.obj: lib\json\json_print.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\json\json_print.c
build\bcc32\Debug\json_reader_init.obj: lib\json\json_reader_init.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\json\json_reader_init.c
build\bcc32\Debug\json_read_callback.obj: lib\json\json_read_callback.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\json\json_read_callback.c
build\bcc32\Debug\json_read_tree.obj: lib\json\json_read_tree.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\json\json_read_tree.c
build\bcc32\Debug\linked_list.obj: lib\linked_list.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\linked_list.c
build\bcc32\Debug\list_add_after.obj: lib\list\list_add_after.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\list\list_add_after.c
build\bcc32\Debug\list_add_before.obj: lib\list\list_add_before.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\list\list_add_before.c
build\bcc32\Debug\list_find.obj: lib\list\list_find.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\list\list_find.c
build\bcc32\Debug\list_find_remove.obj: lib\list\list_find_remove.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\list\list_find_remove.c
build\bcc32\Debug\list_init.obj: lib\list\list_init.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\list\list_init.c
build\bcc32\Debug\list_length.obj: lib\list\list_length.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\list\list_length.c
build\bcc32\Debug\list_push.obj: lib\list\list_push.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\list\list_push.c
build\bcc32\Debug\list_remove.obj: lib\list\list_remove.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\list\list_remove.c
build\bcc32\Debug\list_swap.obj: lib\list\list_swap.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\list\list_swap.c
build\bcc32\Debug\list_unshift.obj: lib\list\list_unshift.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\list\list_unshift.c
build\bcc32\Debug\map_deinit.obj: lib\map\map_deinit.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\map\map_deinit.c
build\bcc32\Debug\map_get.obj: lib\map\map_get.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\map\map_get.c
build\bcc32\Debug\map_getref.obj: lib\map\map_getref.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\map\map_getref.c
build\bcc32\Debug\map_iter.obj: lib\map\map_iter.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\map\map_iter.c
build\bcc32\Debug\map_next.obj: lib\map\map_next.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\map\map_next.c
build\bcc32\Debug\map_remove.obj: lib\map\map_remove.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\map\map_remove.c
build\bcc32\Debug\map_set.obj: lib\map\map_set.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\map\map_set.c
build\bcc32\Debug\memalign.obj: lib\memalign.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\memalign.c
build\bcc32\Debug\memory.obj: lib\memory.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\memory.c
build\bcc32\Debug\mmap_map.obj: lib\mmap\mmap_map.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\mmap\mmap_map.c
build\bcc32\Debug\mmap_private.obj: lib\mmap\mmap_private.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\mmap\mmap_private.c
build\bcc32\Debug\mmap_private_fd.obj: lib\mmap\mmap_private_fd.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\mmap\mmap_private_fd.c
build\bcc32\Debug\mmap_read.obj: lib\mmap\mmap_read.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\mmap\mmap_read.c
build\bcc32\Debug\mmap_read_fd.obj: lib\mmap\mmap_read_fd.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\mmap\mmap_read_fd.c
build\bcc32\Debug\mmap_rw_fd.obj: lib\mmap\mmap_rw_fd.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\mmap\mmap_rw_fd.c
build\bcc32\Debug\mmap_shared.obj: lib\mmap\mmap_shared.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\mmap\mmap_shared.c
build\bcc32\Debug\mmap_shared_fd.obj: lib\mmap\mmap_shared_fd.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\mmap\mmap_shared_fd.c
build\bcc32\Debug\mmap_unmap.obj: lib\mmap\mmap_unmap.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\mmap\mmap_unmap.c
build\bcc32\Debug\ndelay_off.obj: lib\ndelay\ndelay_off.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\ndelay\ndelay_off.c
build\bcc32\Debug\ndelay_on.obj: lib\ndelay\ndelay_on.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\ndelay\ndelay_on.c
build\bcc32\Debug\openreadclose.obj: lib\open\openreadclose.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\open\openreadclose.c
build\bcc32\Debug\open_append.obj: lib\open\open_append.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\open\open_append.c
build\bcc32\Debug\open_excl.obj: lib\open\open_excl.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\open\open_excl.c
build\bcc32\Debug\open_read.obj: lib\open\open_read.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\open\open_read.c
build\bcc32\Debug\open_rw.obj: lib\open\open_rw.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\open\open_rw.c
build\bcc32\Debug\open_trunc.obj: lib\open\open_trunc.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\open\open_trunc.c
build\bcc32\Debug\open_write.obj: lib\open\open_write.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\open\open_write.c
build\bcc32\Debug\readclose.obj: lib\open\readclose.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\open\readclose.c
build\bcc32\Debug\path_absolute.obj: lib\path\path_absolute.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\path\path_absolute.c
build\bcc32\Debug\path_basename.obj: lib\path\path_basename.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\path\path_basename.c
build\bcc32\Debug\path_canonicalize.obj: lib\path\path_canonicalize.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\path\path_canonicalize.c
build\bcc32\Debug\path_dirname.obj: lib\path\path_dirname.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\path\path_dirname.c
build\bcc32\Debug\path_exists.obj: lib\path\path_exists.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\path\path_exists.c
build\bcc32\Debug\path_fnmatch.obj: lib\path\path_fnmatch.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\path\path_fnmatch.c
build\bcc32\Debug\path_getcwd.obj: lib\path\path_getcwd.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\path\path_getcwd.c
build\bcc32\Debug\path_gethome.obj: lib\path\path_gethome.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\path\path_gethome.c
build\bcc32\Debug\path_len.obj: lib\path\path_len.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\path\path_len.c
build\bcc32\Debug\path_len_s.obj: lib\path\path_len_s.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\path\path_len_s.c
build\bcc32\Debug\path_num.obj: lib\path\path_num.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\path\path_num.c
build\bcc32\Debug\path_num_sa.obj: lib\path\path_num_sa.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\path\path_num_sa.c
build\bcc32\Debug\path_readlink.obj: lib\path\path_readlink.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\path\path_readlink.c
build\bcc32\Debug\path_realpath.obj: lib\path\path_realpath.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\path\path_realpath.c
build\bcc32\Debug\path_right.obj: lib\path\path_right.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\path\path_right.c
build\bcc32\Debug\path_skip.obj: lib\path\path_skip.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\path\path_skip.c
build\bcc32\Debug\path_skips.obj: lib\path\path_skips.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\path\path_skips.c
build\bcc32\Debug\path_split.obj: lib\path\path_split.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\path\path_split.c
build\bcc32\Debug\pe_get_datadir.obj: lib\pe\pe_get_datadir.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\pe\pe_get_datadir.c
build\bcc32\Debug\pe_get_section.obj: lib\pe\pe_get_section.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\pe\pe_get_section.c
build\bcc32\Debug\pe_get_value.obj: lib\pe\pe_get_value.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\pe\pe_get_value.c
build\bcc32\Debug\pe_header.obj: lib\pe\pe_header.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\pe\pe_header.c
build\bcc32\Debug\pe_offset2rva.obj: lib\pe\pe_offset2rva.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\pe\pe_offset2rva.c
build\bcc32\Debug\pe_rva2offset.obj: lib\pe\pe_rva2offset.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\pe\pe_rva2offset.c
build\bcc32\Debug\pe_rva2ptr.obj: lib\pe\pe_rva2ptr.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\pe\pe_rva2ptr.c
build\bcc32\Debug\pe_thunk.obj: lib\pe\pe_thunk.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\pe\pe_thunk.c
build\bcc32\Debug\pipe2.obj: lib\pipe2.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\pipe2.c
build\bcc32\Debug\playlist_init.obj: lib\playlist\playlist_init.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\playlist\playlist_init.c
build\bcc32\Debug\playlist_m3u.obj: lib\playlist\playlist_m3u.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\playlist\playlist_m3u.c
build\bcc32\Debug\playlist_pls.obj: lib\playlist\playlist_pls.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\playlist\playlist_pls.c
build\bcc32\Debug\playlist_read.obj: lib\playlist\playlist_read.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\playlist\playlist_read.c
build\bcc32\Debug\playlist_write_entry.obj: lib\playlist\playlist_write_entry.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\playlist\playlist_write_entry.c
build\bcc32\Debug\playlist_write_finish.obj: lib\playlist\playlist_write_finish.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\playlist\playlist_write_finish.c
build\bcc32\Debug\playlist_write_start.obj: lib\playlist\playlist_write_start.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\playlist\playlist_write_start.c
build\bcc32\Debug\playlist_xspf.obj: lib\playlist\playlist_xspf.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\playlist\playlist_xspf.c
build\bcc32\Debug\popen.obj: lib\popen.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\popen.c
build\bcc32\Debug\rdir_close.obj: lib\rdir\rdir_close.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\rdir\rdir_close.c
build\bcc32\Debug\rdir_open.obj: lib\rdir\rdir_open.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\rdir\rdir_open.c
build\bcc32\Debug\rdir_read.obj: lib\rdir\rdir_read.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\rdir\rdir_read.c
build\bcc32\Debug\readlink.obj: lib\readlink.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\readlink.c
build\bcc32\Debug\round.obj: lib\round.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\round.c
build\bcc32\Debug\scan_8int.obj: lib\scan\scan_8int.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\scan\scan_8int.c
build\bcc32\Debug\scan_8long.obj: lib\scan\scan_8long.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\scan\scan_8long.c
build\bcc32\Debug\scan_octal.obj: lib\scan\scan_octal.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\scan\scan_octal.c
build\bcc32\Debug\scan_8longn.obj: lib\scan\scan_8longn.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\scan\scan_8longn.c
build\bcc32\Debug\scan_8short.obj: lib\scan\scan_8short.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\scan\scan_8short.c
build\bcc32\Debug\scan_asn1derlength.obj: lib\scan\scan_asn1derlength.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\scan\scan_asn1derlength.c
build\bcc32\Debug\scan_asn1dertag.obj: lib\scan\scan_asn1dertag.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\scan\scan_asn1dertag.c
build\bcc32\Debug\scan_charsetnskip.obj: lib\scan\scan_charsetnskip.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\scan\scan_charsetnskip.c
build\bcc32\Debug\scan_double.obj: lib\scan\scan_double.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\scan\scan_double.c
build\bcc32\Debug\scan_fromhex.obj: lib\scan\scan_fromhex.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\scan\scan_fromhex.c
build\bcc32\Debug\scan_httpdate.obj: lib\scan\scan_httpdate.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\scan\scan_httpdate.c
build\bcc32\Debug\scan_int.obj: lib\scan\scan_int.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\scan\scan_int.c
build\bcc32\Debug\scan_long.obj: lib\scan\scan_long.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\scan\scan_long.c
build\bcc32\Debug\scan_longlong.obj: lib\scan\scan_longlong.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\scan\scan_longlong.c
build\bcc32\Debug\scan_longn.obj: lib\scan\scan_longn.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\scan\scan_longn.c
build\bcc32\Debug\scan_netstring.obj: lib\scan\scan_netstring.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\scan\scan_netstring.c
build\bcc32\Debug\scan_noncharsetnskip.obj: lib\scan\scan_noncharsetnskip.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\scan\scan_noncharsetnskip.c
build\bcc32\Debug\scan_nonwhitenskip.obj: lib\scan\scan_nonwhitenskip.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\scan\scan_nonwhitenskip.c
build\bcc32\Debug\scan_pb_tag.obj: lib\scan\scan_pb_tag.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\scan\scan_pb_tag.c
build\bcc32\Debug\scan_pb_type0_sint.obj: lib\scan\scan_pb_type0_sint.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\scan\scan_pb_type0_sint.c
build\bcc32\Debug\scan_pb_type1_fixed64.obj: lib\scan\scan_pb_type1_fixed64.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\scan\scan_pb_type1_fixed64.c
build\bcc32\Debug\scan_pb_type5_fixed32.obj: lib\scan\scan_pb_type5_fixed32.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\scan\scan_pb_type5_fixed32.c
build\bcc32\Debug\scan_plusminus.obj: lib\scan\scan_plusminus.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\scan\scan_plusminus.c
build\bcc32\Debug\scan_short.obj: lib\scan\scan_short.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\scan\scan_short.c
build\bcc32\Debug\scan_uint.obj: lib\scan\scan_uint.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\scan\scan_uint.c
build\bcc32\Debug\scan_ulong.obj: lib\scan\scan_ulong.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\scan\scan_ulong.c
build\bcc32\Debug\scan_ulonglong.obj: lib\scan\scan_ulonglong.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\scan\scan_ulonglong.c
build\bcc32\Debug\scan_ulongn.obj: lib\scan\scan_ulongn.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\scan\scan_ulongn.c
build\bcc32\Debug\scan_ushort.obj: lib\scan\scan_ushort.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\scan\scan_ushort.c
build\bcc32\Debug\scan_utf8.obj: lib\scan\scan_utf8.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\scan\scan_utf8.c
build\bcc32\Debug\scan_varint.obj: lib\scan\scan_varint.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\scan\scan_varint.c
build\bcc32\Debug\scan_whitenskip.obj: lib\scan\scan_whitenskip.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\scan\scan_whitenskip.c
build\bcc32\Debug\scan_xint.obj: lib\scan\scan_xint.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\scan\scan_xint.c
build\bcc32\Debug\scan_xlong.obj: lib\scan\scan_xlong.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\scan\scan_xlong.c
build\bcc32\Debug\scan_xlonglong.obj: lib\scan\scan_xlonglong.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\scan\scan_xlonglong.c
build\bcc32\Debug\scan_xlongn.obj: lib\scan\scan_xlongn.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\scan\scan_xlongn.c
build\bcc32\Debug\scan_xmlescape.obj: lib\scan\scan_xmlescape.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\scan\scan_xmlescape.c
build\bcc32\Debug\scan_xshort.obj: lib\scan\scan_xshort.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\scan\scan_xshort.c
build\bcc32\Debug\setenv.obj: lib\setenv.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\setenv.c
build\bcc32\Debug\sig_block.obj: lib\sig\sig_block.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\sig\sig_block.c
build\bcc32\Debug\sig_unblock.obj: lib\sig\sig_unblock.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\sig\sig_unblock.c
build\bcc32\Debug\slist_add_after.obj: lib\slist\slist_add_after.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\slist\slist_add_after.c
build\bcc32\Debug\slist_add_before.obj: lib\slist\slist_add_before.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\slist\slist_add_before.c
build\bcc32\Debug\slist_find.obj: lib\slist\slist_find.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\slist\slist_find.c
build\bcc32\Debug\slist_find_remove.obj: lib\slist\slist_find_remove.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\slist\slist_find_remove.c
build\bcc32\Debug\slist_init.obj: lib\slist\slist_init.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\slist\slist_init.c
build\bcc32\Debug\slist_length.obj: lib\slist\slist_length.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\slist\slist_length.c
build\bcc32\Debug\slist_move_head.obj: lib\slist\slist_move_head.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\slist\slist_move_head.c
build\bcc32\Debug\slist_move_tail.obj: lib\slist\slist_move_tail.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\slist\slist_move_tail.c
build\bcc32\Debug\slist_push.obj: lib\slist\slist_push.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\slist\slist_push.c
build\bcc32\Debug\slist_pushs.obj: lib\slist\slist_pushs.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\slist\slist_pushs.c
build\bcc32\Debug\slist_remove.obj: lib\slist\slist_remove.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\slist\slist_remove.c
build\bcc32\Debug\slist_unshift.obj: lib\slist\slist_unshift.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\slist\slist_unshift.c
build\bcc32\Debug\fmt_ip4.obj: lib\socket\fmt_ip4.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\socket\fmt_ip4.c
build\bcc32\Debug\fmt_ip6.obj: lib\socket\fmt_ip6.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\socket\fmt_ip6.c
build\bcc32\Debug\fmt_ip6c.obj: lib\socket\fmt_ip6c.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\socket\fmt_ip6c.c
build\bcc32\Debug\fmt_ip6if.obj: lib\socket\fmt_ip6if.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\socket\fmt_ip6if.c
build\bcc32\Debug\fmt_ip6ifc.obj: lib\socket\fmt_ip6ifc.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\socket\fmt_ip6ifc.c
build\bcc32\Debug\fmt_ip6_flat.obj: lib\socket\fmt_ip6_flat.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\socket\fmt_ip6_flat.c
build\bcc32\Debug\init.obj: lib\socket\init.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\socket\init.c
build\bcc32\Debug\scan_ip4.obj: lib\socket\scan_ip4.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\socket\scan_ip4.c
build\bcc32\Debug\scan_ip6.obj: lib\socket\scan_ip6.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\socket\scan_ip6.c
build\bcc32\Debug\scan_ip6if.obj: lib\socket\scan_ip6if.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\socket\scan_ip6if.c
build\bcc32\Debug\scan_ip6_flat.obj: lib\socket\scan_ip6_flat.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\socket\scan_ip6_flat.c
build\bcc32\Debug\socket_accept4.obj: lib\socket\socket_accept4.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\socket\socket_accept4.c
build\bcc32\Debug\socket_accept6.obj: lib\socket\socket_accept6.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\socket\socket_accept6.c
build\bcc32\Debug\socket_bind4.obj: lib\socket\socket_bind4.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\socket\socket_bind4.c
build\bcc32\Debug\socket_bind4_reuse.obj: lib\socket\socket_bind4_reuse.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\socket\socket_bind4_reuse.c
build\bcc32\Debug\socket_bind6.obj: lib\socket\socket_bind6.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\socket\socket_bind6.c
build\bcc32\Debug\socket_bind6_reuse.obj: lib\socket\socket_bind6_reuse.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\socket\socket_bind6_reuse.c
build\bcc32\Debug\socket_broadcast.obj: lib\socket\socket_broadcast.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\socket\socket_broadcast.c
build\bcc32\Debug\socket_connect4.obj: lib\socket\socket_connect4.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\socket\socket_connect4.c
build\bcc32\Debug\socket_connect6.obj: lib\socket\socket_connect6.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\socket\socket_connect6.c
build\bcc32\Debug\socket_connected.obj: lib\socket\socket_connected.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\socket\socket_connected.c
build\bcc32\Debug\socket_deferaccept.obj: lib\socket\socket_deferaccept.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\socket\socket_deferaccept.c
build\bcc32\Debug\socket_fastopen.obj: lib\socket\socket_fastopen.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\socket\socket_fastopen.c
build\bcc32\Debug\socket_fastopen_connect4.obj: lib\socket\socket_fastopen_connect4.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\socket\socket_fastopen_connect4.c
build\bcc32\Debug\socket_fastopen_connect6.obj: lib\socket\socket_fastopen_connect6.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\socket\socket_fastopen_connect6.c
build\bcc32\Debug\socket_getifidx.obj: lib\socket\socket_getifidx.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\socket\socket_getifidx.c
build\bcc32\Debug\socket_getifname.obj: lib\socket\socket_getifname.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\socket\socket_getifname.c
build\bcc32\Debug\socket_ip4loopback.obj: lib\socket\socket_ip4loopback.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\socket\socket_ip4loopback.c
build\bcc32\Debug\socket_listen.obj: lib\socket\socket_listen.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\socket\socket_listen.c
build\bcc32\Debug\socket_local4.obj: lib\socket\socket_local4.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\socket\socket_local4.c
build\bcc32\Debug\socket_local6.obj: lib\socket\socket_local6.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\socket\socket_local6.c
build\bcc32\Debug\socket_mchopcount6.obj: lib\socket\socket_mchopcount6.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\socket\socket_mchopcount6.c
build\bcc32\Debug\socket_mcjoin4.obj: lib\socket\socket_mcjoin4.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\socket\socket_mcjoin4.c
build\bcc32\Debug\socket_mcjoin6.obj: lib\socket\socket_mcjoin6.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\socket\socket_mcjoin6.c
build\bcc32\Debug\socket_mcleave4.obj: lib\socket\socket_mcleave4.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\socket\socket_mcleave4.c
build\bcc32\Debug\socket_mcleave6.obj: lib\socket\socket_mcleave6.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\socket\socket_mcleave6.c
build\bcc32\Debug\socket_mcloop4.obj: lib\socket\socket_mcloop4.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\socket\socket_mcloop4.c
build\bcc32\Debug\socket_mcloop6.obj: lib\socket\socket_mcloop6.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\socket\socket_mcloop6.c
build\bcc32\Debug\socket_mcttl4.obj: lib\socket\socket_mcttl4.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\socket\socket_mcttl4.c
build\bcc32\Debug\socket_noipv6.obj: lib\socket\socket_noipv6.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\socket\socket_noipv6.c
build\bcc32\Debug\socket_quickack.obj: lib\socket\socket_quickack.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\socket\socket_quickack.c
build\bcc32\Debug\socket_recv4.obj: lib\socket\socket_recv4.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\socket\socket_recv4.c
build\bcc32\Debug\socket_recv6.obj: lib\socket\socket_recv6.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\socket\socket_recv6.c
build\bcc32\Debug\socket_remote4.obj: lib\socket\socket_remote4.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\socket\socket_remote4.c
build\bcc32\Debug\socket_remote6.obj: lib\socket\socket_remote6.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\socket\socket_remote6.c
build\bcc32\Debug\socket_sctp4.obj: lib\socket\socket_sctp4.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\socket\socket_sctp4.c
build\bcc32\Debug\socket_sctp4b.obj: lib\socket\socket_sctp4b.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\socket\socket_sctp4b.c
build\bcc32\Debug\socket_sctp6.obj: lib\socket\socket_sctp6.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\socket\socket_sctp6.c
build\bcc32\Debug\socket_sctp6b.obj: lib\socket\socket_sctp6b.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\socket\socket_sctp6b.c
build\bcc32\Debug\socket_send4.obj: lib\socket\socket_send4.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\socket\socket_send4.c
build\bcc32\Debug\socket_send6.obj: lib\socket\socket_send6.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\socket\socket_send6.c
build\bcc32\Debug\socket_tcp4.obj: lib\socket\socket_tcp4.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\socket\socket_tcp4.c
build\bcc32\Debug\socket_tcp4b.obj: lib\socket\socket_tcp4b.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\socket\socket_tcp4b.c
build\bcc32\Debug\socket_tcp6.obj: lib\socket\socket_tcp6.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\socket\socket_tcp6.c
build\bcc32\Debug\socket_tcp6b.obj: lib\socket\socket_tcp6b.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\socket\socket_tcp6b.c
build\bcc32\Debug\socket_tryreservein.obj: lib\socket\socket_tryreservein.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\socket\socket_tryreservein.c
build\bcc32\Debug\socket_udp4.obj: lib\socket\socket_udp4.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\socket\socket_udp4.c
build\bcc32\Debug\socket_udp6.obj: lib\socket\socket_udp6.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\socket\socket_udp6.c
build\bcc32\Debug\socket_v4mappedprefix.obj: lib\socket\socket_v4mappedprefix.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\socket\socket_v4mappedprefix.c
build\bcc32\Debug\socket_v6any.obj: lib\socket\socket_v6any.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\socket\socket_v6any.c
build\bcc32\Debug\socket_v6loopback.obj: lib\socket\socket_v6loopback.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\socket\socket_v6loopback.c
build\bcc32\Debug\winsock2errno.obj: lib\socket\winsock2errno.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\socket\winsock2errno.c
build\bcc32\Debug\winsock_init.obj: lib\socket\winsock_init.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\socket\winsock_init.c
build\bcc32\Debug\winsock_socketpair.obj: lib\socket\winsock_socketpair.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\socket\winsock_socketpair.c
build\bcc32\Debug\str_basename.obj: lib\str\str_basename.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\str\str_basename.c
build\bcc32\Debug\str_case_diff.obj: lib\str\str_case_diff.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\str\str_case_diff.c
build\bcc32\Debug\str_case_diffn.obj: lib\str\str_case_diffn.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\str\str_case_diffn.c
build\bcc32\Debug\str_case_equal.obj: lib\str\str_case_equal.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\str\str_case_equal.c
build\bcc32\Debug\str_case_start.obj: lib\str\str_case_start.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\str\str_case_start.c
build\bcc32\Debug\str_cat.obj: lib\str\str_cat.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\str\str_cat.c
build\bcc32\Debug\str_chr.obj: lib\str\str_chr.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\str\str_chr.c
build\bcc32\Debug\str_copy.obj: lib\str\str_copy.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\str\str_copy.c
build\bcc32\Debug\str_copyb.obj: lib\str\str_copyb.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\str\str_copyb.c
build\bcc32\Debug\str_copyn.obj: lib\str\str_copyn.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\str\str_copyn.c
build\bcc32\Debug\str_decamelize.obj: lib\str\str_decamelize.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\str\str_decamelize.c
build\bcc32\Debug\str_diff.obj: lib\str\str_diff.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\str\str_diff.c
build\bcc32\Debug\str_diffn.obj: lib\str\str_diffn.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\str\str_diffn.c
build\bcc32\Debug\str_dup.obj: lib\str\str_dup.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\str\str_dup.c
build\bcc32\Debug\str_equal.obj: lib\str\str_equal.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\str\str_equal.c
build\bcc32\Debug\str_find.obj: lib\str\str_find.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\str\str_find.c
build\bcc32\Debug\str_findb.obj: lib\str\str_findb.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\str\str_findb.c
build\bcc32\Debug\str_istr.obj: lib\str\str_istr.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\str\str_istr.c
build\bcc32\Debug\str_len.obj: lib\str\str_len.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\str\str_len.c
build\bcc32\Debug\str_lower.obj: lib\str\str_lower.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\str\str_lower.c
build\bcc32\Debug\str_ndup.obj: lib\str\str_ndup.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\str\str_ndup.c
build\bcc32\Debug\str_ptime.obj: lib\str\str_ptime.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\str\str_ptime.c
build\bcc32\Debug\str_rchr.obj: lib\str\str_rchr.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\str\str_rchr.c
build\bcc32\Debug\str_rchrs.obj: lib\str\str_rchrs.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\str\str_rchrs.c
build\bcc32\Debug\str_start.obj: lib\str\str_start.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\str\str_start.c
build\bcc32\Debug\str_tok.obj: lib\str\str_tok.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\str\str_tok.c
build\bcc32\Debug\mmap_filename.obj: lib\stralloc\mmap_filename.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\stralloc\mmap_filename.c
build\bcc32\Debug\stralloc_0.obj: lib\stralloc\stralloc_0.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\stralloc\stralloc_0.c
build\bcc32\Debug\stralloc_append.obj: lib\stralloc\stralloc_append.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\stralloc\stralloc_append.c
build\bcc32\Debug\stralloc_buffer.obj: lib\stralloc\stralloc_buffer.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\stralloc\stralloc_buffer.c
build\bcc32\Debug\stralloc_case_diff.obj: lib\stralloc\stralloc_case_diff.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\stralloc\stralloc_case_diff.c
build\bcc32\Debug\stralloc_case_diffs.obj: lib\stralloc\stralloc_case_diffs.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\stralloc\stralloc_case_diffs.c
build\bcc32\Debug\stralloc_case_end.obj: lib\stralloc\stralloc_case_end.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\stralloc\stralloc_case_end.c
build\bcc32\Debug\stralloc_case_equal.obj: lib\stralloc\stralloc_case_equal.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\stralloc\stralloc_case_equal.c
build\bcc32\Debug\stralloc_case_equals.obj: lib\stralloc\stralloc_case_equals.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\stralloc\stralloc_case_equals.c
build\bcc32\Debug\stralloc_case_start.obj: lib\stralloc\stralloc_case_start.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\stralloc\stralloc_case_start.c
build\bcc32\Debug\stralloc_case_starts.obj: lib\stralloc\stralloc_case_starts.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\stralloc\stralloc_case_starts.c
build\bcc32\Debug\stralloc_cat.obj: lib\stralloc\stralloc_cat.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\stralloc\stralloc_cat.c
build\bcc32\Debug\stralloc_catb.obj: lib\stralloc\stralloc_catb.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\stralloc\stralloc_catb.c
build\bcc32\Debug\stralloc_catc.obj: lib\stralloc\stralloc_catc.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\stralloc\stralloc_catc.c
build\bcc32\Debug\stralloc_catdouble.obj: lib\stralloc\stralloc_catdouble.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\stralloc\stralloc_catdouble.c
build\bcc32\Debug\stralloc_cathexb.obj: lib\stralloc\stralloc_cathexb.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\stralloc\stralloc_cathexb.c
build\bcc32\Debug\stralloc_catint.obj: lib\stralloc\stralloc_catint.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\stralloc\stralloc_catint.c
build\bcc32\Debug\stralloc_catint0.obj: lib\stralloc\stralloc_catint0.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\stralloc\stralloc_catint0.c
build\bcc32\Debug\stralloc_catlong.obj: lib\stralloc\stralloc_catlong.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\stralloc\stralloc_catlong.c
build\bcc32\Debug\stralloc_catlong0.obj: lib\stralloc\stralloc_catlong0.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\stralloc\stralloc_catlong0.c
build\bcc32\Debug\stralloc_catm_internal.obj: lib\stralloc\stralloc_catm_internal.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\stralloc\stralloc_catm_internal.c
build\bcc32\Debug\stralloc_cats.obj: lib\stralloc\stralloc_cats.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\stralloc\stralloc_cats.c
build\bcc32\Debug\stralloc_catuint.obj: lib\stralloc\stralloc_catuint.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\stralloc\stralloc_catuint.c
build\bcc32\Debug\stralloc_catuint0.obj: lib\stralloc\stralloc_catuint0.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\stralloc\stralloc_catuint0.c
build\bcc32\Debug\stralloc_catulong.obj: lib\stralloc\stralloc_catulong.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\stralloc\stralloc_catulong.c
build\bcc32\Debug\stralloc_catulong0.obj: lib\stralloc\stralloc_catulong0.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\stralloc\stralloc_catulong0.c
build\bcc32\Debug\stralloc_catxlong.obj: lib\stralloc\stralloc_catxlong.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\stralloc\stralloc_catxlong.c
build\bcc32\Debug\stralloc_chomp.obj: lib\stralloc\stralloc_chomp.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\stralloc\stralloc_chomp.c
build\bcc32\Debug\stralloc_chop.obj: lib\stralloc\stralloc_chop.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\stralloc\stralloc_chop.c
build\bcc32\Debug\stralloc_chr.obj: lib\stralloc\stralloc_chr.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\stralloc\stralloc_chr.c
build\bcc32\Debug\stralloc_copy.obj: lib\stralloc\stralloc_copy.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\stralloc\stralloc_copy.c
build\bcc32\Debug\stralloc_copyb.obj: lib\stralloc\stralloc_copyb.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\stralloc\stralloc_copyb.c
build\bcc32\Debug\stralloc_copys.obj: lib\stralloc\stralloc_copys.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\stralloc\stralloc_copys.c
build\bcc32\Debug\stralloc_copywcb.obj: lib\stralloc\stralloc_copywcb.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\stralloc\stralloc_copywcb.c
build\bcc32\Debug\stralloc_copywcs.obj: lib\stralloc\stralloc_copywcs.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\stralloc\stralloc_copywcs.c
build\bcc32\Debug\stralloc_count.obj: lib\stralloc\stralloc_count.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\stralloc\stralloc_count.c
build\bcc32\Debug\stralloc_decamelize.obj: lib\stralloc\stralloc_decamelize.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\stralloc\stralloc_decamelize.c
build\bcc32\Debug\stralloc_diff.obj: lib\stralloc\stralloc_diff.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\stralloc\stralloc_diff.c
build\bcc32\Debug\stralloc_diffb.obj: lib\stralloc\stralloc_diffb.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\stralloc\stralloc_diffb.c
build\bcc32\Debug\stralloc_diffs.obj: lib\stralloc\stralloc_diffs.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\stralloc\stralloc_diffs.c
build\bcc32\Debug\stralloc_endb.obj: lib\stralloc\stralloc_endb.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\stralloc\stralloc_endb.c
build\bcc32\Debug\stralloc_ends.obj: lib\stralloc\stralloc_ends.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\stralloc\stralloc_ends.c
build\bcc32\Debug\stralloc_equal.obj: lib\stralloc\stralloc_equal.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\stralloc\stralloc_equal.c
build\bcc32\Debug\stralloc_equalb.obj: lib\stralloc\stralloc_equalb.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\stralloc\stralloc_equalb.c
build\bcc32\Debug\stralloc_equals.obj: lib\stralloc\stralloc_equals.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\stralloc\stralloc_equals.c
build\bcc32\Debug\stralloc_erase.obj: lib\stralloc\stralloc_erase.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\stralloc\stralloc_erase.c
build\bcc32\Debug\stralloc_expand.obj: lib\stralloc\stralloc_expand.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\stralloc\stralloc_expand.c
build\bcc32\Debug\stralloc_find.obj: lib\stralloc\stralloc_find.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\stralloc\stralloc_find.c
build\bcc32\Debug\stralloc_findb.obj: lib\stralloc\stralloc_findb.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\stralloc\stralloc_findb.c
build\bcc32\Debug\stralloc_finds.obj: lib\stralloc\stralloc_finds.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\stralloc\stralloc_finds.c
build\bcc32\Debug\stralloc_fmt.obj: lib\stralloc\stralloc_fmt.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\stralloc\stralloc_fmt.c
build\bcc32\Debug\stralloc_fmt_call.obj: lib\stralloc\stralloc_fmt_call.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\stralloc\stralloc_fmt_call.c
build\bcc32\Debug\stralloc_fmt_pred.obj: lib\stralloc\stralloc_fmt_pred.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\stralloc\stralloc_fmt_pred.c
build\bcc32\Debug\stralloc_free.obj: lib\stralloc\stralloc_free.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\stralloc\stralloc_free.c
build\bcc32\Debug\stralloc_init.obj: lib\stralloc\stralloc_init.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\stralloc\stralloc_init.c
build\bcc32\Debug\stralloc_insert.obj: lib\stralloc\stralloc_insert.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\stralloc\stralloc_insert.c
build\bcc32\Debug\stralloc_insertb.obj: lib\stralloc\stralloc_insertb.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\stralloc\stralloc_insertb.c
build\bcc32\Debug\stralloc_inserts.obj: lib\stralloc\stralloc_inserts.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\stralloc\stralloc_inserts.c
build\bcc32\Debug\stralloc_lower.obj: lib\stralloc\stralloc_lower.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\stralloc\stralloc_lower.c
build\bcc32\Debug\stralloc_move.obj: lib\stralloc\stralloc_move.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\stralloc\stralloc_move.c
build\bcc32\Debug\stralloc_nul.obj: lib\stralloc\stralloc_nul.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\stralloc\stralloc_nul.c
build\bcc32\Debug\stralloc_prepend.obj: lib\stralloc\stralloc_prepend.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\stralloc\stralloc_prepend.c
build\bcc32\Debug\stralloc_prependb.obj: lib\stralloc\stralloc_prependb.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\stralloc\stralloc_prependb.c
build\bcc32\Debug\stralloc_prepends.obj: lib\stralloc\stralloc_prepends.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\stralloc\stralloc_prepends.c
build\bcc32\Debug\stralloc_rchr.obj: lib\stralloc\stralloc_rchr.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\stralloc\stralloc_rchr.c
build\bcc32\Debug\stralloc_ready.obj: lib\stralloc\stralloc_ready.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\stralloc\stralloc_ready.c
build\bcc32\Debug\stralloc_readyplus.obj: lib\stralloc\stralloc_readyplus.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\stralloc\stralloc_readyplus.c
build\bcc32\Debug\stralloc_ready_tuned.obj: lib\stralloc\stralloc_ready_tuned.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\stralloc\stralloc_ready_tuned.c
build\bcc32\Debug\stralloc_remove.obj: lib\stralloc\stralloc_remove.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\stralloc\stralloc_remove.c
build\bcc32\Debug\stralloc_remove_all.obj: lib\stralloc\stralloc_remove_all.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\stralloc\stralloc_remove_all.c
build\bcc32\Debug\stralloc_replace.obj: lib\stralloc\stralloc_replace.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\stralloc\stralloc_replace.c
build\bcc32\Debug\stralloc_replace_non_printable.obj: lib\stralloc\stralloc_replace_non_printable.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\stralloc\stralloc_replace_non_printable.c
build\bcc32\Debug\stralloc_reverse.obj: lib\stralloc\stralloc_reverse.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\stralloc\stralloc_reverse.c
build\bcc32\Debug\stralloc_reverse_blocks.obj: lib\stralloc\stralloc_reverse_blocks.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\stralloc\stralloc_reverse_blocks.c
build\bcc32\Debug\stralloc_scan.obj: lib\stralloc\stralloc_scan.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\stralloc\stralloc_scan.c
build\bcc32\Debug\stralloc_shrink.obj: lib\stralloc\stralloc_shrink.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\stralloc\stralloc_shrink.c
build\bcc32\Debug\stralloc_start.obj: lib\stralloc\stralloc_start.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\stralloc\stralloc_start.c
build\bcc32\Debug\stralloc_startb.obj: lib\stralloc\stralloc_startb.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\stralloc\stralloc_startb.c
build\bcc32\Debug\stralloc_starts.obj: lib\stralloc\stralloc_starts.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\stralloc\stralloc_starts.c
build\bcc32\Debug\stralloc_subst.obj: lib\stralloc\stralloc_subst.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\stralloc\stralloc_subst.c
build\bcc32\Debug\stralloc_trim.obj: lib\stralloc\stralloc_trim.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\stralloc\stralloc_trim.c
build\bcc32\Debug\stralloc_trunc.obj: lib\stralloc\stralloc_trunc.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\stralloc\stralloc_trunc.c
build\bcc32\Debug\stralloc_upper.obj: lib\stralloc\stralloc_upper.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\stralloc\stralloc_upper.c
build\bcc32\Debug\stralloc_write.obj: lib\stralloc\stralloc_write.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\stralloc\stralloc_write.c
build\bcc32\Debug\stralloc_zero.obj: lib\stralloc\stralloc_zero.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\stralloc\stralloc_zero.c
build\bcc32\Debug\strarray_appends.obj: lib\strarray\strarray_appends.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\strarray\strarray_appends.c
build\bcc32\Debug\strarray_from_argv.obj: lib\strarray\strarray_from_argv.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\strarray\strarray_from_argv.c
build\bcc32\Debug\strarray_index_of.obj: lib\strarray\strarray_index_of.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\strarray\strarray_index_of.c
build\bcc32\Debug\strarray_joins.obj: lib\strarray\strarray_joins.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\strarray\strarray_joins.c
build\bcc32\Debug\strarray_pop.obj: lib\strarray\strarray_pop.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\strarray\strarray_pop.c
build\bcc32\Debug\strarray_prepends.obj: lib\strarray\strarray_prepends.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\strarray\strarray_prepends.c
build\bcc32\Debug\strarray_push.obj: lib\strarray\strarray_push.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\strarray\strarray_push.c
build\bcc32\Debug\strarray_pushb.obj: lib\strarray\strarray_pushb.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\strarray\strarray_pushb.c
build\bcc32\Debug\strarray_pushd.obj: lib\strarray\strarray_pushd.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\strarray\strarray_pushd.c
build\bcc32\Debug\strarray_push_sa.obj: lib\strarray\strarray_push_sa.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\strarray\strarray_push_sa.c
build\bcc32\Debug\strarray_removeprefixs.obj: lib\strarray\strarray_removeprefixs.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\strarray\strarray_removeprefixs.c
build\bcc32\Debug\strarray_removesuffixs.obj: lib\strarray\strarray_removesuffixs.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\strarray\strarray_removesuffixs.c
build\bcc32\Debug\strarray_set.obj: lib\strarray\strarray_set.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\strarray\strarray_set.c
build\bcc32\Debug\strarray_sort.obj: lib\strarray\strarray_sort.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\strarray\strarray_sort.c
build\bcc32\Debug\strarray_to_argv.obj: lib\strarray\strarray_to_argv.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\strarray\strarray_to_argv.c
build\bcc32\Debug\strarray_transform.obj: lib\strarray\strarray_transform.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\strarray\strarray_transform.c
build\bcc32\Debug\strlist_append_sa.obj: lib\strlist\strlist_append_sa.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\strlist\strlist_append_sa.c
build\bcc32\Debug\strlist_at.obj: lib\strlist\strlist_at.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\strlist\strlist_at.c
build\bcc32\Debug\strlist_at_n.obj: lib\strlist\strlist_at_n.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\strlist\strlist_at_n.c
build\bcc32\Debug\strlist_at_sa.obj: lib\strlist\strlist_at_sa.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\strlist\strlist_at_sa.c
build\bcc32\Debug\strlist_cat.obj: lib\strlist\strlist_cat.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\strlist\strlist_cat.c
build\bcc32\Debug\strlist_contains.obj: lib\strlist\strlist_contains.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\strlist\strlist_contains.c
build\bcc32\Debug\strlist_contains_sa.obj: lib\strlist\strlist_contains_sa.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\strlist\strlist_contains_sa.c
build\bcc32\Debug\strlist_count.obj: lib\strlist\strlist_count.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\strlist\strlist_count.c
build\bcc32\Debug\strlist_dump.obj: lib\strlist\strlist_dump.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\strlist\strlist_dump.c
build\bcc32\Debug\strlist_froms.obj: lib\strlist\strlist_froms.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\strlist\strlist_froms.c
build\bcc32\Debug\strlist_index_of.obj: lib\strlist\strlist_index_of.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\strlist\strlist_index_of.c
build\bcc32\Debug\strlist_join.obj: lib\strlist\strlist_join.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\strlist\strlist_join.c
build\bcc32\Debug\strlist_joins.obj: lib\strlist\strlist_joins.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\strlist\strlist_joins.c
build\bcc32\Debug\strlist_prependb.obj: lib\strlist\strlist_prependb.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\strlist\strlist_prependb.c
build\bcc32\Debug\strlist_prepends.obj: lib\strlist\strlist_prepends.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\strlist\strlist_prepends.c
build\bcc32\Debug\strlist_prepend_sa.obj: lib\strlist\strlist_prepend_sa.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\strlist\strlist_prepend_sa.c
build\bcc32\Debug\strlist_push.obj: lib\strlist\strlist_push.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\strlist\strlist_push.c
build\bcc32\Debug\strlist_pushb.obj: lib\strlist\strlist_pushb.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\strlist\strlist_pushb.c
build\bcc32\Debug\strlist_pushm_internal.obj: lib\strlist\strlist_pushm_internal.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\strlist\strlist_pushm_internal.c
build\bcc32\Debug\strlist_push_sa.obj: lib\strlist\strlist_push_sa.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\strlist\strlist_push_sa.c
build\bcc32\Debug\strlist_push_tokens.obj: lib\strlist\strlist_push_tokens.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\strlist\strlist_push_tokens.c
build\bcc32\Debug\strlist_push_unique.obj: lib\strlist\strlist_push_unique.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\strlist\strlist_push_unique.c
build\bcc32\Debug\strlist_push_unique_sa.obj: lib\strlist\strlist_push_unique_sa.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\strlist\strlist_push_unique_sa.c
build\bcc32\Debug\strlist_range.obj: lib\strlist\strlist_range.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\strlist\strlist_range.c
build\bcc32\Debug\strlist_shift.obj: lib\strlist\strlist_shift.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\strlist\strlist_shift.c
build\bcc32\Debug\strlist_shift_n.obj: lib\strlist\strlist_shift_n.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\strlist\strlist_shift_n.c
build\bcc32\Debug\strlist_sort.obj: lib\strlist\strlist_sort.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\strlist\strlist_sort.c
build\bcc32\Debug\strlist_to_argv.obj: lib\strlist\strlist_to_argv.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\strlist\strlist_to_argv.c
build\bcc32\Debug\strlist_unshift.obj: lib\strlist\strlist_unshift.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\strlist\strlist_unshift.c
build\bcc32\Debug\strptime.obj: lib\strptime.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\strptime.c
build\bcc32\Debug\symlink.obj: lib\symlink.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\symlink.c
build\bcc32\Debug\tai_add.obj: lib\tai\tai_add.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\tai\tai_add.c
build\bcc32\Debug\tai_now.obj: lib\tai\tai_now.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\tai\tai_now.c
build\bcc32\Debug\tai_pack.obj: lib\tai\tai_pack.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\tai\tai_pack.c
build\bcc32\Debug\tai_sub.obj: lib\tai\tai_sub.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\tai\tai_sub.c
build\bcc32\Debug\tai_uint.obj: lib\tai\tai_uint.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\tai\tai_uint.c
build\bcc32\Debug\tai_unpack.obj: lib\tai\tai_unpack.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\tai\tai_unpack.c
build\bcc32\Debug\taia_add.obj: lib\taia\taia_add.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\taia\taia_add.c
build\bcc32\Debug\taia_addsec.obj: lib\taia\taia_addsec.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\taia\taia_addsec.c
build\bcc32\Debug\taia_approx.obj: lib\taia\taia_approx.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\taia\taia_approx.c
build\bcc32\Debug\taia_frac.obj: lib\taia\taia_frac.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\taia\taia_frac.c
build\bcc32\Debug\taia_half.obj: lib\taia\taia_half.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\taia\taia_half.c
build\bcc32\Debug\taia_less.obj: lib\taia\taia_less.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\taia\taia_less.c
build\bcc32\Debug\taia_now.obj: lib\taia\taia_now.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\taia\taia_now.c
build\bcc32\Debug\taia_pack.obj: lib\taia\taia_pack.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\taia\taia_pack.c
build\bcc32\Debug\taia_sub.obj: lib\taia\taia_sub.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\taia\taia_sub.c
build\bcc32\Debug\taia_tai.obj: lib\taia\taia_tai.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\taia\taia_tai.c
build\bcc32\Debug\taia_uint.obj: lib\taia\taia_uint.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\taia\taia_uint.c
build\bcc32\Debug\taia_unpack.obj: lib\taia\taia_unpack.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\taia\taia_unpack.c
build\bcc32\Debug\is_textbuf.obj: lib\textbuf\is_textbuf.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\textbuf\is_textbuf.c
build\bcc32\Debug\textbuf_column.obj: lib\textbuf\textbuf_column.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\textbuf\textbuf_column.c
build\bcc32\Debug\textbuf_free.obj: lib\textbuf\textbuf_free.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\textbuf\textbuf_free.c
build\bcc32\Debug\textbuf_init.obj: lib\textbuf\textbuf_init.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\textbuf\textbuf_init.c
build\bcc32\Debug\textbuf_line.obj: lib\textbuf\textbuf_line.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\textbuf\textbuf_line.c
build\bcc32\Debug\textbuf_read.obj: lib\textbuf\textbuf_read.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\textbuf\textbuf_read.c
build\bcc32\Debug\time_table_spd.obj: lib\time_table_spd.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\time_table_spd.c
build\bcc32\Debug\uint16_pack.obj: lib\uint16\uint16_pack.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\uint16\uint16_pack.c
build\bcc32\Debug\uint16_pack_big.obj: lib\uint16\uint16_pack_big.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\uint16\uint16_pack_big.c
build\bcc32\Debug\uint16_read.obj: lib\uint16\uint16_read.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\uint16\uint16_read.c
build\bcc32\Debug\uint16_read_big.obj: lib\uint16\uint16_read_big.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\uint16\uint16_read_big.c
build\bcc32\Debug\uint16_unpack.obj: lib\uint16\uint16_unpack.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\uint16\uint16_unpack.c
build\bcc32\Debug\uint16_unpack_big.obj: lib\uint16\uint16_unpack_big.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\uint16\uint16_unpack_big.c
build\bcc32\Debug\uint32_pack.obj: lib\uint32\uint32_pack.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\uint32\uint32_pack.c
build\bcc32\Debug\uint32_pack_big.obj: lib\uint32\uint32_pack_big.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\uint32\uint32_pack_big.c
build\bcc32\Debug\uint32_read.obj: lib\uint32\uint32_read.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\uint32\uint32_read.c
build\bcc32\Debug\uint32_read_big.obj: lib\uint32\uint32_read_big.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\uint32\uint32_read_big.c
build\bcc32\Debug\uint32_unpack.obj: lib\uint32\uint32_unpack.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\uint32\uint32_unpack.c
build\bcc32\Debug\uint32_unpack_big.obj: lib\uint32\uint32_unpack_big.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\uint32\uint32_unpack_big.c
build\bcc32\Debug\uint64_pack.obj: lib\uint64\uint64_pack.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\uint64\uint64_pack.c
build\bcc32\Debug\uint64_pack_big.obj: lib\uint64\uint64_pack_big.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\uint64\uint64_pack_big.c
build\bcc32\Debug\uint64_read.obj: lib\uint64\uint64_read.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\uint64\uint64_read.c
build\bcc32\Debug\uint64_read_big.obj: lib\uint64\uint64_read_big.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\uint64\uint64_read_big.c
build\bcc32\Debug\uint64_unpack.obj: lib\uint64\uint64_unpack.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\uint64\uint64_unpack.c
build\bcc32\Debug\uint64_unpack_big.obj: lib\uint64\uint64_unpack_big.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\uint64\uint64_unpack_big.c
build\bcc32\Debug\umult64.obj: lib\umult64.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\umult64.c
build\bcc32\Debug\utf8.obj: lib\utf8.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\utf8.c
build\bcc32\Debug\var_bsearch.obj: lib\var\var_bsearch.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\var\var_bsearch.c
build\bcc32\Debug\var_chflg.obj: lib\var\var_chflg.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\var\var_chflg.c
build\bcc32\Debug\var_cleanup.obj: lib\var\var_cleanup.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\var\var_cleanup.c
build\bcc32\Debug\var_copys.obj: lib\var\var_copys.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\var\var_copys.c
build\bcc32\Debug\var_count.obj: lib\var\var_count.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\var\var_count.c
build\bcc32\Debug\var_create.obj: lib\var\var_create.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\var\var_create.c
build\bcc32\Debug\var_dump.obj: lib\var\var_dump.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\var\var_dump.c
build\bcc32\Debug\var_export.obj: lib\var\var_export.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\var\var_export.c
build\bcc32\Debug\var_get.obj: lib\var\var_get.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\var\var_get.c
build\bcc32\Debug\var_hsearch.obj: lib\var\var_hsearch.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\var\var_hsearch.c
build\bcc32\Debug\var_import.obj: lib\var\var_import.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\var\var_import.c
build\bcc32\Debug\var_init.obj: lib\var\var_init.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\var\var_init.c
build\bcc32\Debug\var_len.obj: lib\var\var_len.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\var\var_len.c
build\bcc32\Debug\var_lexhash.obj: lib\var\var_lexhash.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\var\var_lexhash.c
build\bcc32\Debug\var_print.obj: lib\var\var_print.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\var\var_print.c
build\bcc32\Debug\var_rndhash.obj: lib\var\var_rndhash.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\var\var_rndhash.c
build\bcc32\Debug\var_search.obj: lib\var\var_search.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\var\var_search.c
build\bcc32\Debug\var_set.obj: lib\var\var_set.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\var\var_set.c
build\bcc32\Debug\var_setsa.obj: lib\var\var_setsa.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\var\var_setsa.c
build\bcc32\Debug\var_setvint.obj: lib\var\var_setvint.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\var\var_setvint.c
build\bcc32\Debug\var_setvsa.obj: lib\var\var_setvsa.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\var\var_setvsa.c
build\bcc32\Debug\var_unset.obj: lib\var\var_unset.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\var\var_unset.c
build\bcc32\Debug\var_valid.obj: lib\var\var_valid.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\var\var_valid.c
build\bcc32\Debug\var_value.obj: lib\var\var_value.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\var\var_value.c
build\bcc32\Debug\var_vdefault.obj: lib\var\var_vdefault.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\var\var_vdefault.c
build\bcc32\Debug\var_vlen.obj: lib\var\var_vlen.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\var\var_vlen.c
build\bcc32\Debug\vartab_add.obj: lib\vartab\vartab_add.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\vartab\vartab_add.c
build\bcc32\Debug\vartab_cleanup.obj: lib\vartab\vartab_cleanup.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\vartab\vartab_cleanup.c
build\bcc32\Debug\vartab_dump.obj: lib\vartab\vartab_dump.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\vartab\vartab_dump.c
build\bcc32\Debug\vartab_hash.obj: lib\vartab\vartab_hash.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\vartab\vartab_hash.c
build\bcc32\Debug\vartab_pop.obj: lib\vartab\vartab_pop.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\vartab\vartab_pop.c
build\bcc32\Debug\vartab_print.obj: lib\vartab\vartab_print.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\vartab\vartab_print.c
build\bcc32\Debug\vartab_push.obj: lib\vartab\vartab_push.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\vartab\vartab_push.c
build\bcc32\Debug\vartab_root.obj: lib\vartab\vartab_root.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\vartab\vartab_root.c
build\bcc32\Debug\vartab_search.obj: lib\vartab\vartab_search.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\vartab\vartab_search.c
build\bcc32\Debug\waitn.obj: lib\wait\waitn.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\wait\waitn.c
build\bcc32\Debug\waitn_reap.obj: lib\wait\waitn_reap.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\wait\waitn_reap.c
build\bcc32\Debug\waitpid_nointr.obj: lib\wait\waitpid_nointr.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\wait\waitpid_nointr.c
build\bcc32\Debug\wait_nointr.obj: lib\wait\wait_nointr.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\wait\wait_nointr.c
build\bcc32\Debug\wait_pids_nohang.obj: lib\wait\wait_pids_nohang.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\wait\wait_pids_nohang.c
build\bcc32\Debug\wait_pid_nohang.obj: lib\wait\wait_pid_nohang.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\wait\wait_pid_nohang.c
build\bcc32\Debug\wait_reap.obj: lib\wait\wait_reap.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\wait\wait_reap.c
build\bcc32\Debug\wordexp.obj: lib\wordexp.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\wordexp.c
build\bcc32\Debug\xml_add_child.obj: lib\xml\xml_add_child.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\xml\xml_add_child.c
build\bcc32\Debug\xml_attrnode.obj: lib\xml\xml_attrnode.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\xml\xml_attrnode.c
build\bcc32\Debug\xml_content.obj: lib\xml\xml_content.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\xml\xml_content.c
build\bcc32\Debug\xml_content_sa.obj: lib\xml\xml_content_sa.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\xml\xml_content_sa.c
build\bcc32\Debug\xml_debug.obj: lib\xml\xml_debug.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\xml\xml_debug.c
build\bcc32\Debug\xml_delete.obj: lib\xml\xml_delete.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\xml\xml_delete.c
build\bcc32\Debug\xml_element.obj: lib\xml\xml_element.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\xml\xml_element.c
build\bcc32\Debug\xml_escape.obj: lib\xml\xml_escape.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\xml\xml_escape.c
build\bcc32\Debug\xml_find_all_pred.obj: lib\xml\xml_find_all_pred.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\xml\xml_find_all_pred.c
build\bcc32\Debug\xml_find_element.obj: lib\xml\xml_find_element.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\xml\xml_find_element.c
build\bcc32\Debug\xml_find_element_attr.obj: lib\xml\xml_find_element_attr.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\xml\xml_find_element_attr.c
build\bcc32\Debug\xml_find_parent.obj: lib\xml\xml_find_parent.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\xml\xml_find_parent.c
build\bcc32\Debug\xml_find_pred.obj: lib\xml\xml_find_pred.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\xml\xml_find_pred.c
build\bcc32\Debug\xml_find_predicates.obj: lib\xml\xml_find_predicates.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\xml\xml_find_predicates.c
build\bcc32\Debug\xml_free.obj: lib\xml\xml_free.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\xml\xml_free.c
build\bcc32\Debug\xml_get_attribute.obj: lib\xml\xml_get_attribute.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\xml\xml_get_attribute.c
build\bcc32\Debug\xml_get_attribute_double.obj: lib\xml\xml_get_attribute_double.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\xml\xml_get_attribute_double.c
build\bcc32\Debug\xml_get_attribute_sa.obj: lib\xml\xml_get_attribute_sa.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\xml\xml_get_attribute_sa.c
build\bcc32\Debug\xml_get_document.obj: lib\xml\xml_get_document.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\xml\xml_get_document.c
build\bcc32\Debug\xml_has_attribute.obj: lib\xml\xml_has_attribute.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\xml\xml_has_attribute.c
build\bcc32\Debug\xml_insert.obj: lib\xml\xml_insert.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\xml\xml_insert.c
build\bcc32\Debug\xml_newnode.obj: lib\xml\xml_newnode.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\xml\xml_newnode.c
build\bcc32\Debug\xml_path.obj: lib\xml\xml_path.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\xml\xml_path.c
build\bcc32\Debug\xml_print.obj: lib\xml\xml_print.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\xml\xml_print.c
build\bcc32\Debug\xml_print_attributes.obj: lib\xml\xml_print_attributes.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\xml\xml_print_attributes.c
build\bcc32\Debug\xml_print_nodeset.obj: lib\xml\xml_print_nodeset.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\xml\xml_print_nodeset.c
build\bcc32\Debug\xml_reader_init.obj: lib\xml\xml_reader_init.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\xml\xml_reader_init.c
build\bcc32\Debug\xml_read_callback.obj: lib\xml\xml_read_callback.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\xml\xml_read_callback.c
build\bcc32\Debug\xml_read_tree.obj: lib\xml\xml_read_tree.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\xml\xml_read_tree.c
build\bcc32\Debug\xml_remove.obj: lib\xml\xml_remove.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\xml\xml_remove.c
build\bcc32\Debug\xml_root_element.obj: lib\xml\xml_root_element.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\xml\xml_root_element.c
build\bcc32\Debug\xml_set_attribute.obj: lib\xml\xml_set_attribute.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\xml\xml_set_attribute.c
build\bcc32\Debug\xml_set_attribute_double.obj: lib\xml\xml_set_attribute_double.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\xml\xml_set_attribute_double.c
build\bcc32\Debug\xml_textnode.obj: lib\xml\xml_textnode.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ lib\xml\xml_textnode.c
build\bcc32\Debug\libntldd.obj: libntldd.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ libntldd.c
build\bcc32\Debug\list-r.obj: list-r.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ list-r.c
build\bcc32\Debug\macho32list.obj: macho32list.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ macho32list.c
build\bcc32\Debug\map.obj: map.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ map.c
build\bcc32\Debug\mediathek-list.obj: mediathek-list.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ mediathek-list.c
build\bcc32\Debug\mediathek-parser.obj: mediathek-parser.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ mediathek-parser.c
build\bcc32\Debug\ntldd.obj: ntldd.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ ntldd.c
build\bcc32\Debug\opensearch-dump.obj: opensearch-dump.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ opensearch-dump.c
build\bcc32\Debug\pathtool.obj: pathtool.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ pathtool.c
build\bcc32\Debug\pelist.obj: pelist.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ pelist.c
build\bcc32\Debug\pkgcfg.obj: pkgcfg.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ pkgcfg.c
build\bcc32\Debug\plsconv.obj: plsconv.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ plsconv.c
build\bcc32\Debug\rdir-test.obj: rdir-test.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ rdir-test.c
build\bcc32\Debug\reg2cmd.obj: reg2cmd.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ reg2cmd.c
build\bcc32\Debug\regfilter.obj: regfilter.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ regfilter.c
build\bcc32\Debug\sln.obj: sln.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ sln.c
build\bcc32\Debug\strarraytest.obj: strarraytest.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ strarraytest.c
build\bcc32\Debug\test_lseek.obj: test_lseek.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ test_lseek.c
build\bcc32\Debug\torrent-progress.obj: torrent-progress.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ torrent-progress.c
build\bcc32\Debug\xmlpp.obj: xmlpp.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ xmlpp.c
build\bcc32\Debug\xmltest.obj: xmltest.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ xmltest.c
build\bcc32\Debug\xmltest2.obj: xmltest2.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ xmltest2.c
build\bcc32\Debug\xmltest3.obj: xmltest3.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ xmltest3.c
build\bcc32\Debug\xmltest4.obj: xmltest4.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ xmltest4.c
build\bcc32\Debug\ziptest.obj: ziptest.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) -c -o$@ ziptest.c


build\bcc32\Debug\array.lib: build\bcc32\Debug\array_allocate.obj build\bcc32\Debug\array_bytes.obj build\bcc32\Debug\array_cat.obj build\bcc32\Debug\array_cat0.obj build\bcc32\Debug\array_catb.obj build\bcc32\Debug\array_cate.obj build\bcc32\Debug\array_cats.obj build\bcc32\Debug\array_cats0.obj build\bcc32\Debug\array_end.obj build\bcc32\Debug\array_equal.obj build\bcc32\Debug\array_fail.obj build\bcc32\Debug\array_get.obj build\bcc32\Debug\array_length.obj build\bcc32\Debug\array_reset.obj build\bcc32\Debug\array_start.obj build\bcc32\Debug\array_trunc.obj build\bcc32\Debug\array_truncate.obj build\bcc32\Debug\umult64.obj
	tlib @&&|
/a /u "$@" build\bcc32\Debug\array_allocate.obj build\bcc32\Debug\array_bytes.obj build\bcc32\Debug\array_cat.obj build\bcc32\Debug\array_cat0.obj build\bcc32\Debug\array_catb.obj build\bcc32\Debug\array_cate.obj build\bcc32\Debug\array_cats.obj build\bcc32\Debug\array_cats0.obj build\bcc32\Debug\array_end.obj build\bcc32\Debug\array_equal.obj build\bcc32\Debug\array_fail.obj build\bcc32\Debug\array_get.obj build\bcc32\Debug\array_length.obj build\bcc32\Debug\array_reset.obj build\bcc32\Debug\array_start.obj build\bcc32\Debug\array_trunc.obj build\bcc32\Debug\array_truncate.obj build\bcc32\Debug\umult64.obj
|


build\bcc32\Debug\binfmt.lib: build\bcc32\Debug\binfmt_open.obj
	tlib @&&|
/a /u "$@" build\bcc32\Debug\binfmt_open.obj
|


build\bcc32\Debug\buffer.lib: build\bcc32\Debug\buffer_0.obj build\bcc32\Debug\buffer_0small.obj build\bcc32\Debug\buffer_1.obj build\bcc32\Debug\buffer_1small.obj build\bcc32\Debug\buffer_2.obj build\bcc32\Debug\buffer_bz2.obj build\bcc32\Debug\buffer_bzip.obj build\bcc32\Debug\buffer_close.obj build\bcc32\Debug\buffer_copy.obj build\bcc32\Debug\buffer_default.obj build\bcc32\Debug\buffer_deflate.obj build\bcc32\Debug\buffer_dummyread.obj build\bcc32\Debug\buffer_dummyreadmmap.obj build\bcc32\Debug\buffer_dump.obj build\bcc32\Debug\buffer_feed.obj build\bcc32\Debug\buffer_flush.obj build\bcc32\Debug\buffer_free.obj build\bcc32\Debug\buffer_freshen.obj build\bcc32\Debug\buffer_fromarray.obj build\bcc32\Debug\buffer_frombuf.obj build\bcc32\Debug\buffer_fromsa.obj build\bcc32\Debug\buffer_fromstr.obj build\bcc32\Debug\buffer_get.obj build\bcc32\Debug\buffer_get_new_token_sa.obj build\bcc32\Debug\buffer_get_new_token_sa_pred.obj build\bcc32\Debug\buffer_get_token.obj build\bcc32\Debug\buffer_get_token_pred.obj build\bcc32\Debug\buffer_get_token_sa.obj build\bcc32\Debug\buffer_get_token_sa_pred.obj build\bcc32\Debug\buffer_get_until.obj build\bcc32\Debug\buffer_getc.obj build\bcc32\Debug\buffer_getline.obj build\bcc32\Debug\buffer_getline_sa.obj build\bcc32\Debug\buffer_getn.obj build\bcc32\Debug\buffer_getnewline_sa.obj build\bcc32\Debug\buffer_gettok_sa.obj build\bcc32\Debug\buffer_gzip.obj build\bcc32\Debug\buffer_inflate.obj build\bcc32\Debug\buffer_init.obj build\bcc32\Debug\buffer_init_free.obj build\bcc32\Debug\buffer_lzma.obj build\bcc32\Debug\buffer_mmapprivate.obj build\bcc32\Debug\buffer_mmapprivate_fd.obj build\bcc32\Debug\buffer_mmapread.obj build\bcc32\Debug\buffer_mmapread_fd.obj build\bcc32\Debug\buffer_mmapshared_fd.obj build\bcc32\Debug\buffer_munmap.obj build\bcc32\Debug\buffer_peek.obj build\bcc32\Debug\buffer_peekc.obj build\bcc32\Debug\buffer_prefetch.obj build\bcc32\Debug\buffer_put.obj build\bcc32\Debug\buffer_put8long.obj build\bcc32\Debug\buffer_put_escaped.obj build\bcc32\Debug\buffer_putalign.obj build\bcc32\Debug\buffer_putc.obj build\bcc32\Debug\buffer_putdouble.obj build\bcc32\Debug\buffer_puterror.obj build\bcc32\Debug\buffer_puterror2.obj build\bcc32\Debug\buffer_putflush.obj build\bcc32\Debug\buffer_putlonglong.obj build\bcc32\Debug\buffer_putlong.obj build\bcc32\Debug\buffer_putlonglong.obj build\bcc32\Debug\buffer_putm_internal.obj build\bcc32\Debug\buffer_putm_internal_flush.obj build\bcc32\Debug\buffer_putnc.obj build\bcc32\Debug\buffer_putnlflush.obj build\bcc32\Debug\buffer_putnspace.obj build\bcc32\Debug\buffer_putptr.obj build\bcc32\Debug\buffer_puts.obj build\bcc32\Debug\buffer_puts_escaped.obj build\bcc32\Debug\buffer_putsa.obj build\bcc32\Debug\buffer_putsaflush.obj build\bcc32\Debug\buffer_putsalign.obj build\bcc32\Debug\buffer_putsflush.obj build\bcc32\Debug\buffer_putspace.obj build\bcc32\Debug\buffer_putspad.obj build\bcc32\Debug\buffer_putulonglong.obj build\bcc32\Debug\buffer_putulong.obj build\bcc32\Debug\buffer_putulong0.obj build\bcc32\Debug\buffer_putulonglong.obj build\bcc32\Debug\buffer_putxlong.obj build\bcc32\Debug\buffer_putxlong0.obj build\bcc32\Debug\buffer_putxlonglong.obj build\bcc32\Debug\buffer_putxlonglong0.obj build\bcc32\Debug\buffer_read.obj build\bcc32\Debug\buffer_read_fd.obj build\bcc32\Debug\buffer_seek.obj build\bcc32\Debug\buffer_skip_pred.obj build\bcc32\Debug\buffer_skip_until.obj build\bcc32\Debug\buffer_skipc.obj build\bcc32\Debug\buffer_skipn.obj build\bcc32\Debug\buffer_skipspace.obj build\bcc32\Debug\buffer_stubborn.obj build\bcc32\Debug\buffer_stubborn2.obj build\bcc32\Debug\buffer_tosa.obj build\bcc32\Debug\buffer_truncfile.obj build\bcc32\Debug\buffer_write.obj build\bcc32\Debug\buffer_write_fd.obj
	tlib @&&|
/a /u "$@" build\bcc32\Debug\buffer_0.obj build\bcc32\Debug\buffer_0small.obj build\bcc32\Debug\buffer_1.obj build\bcc32\Debug\buffer_1small.obj build\bcc32\Debug\buffer_2.obj build\bcc32\Debug\buffer_bz2.obj build\bcc32\Debug\buffer_bzip.obj build\bcc32\Debug\buffer_close.obj build\bcc32\Debug\buffer_copy.obj build\bcc32\Debug\buffer_default.obj build\bcc32\Debug\buffer_deflate.obj build\bcc32\Debug\buffer_dummyread.obj build\bcc32\Debug\buffer_dummyreadmmap.obj build\bcc32\Debug\buffer_dump.obj build\bcc32\Debug\buffer_feed.obj build\bcc32\Debug\buffer_flush.obj build\bcc32\Debug\buffer_free.obj build\bcc32\Debug\buffer_freshen.obj build\bcc32\Debug\buffer_fromarray.obj build\bcc32\Debug\buffer_frombuf.obj build\bcc32\Debug\buffer_fromsa.obj build\bcc32\Debug\buffer_fromstr.obj build\bcc32\Debug\buffer_get.obj build\bcc32\Debug\buffer_get_new_token_sa.obj build\bcc32\Debug\buffer_get_new_token_sa_pred.obj build\bcc32\Debug\buffer_get_token.obj build\bcc32\Debug\buffer_get_token_pred.obj build\bcc32\Debug\buffer_get_token_sa.obj build\bcc32\Debug\buffer_get_token_sa_pred.obj build\bcc32\Debug\buffer_get_until.obj build\bcc32\Debug\buffer_getc.obj build\bcc32\Debug\buffer_getline.obj build\bcc32\Debug\buffer_getline_sa.obj build\bcc32\Debug\buffer_getn.obj build\bcc32\Debug\buffer_getnewline_sa.obj build\bcc32\Debug\buffer_gettok_sa.obj build\bcc32\Debug\buffer_gzip.obj build\bcc32\Debug\buffer_inflate.obj build\bcc32\Debug\buffer_init.obj build\bcc32\Debug\buffer_init_free.obj build\bcc32\Debug\buffer_lzma.obj build\bcc32\Debug\buffer_mmapprivate.obj build\bcc32\Debug\buffer_mmapprivate_fd.obj build\bcc32\Debug\buffer_mmapread.obj build\bcc32\Debug\buffer_mmapread_fd.obj build\bcc32\Debug\buffer_mmapshared_fd.obj build\bcc32\Debug\buffer_munmap.obj build\bcc32\Debug\buffer_peek.obj build\bcc32\Debug\buffer_peekc.obj build\bcc32\Debug\buffer_prefetch.obj build\bcc32\Debug\buffer_put.obj build\bcc32\Debug\buffer_put8long.obj build\bcc32\Debug\buffer_put_escaped.obj build\bcc32\Debug\buffer_putalign.obj build\bcc32\Debug\buffer_putc.obj build\bcc32\Debug\buffer_putdouble.obj build\bcc32\Debug\buffer_puterror.obj build\bcc32\Debug\buffer_puterror2.obj build\bcc32\Debug\buffer_putflush.obj build\bcc32\Debug\buffer_putlonglong.obj build\bcc32\Debug\buffer_putlong.obj build\bcc32\Debug\buffer_putlonglong.obj build\bcc32\Debug\buffer_putm_internal.obj build\bcc32\Debug\buffer_putm_internal_flush.obj build\bcc32\Debug\buffer_putnc.obj build\bcc32\Debug\buffer_putnlflush.obj build\bcc32\Debug\buffer_putnspace.obj build\bcc32\Debug\buffer_putptr.obj build\bcc32\Debug\buffer_puts.obj build\bcc32\Debug\buffer_puts_escaped.obj build\bcc32\Debug\buffer_putsa.obj build\bcc32\Debug\buffer_putsaflush.obj build\bcc32\Debug\buffer_putsalign.obj build\bcc32\Debug\buffer_putsflush.obj build\bcc32\Debug\buffer_putspace.obj build\bcc32\Debug\buffer_putspad.obj build\bcc32\Debug\buffer_putulonglong.obj build\bcc32\Debug\buffer_putulong.obj build\bcc32\Debug\buffer_putulong0.obj build\bcc32\Debug\buffer_putulonglong.obj build\bcc32\Debug\buffer_putxlong.obj build\bcc32\Debug\buffer_putxlong0.obj build\bcc32\Debug\buffer_putxlonglong.obj build\bcc32\Debug\buffer_putxlonglong0.obj build\bcc32\Debug\buffer_read.obj build\bcc32\Debug\buffer_read_fd.obj build\bcc32\Debug\buffer_seek.obj build\bcc32\Debug\buffer_skip_pred.obj build\bcc32\Debug\buffer_skip_until.obj build\bcc32\Debug\buffer_skipc.obj build\bcc32\Debug\buffer_skipn.obj build\bcc32\Debug\buffer_skipspace.obj build\bcc32\Debug\buffer_stubborn.obj build\bcc32\Debug\buffer_stubborn2.obj build\bcc32\Debug\buffer_tosa.obj build\bcc32\Debug\buffer_truncfile.obj build\bcc32\Debug\buffer_write.obj build\bcc32\Debug\buffer_write_fd.obj
|


build\bcc32\Debug\byte.lib: build\bcc32\Debug\byte_case_diff.obj build\bcc32\Debug\byte_case_equal.obj build\bcc32\Debug\byte_case_start.obj build\bcc32\Debug\byte_chr.obj build\bcc32\Debug\byte_copy.obj build\bcc32\Debug\byte_copyr.obj build\bcc32\Debug\byte_count.obj build\bcc32\Debug\byte_diff.obj build\bcc32\Debug\byte_equal.obj build\bcc32\Debug\byte_fill.obj build\bcc32\Debug\byte_fmt.obj build\bcc32\Debug\byte_lower.obj build\bcc32\Debug\byte_rchr.obj build\bcc32\Debug\byte_scan.obj build\bcc32\Debug\byte_upper.obj build\bcc32\Debug\byte_zero.obj
	tlib @&&|
/a /u "$@" build\bcc32\Debug\byte_case_diff.obj build\bcc32\Debug\byte_case_equal.obj build\bcc32\Debug\byte_case_start.obj build\bcc32\Debug\byte_chr.obj build\bcc32\Debug\byte_copy.obj build\bcc32\Debug\byte_copyr.obj build\bcc32\Debug\byte_count.obj build\bcc32\Debug\byte_diff.obj build\bcc32\Debug\byte_equal.obj build\bcc32\Debug\byte_fill.obj build\bcc32\Debug\byte_fmt.obj build\bcc32\Debug\byte_lower.obj build\bcc32\Debug\byte_rchr.obj build\bcc32\Debug\byte_scan.obj build\bcc32\Debug\byte_upper.obj build\bcc32\Debug\byte_zero.obj
|


build\bcc32\Debug\case.lib: build\bcc32\Debug\case_diffb.obj build\bcc32\Debug\case_diffs.obj build\bcc32\Debug\case_lowerb.obj build\bcc32\Debug\case_lowers.obj build\bcc32\Debug\case_starts.obj
	tlib @&&|
/a /u "$@" build\bcc32\Debug\case_diffb.obj build\bcc32\Debug\case_diffs.obj build\bcc32\Debug\case_lowerb.obj build\bcc32\Debug\case_lowers.obj build\bcc32\Debug\case_starts.obj
|


build\bcc32\Debug\cb.lib: build\bcc32\Debug\cb_clear.obj build\bcc32\Debug\cb_erase.obj build\bcc32\Debug\cb_find.obj build\bcc32\Debug\cb_find_prefix.obj build\bcc32\Debug\cb_foreach.obj build\bcc32\Debug\cb_get_kv.obj build\bcc32\Debug\cb_get_kv_ex.obj build\bcc32\Debug\cb_insert.obj build\bcc32\Debug\cb_new_kv.obj
	tlib @&&|
/a /u "$@" build\bcc32\Debug\cb_clear.obj build\bcc32\Debug\cb_erase.obj build\bcc32\Debug\cb_find.obj build\bcc32\Debug\cb_find_prefix.obj build\bcc32\Debug\cb_foreach.obj build\bcc32\Debug\cb_get_kv.obj build\bcc32\Debug\cb_get_kv_ex.obj build\bcc32\Debug\cb_insert.obj build\bcc32\Debug\cb_new_kv.obj
|


build\bcc32\Debug\cbmap.lib: build\bcc32\Debug\alloc.obj build\bcc32\Debug\cbmap_count.obj build\bcc32\Debug\cbmap_data_node_destroy.obj build\bcc32\Debug\cbmap_delete.obj build\bcc32\Debug\cbmap_destroy.obj build\bcc32\Debug\cbmap_get.obj build\bcc32\Debug\cbmap_insert.obj build\bcc32\Debug\cbmap_internal_node.obj build\bcc32\Debug\cbmap_new.obj build\bcc32\Debug\cbmap_visit.obj build\bcc32\Debug\cbmap_visit_all.obj build\bcc32\Debug\memalign.obj
	tlib @&&|
/a /u "$@" build\bcc32\Debug\alloc.obj build\bcc32\Debug\cbmap_count.obj build\bcc32\Debug\cbmap_data_node_destroy.obj build\bcc32\Debug\cbmap_delete.obj build\bcc32\Debug\cbmap_destroy.obj build\bcc32\Debug\cbmap_get.obj build\bcc32\Debug\cbmap_insert.obj build\bcc32\Debug\cbmap_internal_node.obj build\bcc32\Debug\cbmap_new.obj build\bcc32\Debug\cbmap_visit.obj build\bcc32\Debug\cbmap_visit_all.obj build\bcc32\Debug\memalign.obj
|


build\bcc32\Debug\charbuf.lib: build\bcc32\Debug\charbuf_close.obj build\bcc32\Debug\charbuf_get.obj build\bcc32\Debug\charbuf_getc.obj build\bcc32\Debug\charbuf_init.obj build\bcc32\Debug\charbuf_nextc.obj build\bcc32\Debug\charbuf_peek.obj build\bcc32\Debug\charbuf_peekc.obj build\bcc32\Debug\charbuf_skip.obj build\bcc32\Debug\charbuf_skip_pred.obj build\bcc32\Debug\charbuf_skip_until.obj
	tlib @&&|
/a /u "$@" build\bcc32\Debug\charbuf_close.obj build\bcc32\Debug\charbuf_get.obj build\bcc32\Debug\charbuf_getc.obj build\bcc32\Debug\charbuf_init.obj build\bcc32\Debug\charbuf_nextc.obj build\bcc32\Debug\charbuf_peek.obj build\bcc32\Debug\charbuf_peekc.obj build\bcc32\Debug\charbuf_skip.obj build\bcc32\Debug\charbuf_skip_pred.obj build\bcc32\Debug\charbuf_skip_until.obj
|


build\bcc32\Debug\dir.lib: build\bcc32\Debug\dir_close.obj build\bcc32\Debug\dir_name.obj build\bcc32\Debug\dir_open.obj build\bcc32\Debug\dir_read.obj build\bcc32\Debug\dir_time.obj build\bcc32\Debug\dir_type.obj build\bcc32\Debug\utf8.obj
	tlib @&&|
/a /u "$@" build\bcc32\Debug\dir_close.obj build\bcc32\Debug\dir_name.obj build\bcc32\Debug\dir_open.obj build\bcc32\Debug\dir_read.obj build\bcc32\Debug\dir_time.obj build\bcc32\Debug\dir_type.obj build\bcc32\Debug\utf8.obj
|


build\bcc32\Debug\dns.lib: build\bcc32\Debug\dns_dfd.obj build\bcc32\Debug\dns_domain.obj build\bcc32\Debug\dns_dtda.obj build\bcc32\Debug\dns_ip4.obj build\bcc32\Debug\dns_ip6.obj build\bcc32\Debug\dns_ipq.obj build\bcc32\Debug\dns_ipq6.obj build\bcc32\Debug\dns_mx.obj build\bcc32\Debug\dns_name.obj build\bcc32\Debug\dns_nd.obj build\bcc32\Debug\dns_nd6.obj build\bcc32\Debug\dns_packet.obj build\bcc32\Debug\dns_random.obj build\bcc32\Debug\dns_rcip.obj build\bcc32\Debug\dns_rcrw.obj build\bcc32\Debug\dns_resolve.obj build\bcc32\Debug\dns_sortip.obj build\bcc32\Debug\dns_sortip6.obj build\bcc32\Debug\dns_transmit.obj build\bcc32\Debug\dns_txt.obj build\bcc32\Debug\init.obj
	tlib @&&|
/a /u "$@" build\bcc32\Debug\dns_dfd.obj build\bcc32\Debug\dns_domain.obj build\bcc32\Debug\dns_dtda.obj build\bcc32\Debug\dns_ip4.obj build\bcc32\Debug\dns_ip6.obj build\bcc32\Debug\dns_ipq.obj build\bcc32\Debug\dns_ipq6.obj build\bcc32\Debug\dns_mx.obj build\bcc32\Debug\dns_name.obj build\bcc32\Debug\dns_nd.obj build\bcc32\Debug\dns_nd6.obj build\bcc32\Debug\dns_packet.obj build\bcc32\Debug\dns_random.obj build\bcc32\Debug\dns_rcip.obj build\bcc32\Debug\dns_rcrw.obj build\bcc32\Debug\dns_resolve.obj build\bcc32\Debug\dns_sortip.obj build\bcc32\Debug\dns_sortip6.obj build\bcc32\Debug\dns_transmit.obj build\bcc32\Debug\dns_txt.obj build\bcc32\Debug\init.obj
|


build\bcc32\Debug\elf.lib: build\bcc32\Debug\elf_dynamic_section.obj build\bcc32\Debug\elf_get_section.obj build\bcc32\Debug\elf_get_value.obj build\bcc32\Debug\elf_header.obj build\bcc32\Debug\elf_program_headers.obj build\bcc32\Debug\elf_section_headers.obj build\bcc32\Debug\elf_section_index.obj build\bcc32\Debug\elf_section_offset.obj build\bcc32\Debug\elf_section_size.obj build\bcc32\Debug\elf_section_type.obj build\bcc32\Debug\elf_shstrtab.obj
	tlib @&&|
/a /u "$@" build\bcc32\Debug\elf_dynamic_section.obj build\bcc32\Debug\elf_get_section.obj build\bcc32\Debug\elf_get_value.obj build\bcc32\Debug\elf_header.obj build\bcc32\Debug\elf_program_headers.obj build\bcc32\Debug\elf_section_headers.obj build\bcc32\Debug\elf_section_index.obj build\bcc32\Debug\elf_section_offset.obj build\bcc32\Debug\elf_section_size.obj build\bcc32\Debug\elf_section_type.obj build\bcc32\Debug\elf_shstrtab.obj
|


build\bcc32\Debug\env.lib: build\bcc32\Debug\env_addmodif.obj build\bcc32\Debug\env_get.obj build\bcc32\Debug\env_get2.obj build\bcc32\Debug\env_len.obj build\bcc32\Debug\env_make.obj build\bcc32\Debug\env_merge.obj build\bcc32\Debug\env_pick.obj build\bcc32\Debug\env_put.obj build\bcc32\Debug\env_set.obj build\bcc32\Debug\env_string.obj build\bcc32\Debug\env_unset.obj build\bcc32\Debug\getenv.obj build\bcc32\Debug\setenv.obj
	tlib @&&|
/a /u "$@" build\bcc32\Debug\env_addmodif.obj build\bcc32\Debug\env_get.obj build\bcc32\Debug\env_get2.obj build\bcc32\Debug\env_len.obj build\bcc32\Debug\env_make.obj build\bcc32\Debug\env_merge.obj build\bcc32\Debug\env_pick.obj build\bcc32\Debug\env_put.obj build\bcc32\Debug\env_set.obj build\bcc32\Debug\env_string.obj build\bcc32\Debug\env_unset.obj build\bcc32\Debug\getenv.obj build\bcc32\Debug\setenv.obj
|


build\bcc32\Debug\errmsg.lib: build\bcc32\Debug\errmsg_iam.obj build\bcc32\Debug\errmsg_info.obj build\bcc32\Debug\errmsg_infosys.obj build\bcc32\Debug\errmsg_puts.obj build\bcc32\Debug\errmsg_warn.obj build\bcc32\Debug\errmsg_warnsys.obj build\bcc32\Debug\errmsg_write.obj
	tlib @&&|
/a /u "$@" build\bcc32\Debug\errmsg_iam.obj build\bcc32\Debug\errmsg_info.obj build\bcc32\Debug\errmsg_infosys.obj build\bcc32\Debug\errmsg_puts.obj build\bcc32\Debug\errmsg_warn.obj build\bcc32\Debug\errmsg_warnsys.obj build\bcc32\Debug\errmsg_write.obj
|


build\bcc32\Debug\expand.lib: build\bcc32\Debug\expand_arg.obj build\bcc32\Debug\expand_args.obj build\bcc32\Debug\expand_argv.obj build\bcc32\Debug\expand_arith.obj build\bcc32\Debug\expand_cat.obj build\bcc32\Debug\expand_catsa.obj build\bcc32\Debug\expand_command.obj build\bcc32\Debug\expand_copysa.obj build\bcc32\Debug\expand_escape.obj build\bcc32\Debug\expand_glob.obj build\bcc32\Debug\expand_param.obj build\bcc32\Debug\expand_unescape.obj build\bcc32\Debug\expand_vars.obj build\bcc32\Debug\glob.obj
	tlib @&&|
/a /u "$@" build\bcc32\Debug\expand_arg.obj build\bcc32\Debug\expand_args.obj build\bcc32\Debug\expand_argv.obj build\bcc32\Debug\expand_arith.obj build\bcc32\Debug\expand_cat.obj build\bcc32\Debug\expand_catsa.obj build\bcc32\Debug\expand_command.obj build\bcc32\Debug\expand_copysa.obj build\bcc32\Debug\expand_escape.obj build\bcc32\Debug\expand_glob.obj build\bcc32\Debug\expand_param.obj build\bcc32\Debug\expand_unescape.obj build\bcc32\Debug\expand_vars.obj build\bcc32\Debug\glob.obj
|


build\bcc32\Debug\fmt.lib: build\bcc32\Debug\fmt_8long.obj build\bcc32\Debug\fmt_octal.obj build\bcc32\Debug\fmt_asn1derlength.obj build\bcc32\Debug\fmt_asn1dertag.obj build\bcc32\Debug\fmt_double.obj build\bcc32\Debug\fmt_escapecharc.obj build\bcc32\Debug\fmt_escapecharhtml.obj build\bcc32\Debug\fmt_escapecharquotedprintable.obj build\bcc32\Debug\fmt_escapecharquotedprintableutf8.obj build\bcc32\Debug\fmt_escapecharshell.obj build\bcc32\Debug\fmt_escapecharxml.obj build\bcc32\Debug\fmt_fill.obj build\bcc32\Debug\fmt_hexb.obj build\bcc32\Debug\fmt_httpdate.obj build\bcc32\Debug\fmt_human.obj build\bcc32\Debug\fmt_humank.obj build\bcc32\Debug\fmt_iso8601.obj build\bcc32\Debug\fmt_long.obj build\bcc32\Debug\fmt_longlong.obj build\bcc32\Debug\fmt_minus.obj build\bcc32\Debug\fmt_pad.obj build\bcc32\Debug\fmt_plusminus.obj build\bcc32\Debug\fmt_repeat.obj build\bcc32\Debug\fmt_str.obj build\bcc32\Debug\fmt_strm_internal.obj build\bcc32\Debug\fmt_strn.obj build\bcc32\Debug\fmt_tohex.obj build\bcc32\Debug\fmt_ulonglong.obj build\bcc32\Debug\fmt_ulong.obj build\bcc32\Debug\fmt_ulong0.obj build\bcc32\Debug\fmt_ulonglong.obj build\bcc32\Debug\fmt_utf8.obj build\bcc32\Debug\fmt_xlong.obj build\bcc32\Debug\fmt_xlonglong.obj build\bcc32\Debug\fmt_xmlescape.obj
	tlib @&&|
/a /u "$@" build\bcc32\Debug\fmt_8long.obj build\bcc32\Debug\fmt_octal.obj build\bcc32\Debug\fmt_asn1derlength.obj build\bcc32\Debug\fmt_asn1dertag.obj build\bcc32\Debug\fmt_double.obj build\bcc32\Debug\fmt_escapecharc.obj build\bcc32\Debug\fmt_escapecharhtml.obj build\bcc32\Debug\fmt_escapecharquotedprintable.obj build\bcc32\Debug\fmt_escapecharquotedprintableutf8.obj build\bcc32\Debug\fmt_escapecharshell.obj build\bcc32\Debug\fmt_escapecharxml.obj build\bcc32\Debug\fmt_fill.obj build\bcc32\Debug\fmt_hexb.obj build\bcc32\Debug\fmt_httpdate.obj build\bcc32\Debug\fmt_human.obj build\bcc32\Debug\fmt_humank.obj build\bcc32\Debug\fmt_iso8601.obj build\bcc32\Debug\fmt_long.obj build\bcc32\Debug\fmt_longlong.obj build\bcc32\Debug\fmt_minus.obj build\bcc32\Debug\fmt_pad.obj build\bcc32\Debug\fmt_plusminus.obj build\bcc32\Debug\fmt_repeat.obj build\bcc32\Debug\fmt_str.obj build\bcc32\Debug\fmt_strm_internal.obj build\bcc32\Debug\fmt_strn.obj build\bcc32\Debug\fmt_tohex.obj build\bcc32\Debug\fmt_ulonglong.obj build\bcc32\Debug\fmt_ulong.obj build\bcc32\Debug\fmt_ulong0.obj build\bcc32\Debug\fmt_ulonglong.obj build\bcc32\Debug\fmt_utf8.obj build\bcc32\Debug\fmt_xlong.obj build\bcc32\Debug\fmt_xlonglong.obj build\bcc32\Debug\fmt_xmlescape.obj
|


build\bcc32\Debug\gpio.lib: build\bcc32\Debug\gpio_init.obj build\bcc32\Debug\gpio_read.obj build\bcc32\Debug\gpio_set_input_pull.obj build\bcc32\Debug\gpio_set_input_pull_down.obj build\bcc32\Debug\gpio_set_input_pull_up.obj build\bcc32\Debug\gpio_set_input_resistor_pull.obj build\bcc32\Debug\gpio_set_output.obj build\bcc32\Debug\gpio_set_pin_mode.obj build\bcc32\Debug\gpio_short_wait.obj build\bcc32\Debug\gpio_write.obj
	tlib @&&|
/a /u "$@" build\bcc32\Debug\gpio_init.obj build\bcc32\Debug\gpio_read.obj build\bcc32\Debug\gpio_set_input_pull.obj build\bcc32\Debug\gpio_set_input_pull_down.obj build\bcc32\Debug\gpio_set_input_pull_up.obj build\bcc32\Debug\gpio_set_input_resistor_pull.obj build\bcc32\Debug\gpio_set_output.obj build\bcc32\Debug\gpio_set_pin_mode.obj build\bcc32\Debug\gpio_short_wait.obj build\bcc32\Debug\gpio_write.obj
|


build\bcc32\Debug\hmap.lib: build\bcc32\Debug\hmap_add.obj build\bcc32\Debug\hmap_add_tuple_with_data.obj build\bcc32\Debug\hmap_delete.obj build\bcc32\Debug\hmap_destroy.obj build\bcc32\Debug\hmap_dump.obj build\bcc32\Debug\hmap_free_data.obj build\bcc32\Debug\hmap_init.obj build\bcc32\Debug\hmap_internal.obj build\bcc32\Debug\hmap_is_locate.obj build\bcc32\Debug\hmap_print_list.obj build\bcc32\Debug\hmap_print_table.obj build\bcc32\Debug\hmap_print_tree.obj build\bcc32\Debug\hmap_search.obj build\bcc32\Debug\hmap_set.obj build\bcc32\Debug\hmap_set_chars.obj build\bcc32\Debug\hmap_set_stralloc.obj build\bcc32\Debug\hmap_size.obj build\bcc32\Debug\hmap_truncate.obj
	tlib @&&|
/a /u "$@" build\bcc32\Debug\hmap_add.obj build\bcc32\Debug\hmap_add_tuple_with_data.obj build\bcc32\Debug\hmap_delete.obj build\bcc32\Debug\hmap_destroy.obj build\bcc32\Debug\hmap_dump.obj build\bcc32\Debug\hmap_free_data.obj build\bcc32\Debug\hmap_init.obj build\bcc32\Debug\hmap_internal.obj build\bcc32\Debug\hmap_is_locate.obj build\bcc32\Debug\hmap_print_list.obj build\bcc32\Debug\hmap_print_table.obj build\bcc32\Debug\hmap_print_tree.obj build\bcc32\Debug\hmap_search.obj build\bcc32\Debug\hmap_set.obj build\bcc32\Debug\hmap_set_chars.obj build\bcc32\Debug\hmap_set_stralloc.obj build\bcc32\Debug\hmap_size.obj build\bcc32\Debug\hmap_truncate.obj
|


build\bcc32\Debug\http.lib: build\bcc32\Debug\http_get.obj build\bcc32\Debug\http_init.obj build\bcc32\Debug\http_readable.obj build\bcc32\Debug\http_sendreq.obj build\bcc32\Debug\http_socket.obj build\bcc32\Debug\http_writeable.obj
	tlib @&&|
/a /u "$@" build\bcc32\Debug\http_get.obj build\bcc32\Debug\http_init.obj build\bcc32\Debug\http_readable.obj build\bcc32\Debug\http_sendreq.obj build\bcc32\Debug\http_socket.obj build\bcc32\Debug\http_writeable.obj
|


build\bcc32\Debug\iarray.lib: build\bcc32\Debug\iarray_allocate.obj build\bcc32\Debug\iarray_free.obj build\bcc32\Debug\iarray_get.obj build\bcc32\Debug\iarray_init.obj build\bcc32\Debug\iarray_length.obj
	tlib @&&|
/a /u "$@" build\bcc32\Debug\iarray_allocate.obj build\bcc32\Debug\iarray_free.obj build\bcc32\Debug\iarray_get.obj build\bcc32\Debug\iarray_init.obj build\bcc32\Debug\iarray_length.obj
|


build\bcc32\Debug\io.lib: build\bcc32\Debug\io_appendfile.obj build\bcc32\Debug\io_block.obj build\bcc32\Debug\io_canread.obj build\bcc32\Debug\io_canwrite.obj build\bcc32\Debug\io_check.obj build\bcc32\Debug\io_close.obj build\bcc32\Debug\io_closeonexec.obj build\bcc32\Debug\io_createfile.obj build\bcc32\Debug\io_debugstring.obj build\bcc32\Debug\io_dontwantread.obj build\bcc32\Debug\io_dontwantwrite.obj build\bcc32\Debug\io_dup.obj build\bcc32\Debug\io_eagain.obj build\bcc32\Debug\io_eagain_read.obj build\bcc32\Debug\io_eagain_write.obj build\bcc32\Debug\io_fd.obj build\bcc32\Debug\io_finishandshutdown.obj build\bcc32\Debug\io_getcookie.obj build\bcc32\Debug\io_mmapwritefile.obj build\bcc32\Debug\io_nonblock.obj build\bcc32\Debug\io_pipe.obj build\bcc32\Debug\io_readfile.obj build\bcc32\Debug\io_readwritefile.obj build\bcc32\Debug\io_sendfile.obj build\bcc32\Debug\io_setcookie.obj build\bcc32\Debug\io_sigpipe.obj build\bcc32\Debug\io_socketpair.obj build\bcc32\Debug\io_timedout.obj build\bcc32\Debug\io_timeout.obj build\bcc32\Debug\io_timeouted.obj build\bcc32\Debug\io_tryread.obj build\bcc32\Debug\io_tryreadtimeout.obj build\bcc32\Debug\io_trywrite.obj build\bcc32\Debug\io_trywritetimeout.obj build\bcc32\Debug\io_wait.obj build\bcc32\Debug\io_waitread.obj build\bcc32\Debug\io_waituntil.obj build\bcc32\Debug\io_waituntil2.obj build\bcc32\Debug\io_waitwrite.obj build\bcc32\Debug\io_wantread.obj build\bcc32\Debug\io_wantwrite.obj build\bcc32\Debug\iopause.obj
	tlib @&&|
/a /u "$@" build\bcc32\Debug\io_appendfile.obj build\bcc32\Debug\io_block.obj build\bcc32\Debug\io_canread.obj build\bcc32\Debug\io_canwrite.obj build\bcc32\Debug\io_check.obj build\bcc32\Debug\io_close.obj build\bcc32\Debug\io_closeonexec.obj build\bcc32\Debug\io_createfile.obj build\bcc32\Debug\io_debugstring.obj build\bcc32\Debug\io_dontwantread.obj build\bcc32\Debug\io_dontwantwrite.obj build\bcc32\Debug\io_dup.obj build\bcc32\Debug\io_eagain.obj build\bcc32\Debug\io_eagain_read.obj build\bcc32\Debug\io_eagain_write.obj build\bcc32\Debug\io_fd.obj build\bcc32\Debug\io_finishandshutdown.obj build\bcc32\Debug\io_getcookie.obj build\bcc32\Debug\io_mmapwritefile.obj build\bcc32\Debug\io_nonblock.obj build\bcc32\Debug\io_pipe.obj build\bcc32\Debug\io_readfile.obj build\bcc32\Debug\io_readwritefile.obj build\bcc32\Debug\io_sendfile.obj build\bcc32\Debug\io_setcookie.obj build\bcc32\Debug\io_sigpipe.obj build\bcc32\Debug\io_socketpair.obj build\bcc32\Debug\io_timedout.obj build\bcc32\Debug\io_timeout.obj build\bcc32\Debug\io_timeouted.obj build\bcc32\Debug\io_tryread.obj build\bcc32\Debug\io_tryreadtimeout.obj build\bcc32\Debug\io_trywrite.obj build\bcc32\Debug\io_trywritetimeout.obj build\bcc32\Debug\io_wait.obj build\bcc32\Debug\io_waitread.obj build\bcc32\Debug\io_waituntil.obj build\bcc32\Debug\io_waituntil2.obj build\bcc32\Debug\io_waitwrite.obj build\bcc32\Debug\io_wantread.obj build\bcc32\Debug\io_wantwrite.obj build\bcc32\Debug\iopause.obj
|


build\bcc32\Debug\json.lib: build\bcc32\Debug\json_free.obj build\bcc32\Debug\json_newnode.obj build\bcc32\Debug\json_parse.obj build\bcc32\Debug\json_print.obj build\bcc32\Debug\json_read_callback.obj build\bcc32\Debug\json_read_tree.obj build\bcc32\Debug\json_reader_init.obj
	tlib @&&|
/a /u "$@" build\bcc32\Debug\json_free.obj build\bcc32\Debug\json_newnode.obj build\bcc32\Debug\json_parse.obj build\bcc32\Debug\json_print.obj build\bcc32\Debug\json_read_callback.obj build\bcc32\Debug\json_read_tree.obj build\bcc32\Debug\json_reader_init.obj
|


build\bcc32\Debug\list.lib: build\bcc32\Debug\list_add_after.obj build\bcc32\Debug\list_add_before.obj build\bcc32\Debug\list_find.obj build\bcc32\Debug\list_find_remove.obj build\bcc32\Debug\list_init.obj build\bcc32\Debug\list_length.obj build\bcc32\Debug\list_move_head.obj build\bcc32\Debug\list_move_tail.obj build\bcc32\Debug\list_push.obj build\bcc32\Debug\list_remove.obj build\bcc32\Debug\list_swap.obj build\bcc32\Debug\list_unshift.obj
	tlib @&&|
/a /u "$@" build\bcc32\Debug\list_add_after.obj build\bcc32\Debug\list_add_before.obj build\bcc32\Debug\list_find.obj build\bcc32\Debug\list_find_remove.obj build\bcc32\Debug\list_init.obj build\bcc32\Debug\list_length.obj build\bcc32\Debug\list_move_head.obj build\bcc32\Debug\list_move_tail.obj build\bcc32\Debug\list_push.obj build\bcc32\Debug\list_remove.obj build\bcc32\Debug\list_swap.obj build\bcc32\Debug\list_unshift.obj
|


build\bcc32\Debug\map.lib: build\bcc32\Debug\map_deinit.obj build\bcc32\Debug\map_get.obj build\bcc32\Debug\map_getref.obj build\bcc32\Debug\map_iter.obj build\bcc32\Debug\map_next.obj build\bcc32\Debug\map_remove.obj build\bcc32\Debug\map_set.obj
	tlib @&&|
/a /u "$@" build\bcc32\Debug\map_deinit.obj build\bcc32\Debug\map_get.obj build\bcc32\Debug\map_getref.obj build\bcc32\Debug\map_iter.obj build\bcc32\Debug\map_next.obj build\bcc32\Debug\map_remove.obj build\bcc32\Debug\map_set.obj
|


build\bcc32\Debug\mmap.lib: build\bcc32\Debug\mmap_map.obj build\bcc32\Debug\mmap_private.obj build\bcc32\Debug\mmap_private_fd.obj build\bcc32\Debug\mmap_read.obj build\bcc32\Debug\mmap_read_fd.obj build\bcc32\Debug\mmap_rw_fd.obj build\bcc32\Debug\mmap_shared.obj build\bcc32\Debug\mmap_shared_fd.obj build\bcc32\Debug\mmap_unmap.obj
	tlib @&&|
/a /u "$@" build\bcc32\Debug\mmap_map.obj build\bcc32\Debug\mmap_private.obj build\bcc32\Debug\mmap_private_fd.obj build\bcc32\Debug\mmap_read.obj build\bcc32\Debug\mmap_read_fd.obj build\bcc32\Debug\mmap_rw_fd.obj build\bcc32\Debug\mmap_shared.obj build\bcc32\Debug\mmap_shared_fd.obj build\bcc32\Debug\mmap_unmap.obj
|


build\bcc32\Debug\ndelay.lib: build\bcc32\Debug\ndelay_off.obj build\bcc32\Debug\ndelay_on.obj
	tlib @&&|
/a /u "$@" build\bcc32\Debug\ndelay_off.obj build\bcc32\Debug\ndelay_on.obj
|


build\bcc32\Debug\open.lib: build\bcc32\Debug\open_append.obj build\bcc32\Debug\open_excl.obj build\bcc32\Debug\open_read.obj build\bcc32\Debug\open_rw.obj build\bcc32\Debug\open_trunc.obj build\bcc32\Debug\open_write.obj build\bcc32\Debug\openreadclose.obj build\bcc32\Debug\readclose.obj
	tlib @&&|
/a /u "$@" build\bcc32\Debug\open_append.obj build\bcc32\Debug\open_excl.obj build\bcc32\Debug\open_read.obj build\bcc32\Debug\open_rw.obj build\bcc32\Debug\open_trunc.obj build\bcc32\Debug\open_write.obj build\bcc32\Debug\openreadclose.obj build\bcc32\Debug\readclose.obj
|


build\bcc32\Debug\path.lib: build\bcc32\Debug\fork.obj build\bcc32\Debug\getdelim.obj build\bcc32\Debug\path_absolute.obj build\bcc32\Debug\path_basename.obj build\bcc32\Debug\path_canonicalize.obj build\bcc32\Debug\path_dirname.obj build\bcc32\Debug\path_exists.obj build\bcc32\Debug\path_fnmatch.obj build\bcc32\Debug\path_getcwd.obj build\bcc32\Debug\path_gethome.obj build\bcc32\Debug\path_len.obj build\bcc32\Debug\path_len_s.obj build\bcc32\Debug\path_num.obj build\bcc32\Debug\path_num_sa.obj build\bcc32\Debug\path_readlink.obj build\bcc32\Debug\path_realpath.obj build\bcc32\Debug\path_right.obj build\bcc32\Debug\path_skip.obj build\bcc32\Debug\path_skips.obj build\bcc32\Debug\path_split.obj build\bcc32\Debug\pipe2.obj build\bcc32\Debug\readlink.obj build\bcc32\Debug\symlink.obj build\bcc32\Debug\wordexp.obj
	tlib @&&|
/a /u "$@" build\bcc32\Debug\fork.obj build\bcc32\Debug\getdelim.obj build\bcc32\Debug\path_absolute.obj build\bcc32\Debug\path_basename.obj build\bcc32\Debug\path_canonicalize.obj build\bcc32\Debug\path_dirname.obj build\bcc32\Debug\path_exists.obj build\bcc32\Debug\path_fnmatch.obj build\bcc32\Debug\path_getcwd.obj build\bcc32\Debug\path_gethome.obj build\bcc32\Debug\path_len.obj build\bcc32\Debug\path_len_s.obj build\bcc32\Debug\path_num.obj build\bcc32\Debug\path_num_sa.obj build\bcc32\Debug\path_readlink.obj build\bcc32\Debug\path_realpath.obj build\bcc32\Debug\path_right.obj build\bcc32\Debug\path_skip.obj build\bcc32\Debug\path_skips.obj build\bcc32\Debug\path_split.obj build\bcc32\Debug\pipe2.obj build\bcc32\Debug\readlink.obj build\bcc32\Debug\symlink.obj build\bcc32\Debug\wordexp.obj
|


build\bcc32\Debug\pe.lib: build\bcc32\Debug\pe_get_datadir.obj build\bcc32\Debug\pe_get_section.obj build\bcc32\Debug\pe_get_value.obj build\bcc32\Debug\pe_header.obj build\bcc32\Debug\pe_offset2rva.obj build\bcc32\Debug\pe_rva2offset.obj build\bcc32\Debug\pe_rva2ptr.obj build\bcc32\Debug\pe_thunk.obj
	tlib @&&|
/a /u "$@" build\bcc32\Debug\pe_get_datadir.obj build\bcc32\Debug\pe_get_section.obj build\bcc32\Debug\pe_get_value.obj build\bcc32\Debug\pe_header.obj build\bcc32\Debug\pe_offset2rva.obj build\bcc32\Debug\pe_rva2offset.obj build\bcc32\Debug\pe_rva2ptr.obj build\bcc32\Debug\pe_thunk.obj
|


build\bcc32\Debug\playlist.lib: build\bcc32\Debug\playlist_init.obj build\bcc32\Debug\playlist_m3u.obj build\bcc32\Debug\playlist_pls.obj build\bcc32\Debug\playlist_read.obj build\bcc32\Debug\playlist_write_entry.obj build\bcc32\Debug\playlist_write_finish.obj build\bcc32\Debug\playlist_write_start.obj build\bcc32\Debug\playlist_xspf.obj
	tlib @&&|
/a /u "$@" build\bcc32\Debug\playlist_init.obj build\bcc32\Debug\playlist_m3u.obj build\bcc32\Debug\playlist_pls.obj build\bcc32\Debug\playlist_read.obj build\bcc32\Debug\playlist_write_entry.obj build\bcc32\Debug\playlist_write_finish.obj build\bcc32\Debug\playlist_write_start.obj build\bcc32\Debug\playlist_xspf.obj
|


build\bcc32\Debug\rdir.lib: build\bcc32\Debug\rdir_close.obj build\bcc32\Debug\rdir_open.obj build\bcc32\Debug\rdir_read.obj
	tlib @&&|
/a /u "$@" build\bcc32\Debug\rdir_close.obj build\bcc32\Debug\rdir_open.obj build\bcc32\Debug\rdir_read.obj
|


build\bcc32\Debug\scan.lib: build\bcc32\Debug\scan_8int.obj build\bcc32\Debug\scan_8long.obj build\bcc32\Debug\scan_octal.obj build\bcc32\Debug\scan_8longn.obj build\bcc32\Debug\scan_8short.obj build\bcc32\Debug\scan_asn1derlength.obj build\bcc32\Debug\scan_asn1dertag.obj build\bcc32\Debug\scan_charsetnskip.obj build\bcc32\Debug\scan_double.obj build\bcc32\Debug\scan_fromhex.obj build\bcc32\Debug\scan_httpdate.obj build\bcc32\Debug\scan_int.obj build\bcc32\Debug\scan_long.obj build\bcc32\Debug\scan_longlong.obj build\bcc32\Debug\scan_longn.obj build\bcc32\Debug\scan_netstring.obj build\bcc32\Debug\scan_noncharsetnskip.obj build\bcc32\Debug\scan_nonwhitenskip.obj build\bcc32\Debug\scan_pb_tag.obj build\bcc32\Debug\scan_pb_type0_sint.obj build\bcc32\Debug\scan_pb_type1_fixed64.obj build\bcc32\Debug\scan_pb_type5_fixed32.obj build\bcc32\Debug\scan_plusminus.obj build\bcc32\Debug\scan_short.obj build\bcc32\Debug\scan_uint.obj build\bcc32\Debug\scan_ulong.obj build\bcc32\Debug\scan_ulonglong.obj build\bcc32\Debug\scan_ulongn.obj build\bcc32\Debug\scan_ushort.obj build\bcc32\Debug\scan_utf8.obj build\bcc32\Debug\scan_varint.obj build\bcc32\Debug\scan_whitenskip.obj build\bcc32\Debug\scan_xint.obj build\bcc32\Debug\scan_xlong.obj build\bcc32\Debug\scan_xlonglong.obj build\bcc32\Debug\scan_xlongn.obj build\bcc32\Debug\scan_xmlescape.obj build\bcc32\Debug\scan_xshort.obj
	tlib @&&|
/a /u "$@" build\bcc32\Debug\scan_8int.obj build\bcc32\Debug\scan_8long.obj build\bcc32\Debug\scan_octal.obj build\bcc32\Debug\scan_8longn.obj build\bcc32\Debug\scan_8short.obj build\bcc32\Debug\scan_asn1derlength.obj build\bcc32\Debug\scan_asn1dertag.obj build\bcc32\Debug\scan_charsetnskip.obj build\bcc32\Debug\scan_double.obj build\bcc32\Debug\scan_fromhex.obj build\bcc32\Debug\scan_httpdate.obj build\bcc32\Debug\scan_int.obj build\bcc32\Debug\scan_long.obj build\bcc32\Debug\scan_longlong.obj build\bcc32\Debug\scan_longn.obj build\bcc32\Debug\scan_netstring.obj build\bcc32\Debug\scan_noncharsetnskip.obj build\bcc32\Debug\scan_nonwhitenskip.obj build\bcc32\Debug\scan_pb_tag.obj build\bcc32\Debug\scan_pb_type0_sint.obj build\bcc32\Debug\scan_pb_type1_fixed64.obj build\bcc32\Debug\scan_pb_type5_fixed32.obj build\bcc32\Debug\scan_plusminus.obj build\bcc32\Debug\scan_short.obj build\bcc32\Debug\scan_uint.obj build\bcc32\Debug\scan_ulong.obj build\bcc32\Debug\scan_ulonglong.obj build\bcc32\Debug\scan_ulongn.obj build\bcc32\Debug\scan_ushort.obj build\bcc32\Debug\scan_utf8.obj build\bcc32\Debug\scan_varint.obj build\bcc32\Debug\scan_whitenskip.obj build\bcc32\Debug\scan_xint.obj build\bcc32\Debug\scan_xlong.obj build\bcc32\Debug\scan_xlonglong.obj build\bcc32\Debug\scan_xlongn.obj build\bcc32\Debug\scan_xmlescape.obj build\bcc32\Debug\scan_xshort.obj
|


build\bcc32\Debug\sig.lib: build\bcc32\Debug\sig_block.obj build\bcc32\Debug\sig_unblock.obj
	tlib @&&|
/a /u "$@" build\bcc32\Debug\sig_block.obj build\bcc32\Debug\sig_unblock.obj
|


build\bcc32\Debug\slist.lib: build\bcc32\Debug\slist_add_after.obj build\bcc32\Debug\slist_add_before.obj build\bcc32\Debug\slist_find.obj build\bcc32\Debug\slist_find_remove.obj build\bcc32\Debug\slist_init.obj build\bcc32\Debug\slist_length.obj build\bcc32\Debug\slist_move_head.obj build\bcc32\Debug\slist_move_tail.obj build\bcc32\Debug\slist_push.obj build\bcc32\Debug\slist_pushs.obj build\bcc32\Debug\slist_remove.obj build\bcc32\Debug\slist_unshift.obj
	tlib @&&|
/a /u "$@" build\bcc32\Debug\slist_add_after.obj build\bcc32\Debug\slist_add_before.obj build\bcc32\Debug\slist_find.obj build\bcc32\Debug\slist_find_remove.obj build\bcc32\Debug\slist_init.obj build\bcc32\Debug\slist_length.obj build\bcc32\Debug\slist_move_head.obj build\bcc32\Debug\slist_move_tail.obj build\bcc32\Debug\slist_push.obj build\bcc32\Debug\slist_pushs.obj build\bcc32\Debug\slist_remove.obj build\bcc32\Debug\slist_unshift.obj
|


build\bcc32\Debug\socket.lib: build\bcc32\Debug\fmt_ip4.obj build\bcc32\Debug\fmt_ip6.obj build\bcc32\Debug\fmt_ip6_flat.obj build\bcc32\Debug\fmt_ip6c.obj build\bcc32\Debug\fmt_ip6if.obj build\bcc32\Debug\fmt_ip6ifc.obj build\bcc32\Debug\init.obj build\bcc32\Debug\scan_ip4.obj build\bcc32\Debug\scan_ip6.obj build\bcc32\Debug\scan_ip6_flat.obj build\bcc32\Debug\scan_ip6if.obj build\bcc32\Debug\socket_accept4.obj build\bcc32\Debug\socket_accept6.obj build\bcc32\Debug\socket_bind4.obj build\bcc32\Debug\socket_bind4_reuse.obj build\bcc32\Debug\socket_bind6.obj build\bcc32\Debug\socket_bind6_reuse.obj build\bcc32\Debug\socket_broadcast.obj build\bcc32\Debug\socket_connect4.obj build\bcc32\Debug\socket_connect6.obj build\bcc32\Debug\socket_connected.obj build\bcc32\Debug\socket_deferaccept.obj build\bcc32\Debug\socket_fastopen.obj build\bcc32\Debug\socket_fastopen_connect4.obj build\bcc32\Debug\socket_fastopen_connect6.obj build\bcc32\Debug\socket_getifidx.obj build\bcc32\Debug\socket_getifname.obj build\bcc32\Debug\socket_ip4loopback.obj build\bcc32\Debug\socket_listen.obj build\bcc32\Debug\socket_local4.obj build\bcc32\Debug\socket_local6.obj build\bcc32\Debug\socket_mchopcount6.obj build\bcc32\Debug\socket_mcjoin4.obj build\bcc32\Debug\socket_mcjoin6.obj build\bcc32\Debug\socket_mcleave4.obj build\bcc32\Debug\socket_mcleave6.obj build\bcc32\Debug\socket_mcloop4.obj build\bcc32\Debug\socket_mcloop6.obj build\bcc32\Debug\socket_mcttl4.obj build\bcc32\Debug\socket_noipv6.obj build\bcc32\Debug\socket_quickack.obj build\bcc32\Debug\socket_recv4.obj build\bcc32\Debug\socket_recv6.obj build\bcc32\Debug\socket_remote4.obj build\bcc32\Debug\socket_remote6.obj build\bcc32\Debug\socket_sctp4.obj build\bcc32\Debug\socket_sctp4b.obj build\bcc32\Debug\socket_sctp6.obj build\bcc32\Debug\socket_sctp6b.obj build\bcc32\Debug\socket_send4.obj build\bcc32\Debug\socket_send6.obj build\bcc32\Debug\socket_tcp4.obj build\bcc32\Debug\socket_tcp4b.obj build\bcc32\Debug\socket_tcp6.obj build\bcc32\Debug\socket_tcp6b.obj build\bcc32\Debug\socket_tryreservein.obj build\bcc32\Debug\socket_udp4.obj build\bcc32\Debug\socket_udp6.obj build\bcc32\Debug\socket_v4mappedprefix.obj build\bcc32\Debug\socket_v6any.obj build\bcc32\Debug\socket_v6loopback.obj build\bcc32\Debug\winsock2errno.obj build\bcc32\Debug\winsock_init.obj build\bcc32\Debug\winsock_socketpair.obj
	tlib @&&|
/a /u "$@" build\bcc32\Debug\fmt_ip4.obj build\bcc32\Debug\fmt_ip6.obj build\bcc32\Debug\fmt_ip6_flat.obj build\bcc32\Debug\fmt_ip6c.obj build\bcc32\Debug\fmt_ip6if.obj build\bcc32\Debug\fmt_ip6ifc.obj build\bcc32\Debug\init.obj build\bcc32\Debug\scan_ip4.obj build\bcc32\Debug\scan_ip6.obj build\bcc32\Debug\scan_ip6_flat.obj build\bcc32\Debug\scan_ip6if.obj build\bcc32\Debug\socket_accept4.obj build\bcc32\Debug\socket_accept6.obj build\bcc32\Debug\socket_bind4.obj build\bcc32\Debug\socket_bind4_reuse.obj build\bcc32\Debug\socket_bind6.obj build\bcc32\Debug\socket_bind6_reuse.obj build\bcc32\Debug\socket_broadcast.obj build\bcc32\Debug\socket_connect4.obj build\bcc32\Debug\socket_connect6.obj build\bcc32\Debug\socket_connected.obj build\bcc32\Debug\socket_deferaccept.obj build\bcc32\Debug\socket_fastopen.obj build\bcc32\Debug\socket_fastopen_connect4.obj build\bcc32\Debug\socket_fastopen_connect6.obj build\bcc32\Debug\socket_getifidx.obj build\bcc32\Debug\socket_getifname.obj build\bcc32\Debug\socket_ip4loopback.obj build\bcc32\Debug\socket_listen.obj build\bcc32\Debug\socket_local4.obj build\bcc32\Debug\socket_local6.obj build\bcc32\Debug\socket_mchopcount6.obj build\bcc32\Debug\socket_mcjoin4.obj build\bcc32\Debug\socket_mcjoin6.obj build\bcc32\Debug\socket_mcleave4.obj build\bcc32\Debug\socket_mcleave6.obj build\bcc32\Debug\socket_mcloop4.obj build\bcc32\Debug\socket_mcloop6.obj build\bcc32\Debug\socket_mcttl4.obj build\bcc32\Debug\socket_noipv6.obj build\bcc32\Debug\socket_quickack.obj build\bcc32\Debug\socket_recv4.obj build\bcc32\Debug\socket_recv6.obj build\bcc32\Debug\socket_remote4.obj build\bcc32\Debug\socket_remote6.obj build\bcc32\Debug\socket_sctp4.obj build\bcc32\Debug\socket_sctp4b.obj build\bcc32\Debug\socket_sctp6.obj build\bcc32\Debug\socket_sctp6b.obj build\bcc32\Debug\socket_send4.obj build\bcc32\Debug\socket_send6.obj build\bcc32\Debug\socket_tcp4.obj build\bcc32\Debug\socket_tcp4b.obj build\bcc32\Debug\socket_tcp6.obj build\bcc32\Debug\socket_tcp6b.obj build\bcc32\Debug\socket_tryreservein.obj build\bcc32\Debug\socket_udp4.obj build\bcc32\Debug\socket_udp6.obj build\bcc32\Debug\socket_v4mappedprefix.obj build\bcc32\Debug\socket_v6any.obj build\bcc32\Debug\socket_v6loopback.obj build\bcc32\Debug\winsock2errno.obj build\bcc32\Debug\winsock_init.obj build\bcc32\Debug\winsock_socketpair.obj
|


build\bcc32\Debug\str.lib: build\bcc32\Debug\str_basename.obj build\bcc32\Debug\str_case_diff.obj build\bcc32\Debug\str_case_diffn.obj build\bcc32\Debug\str_case_equal.obj build\bcc32\Debug\str_case_start.obj build\bcc32\Debug\str_cat.obj build\bcc32\Debug\str_chr.obj build\bcc32\Debug\str_copy.obj build\bcc32\Debug\str_copyb.obj build\bcc32\Debug\str_copyn.obj build\bcc32\Debug\str_decamelize.obj build\bcc32\Debug\str_diff.obj build\bcc32\Debug\str_diffn.obj build\bcc32\Debug\str_dup.obj build\bcc32\Debug\str_equal.obj build\bcc32\Debug\str_find.obj build\bcc32\Debug\str_findb.obj build\bcc32\Debug\str_istr.obj build\bcc32\Debug\str_len.obj build\bcc32\Debug\str_lower.obj build\bcc32\Debug\str_ndup.obj build\bcc32\Debug\str_ptime.obj build\bcc32\Debug\str_rchr.obj build\bcc32\Debug\str_rchrs.obj build\bcc32\Debug\str_start.obj build\bcc32\Debug\str_tok.obj build\bcc32\Debug\isleap.obj build\bcc32\Debug\time_table_spd.obj
	tlib @&&|
/a /u "$@" build\bcc32\Debug\str_basename.obj build\bcc32\Debug\str_case_diff.obj build\bcc32\Debug\str_case_diffn.obj build\bcc32\Debug\str_case_equal.obj build\bcc32\Debug\str_case_start.obj build\bcc32\Debug\str_cat.obj build\bcc32\Debug\str_chr.obj build\bcc32\Debug\str_copy.obj build\bcc32\Debug\str_copyb.obj build\bcc32\Debug\str_copyn.obj build\bcc32\Debug\str_decamelize.obj build\bcc32\Debug\str_diff.obj build\bcc32\Debug\str_diffn.obj build\bcc32\Debug\str_dup.obj build\bcc32\Debug\str_equal.obj build\bcc32\Debug\str_find.obj build\bcc32\Debug\str_findb.obj build\bcc32\Debug\str_istr.obj build\bcc32\Debug\str_len.obj build\bcc32\Debug\str_lower.obj build\bcc32\Debug\str_ndup.obj build\bcc32\Debug\str_ptime.obj build\bcc32\Debug\str_rchr.obj build\bcc32\Debug\str_rchrs.obj build\bcc32\Debug\str_start.obj build\bcc32\Debug\str_tok.obj build\bcc32\Debug\isleap.obj build\bcc32\Debug\time_table_spd.obj
|


build\bcc32\Debug\stralloc.lib: build\bcc32\Debug\mmap_filename.obj build\bcc32\Debug\stralloc_0.obj build\bcc32\Debug\stralloc_append.obj build\bcc32\Debug\stralloc_buffer.obj build\bcc32\Debug\stralloc_case_diff.obj build\bcc32\Debug\stralloc_case_diffs.obj build\bcc32\Debug\stralloc_case_end.obj build\bcc32\Debug\stralloc_case_equal.obj build\bcc32\Debug\stralloc_case_equals.obj build\bcc32\Debug\stralloc_case_start.obj build\bcc32\Debug\stralloc_case_starts.obj build\bcc32\Debug\stralloc_cat.obj build\bcc32\Debug\stralloc_catb.obj build\bcc32\Debug\stralloc_catc.obj build\bcc32\Debug\stralloc_catdouble.obj build\bcc32\Debug\stralloc_cathexb.obj build\bcc32\Debug\stralloc_catint.obj build\bcc32\Debug\stralloc_catint0.obj build\bcc32\Debug\stralloc_catlong.obj build\bcc32\Debug\stralloc_catlong0.obj build\bcc32\Debug\stralloc_catm_internal.obj build\bcc32\Debug\stralloc_cats.obj build\bcc32\Debug\stralloc_catuint.obj build\bcc32\Debug\stralloc_catuint0.obj build\bcc32\Debug\stralloc_catulong.obj build\bcc32\Debug\stralloc_catulong0.obj build\bcc32\Debug\stralloc_catxlong.obj build\bcc32\Debug\stralloc_chomp.obj build\bcc32\Debug\stralloc_chop.obj build\bcc32\Debug\stralloc_chr.obj build\bcc32\Debug\stralloc_copy.obj build\bcc32\Debug\stralloc_copyb.obj build\bcc32\Debug\stralloc_copys.obj build\bcc32\Debug\stralloc_copywcb.obj build\bcc32\Debug\stralloc_copywcs.obj build\bcc32\Debug\stralloc_count.obj build\bcc32\Debug\stralloc_decamelize.obj build\bcc32\Debug\stralloc_diff.obj build\bcc32\Debug\stralloc_diffb.obj build\bcc32\Debug\stralloc_diffs.obj build\bcc32\Debug\stralloc_endb.obj build\bcc32\Debug\stralloc_ends.obj build\bcc32\Debug\stralloc_equal.obj build\bcc32\Debug\stralloc_equalb.obj build\bcc32\Debug\stralloc_equals.obj build\bcc32\Debug\stralloc_erase.obj build\bcc32\Debug\stralloc_expand.obj build\bcc32\Debug\stralloc_find.obj build\bcc32\Debug\stralloc_findb.obj build\bcc32\Debug\stralloc_finds.obj build\bcc32\Debug\stralloc_fmt.obj build\bcc32\Debug\stralloc_fmt_call.obj build\bcc32\Debug\stralloc_fmt_pred.obj build\bcc32\Debug\stralloc_free.obj build\bcc32\Debug\stralloc_init.obj build\bcc32\Debug\stralloc_insert.obj build\bcc32\Debug\stralloc_insertb.obj build\bcc32\Debug\stralloc_inserts.obj build\bcc32\Debug\stralloc_lower.obj build\bcc32\Debug\stralloc_move.obj build\bcc32\Debug\stralloc_nul.obj build\bcc32\Debug\stralloc_prepend.obj build\bcc32\Debug\stralloc_prependb.obj build\bcc32\Debug\stralloc_prepends.obj build\bcc32\Debug\stralloc_rchr.obj build\bcc32\Debug\stralloc_ready.obj build\bcc32\Debug\stralloc_ready_tuned.obj build\bcc32\Debug\stralloc_readyplus.obj build\bcc32\Debug\stralloc_remove.obj build\bcc32\Debug\stralloc_remove_all.obj build\bcc32\Debug\stralloc_replace.obj build\bcc32\Debug\stralloc_replace_non_printable.obj build\bcc32\Debug\stralloc_reverse.obj build\bcc32\Debug\stralloc_reverse_blocks.obj build\bcc32\Debug\stralloc_scan.obj build\bcc32\Debug\stralloc_shrink.obj build\bcc32\Debug\stralloc_start.obj build\bcc32\Debug\stralloc_startb.obj build\bcc32\Debug\stralloc_starts.obj build\bcc32\Debug\stralloc_subst.obj build\bcc32\Debug\stralloc_trim.obj build\bcc32\Debug\stralloc_trunc.obj build\bcc32\Debug\stralloc_upper.obj build\bcc32\Debug\stralloc_write.obj build\bcc32\Debug\stralloc_zero.obj
	tlib @&&|
/a /u "$@" build\bcc32\Debug\mmap_filename.obj build\bcc32\Debug\stralloc_0.obj build\bcc32\Debug\stralloc_append.obj build\bcc32\Debug\stralloc_buffer.obj build\bcc32\Debug\stralloc_case_diff.obj build\bcc32\Debug\stralloc_case_diffs.obj build\bcc32\Debug\stralloc_case_end.obj build\bcc32\Debug\stralloc_case_equal.obj build\bcc32\Debug\stralloc_case_equals.obj build\bcc32\Debug\stralloc_case_start.obj build\bcc32\Debug\stralloc_case_starts.obj build\bcc32\Debug\stralloc_cat.obj build\bcc32\Debug\stralloc_catb.obj build\bcc32\Debug\stralloc_catc.obj build\bcc32\Debug\stralloc_catdouble.obj build\bcc32\Debug\stralloc_cathexb.obj build\bcc32\Debug\stralloc_catint.obj build\bcc32\Debug\stralloc_catint0.obj build\bcc32\Debug\stralloc_catlong.obj build\bcc32\Debug\stralloc_catlong0.obj build\bcc32\Debug\stralloc_catm_internal.obj build\bcc32\Debug\stralloc_cats.obj build\bcc32\Debug\stralloc_catuint.obj build\bcc32\Debug\stralloc_catuint0.obj build\bcc32\Debug\stralloc_catulong.obj build\bcc32\Debug\stralloc_catulong0.obj build\bcc32\Debug\stralloc_catxlong.obj build\bcc32\Debug\stralloc_chomp.obj build\bcc32\Debug\stralloc_chop.obj build\bcc32\Debug\stralloc_chr.obj build\bcc32\Debug\stralloc_copy.obj build\bcc32\Debug\stralloc_copyb.obj build\bcc32\Debug\stralloc_copys.obj build\bcc32\Debug\stralloc_copywcb.obj build\bcc32\Debug\stralloc_copywcs.obj build\bcc32\Debug\stralloc_count.obj build\bcc32\Debug\stralloc_decamelize.obj build\bcc32\Debug\stralloc_diff.obj build\bcc32\Debug\stralloc_diffb.obj build\bcc32\Debug\stralloc_diffs.obj build\bcc32\Debug\stralloc_endb.obj build\bcc32\Debug\stralloc_ends.obj build\bcc32\Debug\stralloc_equal.obj build\bcc32\Debug\stralloc_equalb.obj build\bcc32\Debug\stralloc_equals.obj build\bcc32\Debug\stralloc_erase.obj build\bcc32\Debug\stralloc_expand.obj build\bcc32\Debug\stralloc_find.obj build\bcc32\Debug\stralloc_findb.obj build\bcc32\Debug\stralloc_finds.obj build\bcc32\Debug\stralloc_fmt.obj build\bcc32\Debug\stralloc_fmt_call.obj build\bcc32\Debug\stralloc_fmt_pred.obj build\bcc32\Debug\stralloc_free.obj build\bcc32\Debug\stralloc_init.obj build\bcc32\Debug\stralloc_insert.obj build\bcc32\Debug\stralloc_insertb.obj build\bcc32\Debug\stralloc_inserts.obj build\bcc32\Debug\stralloc_lower.obj build\bcc32\Debug\stralloc_move.obj build\bcc32\Debug\stralloc_nul.obj build\bcc32\Debug\stralloc_prepend.obj build\bcc32\Debug\stralloc_prependb.obj build\bcc32\Debug\stralloc_prepends.obj build\bcc32\Debug\stralloc_rchr.obj build\bcc32\Debug\stralloc_ready.obj build\bcc32\Debug\stralloc_ready_tuned.obj build\bcc32\Debug\stralloc_readyplus.obj build\bcc32\Debug\stralloc_remove.obj build\bcc32\Debug\stralloc_remove_all.obj build\bcc32\Debug\stralloc_replace.obj build\bcc32\Debug\stralloc_replace_non_printable.obj build\bcc32\Debug\stralloc_reverse.obj build\bcc32\Debug\stralloc_reverse_blocks.obj build\bcc32\Debug\stralloc_scan.obj build\bcc32\Debug\stralloc_shrink.obj build\bcc32\Debug\stralloc_start.obj build\bcc32\Debug\stralloc_startb.obj build\bcc32\Debug\stralloc_starts.obj build\bcc32\Debug\stralloc_subst.obj build\bcc32\Debug\stralloc_trim.obj build\bcc32\Debug\stralloc_trunc.obj build\bcc32\Debug\stralloc_upper.obj build\bcc32\Debug\stralloc_write.obj build\bcc32\Debug\stralloc_zero.obj
|


build\bcc32\Debug\strarray.lib: build\bcc32\Debug\fnmatch.obj build\bcc32\Debug\strarray_appends.obj build\bcc32\Debug\strarray_from_argv.obj build\bcc32\Debug\strarray_index_of.obj build\bcc32\Debug\strarray_joins.obj build\bcc32\Debug\strarray_pop.obj build\bcc32\Debug\strarray_prepends.obj build\bcc32\Debug\strarray_push.obj build\bcc32\Debug\strarray_push_sa.obj build\bcc32\Debug\strarray_pushb.obj build\bcc32\Debug\strarray_pushd.obj build\bcc32\Debug\strarray_removeprefixs.obj build\bcc32\Debug\strarray_removesuffixs.obj build\bcc32\Debug\strarray_set.obj build\bcc32\Debug\strarray_sort.obj build\bcc32\Debug\strarray_to_argv.obj build\bcc32\Debug\strarray_transform.obj
	tlib @&&|
/a /u "$@" build\bcc32\Debug\fnmatch.obj build\bcc32\Debug\strarray_appends.obj build\bcc32\Debug\strarray_from_argv.obj build\bcc32\Debug\strarray_index_of.obj build\bcc32\Debug\strarray_joins.obj build\bcc32\Debug\strarray_pop.obj build\bcc32\Debug\strarray_prepends.obj build\bcc32\Debug\strarray_push.obj build\bcc32\Debug\strarray_push_sa.obj build\bcc32\Debug\strarray_pushb.obj build\bcc32\Debug\strarray_pushd.obj build\bcc32\Debug\strarray_removeprefixs.obj build\bcc32\Debug\strarray_removesuffixs.obj build\bcc32\Debug\strarray_set.obj build\bcc32\Debug\strarray_sort.obj build\bcc32\Debug\strarray_to_argv.obj build\bcc32\Debug\strarray_transform.obj
|


build\bcc32\Debug\strlist.lib: build\bcc32\Debug\strlist_append_sa.obj build\bcc32\Debug\strlist_at.obj build\bcc32\Debug\strlist_at_n.obj build\bcc32\Debug\strlist_at_sa.obj build\bcc32\Debug\strlist_cat.obj build\bcc32\Debug\strlist_contains.obj build\bcc32\Debug\strlist_contains_sa.obj build\bcc32\Debug\strlist_count.obj build\bcc32\Debug\strlist_dump.obj build\bcc32\Debug\strlist_froms.obj build\bcc32\Debug\strlist_index_of.obj build\bcc32\Debug\strlist_join.obj build\bcc32\Debug\strlist_joins.obj build\bcc32\Debug\strlist_prepend_sa.obj build\bcc32\Debug\strlist_prependb.obj build\bcc32\Debug\strlist_prepends.obj build\bcc32\Debug\strlist_push.obj build\bcc32\Debug\strlist_push_sa.obj build\bcc32\Debug\strlist_push_tokens.obj build\bcc32\Debug\strlist_push_unique.obj build\bcc32\Debug\strlist_push_unique_sa.obj build\bcc32\Debug\strlist_pushb.obj build\bcc32\Debug\strlist_pushm_internal.obj build\bcc32\Debug\strlist_range.obj build\bcc32\Debug\strlist_shift.obj build\bcc32\Debug\strlist_shift_n.obj build\bcc32\Debug\strlist_sort.obj build\bcc32\Debug\strlist_to_argv.obj build\bcc32\Debug\strlist_unshift.obj
	tlib @&&|
/a /u "$@" build\bcc32\Debug\strlist_append_sa.obj build\bcc32\Debug\strlist_at.obj build\bcc32\Debug\strlist_at_n.obj build\bcc32\Debug\strlist_at_sa.obj build\bcc32\Debug\strlist_cat.obj build\bcc32\Debug\strlist_contains.obj build\bcc32\Debug\strlist_contains_sa.obj build\bcc32\Debug\strlist_count.obj build\bcc32\Debug\strlist_dump.obj build\bcc32\Debug\strlist_froms.obj build\bcc32\Debug\strlist_index_of.obj build\bcc32\Debug\strlist_join.obj build\bcc32\Debug\strlist_joins.obj build\bcc32\Debug\strlist_prepend_sa.obj build\bcc32\Debug\strlist_prependb.obj build\bcc32\Debug\strlist_prepends.obj build\bcc32\Debug\strlist_push.obj build\bcc32\Debug\strlist_push_sa.obj build\bcc32\Debug\strlist_push_tokens.obj build\bcc32\Debug\strlist_push_unique.obj build\bcc32\Debug\strlist_push_unique_sa.obj build\bcc32\Debug\strlist_pushb.obj build\bcc32\Debug\strlist_pushm_internal.obj build\bcc32\Debug\strlist_range.obj build\bcc32\Debug\strlist_shift.obj build\bcc32\Debug\strlist_shift_n.obj build\bcc32\Debug\strlist_sort.obj build\bcc32\Debug\strlist_to_argv.obj build\bcc32\Debug\strlist_unshift.obj
|


build\bcc32\Debug\tai.lib: build\bcc32\Debug\tai_add.obj build\bcc32\Debug\tai_now.obj build\bcc32\Debug\tai_pack.obj build\bcc32\Debug\tai_sub.obj build\bcc32\Debug\tai_uint.obj build\bcc32\Debug\tai_unpack.obj
	tlib @&&|
/a /u "$@" build\bcc32\Debug\tai_add.obj build\bcc32\Debug\tai_now.obj build\bcc32\Debug\tai_pack.obj build\bcc32\Debug\tai_sub.obj build\bcc32\Debug\tai_uint.obj build\bcc32\Debug\tai_unpack.obj
|


build\bcc32\Debug\taia.lib: build\bcc32\Debug\taia_add.obj build\bcc32\Debug\taia_addsec.obj build\bcc32\Debug\taia_approx.obj build\bcc32\Debug\taia_frac.obj build\bcc32\Debug\taia_half.obj build\bcc32\Debug\taia_less.obj build\bcc32\Debug\taia_now.obj build\bcc32\Debug\taia_pack.obj build\bcc32\Debug\taia_sub.obj build\bcc32\Debug\taia_tai.obj build\bcc32\Debug\taia_uint.obj build\bcc32\Debug\taia_unpack.obj
	tlib @&&|
/a /u "$@" build\bcc32\Debug\taia_add.obj build\bcc32\Debug\taia_addsec.obj build\bcc32\Debug\taia_approx.obj build\bcc32\Debug\taia_frac.obj build\bcc32\Debug\taia_half.obj build\bcc32\Debug\taia_less.obj build\bcc32\Debug\taia_now.obj build\bcc32\Debug\taia_pack.obj build\bcc32\Debug\taia_sub.obj build\bcc32\Debug\taia_tai.obj build\bcc32\Debug\taia_uint.obj build\bcc32\Debug\taia_unpack.obj
|


build\bcc32\Debug\textbuf.lib: build\bcc32\Debug\is_textbuf.obj build\bcc32\Debug\textbuf_column.obj build\bcc32\Debug\textbuf_free.obj build\bcc32\Debug\textbuf_init.obj build\bcc32\Debug\textbuf_line.obj build\bcc32\Debug\textbuf_read.obj
	tlib @&&|
/a /u "$@" build\bcc32\Debug\is_textbuf.obj build\bcc32\Debug\textbuf_column.obj build\bcc32\Debug\textbuf_free.obj build\bcc32\Debug\textbuf_init.obj build\bcc32\Debug\textbuf_line.obj build\bcc32\Debug\textbuf_read.obj
|


build\bcc32\Debug\uint16.lib: build\bcc32\Debug\uint16_pack.obj build\bcc32\Debug\uint16_pack_big.obj build\bcc32\Debug\uint16_read.obj build\bcc32\Debug\uint16_read_big.obj build\bcc32\Debug\uint16_unpack.obj build\bcc32\Debug\uint16_unpack_big.obj
	tlib @&&|
/a /u "$@" build\bcc32\Debug\uint16_pack.obj build\bcc32\Debug\uint16_pack_big.obj build\bcc32\Debug\uint16_read.obj build\bcc32\Debug\uint16_read_big.obj build\bcc32\Debug\uint16_unpack.obj build\bcc32\Debug\uint16_unpack_big.obj
|


build\bcc32\Debug\uint32.lib: build\bcc32\Debug\uint32_pack.obj build\bcc32\Debug\uint32_pack_big.obj build\bcc32\Debug\uint32_read.obj build\bcc32\Debug\uint32_read_big.obj build\bcc32\Debug\uint32_unpack.obj build\bcc32\Debug\uint32_unpack_big.obj
	tlib @&&|
/a /u "$@" build\bcc32\Debug\uint32_pack.obj build\bcc32\Debug\uint32_pack_big.obj build\bcc32\Debug\uint32_read.obj build\bcc32\Debug\uint32_read_big.obj build\bcc32\Debug\uint32_unpack.obj build\bcc32\Debug\uint32_unpack_big.obj
|


build\bcc32\Debug\uint64.lib: build\bcc32\Debug\uint64_pack.obj build\bcc32\Debug\uint64_pack_big.obj build\bcc32\Debug\uint64_read.obj build\bcc32\Debug\uint64_read_big.obj build\bcc32\Debug\uint64_unpack.obj build\bcc32\Debug\uint64_unpack_big.obj
	tlib @&&|
/a /u "$@" build\bcc32\Debug\uint64_pack.obj build\bcc32\Debug\uint64_pack_big.obj build\bcc32\Debug\uint64_read.obj build\bcc32\Debug\uint64_read_big.obj build\bcc32\Debug\uint64_unpack.obj build\bcc32\Debug\uint64_unpack_big.obj
|


build\bcc32\Debug\var.lib: build\bcc32\Debug\var_bsearch.obj build\bcc32\Debug\var_chflg.obj build\bcc32\Debug\var_cleanup.obj build\bcc32\Debug\var_copys.obj build\bcc32\Debug\var_count.obj build\bcc32\Debug\var_create.obj build\bcc32\Debug\var_dump.obj build\bcc32\Debug\var_export.obj build\bcc32\Debug\var_get.obj build\bcc32\Debug\var_hsearch.obj build\bcc32\Debug\var_import.obj build\bcc32\Debug\var_init.obj build\bcc32\Debug\var_len.obj build\bcc32\Debug\var_lexhash.obj build\bcc32\Debug\var_print.obj build\bcc32\Debug\var_rndhash.obj build\bcc32\Debug\var_search.obj build\bcc32\Debug\var_set.obj build\bcc32\Debug\var_setsa.obj build\bcc32\Debug\var_setvint.obj build\bcc32\Debug\var_setvsa.obj build\bcc32\Debug\var_unset.obj build\bcc32\Debug\var_valid.obj build\bcc32\Debug\var_value.obj build\bcc32\Debug\var_vdefault.obj build\bcc32\Debug\var_vlen.obj
	tlib @&&|
/a /u "$@" build\bcc32\Debug\var_bsearch.obj build\bcc32\Debug\var_chflg.obj build\bcc32\Debug\var_cleanup.obj build\bcc32\Debug\var_copys.obj build\bcc32\Debug\var_count.obj build\bcc32\Debug\var_create.obj build\bcc32\Debug\var_dump.obj build\bcc32\Debug\var_export.obj build\bcc32\Debug\var_get.obj build\bcc32\Debug\var_hsearch.obj build\bcc32\Debug\var_import.obj build\bcc32\Debug\var_init.obj build\bcc32\Debug\var_len.obj build\bcc32\Debug\var_lexhash.obj build\bcc32\Debug\var_print.obj build\bcc32\Debug\var_rndhash.obj build\bcc32\Debug\var_search.obj build\bcc32\Debug\var_set.obj build\bcc32\Debug\var_setsa.obj build\bcc32\Debug\var_setvint.obj build\bcc32\Debug\var_setvsa.obj build\bcc32\Debug\var_unset.obj build\bcc32\Debug\var_valid.obj build\bcc32\Debug\var_value.obj build\bcc32\Debug\var_vdefault.obj build\bcc32\Debug\var_vlen.obj
|


build\bcc32\Debug\vartab.lib: build\bcc32\Debug\vartab_add.obj build\bcc32\Debug\vartab_cleanup.obj build\bcc32\Debug\vartab_dump.obj build\bcc32\Debug\vartab_hash.obj build\bcc32\Debug\vartab_pop.obj build\bcc32\Debug\vartab_print.obj build\bcc32\Debug\vartab_push.obj build\bcc32\Debug\vartab_root.obj build\bcc32\Debug\vartab_search.obj
	tlib @&&|
/a /u "$@" build\bcc32\Debug\vartab_add.obj build\bcc32\Debug\vartab_cleanup.obj build\bcc32\Debug\vartab_dump.obj build\bcc32\Debug\vartab_hash.obj build\bcc32\Debug\vartab_pop.obj build\bcc32\Debug\vartab_print.obj build\bcc32\Debug\vartab_push.obj build\bcc32\Debug\vartab_root.obj build\bcc32\Debug\vartab_search.obj
|


build\bcc32\Debug\xml.lib: build\bcc32\Debug\xml_add_child.obj build\bcc32\Debug\xml_attrnode.obj build\bcc32\Debug\xml_content.obj build\bcc32\Debug\xml_content_sa.obj build\bcc32\Debug\xml_debug.obj build\bcc32\Debug\xml_delete.obj build\bcc32\Debug\xml_element.obj build\bcc32\Debug\xml_escape.obj build\bcc32\Debug\xml_find_all_pred.obj build\bcc32\Debug\xml_find_element.obj build\bcc32\Debug\xml_find_element_attr.obj build\bcc32\Debug\xml_find_parent.obj build\bcc32\Debug\xml_find_pred.obj build\bcc32\Debug\xml_find_predicates.obj build\bcc32\Debug\xml_free.obj build\bcc32\Debug\xml_get_attribute.obj build\bcc32\Debug\xml_get_attribute_double.obj build\bcc32\Debug\xml_get_attribute_sa.obj build\bcc32\Debug\xml_get_document.obj build\bcc32\Debug\xml_has_attribute.obj build\bcc32\Debug\xml_insert.obj build\bcc32\Debug\xml_newnode.obj build\bcc32\Debug\xml_path.obj build\bcc32\Debug\xml_print.obj build\bcc32\Debug\xml_print_attributes.obj build\bcc32\Debug\xml_print_nodeset.obj build\bcc32\Debug\xml_read_callback.obj build\bcc32\Debug\xml_read_tree.obj build\bcc32\Debug\xml_reader_init.obj build\bcc32\Debug\xml_remove.obj build\bcc32\Debug\xml_root_element.obj build\bcc32\Debug\xml_set_attribute.obj build\bcc32\Debug\xml_set_attribute_double.obj build\bcc32\Debug\xml_textnode.obj
	tlib @&&|
/a /u "$@" build\bcc32\Debug\xml_add_child.obj build\bcc32\Debug\xml_attrnode.obj build\bcc32\Debug\xml_content.obj build\bcc32\Debug\xml_content_sa.obj build\bcc32\Debug\xml_debug.obj build\bcc32\Debug\xml_delete.obj build\bcc32\Debug\xml_element.obj build\bcc32\Debug\xml_escape.obj build\bcc32\Debug\xml_find_all_pred.obj build\bcc32\Debug\xml_find_element.obj build\bcc32\Debug\xml_find_element_attr.obj build\bcc32\Debug\xml_find_parent.obj build\bcc32\Debug\xml_find_pred.obj build\bcc32\Debug\xml_find_predicates.obj build\bcc32\Debug\xml_free.obj build\bcc32\Debug\xml_get_attribute.obj build\bcc32\Debug\xml_get_attribute_double.obj build\bcc32\Debug\xml_get_attribute_sa.obj build\bcc32\Debug\xml_get_document.obj build\bcc32\Debug\xml_has_attribute.obj build\bcc32\Debug\xml_insert.obj build\bcc32\Debug\xml_newnode.obj build\bcc32\Debug\xml_path.obj build\bcc32\Debug\xml_print.obj build\bcc32\Debug\xml_print_attributes.obj build\bcc32\Debug\xml_print_nodeset.obj build\bcc32\Debug\xml_read_callback.obj build\bcc32\Debug\xml_read_tree.obj build\bcc32\Debug\xml_reader_init.obj build\bcc32\Debug\xml_remove.obj build\bcc32\Debug\xml_root_element.obj build\bcc32\Debug\xml_set_attribute.obj build\bcc32\Debug\xml_set_attribute_double.obj build\bcc32\Debug\xml_textnode.obj
|
