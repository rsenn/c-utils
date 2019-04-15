$(BUILDDIR)jsonpp.o: jsonpp.c lib/json.h lib/charbuf.h lib/typedefs.h lib/byte.h lib/hmap.h lib/stralloc.h lib/buffer.h lib/slist.h lib/alloc.h lib/uint64.h lib/str.h lib/open.h lib/windoze.h lib/fmt.h lib/uint32.h lib/errmsg.h lib/str.h lib/scan.h lib/getopt.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)binfmttest.o: binfmttest.c lib/byte.h lib/typedefs.h lib/binfmt.h lib/uint8.h lib/uint32.h lib/uint64.h lib/range.h lib/mmap.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)jsontest.o: jsontest.c lib/windoze.h lib/charbuf.h lib/typedefs.h lib/byte.h lib/mmap.h lib/uint64.h lib/byte.h lib/fmt.h lib/uint32.h lib/hmap.h lib/stralloc.h lib/buffer.h lib/iterator.h lib/stralloc.h lib/json.h lib/charbuf.h lib/hmap.h lib/slist.h lib/alloc.h lib/str.h lib/open.h lib/windoze.h lib/io.h lib/iarray.h lib/taia.h lib/tai.h lib/iarray.h lib/str.h lib/scan.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)xmltest.o: xmltest.c lib/xml.h lib/buffer.h lib/typedefs.h lib/hmap.h lib/stralloc.h lib/textbuf.h lib/uint64.h lib/buffer.h lib/byte.h lib/fmt.h lib/uint32.h lib/hmap.h lib/iterator.h lib/stralloc.h lib/str.h lib/mmap.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)torrent-progress.o: torrent-progress.c lib/windoze.h lib/uint64.h lib/buffer.h lib/typedefs.h lib/io.h lib/windoze.h lib/iarray.h lib/uint64.h lib/taia.h lib/tai.h lib/uint32.h lib/iarray.h lib/mmap.h lib/open.h lib/stralloc.h lib/byte.h lib/fmt.h lib/str.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)opensearch-dump.o: opensearch-dump.c lib/buffer.h lib/typedefs.h lib/stralloc.h lib/xml.h lib/buffer.h lib/hmap.h lib/stralloc.h lib/textbuf.h lib/uint64.h lib/str.h lib/byte.h lib/fmt.h lib/uint32.h lib/scan.h lib/mmap.h lib/hmap.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ntldd.o: ntldd.c lib/windoze.h lib/uint64.h lib/buffer.h lib/typedefs.h lib/byte.h lib/unix.h lib/windoze.h lib/getopt.h lib/readlink.h lib/path.h lib/stralloc.h lib/str.h lib/str.h lib/pe.h lib/uint64.h lib/uint32.h lib/uint16.h lib/uint8.h lib/mmap.h lib/fmt.h lib/stralloc.h lib/strlist.h lib/byte.h lib/buffer.h libntldd.h lib/uint16.h lib/uint32.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)pkgcfg.o: pkgcfg.c lib/unix.h lib/typedefs.h lib/windoze.h lib/getopt.h lib/readlink.h lib/algorithm.h lib/iterator.h lib/buffer.h lib/byte.h lib/cbmap.h lib/uint8.h lib/dir.h lib/uint64.h lib/env.h lib/errmsg.h lib/iterator.h lib/path.h lib/stralloc.h lib/str.h lib/slist.h lib/alloc.h lib/str.h lib/stralloc.h lib/strarray.h lib/array.h lib/byte.h lib/strlist.h lib/buffer.h lib/wordexp.h lib/io.h lib/iarray.h lib/taia.h lib/tai.h lib/uint32.h lib/iarray.h lib/ndelay.h lib/mmap.h lib/array.h lib/socket.h lib/uint16.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cofflist.o: cofflist.c lib/unix.h lib/typedefs.h lib/windoze.h lib/getopt.h lib/readlink.h lib/uint64.h lib/buffer.h lib/mmap.h lib/uint64.h lib/coff.h lib/uint8.h lib/uint16.h lib/uint32.h lib/range.h lib/str.h lib/stralloc.h lib/byte.h lib/fmt.h lib/errmsg.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)genmakefile.o: genmakefile.c lib/windoze.h lib/unix.h lib/typedefs.h lib/windoze.h lib/getopt.h lib/readlink.h lib/hmap.h lib/stralloc.h lib/buffer.h lib/mmap.h lib/uint64.h lib/open.h lib/path.h lib/str.h lib/rdir.h lib/dir.h lib/dir_internal.h lib/scan.h lib/uint32.h lib/slist.h lib/alloc.h lib/str.h lib/stralloc.h lib/strarray.h lib/array.h lib/byte.h lib/strlist.h lib/uint32.h lib/errmsg.h lib/array.h lib/byte.h lib/fmt.h lib/dir.h lib/range.h lib/case.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)pathtool.o: pathtool.c lib/unix.h lib/typedefs.h lib/windoze.h lib/getopt.h lib/readlink.h lib/buffer.h lib/errmsg.h lib/path.h lib/stralloc.h lib/str.h lib/stralloc.h lib/strlist.h lib/byte.h lib/uint64.h lib/buffer.h lib/windoze.h lib/str.h lib/byte.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ziptest.o: ziptest.c lib/unix.h lib/typedefs.h lib/windoze.h lib/getopt.h lib/readlink.h lib/buffer.h lib/io.h lib/iarray.h lib/uint64.h lib/taia.h lib/tai.h lib/uint32.h lib/iarray.h lib/str.h lib/byte.h lib/mmap.h lib/open.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)mediathek-list.o: mediathek-list.c lib/buffer.h lib/typedefs.h lib/byte.h lib/fmt.h lib/uint32.h lib/uint64.h lib/unix.h lib/windoze.h lib/getopt.h lib/readlink.h lib/http.h lib/uint16.h lib/stralloc.h lib/buffer.h lib/ip4.h lib/uint8.h lib/io.h lib/iarray.h lib/taia.h lib/tai.h lib/iarray.h lib/scan.h lib/slist.h lib/alloc.h lib/str.h lib/stralloc.h lib/strarray.h lib/array.h lib/byte.h lib/strlist.h lib/windoze.h lib/errmsg.h lib/case.h lib/hmap.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)macho32list.o: macho32list.c lib/uint32.h lib/macho.h lib/uint8.h lib/uint16.h lib/uint32.h lib/uint64.h lib/mmap.h lib/typedefs.h lib/str.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)elflist.o: elflist.c lib/uint64.h lib/buffer.h lib/typedefs.h lib/elf.h lib/uint8.h lib/uint16.h lib/uint32.h lib/uint64.h lib/range.h lib/mmap.h lib/str.h lib/unix.h lib/windoze.h lib/getopt.h lib/readlink.h lib/getopt.h lib/range.h lib/rangecheck.h lib/byte.h lib/fmt.h lib/strlist.h lib/stralloc.h lib/byte.h lib/buffer.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)regfilter.o: regfilter.c lib/windoze.h lib/uint64.h lib/buffer.h lib/typedefs.h lib/byte.h lib/fmt.h lib/uint32.h lib/uint64.h lib/open.h lib/windoze.h lib/scan.h lib/str.h lib/stralloc.h lib/io.h lib/iarray.h lib/taia.h lib/tai.h lib/iarray.h lib/path.h lib/stralloc.h lib/str.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)sln.o: sln.c lib/windoze.h lib/buffer.h lib/typedefs.h lib/byte.h lib/errmsg.h lib/path.h lib/windoze.h lib/stralloc.h lib/str.h lib/str.h lib/stralloc.h lib/strarray.h lib/uint64.h lib/array.h lib/byte.h lib/mmap.h lib/getopt.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cc-wrap.o: cc-wrap.c lib/windoze.h lib/strlist.h lib/stralloc.h lib/typedefs.h lib/byte.h lib/uint64.h lib/buffer.h lib/strarray.h lib/array.h lib/stralloc.h lib/buffer.h lib/path.h lib/windoze.h lib/str.h lib/process.h lib/env.h lib/errmsg.h lib/byte.h lib/mmap.h lib/scan.h lib/uint32.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)xmltest4.o: xmltest4.c lib/buffer.h lib/typedefs.h lib/byte.h lib/fmt.h lib/uint32.h lib/uint64.h lib/hmap.h lib/stralloc.h lib/buffer.h lib/iterator.h lib/stralloc.h lib/xml.h lib/hmap.h lib/textbuf.h lib/str.h lib/mmap.h lib/scan.h lib/strlist.h lib/byte.h lib/array.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)eagle-to-circuit.o: eagle-to-circuit.c lib/windoze.h lib/buffer.h lib/typedefs.h lib/byte.h lib/cb.h lib/cbmap.h lib/uint8.h lib/fmt.h lib/uint32.h lib/uint64.h lib/hmap.h lib/stralloc.h lib/buffer.h lib/iterator.h lib/mmap.h lib/scan.h lib/str.h lib/stralloc.h lib/strlist.h lib/byte.h lib/xml.h lib/hmap.h lib/textbuf.h lib/array.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)dnstest.o: dnstest.c lib/socket.h lib/typedefs.h lib/uint16.h lib/uint32.h lib/io.h lib/windoze.h lib/iarray.h lib/uint64.h lib/taia.h lib/tai.h lib/iarray.h lib/case.h lib/open.h lib/stralloc.h lib/scan.h lib/fmt.h lib/str.h lib/tai.h lib/taia.h lib/buffer.h lib/dns.h lib/iopause.h lib/io.h lib/stralloc.h lib/errmsg.h lib/ip4.h lib/uint8.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bsdiffcat.o: bsdiffcat.c lib/windoze.h lib/array.h lib/typedefs.h lib/uint64.h lib/uint64.h lib/buffer.h lib/fmt.h lib/uint32.h lib/io.h lib/windoze.h lib/iarray.h lib/taia.h lib/tai.h lib/iarray.h lib/mmap.h lib/open.h lib/byte.h lib/str.h lib/errmsg.h lib/uint32.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)elfwrsec.o: elfwrsec.c lib/buffer.h lib/typedefs.h lib/elf.h lib/uint8.h lib/uint16.h lib/uint32.h lib/uint64.h lib/range.h lib/mmap.h lib/str.h lib/byte.h lib/fmt.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)pelist.o: pelist.c lib/unix.h lib/typedefs.h lib/windoze.h lib/getopt.h lib/readlink.h lib/uint64.h lib/buffer.h lib/mmap.h lib/uint64.h lib/pe.h lib/uint32.h lib/uint16.h lib/uint8.h lib/str.h lib/fmt.h lib/byte.h lib/errmsg.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)omflist.o: omflist.c lib/buffer.h lib/typedefs.h lib/unix.h lib/windoze.h lib/getopt.h lib/readlink.h lib/str.h lib/errmsg.h lib/omf.h lib/uint8.h lib/uint16.h lib/range.h lib/fmt.h lib/uint32.h lib/uint64.h lib/mmap.h lib/byte.h lib/stralloc.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cmake-run.o: cmake-run.c lib/windoze.h lib/strlist.h lib/stralloc.h lib/typedefs.h lib/byte.h lib/uint64.h lib/buffer.h lib/strarray.h lib/array.h lib/open.h lib/windoze.h lib/stralloc.h lib/buffer.h lib/path.h lib/str.h lib/process.h lib/env.h lib/errmsg.h lib/byte.h lib/mmap.h lib/scan.h lib/uint32.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)reg2cmd.o: reg2cmd.c lib/windoze.h lib/io.h lib/windoze.h lib/iarray.h lib/uint64.h lib/typedefs.h lib/taia.h lib/tai.h lib/uint32.h lib/iarray.h lib/stralloc.h lib/uint64.h lib/buffer.h lib/open.h lib/fmt.h lib/byte.h lib/str.h lib/scan.h lib/path.h lib/stralloc.h lib/str.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)hexedit.o: hexedit.c lib/windoze.h lib/unix.h lib/typedefs.h lib/windoze.h lib/getopt.h lib/readlink.h lib/io.h lib/iarray.h lib/uint64.h lib/taia.h lib/tai.h lib/uint32.h lib/iarray.h lib/array.h lib/buffer.h lib/stralloc.h lib/mmap.h lib/scan.h lib/uint64.h lib/uint8.h lib/errmsg.h lib/open.h lib/path.h lib/stralloc.h lib/str.h lib/str.h lib/byte.h lib/fmt.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)impgen.o: impgen.c lib/byte.h lib/typedefs.h lib/buffer.h lib/mmap.h lib/uint64.h lib/open.h lib/windoze.h lib/pe.h lib/uint32.h lib/uint16.h lib/uint8.h lib/uint32.h lib/str.h lib/fmt.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)mediathek-parser.o: mediathek-parser.c lib/unix.h lib/typedefs.h lib/windoze.h lib/getopt.h lib/readlink.h lib/array.h lib/uint64.h lib/buffer.h lib/byte.h lib/io.h lib/iarray.h lib/taia.h lib/tai.h lib/uint32.h lib/iarray.h lib/open.h lib/str.h lib/stralloc.h lib/strlist.h lib/stralloc.h lib/byte.h lib/buffer.h lib/fmt.h lib/mmap.h lib/ucs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_fromsa_example.o: examples/buffer_fromsa_example.c examples/../lib/stralloc.h examples/../lib/typedefs.h examples/../lib/buffer.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)io3.o: examples/io3.c examples/../lib/uint64.h examples/../lib/buffer.h examples/../lib/typedefs.h examples/../lib/io.h examples/../lib/windoze.h examples/../lib/iarray.h examples/../lib/uint64.h examples/../lib/taia.h examples/../lib/tai.h examples/../lib/uint32.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cdbget2.o: examples/cdbget2.c examples/../lib/buffer.h examples/../lib/typedefs.h examples/../lib/errmsg.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)urlencode.o: examples/urlencode.c examples/../lib/buffer.h examples/../lib/typedefs.h examples/../lib/textcode.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)fmt.o: examples/fmt.c examples/../lib/fmt.h examples/../lib/typedefs.h examples/../lib/uint32.h examples/../lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cas.o: examples/cas.c examples/../lib/CAS.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)scan_long_example.o: examples/scan_long_example.c examples/../lib/scan.h examples/../lib/typedefs.h examples/../lib/uint32.h examples/../lib/uint64.h examples/../lib/fmt.h examples/../lib/buffer.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)fmt_ip6_example.o: examples/fmt_ip6_example.c examples/../lib/ip6.h examples/../lib/byte.h examples/../lib/typedefs.h examples/../lib/uint32.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_1_example.o: examples/buffer_1_example.c examples/../lib/buffer.h examples/../lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)iob.o: examples/iob.c examples/../lib/iob.h examples/../lib/io.h examples/../lib/windoze.h examples/../lib/iarray.h examples/../lib/uint64.h examples/../lib/typedefs.h examples/../lib/taia.h examples/../lib/tai.h examples/../lib/uint32.h examples/../lib/array.h examples/../lib/buffer.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)uint.o: examples/uint.c examples/../lib/uint16.h examples/../lib/uint32.h examples/../lib/uint64.h examples/../lib/byte.h examples/../lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)uudecode.o: examples/uudecode.c examples/../lib/textcode.h examples/../lib/str.h examples/../lib/typedefs.h examples/../lib/buffer.h examples/../lib/open.h examples/../lib/windoze.h examples/../lib/uint64.h examples/../lib/stralloc.h examples/../lib/scan.h examples/../lib/uint32.h examples/../lib/case.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)json.o: examples/json.c examples/../lib/textcode.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)iom.o: examples/iom.c examples/../lib/io.h examples/../lib/windoze.h examples/../lib/iarray.h examples/../lib/uint64.h examples/../lib/typedefs.h examples/../lib/taia.h examples/../lib/tai.h examples/../lib/uint32.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cescape.o: examples/cescape.c examples/../lib/buffer.h examples/../lib/typedefs.h examples/../lib/textcode.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)marshal.o: examples/marshal.c examples/../lib/fmt.h examples/../lib/typedefs.h examples/../lib/uint32.h examples/../lib/uint64.h examples/../lib/stralloc.h examples/../lib/buffer.h examples/../lib/scan.h examples/../lib/textcode.h examples/../lib/byte.h examples/../lib/uint16.h examples/../lib/uint32.h examples/../lib/uint64.h examples/../lib/open.h examples/../lib/windoze.h examples/../lib/mmap.h examples/../lib/str.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)stralloc_chomp_example.o: examples/stralloc_chomp_example.c examples/../lib/stralloc.h examples/../lib/typedefs.h examples/../lib/buffer.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)textcode.o: examples/textcode.c examples/../lib/array.h examples/../lib/typedefs.h examples/../lib/uint64.h examples/../lib/textcode.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)io2.o: examples/io2.c examples/../lib/io.h examples/../lib/windoze.h examples/../lib/iarray.h examples/../lib/uint64.h examples/../lib/typedefs.h examples/../lib/taia.h examples/../lib/tai.h examples/../lib/uint32.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)io.o: examples/io.c examples/../lib/io.h examples/../lib/windoze.h examples/../lib/iarray.h examples/../lib/uint64.h examples/../lib/typedefs.h examples/../lib/taia.h examples/../lib/tai.h examples/../lib/uint32.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_mmap.o: examples/buffer_mmap.c examples/../lib/buffer.h examples/../lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)fmt_long_example.o: examples/fmt_long_example.c examples/../lib/fmt.h examples/../lib/typedefs.h examples/../lib/uint32.h examples/../lib/uint64.h examples/../lib/str.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)readhttp.o: examples/readhttp.c examples/../lib/stralloc.h examples/../lib/typedefs.h examples/../lib/buffer.h examples/../lib/byte.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)dllink.o: examples/dllink.c examples/../lib/socket.h examples/../lib/typedefs.h examples/../lib/uint16.h examples/../lib/uint32.h examples/../lib/buffer.h examples/../lib/case.h examples/../lib/ndelay.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)fmt_httpdate_example.o: examples/fmt_httpdate_example.c examples/../lib/scan.h examples/../lib/typedefs.h examples/../lib/uint32.h examples/../lib/uint64.h examples/../lib/buffer.h examples/../lib/fmt.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)range.o: examples/range.c examples/../lib/rangecheck.h examples/../lib/typedefs.h examples/../lib/uint32.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)io5.o: examples/io5.c examples/../lib/socket.h examples/../lib/typedefs.h examples/../lib/uint16.h examples/../lib/uint32.h examples/../lib/io.h examples/../lib/windoze.h examples/../lib/iarray.h examples/../lib/uint64.h examples/../lib/taia.h examples/../lib/tai.h examples/../lib/buffer.h examples/../lib/ip6.h examples/../lib/byte.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)fdpassing.o: examples/fdpassing.c examples/../lib/io.h examples/../lib/windoze.h examples/../lib/iarray.h examples/../lib/uint64.h examples/../lib/typedefs.h examples/../lib/taia.h examples/../lib/tai.h examples/../lib/uint32.h examples/../lib/buffer.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)longlong.o: examples/longlong.c examples/../lib/fmt.h examples/../lib/typedefs.h examples/../lib/uint32.h examples/../lib/uint64.h examples/../lib/str.h examples/../lib/scan.h examples/../lib/byte.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)netstring.o: examples/netstring.c examples/../lib/fmt.h examples/../lib/typedefs.h examples/../lib/uint32.h examples/../lib/uint64.h examples/../lib/scan.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)protobuf.o: examples/protobuf.c examples/../lib/fmt.h examples/../lib/typedefs.h examples/../lib/uint32.h examples/../lib/uint64.h examples/../lib/scan.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)mult.o: examples/mult.c examples/../lib/safemult.h examples/../lib/uint16.h examples/../lib/uint32.h examples/../lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)utf8_example.o: examples/utf8_example.c examples/../lib/scan.h examples/../lib/typedefs.h examples/../lib/uint32.h examples/../lib/uint64.h examples/../lib/fmt.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)fmt_strm_alloca.o: examples/fmt_strm_alloca.c examples/../lib/fmt.h examples/../lib/typedefs.h examples/../lib/uint32.h examples/../lib/uint64.h examples/../lib/byte.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)proxy.o: examples/proxy.c examples/../lib/socket.h examples/../lib/typedefs.h examples/../lib/uint16.h examples/../lib/uint32.h examples/../lib/io.h examples/../lib/windoze.h examples/../lib/iarray.h examples/../lib/uint64.h examples/../lib/taia.h examples/../lib/tai.h examples/../lib/buffer.h examples/../lib/ip6.h examples/../lib/byte.h examples/../lib/str.h examples/../lib/dns.h examples/../lib/iopause.h examples/../lib/io.h examples/../lib/stralloc.h examples/../lib/getopt.h examples/../lib/scan.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)httpd.o: examples/httpd.c examples/../lib/socket.h examples/../lib/typedefs.h examples/../lib/uint16.h examples/../lib/uint32.h examples/../lib/io.h examples/../lib/windoze.h examples/../lib/iarray.h examples/../lib/uint64.h examples/../lib/taia.h examples/../lib/tai.h examples/../lib/buffer.h examples/../lib/ip6.h examples/../lib/byte.h examples/../lib/array.h examples/../lib/case.h examples/../lib/fmt.h examples/../lib/iob.h examples/../lib/io.h examples/../lib/array.h examples/../lib/str.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)stralloc_buffer.o: examples/stralloc_buffer.c examples/../lib/stralloc.h examples/../lib/typedefs.h examples/../lib/buffer.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)dnsip.o: examples/dnsip.c examples/../lib/dns.h examples/../lib/iopause.h examples/../lib/io.h examples/../lib/windoze.h examples/../lib/iarray.h examples/../lib/uint64.h examples/../lib/typedefs.h examples/../lib/taia.h examples/../lib/tai.h examples/../lib/uint32.h examples/../lib/stralloc.h examples/../lib/ip4.h examples/../lib/uint8.h examples/../lib/uint16.h examples/../lib/buffer.h examples/../lib/errmsg.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)fmt_human_example.o: examples/fmt_human_example.c examples/../lib/fmt.h examples/../lib/typedefs.h examples/../lib/uint32.h examples/../lib/uint64.h examples/../lib/buffer.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)b64encode.o: examples/b64encode.c examples/../lib/str.h examples/../lib/typedefs.h examples/../lib/buffer.h examples/../lib/textcode.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)io4.o: examples/io4.c examples/../lib/io.h examples/../lib/windoze.h examples/../lib/iarray.h examples/../lib/uint64.h examples/../lib/typedefs.h examples/../lib/taia.h examples/../lib/tai.h examples/../lib/uint32.h examples/../lib/uint64.h examples/../lib/buffer.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)fmt_iso8691.o: examples/fmt_iso8691.c examples/../lib/fmt.h examples/../lib/typedefs.h examples/../lib/uint32.h examples/../lib/uint64.h examples/../lib/scan.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)scan.o: examples/scan.c examples/../lib/scan.h examples/../lib/typedefs.h examples/../lib/uint32.h examples/../lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)byte_copy_example.o: examples/byte_copy_example.c examples/../lib/byte.h examples/../lib/typedefs.h examples/../lib/errmsg.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_tosa_example.o: examples/buffer_tosa_example.c examples/../lib/byte.h examples/../lib/typedefs.h examples/../lib/stralloc.h examples/../lib/buffer.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)server.o: examples/server.c examples/../lib/scan.h examples/../lib/typedefs.h examples/../lib/uint32.h examples/../lib/uint64.h examples/../lib/ip6.h examples/../lib/byte.h examples/../lib/str.h examples/../lib/socket.h examples/../lib/uint16.h examples/../lib/ndelay.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)unurl.o: examples/unurl.c examples/../lib/buffer.h examples/../lib/typedefs.h examples/../lib/textcode.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)array.o: examples/array.c examples/../lib/array.h examples/../lib/typedefs.h examples/../lib/uint64.h examples/../lib/byte.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)client.o: examples/client.c examples/../lib/scan.h examples/../lib/typedefs.h examples/../lib/uint32.h examples/../lib/uint64.h examples/../lib/ip6.h examples/../lib/byte.h examples/../lib/str.h examples/../lib/socket.h examples/../lib/uint16.h examples/../lib/ndelay.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)vd.o: examples/vd.c examples/../lib/socket.h examples/../lib/typedefs.h examples/../lib/uint16.h examples/../lib/uint32.h examples/../lib/buffer.h examples/../lib/ndelay.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)xmltest2.o: xmltest2.c lib/buffer.h lib/typedefs.h lib/byte.h lib/fmt.h lib/uint32.h lib/uint64.h lib/hmap.h lib/stralloc.h lib/buffer.h lib/iterator.h lib/stralloc.h lib/strlist.h lib/byte.h lib/xml.h lib/hmap.h lib/textbuf.h lib/str.h lib/mmap.h lib/scan.h lib/array.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)strarraytest.o: strarraytest.c lib/buffer.h lib/typedefs.h lib/mmap.h lib/uint64.h lib/stralloc.h lib/strarray.h lib/array.h lib/stralloc.h lib/byte.h lib/str.h lib/byte.h lib/array.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)tcping.o: tcping.c lib/windoze.h lib/buffer.h lib/typedefs.h lib/socket.h lib/uint16.h lib/uint32.h lib/socket_internal.h lib/socket.h lib/ndelay.h lib/unix.h lib/windoze.h lib/getopt.h lib/readlink.h lib/errmsg.h lib/stralloc.h lib/dns.h lib/iopause.h lib/io.h lib/iarray.h lib/uint64.h lib/taia.h lib/tai.h lib/stralloc.h lib/ip4.h lib/uint8.h lib/map.h lib/byte.h lib/safemult.h lib/str.h lib/mmap.h lib/scan.h lib/uint32.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)crc32.o: crc32.c lib/buffer.h lib/typedefs.h lib/errmsg.h lib/uint32.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)xml2json.o: xml2json.c lib/buffer.h lib/typedefs.h lib/byte.h lib/fmt.h lib/uint32.h lib/uint64.h lib/hmap.h lib/stralloc.h lib/buffer.h lib/iterator.h lib/stralloc.h lib/xml.h lib/hmap.h lib/textbuf.h lib/str.h lib/mmap.h lib/scan.h lib/strlist.h lib/byte.h lib/json.h lib/charbuf.h lib/slist.h lib/alloc.h lib/str.h lib/array.h lib/textcode.h lib/errmsg.h lib/open.h lib/windoze.h lib/getopt.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)list-r.o: list-r.c lib/windoze.h lib/getopt.h lib/buffer.h lib/typedefs.h lib/dir.h lib/uint64.h lib/fmt.h lib/uint32.h lib/fnmatch.h lib/io.h lib/windoze.h lib/iarray.h lib/taia.h lib/tai.h lib/iarray.h lib/open.h lib/str.h lib/stralloc.h lib/uint64.h lib/ioctlcmd.h lib/byte.h lib/array.h lib/unix.h lib/getopt.h lib/readlink.h lib/path.h lib/stralloc.h lib/str.h lib/scan.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)dnsip.o: dnsip.c lib/socket.h lib/typedefs.h lib/uint16.h lib/uint32.h lib/io.h lib/windoze.h lib/iarray.h lib/uint64.h lib/taia.h lib/tai.h lib/iarray.h lib/case.h lib/open.h lib/stralloc.h lib/scan.h lib/fmt.h lib/str.h lib/tai.h lib/taia.h lib/buffer.h lib/dns.h lib/iopause.h lib/io.h lib/stralloc.h lib/errmsg.h lib/ip4.h lib/uint8.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ccat.o: ccat.c lib/buffer.h lib/typedefs.h lib/open.h lib/windoze.h lib/uint64.h lib/unix.h lib/getopt.h lib/readlink.h lib/str.h lib/mmap.h lib/byte.h lib/iarray.h lib/io.h lib/iarray.h lib/taia.h lib/tai.h lib/uint32.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)dnsname.o: dnsname.c lib/socket.h lib/typedefs.h lib/uint16.h lib/uint32.h lib/io.h lib/windoze.h lib/iarray.h lib/uint64.h lib/taia.h lib/tai.h lib/iarray.h lib/case.h lib/open.h lib/stralloc.h lib/scan.h lib/fmt.h lib/str.h lib/tai.h lib/taia.h lib/buffer.h lib/dns.h lib/iopause.h lib/io.h lib/stralloc.h lib/errmsg.h lib/ip4.h lib/uint8.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)elf64list.o: elf64list.c lib/elf.h lib/uint8.h lib/uint16.h lib/uint32.h lib/uint64.h lib/range.h lib/typedefs.h lib/mmap.h lib/uint16.h lib/uint32.h lib/uint64.h lib/str.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)eagle-init-brd.o: eagle-init-brd.c lib/windoze.h lib/xml.h lib/buffer.h lib/typedefs.h lib/hmap.h lib/stralloc.h lib/textbuf.h lib/uint64.h lib/hmap.h lib/fmt.h lib/uint32.h lib/byte.h lib/mmap.h lib/scan.h lib/str.h lib/stralloc.h lib/strlist.h lib/byte.h lib/round.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)xmlpp.o: xmlpp.c lib/buffer.h lib/typedefs.h lib/hmap.h lib/stralloc.h lib/buffer.h lib/io.h lib/windoze.h lib/iarray.h lib/uint64.h lib/taia.h lib/tai.h lib/uint32.h lib/iarray.h lib/stralloc.h lib/xml.h lib/hmap.h lib/textbuf.h lib/byte.h lib/fmt.h lib/mmap.h lib/str.h lib/getopt.h lib/errmsg.h lib/scan.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)httptest.o: httptest.c lib/windoze.h lib/http.h lib/uint16.h lib/uint32.h lib/uint64.h lib/stralloc.h lib/typedefs.h lib/buffer.h lib/ip4.h lib/uint8.h lib/byte.h lib/io.h lib/windoze.h lib/iarray.h lib/taia.h lib/tai.h lib/iopause.h lib/io.h lib/socket.h lib/taia.h lib/errmsg.h lib/open.h lib/dns.h lib/iopause.h lib/fmt.h lib/iarray.h lib/scan.h lib/str.h lib/stralloc.h lib/tai.h lib/case.h lib/buffer.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)case_finds.o: lib/case/case_finds.c lib/case.h lib/typedefs.h lib/str.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)case_diffb.o: lib/case/case_diffb.c lib/case.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)case_diffs.o: lib/case/case_diffs.c lib/case.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)case_lowers.o: lib/case/case_lowers.c lib/case.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)case_findb.o: lib/case/case_findb.c lib/case.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)case_lowerc.o: lib/case/case_lowerc.c lib/case.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)case_lowerb.o: lib/case/case_lowerb.c lib/case.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)case_find.o: lib/case/case_find.c lib/case.h lib/typedefs.h lib/str.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)case_starts.o: lib/case/case_starts.c lib/case.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)slist_find_remove.o: lib/slist/slist_find_remove.c lib/slist.h lib/alloc.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)slist_init.o: lib/slist/slist_init.c lib/slist.h lib/alloc.h lib/typedefs.h lib/byte.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)slist_pushs.o: lib/slist/slist_pushs.c lib/slist.h lib/alloc.h lib/typedefs.h lib/str.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)slist_push.o: lib/slist/slist_push.c lib/slist.h lib/alloc.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)slist_unshifts.o: lib/slist/slist_unshifts.c lib/slist.h lib/alloc.h lib/typedefs.h lib/str.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)slist_add_after.o: lib/slist/slist_add_after.c lib/slist.h lib/alloc.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)slist_shift.o: lib/slist/slist_shift.c lib/slist.h lib/alloc.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)slist_length.o: lib/slist/slist_length.c lib/slist.h lib/alloc.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)slist_find.o: lib/slist/slist_find.c lib/slist.h lib/alloc.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)slist_remove.o: lib/slist/slist_remove.c lib/slist.h lib/alloc.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)slist_shifts.o: lib/slist/slist_shifts.c lib/slist.h lib/alloc.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)slist_add_before.o: lib/slist/slist_add_before.c lib/slist.h lib/alloc.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)scan_netstring.o: lib/scan/scan_netstring.c lib/scan.h lib/typedefs.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)scan_pb_tag.o: lib/scan/scan_pb_tag.c lib/scan.h lib/typedefs.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)scan_8longn.o: lib/scan/scan_8longn.c lib/scan.h lib/typedefs.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)scan_plusminus.o: lib/scan/scan_plusminus.c lib/scan.h lib/typedefs.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)scan_xlonglong.o: lib/scan/scan_xlonglong.c lib/scan.h lib/typedefs.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)scan_pb_type0_sint.o: lib/scan/scan_pb_type0_sint.c lib/scan.h lib/typedefs.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)scan_xshort.o: lib/scan/scan_xshort.c lib/scan.h lib/typedefs.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)scan_lineskip.o: lib/scan/scan_lineskip.c lib/scan.h lib/typedefs.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)scan_double.o: lib/scan/scan_double.c lib/scan.h lib/typedefs.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)scan_httpdate.o: lib/scan/scan_httpdate.c lib/windoze.h lib/byte.h lib/typedefs.h lib/case.h lib/scan.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)scan_noncharsetnskip.o: lib/scan/scan_noncharsetnskip.c lib/scan.h lib/typedefs.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)scan_pb_type1_fixed64.o: lib/scan/scan_pb_type1_fixed64.c lib/scan.h lib/typedefs.h lib/uint32.h lib/uint64.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)scan_ulong.o: lib/scan/scan_ulong.c lib/scan.h lib/typedefs.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)scan_8int.o: lib/scan/scan_8int.c lib/scan.h lib/typedefs.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)scan_long.o: lib/scan/scan_long.c lib/scan.h lib/typedefs.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)scan_ulonglong.o: lib/scan/scan_ulonglong.c lib/scan.h lib/typedefs.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)scan_8long.o: lib/scan/scan_8long.c lib/scan.h lib/typedefs.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)scan_longlong.o: lib/scan/scan_longlong.c lib/scan.h lib/typedefs.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)scan_eolskip.o: lib/scan/scan_eolskip.c lib/scan.h lib/typedefs.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)scan_xlongn.o: lib/scan/scan_xlongn.c lib/scan.h lib/typedefs.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)scan_longn.o: lib/scan/scan_longn.c lib/scan.h lib/typedefs.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)scan_varint.o: lib/scan/scan_varint.c lib/scan.h lib/typedefs.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)scan_xlong.o: lib/scan/scan_xlong.c lib/scan.h lib/typedefs.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)scan_uint.o: lib/scan/scan_uint.c lib/scan.h lib/typedefs.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)scan_fromhex.o: lib/scan/scan_fromhex.c lib/scan.h lib/typedefs.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)scan_xint.o: lib/scan/scan_xint.c lib/scan.h lib/typedefs.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)scan_ulongn.o: lib/scan/scan_ulongn.c lib/scan.h lib/typedefs.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)scan_short.o: lib/scan/scan_short.c lib/scan.h lib/typedefs.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)scan_nonwhitenskip.o: lib/scan/scan_nonwhitenskip.c lib/scan.h lib/typedefs.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)scan_8short.o: lib/scan/scan_8short.c lib/scan.h lib/typedefs.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)scan_ushort.o: lib/scan/scan_ushort.c lib/scan.h lib/typedefs.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)scan_line.o: lib/scan/scan_line.c lib/scan.h lib/typedefs.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)scan_asn1dertag.o: lib/scan/scan_asn1dertag.c lib/scan.h lib/typedefs.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)scan_int.o: lib/scan/scan_int.c lib/scan.h lib/typedefs.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)scan_utf8_sem.o: lib/scan/scan_utf8_sem.c lib/scan.h lib/typedefs.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)scan_xmlescape.o: lib/scan/scan_xmlescape.c lib/scan.h lib/typedefs.h lib/uint32.h lib/uint64.h lib/str.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)scan_whitenskip.o: lib/scan/scan_whitenskip.c lib/scan.h lib/typedefs.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)scan_octal.o: lib/scan/scan_octal.c lib/scan.h lib/typedefs.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)scan_pb_type5_fixed32.o: lib/scan/scan_pb_type5_fixed32.c lib/scan.h lib/typedefs.h lib/uint32.h lib/uint64.h lib/uint32.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)scan_asn1derlength.o: lib/scan/scan_asn1derlength.c lib/scan.h lib/typedefs.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)scan_utf8.o: lib/scan/scan_utf8.c lib/fmt.h lib/typedefs.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)scan_charsetnskip.o: lib/scan/scan_charsetnskip.c lib/scan.h lib/typedefs.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cb_get_kv.o: lib/cb/cb_get_kv.c lib/cb_internal.h lib/byte.h lib/typedefs.h lib/cb.h lib/str.h lib/byte.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cb_find.o: lib/cb/cb_find.c lib/cb_internal.h lib/byte.h lib/typedefs.h lib/cb.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cb_new_kv.o: lib/cb/cb_new_kv.c lib/cb_internal.h lib/byte.h lib/typedefs.h lib/cb.h lib/byte.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cb_clear.o: lib/cb/cb_clear.c lib/cb_internal.h lib/byte.h lib/typedefs.h lib/cb.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cb_get_kv_ex.o: lib/cb/cb_get_kv_ex.c lib/cb_internal.h lib/byte.h lib/typedefs.h lib/cb.h lib/str.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cb_find_prefix.o: lib/cb/cb_find_prefix.c lib/byte.h lib/typedefs.h lib/cb_internal.h lib/byte.h lib/cb.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cb_insert.o: lib/cb/cb_insert.c lib/cb_internal.h lib/byte.h lib/typedefs.h lib/cb.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cb_foreach.o: lib/cb/cb_foreach.c lib/byte.h lib/typedefs.h lib/cb_internal.h lib/byte.h lib/cb.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cb_erase.o: lib/cb/cb_erase.c lib/byte.h lib/typedefs.h lib/cb_internal.h lib/byte.h lib/cb.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)fmt_strn.o: lib/fmt/fmt_strn.c lib/fmt.h lib/typedefs.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)fmt_str.o: lib/fmt/fmt_str.c lib/fmt.h lib/typedefs.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)fmt_pad.o: lib/fmt/fmt_pad.c lib/fmt.h lib/typedefs.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)fmt_escapecharquotedprintable.o: lib/fmt/fmt_escapecharquotedprintable.c lib/fmt.h lib/typedefs.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)fmt_plusminus.o: lib/fmt/fmt_plusminus.c lib/fmt.h lib/typedefs.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)fmt_httpdate.o: lib/fmt/fmt_httpdate.c lib/byte.h lib/typedefs.h lib/fmt.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)fmt_escapecharc.o: lib/fmt/fmt_escapecharc.c lib/fmt.h lib/typedefs.h lib/uint32.h lib/uint64.h lib/uint32.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)fmt_escapecharxml.o: lib/fmt/fmt_escapecharxml.c lib/byte.h lib/typedefs.h lib/fmt.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)fmt_tohex.o: lib/fmt/fmt_tohex.c lib/fmt.h lib/typedefs.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)fmt_double.o: lib/fmt/fmt_double.c lib/fmt.h lib/typedefs.h lib/uint32.h lib/uint64.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)fmt_escapecharquotedprintableutf8.o: lib/fmt/fmt_escapecharquotedprintableutf8.c lib/fmt.h lib/typedefs.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)fmt_octal.o: lib/fmt/fmt_octal.c lib/fmt.h lib/typedefs.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)fmt_escapecharshell.o: lib/fmt/fmt_escapecharshell.c lib/fmt.h lib/typedefs.h lib/uint32.h lib/uint64.h lib/uint32.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)fmt_hexb.o: lib/fmt/fmt_hexb.c lib/fmt.h lib/typedefs.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)fmt_humank.o: lib/fmt/fmt_humank.c lib/fmt.h lib/typedefs.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)fmt_ulong.o: lib/fmt/fmt_ulong.c lib/fmt.h lib/typedefs.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)fmt_asn1dertag.o: lib/fmt/fmt_asn1dertag.c lib/fmt.h lib/typedefs.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)fmt_utf8.o: lib/fmt/fmt_utf8.c lib/fmt.h lib/typedefs.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)fmt_xmlescape.o: lib/fmt/fmt_xmlescape.c lib/fmt.h lib/typedefs.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)fmt_xlong.o: lib/fmt/fmt_xlong.c lib/fmt.h lib/typedefs.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)fmt_repeat.o: lib/fmt/fmt_repeat.c lib/fmt.h lib/typedefs.h lib/uint32.h lib/uint64.h lib/byte.h lib/str.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)fmt_human.o: lib/fmt/fmt_human.c lib/fmt.h lib/typedefs.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)fmt_longlong.o: lib/fmt/fmt_longlong.c lib/fmt.h lib/typedefs.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)fmt_escapecharhtml.o: lib/fmt/fmt_escapecharhtml.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)fmt_long.o: lib/fmt/fmt_long.c lib/fmt.h lib/typedefs.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)fmt_ulong0.o: lib/fmt/fmt_ulong0.c lib/fmt.h lib/typedefs.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)fmt_fill.o: lib/fmt/fmt_fill.c lib/fmt.h lib/typedefs.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)fmt_xlonglong.o: lib/fmt/fmt_xlonglong.c lib/fmt.h lib/typedefs.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)fmt_escapecharjson.o: lib/fmt/fmt_escapecharjson.c lib/fmt.h lib/typedefs.h lib/uint32.h lib/uint64.h lib/uint16.h lib/uint32.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)fmt_asn1derlength.o: lib/fmt/fmt_asn1derlength.c lib/fmt.h lib/typedefs.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)fmt_ulonglong.o: lib/fmt/fmt_ulonglong.c lib/fmt.h lib/typedefs.h lib/uint32.h lib/uint64.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)fmt_iso8601.o: lib/fmt/fmt_iso8601.c lib/fmt.h lib/typedefs.h lib/uint32.h lib/uint64.h lib/byte.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)fmt_minus.o: lib/fmt/fmt_minus.c lib/fmt.h lib/typedefs.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)fmt_8long.o: lib/fmt/fmt_8long.c lib/fmt.h lib/typedefs.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)fmt_strm_internal.o: lib/fmt/fmt_strm_internal.c lib/fmt.h lib/typedefs.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)elf_symbol_r.o: lib/elf/elf_symbol_r.c lib/elf.h lib/uint8.h lib/uint16.h lib/uint32.h lib/uint64.h lib/range.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)elf_program_headers.o: lib/elf/elf_program_headers.c lib/elf.h lib/uint8.h lib/uint16.h lib/uint32.h lib/uint64.h lib/range.h lib/typedefs.h lib/range.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)elf_section_index.o: lib/elf/elf_section_index.c lib/elf.h lib/uint8.h lib/uint16.h lib/uint32.h lib/uint64.h lib/range.h lib/typedefs.h lib/range.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)elf_get_section.o: lib/elf/elf_get_section.c lib/elf.h lib/uint8.h lib/uint16.h lib/uint32.h lib/uint64.h lib/range.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)elf_get_value.o: lib/elf/elf_get_value.c lib/elf.h lib/uint8.h lib/uint16.h lib/uint32.h lib/uint64.h lib/range.h lib/typedefs.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)elf_dynamic_section.o: lib/elf/elf_dynamic_section.c lib/elf.h lib/uint8.h lib/uint16.h lib/uint32.h lib/uint64.h lib/range.h lib/typedefs.h lib/range.h lib/byte.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)elf_get_symtab_r.o: lib/elf/elf_get_symtab_r.c lib/elf.h lib/uint8.h lib/uint16.h lib/uint32.h lib/uint64.h lib/range.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)elf_get_section_r.o: lib/elf/elf_get_section_r.c lib/elf.h lib/uint8.h lib/uint16.h lib/uint32.h lib/uint64.h lib/range.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)elf_get_symtab.o: lib/elf/elf_get_symtab.c lib/elf.h lib/uint8.h lib/uint16.h lib/uint32.h lib/uint64.h lib/range.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)elf_section_typename.o: lib/elf/elf_section_typename.c lib/elf.h lib/uint8.h lib/uint16.h lib/uint32.h lib/uint64.h lib/range.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)elf_shstrtab.o: lib/elf/elf_shstrtab.c lib/elf.h lib/uint8.h lib/uint16.h lib/uint32.h lib/uint64.h lib/range.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)elf_section.o: lib/elf/elf_section.c lib/elf.h lib/uint8.h lib/uint16.h lib/uint32.h lib/uint64.h lib/range.h lib/typedefs.h lib/range.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)elf_section_name.o: lib/elf/elf_section_name.c lib/elf.h lib/uint8.h lib/uint16.h lib/uint32.h lib/uint64.h lib/range.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)elf_header.o: lib/elf/elf_header.c lib/elf.h lib/uint8.h lib/uint16.h lib/uint32.h lib/uint64.h lib/range.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)elf_section_size.o: lib/elf/elf_section_size.c lib/elf.h lib/uint8.h lib/uint16.h lib/uint32.h lib/uint64.h lib/range.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)elf_section_offset.o: lib/elf/elf_section_offset.c lib/elf.h lib/uint8.h lib/uint16.h lib/uint32.h lib/uint64.h lib/range.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)elf_section_find.o: lib/elf/elf_section_find.c lib/elf.h lib/uint8.h lib/uint16.h lib/uint32.h lib/uint64.h lib/range.h lib/typedefs.h lib/range.h lib/str.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)elf_section_headers.o: lib/elf/elf_section_headers.c lib/elf.h lib/uint8.h lib/uint16.h lib/uint32.h lib/uint64.h lib/range.h lib/typedefs.h lib/range.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ndelay_off.o: lib/ndelay/ndelay_off.c lib/socket_internal.h lib/socket.h lib/typedefs.h lib/uint16.h lib/uint32.h lib/ndelay.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ndelay_on.o: lib/ndelay/ndelay_on.c lib/socket_internal.h lib/socket.h lib/typedefs.h lib/uint16.h lib/uint32.h lib/ndelay.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)sig_unblock.o: lib/sig/sig_unblock.c lib/windoze.h lib/sig.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)sig_block.o: lib/sig/sig_block.c lib/windoze.h lib/sig.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)expand_copysa.o: lib/expand/expand_copysa.c lib/vartab.h lib/uint64.h lib/var.h lib/stralloc.h lib/typedefs.h lib/vartab.h lib/tree.h lib/byte.h lib/expand.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)expand_arg.o: lib/expand/expand_arg.c lib/vartab.h lib/uint64.h lib/var.h lib/stralloc.h lib/typedefs.h lib/vartab.h lib/stralloc.h lib/tree.h lib/expand.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)expand_args.o: lib/expand/expand_args.c lib/vartab.h lib/uint64.h lib/var.h lib/stralloc.h lib/typedefs.h lib/vartab.h lib/stralloc.h lib/tree.h lib/expand.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)expand_catsa.o: lib/expand/expand_catsa.c lib/vartab.h lib/uint64.h lib/var.h lib/stralloc.h lib/typedefs.h lib/vartab.h lib/byte.h lib/tree.h lib/expand.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)expand_command.o: lib/expand/expand_command.c lib/stralloc.h lib/typedefs.h lib/buffer.h lib/tree.h lib/stralloc.h lib/uint64.h lib/vartab.h lib/var.h lib/vartab.h lib/expand.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)expand_param.o: lib/expand/expand_param.c lib/windoze.h lib/vartab.h lib/uint64.h lib/var.h lib/stralloc.h lib/typedefs.h lib/vartab.h lib/stralloc.h lib/fmt.h lib/uint32.h lib/str.h lib/var.h lib/fnmatch.h lib/tree.h lib/expand.h lib/errmsg.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)expand_vars.o: lib/expand/expand_vars.c lib/vartab.h lib/uint64.h lib/var.h lib/stralloc.h lib/typedefs.h lib/vartab.h lib/tree.h lib/expand.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)expand_unescape.o: lib/expand/expand_unescape.c lib/tree.h lib/stralloc.h lib/typedefs.h lib/uint64.h lib/stralloc.h lib/expand.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)expand_argv.o: lib/expand/expand_argv.c lib/tree.h lib/stralloc.h lib/typedefs.h lib/uint64.h lib/expand.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)expand_escape.o: lib/expand/expand_escape.c lib/str.h lib/typedefs.h lib/stralloc.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)expand_glob.o: lib/expand/expand_glob.c lib/vartab.h lib/uint64.h lib/var.h lib/stralloc.h lib/typedefs.h lib/vartab.h lib/tree.h lib/glob.h lib/var.h lib/expand.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)expand_cat.o: lib/expand/expand_cat.c lib/vartab.h lib/uint64.h lib/var.h lib/stralloc.h lib/typedefs.h lib/vartab.h lib/stralloc.h lib/str.h lib/var.h lib/tree.h lib/expand.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)expand_arith.o: lib/expand/expand_arith.c lib/tree.h lib/stralloc.h lib/typedefs.h lib/uint64.h lib/expand.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)coff_get_section.o: lib/coff/coff_get_section.c lib/coff.h lib/uint8.h lib/uint16.h lib/uint32.h lib/range.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)coff_symbol_table.o: lib/coff/coff_symbol_table.c lib/coff.h lib/uint8.h lib/uint16.h lib/uint32.h lib/range.h lib/typedefs.h lib/range.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)coff_sclass_name.o: lib/coff/coff_sclass_name.c lib/coff.h lib/uint8.h lib/uint16.h lib/uint32.h lib/range.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)coff_line_numbers.o: lib/coff/coff_line_numbers.c lib/coff.h lib/uint8.h lib/uint16.h lib/uint32.h lib/range.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)coff_get_symtab.o: lib/coff/coff_get_symtab.c lib/coff.h lib/uint8.h lib/uint16.h lib/uint32.h lib/range.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)coff_storage_class.o: lib/coff/coff_storage_class.c lib/coff.h lib/uint8.h lib/uint16.h lib/uint32.h lib/range.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)coff_header.o: lib/coff/coff_header.c lib/coff.h lib/uint8.h lib/uint16.h lib/uint32.h lib/range.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)coff_symbol_name.o: lib/coff/coff_symbol_name.c lib/coff.h lib/uint8.h lib/uint16.h lib/uint32.h lib/range.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)coff_get_strtab.o: lib/coff/coff_get_strtab.c lib/coff.h lib/uint8.h lib/uint16.h lib/uint32.h lib/range.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)coff_index_symtab.o: lib/coff/coff_index_symtab.c lib/coff.h lib/uint8.h lib/uint16.h lib/uint32.h lib/range.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)uint32_random.o: lib/uint32/uint32_random.c lib/uint32.h lib/taia.h lib/tai.h lib/uint64.h lib/uint32.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)uint32_seed.o: lib/uint32/uint32_seed.c lib/windoze.h lib/uint32.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)uint32_prng.o: lib/uint32/uint32_prng.c lib/uint32.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)uint32_read.o: lib/uint32/uint32_read.c lib/uint32.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)uint32_pack.o: lib/uint32/uint32_pack.c lib/uint32.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)uint32_pack_big.o: lib/uint32/uint32_pack_big.c lib/uint32.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)uint32_unpack.o: lib/uint32/uint32_unpack.c lib/uint32.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)uint32_unpack_big.o: lib/uint32/uint32_unpack_big.c lib/uint32.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)uint32_read_big.o: lib/uint32/uint32_read_big.c lib/uint32.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)gpio_read.o: lib/gpio/gpio_read.c lib/gpio_internal.h lib/gpio.h lib/mmap.h lib/typedefs.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)gpio_short_wait.o: lib/gpio/gpio_short_wait.c lib/windoze.h lib/gpio_internal.h lib/gpio.h lib/mmap.h lib/typedefs.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)gpio_write.o: lib/gpio/gpio_write.c lib/gpio_internal.h lib/gpio.h lib/mmap.h lib/typedefs.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)gpio_init.o: lib/gpio/gpio_init.c lib/windoze.h lib/buffer.h lib/typedefs.h lib/open.h lib/windoze.h lib/uint64.h lib/gpio_internal.h lib/gpio.h lib/mmap.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)gpio_set_input_pull.o: lib/gpio/gpio_set_input_pull.c lib/gpio_internal.h lib/gpio.h lib/mmap.h lib/typedefs.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)gpio_set_pin_mode.o: lib/gpio/gpio_set_pin_mode.c lib/gpio_internal.h lib/gpio.h lib/mmap.h lib/typedefs.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)gpio_set_input_resistor_pull.o: lib/gpio/gpio_set_input_resistor_pull.c lib/gpio_internal.h lib/gpio.h lib/mmap.h lib/typedefs.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)gpio_set_input_pull_down.o: lib/gpio/gpio_set_input_pull_down.c lib/gpio_internal.h lib/gpio.h lib/mmap.h lib/typedefs.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)gpio_set_input_pull_up.o: lib/gpio/gpio_set_input_pull_up.c lib/gpio_internal.h lib/gpio.h lib/mmap.h lib/typedefs.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)gpio_set_output.o: lib/gpio/gpio_set_output.c lib/gpio_internal.h lib/gpio.h lib/mmap.h lib/typedefs.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)process_create.o: lib/process/process_create.c lib/windoze.h lib/io.h lib/windoze.h lib/iarray.h lib/uint64.h lib/typedefs.h lib/taia.h lib/tai.h lib/uint32.h lib/strlist.h lib/stralloc.h lib/byte.h lib/buffer.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)binfmt_open.o: lib/binfmt/binfmt_open.c lib/binfmt.h lib/typedefs.h lib/uint8.h lib/uint32.h lib/uint64.h lib/range.h lib/mmap.h lib/byte.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)byte_replace.o: lib/byte/byte_replace.c lib/byte.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)byte_scan.o: lib/byte/byte_scan.c lib/byte.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)byte_rchr.o: lib/byte/byte_rchr.c lib/byte.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)byte_zero.o: lib/byte/byte_zero.c lib/byte.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)byte_chr.o: lib/byte/byte_chr.c lib/byte.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)byte_copyr.o: lib/byte/byte_copyr.c lib/byte.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)byte_ccopy.o: lib/byte/byte_ccopy.c lib/byte.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)byte_fill.o: lib/byte/byte_fill.c lib/byte.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)byte_upper.o: lib/byte/byte_upper.c lib/byte.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)byte_case_diff.o: lib/byte/byte_case_diff.c lib/byte.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)byte_findb.o: lib/byte/byte_findb.c lib/byte.h lib/typedefs.h lib/str.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)byte_diff.o: lib/byte/byte_diff.c lib/byte.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)byte_count.o: lib/byte/byte_count.c lib/byte.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)byte_case_start.o: lib/byte/byte_case_start.c lib/byte.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)byte_case_equal.o: lib/byte/byte_case_equal.c lib/byte.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)byte_lower.o: lib/byte/byte_lower.c lib/byte.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)byte_finds.o: lib/byte/byte_finds.c lib/byte.h lib/typedefs.h lib/str.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)byte_fmt.o: lib/byte/byte_fmt.c lib/byte.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)byte_equal.o: lib/byte/byte_equal.c lib/byte.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)byte_copy.o: lib/byte/byte_copy.c lib/byte.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)imult64.o: lib/safemult/imult64.c lib/safemult.h lib/uint16.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)umult64.o: lib/safemult/umult64.c lib/safemult.h lib/uint16.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)range_str4inbuf.o: lib/safemult/range_str4inbuf.c lib/likely.h lib/rangecheck.h lib/typedefs.h lib/uint32.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)umult16.o: lib/safemult/umult16.c lib/safemult.h lib/uint16.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)imult16.o: lib/safemult/imult16.c lib/safemult.h lib/uint16.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)range_arrayinbuf.o: lib/safemult/range_arrayinbuf.c lib/likely.h lib/rangecheck.h lib/typedefs.h lib/uint32.h lib/safemult.h lib/uint16.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)imult32.o: lib/safemult/imult32.c lib/safemult.h lib/uint16.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)range_strinbuf.o: lib/safemult/range_strinbuf.c lib/likely.h lib/rangecheck.h lib/typedefs.h lib/uint32.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)range_str2inbuf.o: lib/safemult/range_str2inbuf.c lib/likely.h lib/rangecheck.h lib/typedefs.h lib/uint32.h lib/typedefs.h lib/uint16.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)umult32.o: lib/safemult/umult32.c lib/safemult.h lib/uint16.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_puterror.o: lib/buffer/buffer_puterror.c lib/buffer.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_puterror2.o: lib/buffer/buffer_puterror2.c lib/buffer.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_putalign.o: lib/buffer/buffer_putalign.c lib/buffer.h lib/typedefs.h lib/byte.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_mmapread.o: lib/buffer/buffer_mmapread.c lib/buffer.h lib/typedefs.h lib/mmap.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_putdouble.o: lib/buffer/buffer_putdouble.c lib/buffer.h lib/typedefs.h lib/byte.h lib/fmt.h lib/uint32.h lib/uint64.h lib/str.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_putsalign.o: lib/buffer/buffer_putsalign.c lib/buffer.h lib/typedefs.h lib/str.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_init_free.o: lib/buffer/buffer_init_free.c lib/buffer.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_putc.o: lib/buffer/buffer_putc.c lib/buffer.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_copy.o: lib/buffer/buffer_copy.c lib/buffer.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_putxlong0.o: lib/buffer/buffer_putxlong0.c lib/buffer.h lib/typedefs.h lib/fmt.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_putlonglong.o: lib/buffer/buffer_putlonglong.c lib/buffer.h lib/typedefs.h lib/fmt.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_putlong0.o: lib/buffer/buffer_putlong0.c lib/buffer.h lib/typedefs.h lib/fmt.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_skip_until.o: lib/buffer/buffer_skip_until.c lib/buffer.h lib/typedefs.h lib/byte.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_lzma.o: lib/buffer/buffer_lzma.c lib/buffer.h lib/typedefs.h lib/uint8.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_fromarray.o: lib/buffer/buffer_fromarray.c lib/array.h lib/typedefs.h lib/uint64.h lib/buffer.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_putspad.o: lib/buffer/buffer_putspad.c lib/buffer.h lib/typedefs.h lib/str.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_getn.o: lib/buffer/buffer_getn.c lib/buffer.h lib/typedefs.h lib/byte.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_puts.o: lib/buffer/buffer_puts.c lib/buffer.h lib/typedefs.h lib/str.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_truncfile.o: lib/buffer/buffer_truncfile.c lib/windoze.h lib/buffer.h lib/typedefs.h lib/open.h lib/windoze.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_mmapread_fd.o: lib/buffer/buffer_mmapread_fd.c lib/buffer.h lib/typedefs.h lib/mmap.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_put.o: lib/buffer/buffer_put.c lib/buffer.h lib/typedefs.h lib/byte.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_peek.o: lib/buffer/buffer_peek.c lib/buffer.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_dummyreadmmap.o: lib/buffer/buffer_dummyreadmmap.c lib/buffer.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_0.o: lib/buffer/buffer_0.c lib/windoze.h lib/buffer.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_putflush.o: lib/buffer/buffer_putflush.c lib/windoze.h lib/buffer.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_write.o: lib/buffer/buffer_write.c lib/windoze.h lib/buffer.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_skipc.o: lib/buffer/buffer_skipc.c lib/buffer.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_mmapshared_fd.o: lib/buffer/buffer_mmapshared_fd.c lib/windoze.h lib/buffer.h lib/typedefs.h lib/mmap.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_putsaflush.o: lib/buffer/buffer_putsaflush.c lib/buffer.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_putsflush.o: lib/buffer/buffer_putsflush.c lib/buffer.h lib/typedefs.h lib/str.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_putnspace.o: lib/buffer/buffer_putnspace.c lib/buffer.h lib/typedefs.h lib/byte.h lib/str.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_skipn.o: lib/buffer/buffer_skipn.c lib/buffer.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_dump.o: lib/buffer/buffer_dump.c lib/windoze.h lib/buffer.h lib/typedefs.h lib/fmt.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_0small.o: lib/buffer/buffer_0small.c lib/windoze.h lib/buffer.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_put8long.o: lib/buffer/buffer_put8long.c lib/buffer.h lib/typedefs.h lib/fmt.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_putnc.o: lib/buffer/buffer_putnc.c lib/buffer.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_inflate.o: lib/buffer/buffer_inflate.c lib/windoze.h lib/buffer.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_putxlonglong0.o: lib/buffer/buffer_putxlonglong0.c lib/buffer.h lib/typedefs.h lib/fmt.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_get_token_pred.o: lib/buffer/buffer_get_token_pred.c lib/buffer.h lib/typedefs.h lib/byte.h lib/scan.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_getc.o: lib/buffer/buffer_getc.c lib/buffer.h lib/typedefs.h lib/byte.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_1small.o: lib/buffer/buffer_1small.c lib/windoze.h lib/buffer.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_close.o: lib/buffer/buffer_close.c lib/windoze.h lib/buffer.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_bzip.o: lib/buffer/buffer_bzip.c lib/buffer.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_stubborn.o: lib/buffer/buffer_stubborn.c lib/windoze.h lib/buffer.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_skip_pred.o: lib/buffer/buffer_skip_pred.c lib/buffer.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_skipspace.o: lib/buffer/buffer_skipspace.c lib/buffer.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_dummyread.o: lib/buffer/buffer_dummyread.c lib/buffer.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_putulonglong.o: lib/buffer/buffer_putulonglong.c lib/buffer.h lib/typedefs.h lib/fmt.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_putm_internal_flush.o: lib/buffer/buffer_putm_internal_flush.c lib/buffer.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_frombuf.o: lib/buffer/buffer_frombuf.c lib/buffer.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_putulong.o: lib/buffer/buffer_putulong.c lib/buffer.h lib/typedefs.h lib/fmt.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_putnlflush.o: lib/buffer/buffer_putnlflush.c lib/buffer.h lib/typedefs.h lib/str.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_read_fd.o: lib/buffer/buffer_read_fd.c lib/windoze.h lib/buffer.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_putptr.o: lib/buffer/buffer_putptr.c lib/buffer.h lib/typedefs.h lib/fmt.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_gzip.o: lib/buffer/buffer_gzip.c lib/buffer.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_get_token.o: lib/buffer/buffer_get_token.c lib/buffer.h lib/typedefs.h lib/byte.h lib/scan.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_putm_internal.o: lib/buffer/buffer_putm_internal.c lib/buffer.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_write_fd.o: lib/buffer/buffer_write_fd.c lib/windoze.h lib/buffer.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_mmapprivate_fd.o: lib/buffer/buffer_mmapprivate_fd.c lib/buffer.h lib/typedefs.h lib/mmap.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_stubborn2.o: lib/buffer/buffer_stubborn2.c lib/windoze.h lib/buffer.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_get_until.o: lib/buffer/buffer_get_until.c lib/buffer.h lib/typedefs.h lib/byte.h lib/scan.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_putxlonglong.o: lib/buffer/buffer_putxlonglong.c lib/buffer.h lib/typedefs.h lib/fmt.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_putspace.o: lib/buffer/buffer_putspace.c lib/buffer.h lib/typedefs.h lib/str.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_putsa.o: lib/buffer/buffer_putsa.c lib/buffer.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_putlong.o: lib/buffer/buffer_putlong.c lib/buffer.h lib/typedefs.h lib/fmt.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_flush.o: lib/buffer/buffer_flush.c lib/buffer.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_prefetch.o: lib/buffer/buffer_prefetch.c lib/buffer.h lib/typedefs.h lib/byte.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
