$(BUILDDIR)reg2cmd.o: reg2cmd.c lib/windoze.h lib/io.h lib/windoze.h lib/iarray.h lib/uint64.h lib/typedefs.h lib/taia.h lib/tai.h lib/uint32.h lib/iarray.h lib/stralloc.h lib/uint64.h lib/buffer.h lib/open.h lib/fmt.h lib/byte.h lib/str.h lib/scan.h lib/path.h lib/stralloc.h lib/str.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)strarraytest.o: strarraytest.c lib/buffer.h lib/typedefs.h lib/mmap.h lib/uint64.h lib/stralloc.h lib/strarray.h lib/array.h lib/stralloc.h lib/byte.h lib/str.h lib/byte.h lib/array.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)eagle-gen-cmds.o: eagle-gen-cmds.c lib/unix.h lib/typedefs.h lib/windoze.h lib/getopt.h lib/readlink.h lib/byte.h lib/array.h lib/uint64.h lib/open.h lib/buffer.h lib/cb.h lib/cbmap.h lib/uint8.h lib/errmsg.h lib/fmt.h lib/uint32.h lib/hmap.h lib/stralloc.h lib/buffer.h lib/iterator.h lib/mmap.h lib/scan.h lib/str.h lib/stralloc.h lib/strarray.h lib/array.h lib/byte.h lib/strlist.h lib/xml.h lib/hmap.h lib/textbuf.h lib/round.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cofflist.o: cofflist.c lib/unix.h lib/typedefs.h lib/windoze.h lib/getopt.h lib/readlink.h lib/uint64.h lib/buffer.h lib/mmap.h lib/uint64.h lib/coff.h lib/uint8.h lib/uint16.h lib/uint32.h lib/range.h lib/str.h lib/stralloc.h lib/byte.h lib/fmt.h lib/errmsg.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ar-wrap.o: ar-wrap.c lib/windoze.h lib/strlist.h lib/stralloc.h lib/typedefs.h lib/byte.h lib/uint64.h lib/buffer.h lib/strarray.h lib/array.h lib/open.h lib/windoze.h lib/stralloc.h lib/buffer.h lib/path.h lib/str.h lib/process.h lib/env.h lib/errmsg.h lib/byte.h lib/mmap.h lib/scan.h lib/uint32.h lib/wait.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)pkgcfg.o: pkgcfg.c lib/unix.h lib/typedefs.h lib/windoze.h lib/getopt.h lib/readlink.h lib/algorithm.h lib/iterator.h lib/buffer.h lib/byte.h lib/cbmap.h lib/uint8.h lib/dir.h lib/uint64.h lib/env.h lib/errmsg.h lib/iterator.h lib/path.h lib/stralloc.h lib/str.h lib/slist.h lib/alloc.h lib/str.h lib/stralloc.h lib/strarray.h lib/array.h lib/byte.h lib/strlist.h lib/buffer.h lib/wordexp.h lib/io.h lib/iarray.h lib/taia.h lib/tai.h lib/uint32.h lib/iarray.h lib/ndelay.h lib/mmap.h lib/array.h lib/socket.h lib/uint16.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)io_finishandshutdown.o: lib/io/io_finishandshutdown.c lib/io.h lib/windoze.h lib/iarray.h lib/uint64.h lib/typedefs.h lib/taia.h lib/tai.h lib/uint32.h lib/io_internal.h lib/array.h lib/io.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)io_getcookie.o: lib/io/io_getcookie.c lib/io.h lib/windoze.h lib/iarray.h lib/uint64.h lib/typedefs.h lib/taia.h lib/tai.h lib/uint32.h lib/windoze.h lib/io_internal.h lib/array.h lib/io.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)io_eagain.o: lib/io/io_eagain.c lib/io.h lib/windoze.h lib/iarray.h lib/uint64.h lib/typedefs.h lib/taia.h lib/tai.h lib/uint32.h lib/io_internal.h lib/array.h lib/io.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)io_timeout.o: lib/io/io_timeout.c lib/io.h lib/windoze.h lib/iarray.h lib/uint64.h lib/typedefs.h lib/taia.h lib/tai.h lib/uint32.h lib/io_internal.h lib/array.h lib/io.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)io_eagain_read.o: lib/io/io_eagain_read.c lib/io.h lib/windoze.h lib/iarray.h lib/uint64.h lib/typedefs.h lib/taia.h lib/tai.h lib/uint32.h lib/io_internal.h lib/array.h lib/io.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)io_eagain_write.o: lib/io/io_eagain_write.c lib/io.h lib/windoze.h lib/iarray.h lib/uint64.h lib/typedefs.h lib/taia.h lib/tai.h lib/uint32.h lib/io_internal.h lib/array.h lib/io.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)io_dup.o: lib/io/io_dup.c lib/windoze.h lib/io.h lib/windoze.h lib/iarray.h lib/uint64.h lib/typedefs.h lib/taia.h lib/tai.h lib/uint32.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)iopause.o: lib/io/iopause.c lib/io.h lib/windoze.h lib/iarray.h lib/uint64.h lib/typedefs.h lib/taia.h lib/tai.h lib/uint32.h lib/socket_internal.h lib/socket.h lib/uint16.h lib/select.h lib/io_internal.h lib/array.h lib/io.h lib/iopause.h lib/taia.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)io_timeouted.o: lib/io/io_timeouted.c lib/io.h lib/windoze.h lib/iarray.h lib/uint64.h lib/typedefs.h lib/taia.h lib/tai.h lib/uint32.h lib/io_internal.h lib/array.h lib/io.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)io_wait.o: lib/io/io_wait.c lib/io.h lib/windoze.h lib/iarray.h lib/uint64.h lib/typedefs.h lib/taia.h lib/tai.h lib/uint32.h lib/io_internal.h lib/array.h lib/io.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)io_passfd.o: lib/io/io_passfd.c lib/io.h lib/windoze.h lib/iarray.h lib/uint64.h lib/typedefs.h lib/taia.h lib/tai.h lib/uint32.h lib/windoze.h lib/socket_internal.h lib/socket.h lib/uint16.h lib/io_internal.h lib/array.h lib/io.h lib/typedefs.h lib/byte.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)io_setcookie.o: lib/io/io_setcookie.c lib/io.h lib/windoze.h lib/iarray.h lib/uint64.h lib/typedefs.h lib/taia.h lib/tai.h lib/uint32.h lib/windoze.h lib/io_internal.h lib/array.h lib/io.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)io_block.o: lib/io/io_block.c lib/io.h lib/windoze.h lib/iarray.h lib/uint64.h lib/typedefs.h lib/taia.h lib/tai.h lib/uint32.h lib/windoze.h lib/socket_internal.h lib/socket.h lib/uint16.h lib/io_internal.h lib/array.h lib/io.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)io_canread.o: lib/io/io_canread.c lib/io.h lib/windoze.h lib/iarray.h lib/uint64.h lib/typedefs.h lib/taia.h lib/tai.h lib/uint32.h lib/windoze.h lib/io_internal.h lib/array.h lib/io.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)io_closeonexec.o: lib/io/io_closeonexec.c lib/io.h lib/windoze.h lib/iarray.h lib/uint64.h lib/typedefs.h lib/taia.h lib/tai.h lib/uint32.h lib/windoze.h lib/io_internal.h lib/array.h lib/io.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)io_trywritetimeout.o: lib/io/io_trywritetimeout.c lib/io.h lib/windoze.h lib/iarray.h lib/uint64.h lib/typedefs.h lib/taia.h lib/tai.h lib/uint32.h lib/socket_internal.h lib/socket.h lib/uint16.h lib/io_internal.h lib/array.h lib/io.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)io_debugstring.o: lib/io/io_debugstring.c lib/io.h lib/windoze.h lib/iarray.h lib/uint64.h lib/typedefs.h lib/taia.h lib/tai.h lib/uint32.h lib/windoze.h lib/socket_internal.h lib/socket.h lib/uint16.h lib/fmt.h lib/iarray.h lib/io_internal.h lib/array.h lib/io.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)io_waitwrite.o: lib/io/io_waitwrite.c lib/io.h lib/windoze.h lib/iarray.h lib/uint64.h lib/typedefs.h lib/taia.h lib/tai.h lib/uint32.h lib/socket_internal.h lib/socket.h lib/uint16.h lib/io_internal.h lib/array.h lib/io.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)io_sendfile.o: lib/io/io_sendfile.c lib/io.h lib/windoze.h lib/iarray.h lib/uint64.h lib/typedefs.h lib/taia.h lib/tai.h lib/uint32.h lib/windoze.h lib/socket_internal.h lib/socket.h lib/uint16.h lib/io_internal.h lib/array.h lib/io.h lib/uint8.h lib/iob.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)io_dontwantwrite.o: lib/io/io_dontwantwrite.c lib/io.h lib/windoze.h lib/iarray.h lib/uint64.h lib/typedefs.h lib/taia.h lib/tai.h lib/uint32.h lib/socket_internal.h lib/socket.h lib/uint16.h lib/io_internal.h lib/array.h lib/io.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)io_fd.o: lib/io/io_fd.c lib/io.h lib/windoze.h lib/iarray.h lib/uint64.h lib/typedefs.h lib/taia.h lib/tai.h lib/uint32.h lib/windoze.h lib/socket_internal.h lib/socket.h lib/uint16.h lib/array.h lib/io_internal.h lib/array.h lib/io.h lib/byte.h lib/cas.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)io_waitread.o: lib/io/io_waitread.c lib/io.h lib/windoze.h lib/iarray.h lib/uint64.h lib/typedefs.h lib/taia.h lib/tai.h lib/uint32.h lib/windoze.h lib/uint64.h lib/socket_internal.h lib/socket.h lib/uint16.h lib/io_internal.h lib/array.h lib/io.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)io_wantwrite.o: lib/io/io_wantwrite.c lib/io.h lib/windoze.h lib/iarray.h lib/uint64.h lib/typedefs.h lib/taia.h lib/tai.h lib/uint32.h lib/socket_internal.h lib/socket.h lib/uint16.h lib/io_internal.h lib/array.h lib/io.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)io_tryread.o: lib/io/io_tryread.c lib/io.h lib/windoze.h lib/iarray.h lib/uint64.h lib/typedefs.h lib/taia.h lib/tai.h lib/uint32.h lib/windoze.h lib/byte.h lib/io_internal.h lib/array.h lib/io.h lib/socket_internal.h lib/socket.h lib/uint16.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)io_appendfile.o: lib/io/io_appendfile.c lib/io.h lib/windoze.h lib/iarray.h lib/uint64.h lib/typedefs.h lib/taia.h lib/tai.h lib/uint32.h lib/windoze.h lib/io_internal.h lib/array.h lib/io.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)io_sigpipe.o: lib/io/io_sigpipe.c lib/io.h lib/windoze.h lib/iarray.h lib/uint64.h lib/typedefs.h lib/taia.h lib/tai.h lib/uint32.h lib/io_internal.h lib/array.h lib/io.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)io_tryreadtimeout.o: lib/io/io_tryreadtimeout.c lib/io.h lib/windoze.h lib/iarray.h lib/uint64.h lib/typedefs.h lib/taia.h lib/tai.h lib/uint32.h lib/windoze.h lib/socket_internal.h lib/socket.h lib/uint16.h lib/io_internal.h lib/array.h lib/io.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)io_pipe.o: lib/io/io_pipe.c lib/io.h lib/windoze.h lib/iarray.h lib/uint64.h lib/typedefs.h lib/taia.h lib/tai.h lib/uint32.h lib/windoze.h lib/io_internal.h lib/array.h lib/io.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)io_trywrite.o: lib/io/io_trywrite.c lib/io.h lib/windoze.h lib/iarray.h lib/uint64.h lib/typedefs.h lib/taia.h lib/tai.h lib/uint32.h lib/socket_internal.h lib/socket.h lib/uint16.h lib/io_internal.h lib/array.h lib/io.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)io_canwrite.o: lib/io/io_canwrite.c lib/io.h lib/windoze.h lib/iarray.h lib/uint64.h lib/typedefs.h lib/taia.h lib/tai.h lib/uint32.h lib/windoze.h lib/io_internal.h lib/array.h lib/io.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)io_createfile.o: lib/io/io_createfile.c lib/io.h lib/windoze.h lib/iarray.h lib/uint64.h lib/typedefs.h lib/taia.h lib/tai.h lib/uint32.h lib/windoze.h lib/io_internal.h lib/array.h lib/io.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)io_mmapwritefile.o: lib/io/io_mmapwritefile.c lib/io.h lib/windoze.h lib/iarray.h lib/uint64.h lib/typedefs.h lib/taia.h lib/tai.h lib/uint32.h lib/windoze.h lib/io_internal.h lib/array.h lib/io.h lib/iob.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)io_timedout.o: lib/io/io_timedout.c lib/io.h lib/windoze.h lib/iarray.h lib/uint64.h lib/typedefs.h lib/taia.h lib/tai.h lib/uint32.h lib/io_internal.h lib/array.h lib/io.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)io_close.o: lib/io/io_close.c lib/io.h lib/windoze.h lib/iarray.h lib/uint64.h lib/typedefs.h lib/taia.h lib/tai.h lib/uint32.h lib/windoze.h lib/io_internal.h lib/array.h lib/io.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)io_waituntil.o: lib/io/io_waituntil.c lib/safemult.h lib/uint16.h lib/uint32.h lib/uint64.h lib/io.h lib/windoze.h lib/iarray.h lib/typedefs.h lib/taia.h lib/tai.h lib/taia.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)io_wantread.o: lib/io/io_wantread.c lib/io.h lib/windoze.h lib/iarray.h lib/uint64.h lib/typedefs.h lib/taia.h lib/tai.h lib/uint32.h lib/socket_internal.h lib/socket.h lib/uint16.h lib/io_internal.h lib/array.h lib/io.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)io_check.o: lib/io/io_check.c lib/io.h lib/windoze.h lib/iarray.h lib/uint64.h lib/typedefs.h lib/taia.h lib/tai.h lib/uint32.h lib/io_internal.h lib/array.h lib/io.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)io_readwritefile.o: lib/io/io_readwritefile.c lib/io.h lib/windoze.h lib/iarray.h lib/uint64.h lib/typedefs.h lib/taia.h lib/tai.h lib/uint32.h lib/windoze.h lib/io_internal.h lib/array.h lib/io.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)io_socketpair.o: lib/io/io_socketpair.c lib/io.h lib/windoze.h lib/iarray.h lib/uint64.h lib/typedefs.h lib/taia.h lib/tai.h lib/uint32.h lib/windoze.h lib/socket_internal.h lib/socket.h lib/uint16.h lib/io_internal.h lib/array.h lib/io.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)io_readfile.o: lib/io/io_readfile.c lib/io.h lib/windoze.h lib/iarray.h lib/uint64.h lib/typedefs.h lib/taia.h lib/tai.h lib/uint32.h lib/windoze.h lib/io_internal.h lib/array.h lib/io.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)io_dontwantread.o: lib/io/io_dontwantread.c lib/io.h lib/windoze.h lib/iarray.h lib/uint64.h lib/typedefs.h lib/taia.h lib/tai.h lib/uint32.h lib/socket_internal.h lib/socket.h lib/uint16.h lib/io_internal.h lib/array.h lib/io.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)io_waituntil2.o: lib/io/io_waituntil2.c lib/io.h lib/windoze.h lib/iarray.h lib/uint64.h lib/typedefs.h lib/taia.h lib/tai.h lib/uint32.h lib/socket_internal.h lib/socket.h lib/uint16.h lib/io_internal.h lib/array.h lib/io.h lib/buffer.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)io_nonblock.o: lib/io/io_nonblock.c lib/io.h lib/windoze.h lib/iarray.h lib/uint64.h lib/typedefs.h lib/taia.h lib/tai.h lib/uint32.h lib/windoze.h lib/socket_internal.h lib/socket.h lib/uint16.h lib/io_internal.h lib/array.h lib/io.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)path_dirname.o: lib/path/path_dirname.c lib/path_internal.h lib/str.h lib/typedefs.h lib/path.h lib/windoze.h lib/stralloc.h lib/str.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)path_canonicalize.o: lib/path/path_canonicalize.c lib/windoze.h lib/path_internal.h lib/str.h lib/typedefs.h lib/path.h lib/windoze.h lib/stralloc.h lib/readlink.h lib/buffer.h lib/byte.h lib/str.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)path_skip.o: lib/path/path_skip.c lib/path_internal.h lib/str.h lib/typedefs.h lib/path.h lib/windoze.h lib/stralloc.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)path_len_s.o: lib/path/path_len_s.c lib/path_internal.h lib/str.h lib/typedefs.h lib/path.h lib/windoze.h lib/stralloc.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)path_skip_separator.o: lib/path/path_skip_separator.c lib/path_internal.h lib/str.h lib/typedefs.h lib/path.h lib/windoze.h lib/stralloc.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)symlink.o: lib/path/symlink.c lib/byte.h lib/typedefs.h lib/windoze.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)path_len.o: lib/path/path_len.c lib/path_internal.h lib/str.h lib/typedefs.h lib/path.h lib/windoze.h lib/stralloc.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)path_getcwd.o: lib/path/path_getcwd.c lib/path_internal.h lib/str.h lib/typedefs.h lib/path.h lib/windoze.h lib/stralloc.h lib/windoze.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)path_absolute.o: lib/path/path_absolute.c lib/path_internal.h lib/str.h lib/typedefs.h lib/path.h lib/windoze.h lib/stralloc.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)path_relative.o: lib/path/path_relative.c lib/path_internal.h lib/str.h lib/typedefs.h lib/path.h lib/windoze.h lib/stralloc.h lib/stralloc.h lib/strlist.h lib/byte.h lib/uint64.h lib/buffer.h lib/windoze.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)path_gethome.o: lib/path/path_gethome.c lib/buffer.h lib/typedefs.h lib/path_internal.h lib/str.h lib/path.h lib/windoze.h lib/stralloc.h lib/scan.h lib/uint32.h lib/uint64.h lib/windoze.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)path_split.o: lib/path/path_split.c lib/path_internal.h lib/str.h lib/typedefs.h lib/path.h lib/windoze.h lib/stralloc.h lib/str.h lib/strlist.h lib/byte.h lib/uint64.h lib/buffer.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)path_exists.o: lib/path/path_exists.c lib/path_internal.h lib/str.h lib/typedefs.h lib/path.h lib/windoze.h lib/stralloc.h lib/windoze.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)path_is_absolute.o: lib/path/path_is_absolute.c lib/path_internal.h lib/str.h lib/typedefs.h lib/path.h lib/windoze.h lib/stralloc.h lib/str.h lib/windoze.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)path_collapse.o: lib/path/path_collapse.c lib/path_internal.h lib/str.h lib/typedefs.h lib/path.h lib/windoze.h lib/stralloc.h lib/byte.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)path_getsep.o: lib/path/path_getsep.c lib/path_internal.h lib/str.h lib/typedefs.h lib/path.h lib/windoze.h lib/stralloc.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)path_num_sa.o: lib/path/path_num_sa.c lib/path_internal.h lib/str.h lib/typedefs.h lib/path.h lib/windoze.h lib/stralloc.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)path_basename.o: lib/path/path_basename.c lib/path_internal.h lib/str.h lib/typedefs.h lib/path.h lib/windoze.h lib/stralloc.h lib/str.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)path_num.o: lib/path/path_num.c lib/path_internal.h lib/str.h lib/typedefs.h lib/path.h lib/windoze.h lib/stralloc.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)path_fnmatch.o: lib/path/path_fnmatch.c lib/path_internal.h lib/str.h lib/typedefs.h lib/path.h lib/windoze.h lib/stralloc.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)path_skips.o: lib/path/path_skips.c lib/path_internal.h lib/str.h lib/typedefs.h lib/path.h lib/windoze.h lib/stralloc.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)path_right.o: lib/path/path_right.c lib/path_internal.h lib/str.h lib/typedefs.h lib/path.h lib/windoze.h lib/stralloc.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)path_is_separator.o: lib/path/path_is_separator.c lib/path_internal.h lib/str.h lib/typedefs.h lib/path.h lib/windoze.h lib/stralloc.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)path_is_directory.o: lib/path/path_is_directory.c lib/path_internal.h lib/str.h lib/typedefs.h lib/path.h lib/windoze.h lib/stralloc.h lib/windoze.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)path_find.o: lib/path/path_find.c lib/path_internal.h lib/str.h lib/typedefs.h lib/path.h lib/windoze.h lib/stralloc.h lib/dir.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)path_absolute_sa.o: lib/path/path_absolute_sa.c lib/path_internal.h lib/str.h lib/typedefs.h lib/path.h lib/windoze.h lib/stralloc.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)path_canonical.o: lib/path/path_canonical.c lib/path_internal.h lib/str.h lib/typedefs.h lib/path.h lib/windoze.h lib/stralloc.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)path_readlink.o: lib/path/path_readlink.c lib/windoze.h lib/unix.h lib/typedefs.h lib/windoze.h lib/getopt.h lib/readlink.h lib/path_internal.h lib/str.h lib/path.h lib/stralloc.h lib/readlink.h lib/str.h lib/stralloc.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)path_canonical_sa.o: lib/path/path_canonical_sa.c lib/path_internal.h lib/str.h lib/typedefs.h lib/path.h lib/windoze.h lib/stralloc.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)path_realpath.o: lib/path/path_realpath.c lib/byte.h lib/typedefs.h lib/path_internal.h lib/str.h lib/path.h lib/windoze.h lib/stralloc.h lib/windoze.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)strarray_each.o: lib/strarray/strarray_each.c lib/strarray.h lib/uint64.h lib/array.h lib/typedefs.h lib/stralloc.h lib/byte.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)strarray_removeprefixs.o: lib/strarray/strarray_removeprefixs.c lib/str.h lib/typedefs.h lib/stralloc.h lib/strarray.h lib/uint64.h lib/array.h lib/stralloc.h lib/byte.h lib/byte.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)strarray_pop.o: lib/strarray/strarray_pop.c lib/strarray.h lib/uint64.h lib/array.h lib/typedefs.h lib/stralloc.h lib/byte.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)strarray_push.o: lib/strarray/strarray_push.c lib/array.h lib/typedefs.h lib/uint64.h lib/str.h lib/strarray.h lib/array.h lib/stralloc.h lib/byte.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)strarray_sort.o: lib/strarray/strarray_sort.c lib/strarray.h lib/uint64.h lib/array.h lib/typedefs.h lib/stralloc.h lib/byte.h lib/str.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)strarray_set.o: lib/strarray/strarray_set.c lib/array.h lib/typedefs.h lib/uint64.h lib/str.h lib/strarray.h lib/array.h lib/stralloc.h lib/byte.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)strarray_from_argv.o: lib/strarray/strarray_from_argv.c lib/strarray.h lib/uint64.h lib/array.h lib/typedefs.h lib/stralloc.h lib/byte.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)strarray_joins.o: lib/strarray/strarray_joins.c lib/str.h lib/typedefs.h lib/stralloc.h lib/strarray.h lib/uint64.h lib/array.h lib/stralloc.h lib/byte.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)strarray_glob.o: lib/strarray/strarray_glob.c lib/glob.h lib/strarray.h lib/uint64.h lib/array.h lib/typedefs.h lib/stralloc.h lib/byte.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)strarray_push_sa.o: lib/strarray/strarray_push_sa.c lib/stralloc.h lib/typedefs.h lib/strarray.h lib/uint64.h lib/array.h lib/stralloc.h lib/byte.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)strarray_transform.o: lib/strarray/strarray_transform.c lib/str.h lib/typedefs.h lib/stralloc.h lib/strarray.h lib/uint64.h lib/array.h lib/stralloc.h lib/byte.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)strarray_union.o: lib/strarray/strarray_union.c lib/strarray.h lib/uint64.h lib/array.h lib/typedefs.h lib/stralloc.h lib/byte.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)strarray_appends.o: lib/strarray/strarray_appends.c lib/str.h lib/typedefs.h lib/stralloc.h lib/strarray.h lib/uint64.h lib/array.h lib/stralloc.h lib/byte.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)strarray_push_unique.o: lib/strarray/strarray_push_unique.c lib/array.h lib/typedefs.h lib/uint64.h lib/str.h lib/strarray.h lib/array.h lib/stralloc.h lib/byte.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)strarray_intersection.o: lib/strarray/strarray_intersection.c lib/strarray.h lib/uint64.h lib/array.h lib/typedefs.h lib/stralloc.h lib/byte.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)strarray_prepends.o: lib/strarray/strarray_prepends.c lib/str.h lib/typedefs.h lib/stralloc.h lib/strarray.h lib/uint64.h lib/array.h lib/stralloc.h lib/byte.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)strarray_removesuffixs.o: lib/strarray/strarray_removesuffixs.c lib/str.h lib/typedefs.h lib/stralloc.h lib/strarray.h lib/uint64.h lib/array.h lib/stralloc.h lib/byte.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)strarray_index_of_sa.o: lib/strarray/strarray_index_of_sa.c lib/stralloc.h lib/typedefs.h lib/strarray.h lib/uint64.h lib/array.h lib/stralloc.h lib/byte.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)strarray_setb.o: lib/strarray/strarray_setb.c lib/array.h lib/typedefs.h lib/uint64.h lib/str.h lib/strarray.h lib/array.h lib/stralloc.h lib/byte.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)strarray_index_of.o: lib/strarray/strarray_index_of.c lib/str.h lib/typedefs.h lib/strarray.h lib/uint64.h lib/array.h lib/stralloc.h lib/byte.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)strarray_pushb.o: lib/strarray/strarray_pushb.c lib/str.h lib/typedefs.h lib/strarray.h lib/uint64.h lib/array.h lib/stralloc.h lib/byte.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)strarray_free.o: lib/strarray/strarray_free.c lib/alloc.h lib/typedefs.h lib/strarray.h lib/uint64.h lib/array.h lib/stralloc.h lib/byte.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)strarray_splice.o: lib/strarray/strarray_splice.c lib/strarray.h lib/uint64.h lib/array.h lib/typedefs.h lib/stralloc.h lib/byte.h lib/array.h lib/byte.h lib/safemult.h lib/uint16.h lib/uint32.h lib/uint64.h lib/str.h lib/alloc.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)strarray_to_argv.o: lib/strarray/strarray_to_argv.c lib/alloc.h lib/typedefs.h lib/byte.h lib/strarray.h lib/uint64.h lib/array.h lib/stralloc.h lib/byte.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)strarray_pushd.o: lib/strarray/strarray_pushd.c lib/str.h lib/typedefs.h lib/strarray.h lib/uint64.h lib/array.h lib/stralloc.h lib/byte.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)strarray_index_of_b.o: lib/strarray/strarray_index_of_b.c lib/str.h lib/typedefs.h lib/strarray.h lib/uint64.h lib/array.h lib/stralloc.h lib/byte.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)uint64_read.o: lib/uint64/uint64_read.c lib/uint64.h lib/uint32.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)uint64_unpack.o: lib/uint64/uint64_unpack.c lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)uint64_pack_big.o: lib/uint64/uint64_pack_big.c lib/uint64.h lib/uint32.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)uint64_unpack_big.o: lib/uint64/uint64_unpack_big.c lib/uint64.h lib/uint32.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)uint64_pack.o: lib/uint64/uint64_pack.c lib/uint64.h lib/uint32.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)uint64_read_big.o: lib/uint64/uint64_read_big.c lib/uint64.h lib/uint32.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)hmap_add.o: lib/hmap/hmap_add.c lib/byte.h lib/typedefs.h lib/alloc.h lib/hmap_internal.h lib/byte.h lib/hmap.h lib/stralloc.h lib/buffer.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)hmap_size.o: lib/hmap/hmap_size.c lib/hmap_internal.h lib/byte.h lib/typedefs.h lib/hmap.h lib/stralloc.h lib/buffer.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)hmap_dump.o: lib/hmap/hmap_dump.c lib/uint64.h lib/buffer.h lib/typedefs.h lib/hmap.h lib/stralloc.h lib/buffer.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)hmap_add_tuple_with_data.o: lib/hmap/hmap_add_tuple_with_data.c lib/byte.h lib/typedefs.h lib/hmap_internal.h lib/byte.h lib/hmap.h lib/stralloc.h lib/buffer.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)hmap_free_data.o: lib/hmap/hmap_free_data.c lib/hmap_internal.h lib/byte.h lib/typedefs.h lib/hmap.h lib/stralloc.h lib/buffer.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)hmap_delete.o: lib/hmap/hmap_delete.c lib/hmap_internal.h lib/byte.h lib/typedefs.h lib/hmap.h lib/stralloc.h lib/buffer.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)hmap_print_table.o: lib/hmap/hmap_print_table.c lib/uint64.h lib/buffer.h lib/typedefs.h lib/hmap_internal.h lib/byte.h lib/hmap.h lib/stralloc.h lib/buffer.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)hmap_truncate.o: lib/hmap/hmap_truncate.c lib/hmap_internal.h lib/byte.h lib/typedefs.h lib/hmap.h lib/stralloc.h lib/buffer.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)hmap_set_stralloc.o: lib/hmap/hmap_set_stralloc.c lib/hmap.h lib/stralloc.h lib/typedefs.h lib/buffer.h lib/buffer.h lib/stralloc.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)hmap_set_chars.o: lib/hmap/hmap_set_chars.c lib/hmap.h lib/stralloc.h lib/typedefs.h lib/buffer.h lib/buffer.h lib/str.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)hmap_init.o: lib/hmap/hmap_init.c lib/hmap_internal.h lib/byte.h lib/typedefs.h lib/hmap.h lib/stralloc.h lib/buffer.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)hmap_print_list.o: lib/hmap/hmap_print_list.c lib/uint64.h lib/buffer.h lib/typedefs.h lib/hmap_internal.h lib/byte.h lib/hmap.h lib/stralloc.h lib/buffer.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)hmap_is_locate.o: lib/hmap/hmap_is_locate.c lib/hmap_internal.h lib/byte.h lib/typedefs.h lib/hmap.h lib/stralloc.h lib/buffer.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)hmap_hash.o: lib/hmap/hmap_hash.c lib/hmap_internal.h lib/byte.h lib/typedefs.h lib/hmap.h lib/stralloc.h lib/buffer.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)hmap_get.o: lib/hmap/hmap_get.c lib/hmap.h lib/stralloc.h lib/typedefs.h lib/buffer.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)hmap_distance.o: lib/hmap/hmap_distance.c lib/hmap_internal.h lib/byte.h lib/typedefs.h lib/hmap.h lib/stralloc.h lib/buffer.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)hmap_print_tree.o: lib/hmap/hmap_print_tree.c lib/uint64.h lib/buffer.h lib/typedefs.h lib/hmap_internal.h lib/byte.h lib/hmap.h lib/stralloc.h lib/buffer.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)hmap_destroy.o: lib/hmap/hmap_destroy.c lib/hmap_internal.h lib/byte.h lib/typedefs.h lib/hmap.h lib/stralloc.h lib/buffer.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)hmap_set.o: lib/hmap/hmap_set.c lib/alloc.h lib/typedefs.h lib/hmap_internal.h lib/byte.h lib/hmap.h lib/stralloc.h lib/buffer.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)hmap_cmp.o: lib/hmap/hmap_cmp.c lib/hmap_internal.h lib/byte.h lib/typedefs.h lib/hmap.h lib/stralloc.h lib/buffer.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)hmap_search.o: lib/hmap/hmap_search.c lib/hmap_internal.h lib/byte.h lib/typedefs.h lib/hmap.h lib/stralloc.h lib/buffer.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)binfmt_open.o: lib/binfmt/binfmt_open.c lib/binfmt.h lib/typedefs.h lib/uint8.h lib/uint32.h lib/uint64.h lib/range.h lib/mmap.h lib/byte.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)env_put.o: lib/env/env_put.c lib/alloc.h lib/typedefs.h lib/windoze.h lib/typedefs.h lib/env.h lib/byte.h lib/str.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)env_make.o: lib/env/env_make.c lib/env.h lib/typedefs.h lib/str.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)env_addmodif.o: lib/env/env_addmodif.c lib/env.h lib/typedefs.h lib/stralloc.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)env_set.o: lib/env/env_set.c lib/windoze.h lib/env.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)env_len.o: lib/env/env_len.c lib/env.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)env_get.o: lib/env/env_get.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)env_unset.o: lib/env/env_unset.c lib/windoze.h lib/env.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)env_merge.o: lib/env/env_merge.c lib/byte.h lib/typedefs.h lib/env.h lib/str.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)env_pick.o: lib/env/env_pick.c lib/windoze.h lib/env.h lib/typedefs.h lib/str.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)env_string.o: lib/env/env_string.c lib/env.h lib/typedefs.h lib/stralloc.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)env_get2.o: lib/env/env_get2.c lib/env.h lib/typedefs.h lib/str.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)http_ssl_read.o: lib/http/http_ssl_read.c lib/http.h lib/uint16.h lib/uint32.h lib/uint64.h lib/stralloc.h lib/typedefs.h lib/buffer.h lib/ip4.h lib/uint8.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)http_read.o: lib/http/http_read.c lib/http.h lib/uint16.h lib/uint32.h lib/uint64.h lib/stralloc.h lib/typedefs.h lib/buffer.h lib/ip4.h lib/uint8.h lib/scan.h lib/socket_internal.h lib/socket.h lib/str.h lib/stralloc.h lib/io.h lib/windoze.h lib/iarray.h lib/taia.h lib/tai.h lib/byte.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)http_ssl_socket.o: lib/http/http_ssl_socket.c lib/http.h lib/uint16.h lib/uint32.h lib/uint64.h lib/stralloc.h lib/typedefs.h lib/buffer.h lib/ip4.h lib/uint8.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)http_ssl_write.o: lib/http/http_ssl_write.c lib/http.h lib/uint16.h lib/uint32.h lib/uint64.h lib/stralloc.h lib/typedefs.h lib/buffer.h lib/ip4.h lib/uint8.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)http_ssl_io.o: lib/http/http_ssl_io.c lib/http.h lib/uint16.h lib/uint32.h lib/uint64.h lib/stralloc.h lib/typedefs.h lib/buffer.h lib/ip4.h lib/uint8.h lib/io.h lib/windoze.h lib/iarray.h lib/taia.h lib/tai.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)http_readable.o: lib/http/http_readable.c lib/windoze.h lib/errmsg.h lib/buffer.h lib/typedefs.h lib/byte.h lib/http.h lib/uint16.h lib/uint32.h lib/uint64.h lib/stralloc.h lib/buffer.h lib/ip4.h lib/uint8.h lib/io.h lib/windoze.h lib/iarray.h lib/taia.h lib/tai.h lib/scan.h lib/stralloc.h lib/str.h lib/socket.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)http_socket_read.o: lib/http/http_socket_read.c lib/http.h lib/uint16.h lib/uint32.h lib/uint64.h lib/stralloc.h lib/typedefs.h lib/buffer.h lib/ip4.h lib/uint8.h lib/scan.h lib/socket_internal.h lib/socket.h lib/str.h lib/stralloc.h lib/io.h lib/windoze.h lib/iarray.h lib/taia.h lib/tai.h lib/byte.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)http_init.o: lib/http/http_init.c lib/socket_internal.h lib/socket.h lib/typedefs.h lib/uint16.h lib/uint32.h lib/http.h lib/uint64.h lib/stralloc.h lib/buffer.h lib/ip4.h lib/uint8.h lib/byte.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)http_ssl_ctx.o: lib/http/http_ssl_ctx.c lib/http.h lib/uint16.h lib/uint32.h lib/uint64.h lib/stralloc.h lib/typedefs.h lib/buffer.h lib/ip4.h lib/uint8.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)http_socket_write.o: lib/http/http_socket_write.c lib/http.h lib/uint16.h lib/uint32.h lib/uint64.h lib/stralloc.h lib/typedefs.h lib/buffer.h lib/ip4.h lib/uint8.h lib/scan.h lib/socket_internal.h lib/socket.h lib/str.h lib/stralloc.h lib/io.h lib/windoze.h lib/iarray.h lib/taia.h lib/tai.h lib/byte.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)http_sendreq.o: lib/http/http_sendreq.c lib/socket_internal.h lib/socket.h lib/typedefs.h lib/uint16.h lib/uint32.h lib/buffer.h lib/http.h lib/uint64.h lib/stralloc.h lib/buffer.h lib/ip4.h lib/uint8.h lib/byte.h lib/io.h lib/windoze.h lib/iarray.h lib/taia.h lib/tai.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)http_read_internal.o: lib/http/http_read_internal.c lib/http.h lib/uint16.h lib/uint32.h lib/uint64.h lib/stralloc.h lib/typedefs.h lib/buffer.h lib/ip4.h lib/uint8.h lib/scan.h lib/socket_internal.h lib/socket.h lib/str.h lib/stralloc.h lib/io.h lib/windoze.h lib/iarray.h lib/taia.h lib/tai.h lib/byte.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)http_ssl_connect.o: lib/http/http_ssl_connect.c lib/http.h lib/uint16.h lib/uint32.h lib/uint64.h lib/stralloc.h lib/typedefs.h lib/buffer.h lib/ip4.h lib/uint8.h lib/io.h lib/windoze.h lib/iarray.h lib/taia.h lib/tai.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)http_socket.o: lib/http/http_socket.c lib/windoze.h lib/socket_internal.h lib/socket.h lib/typedefs.h lib/uint16.h lib/uint32.h lib/buffer.h lib/http.h lib/uint64.h lib/stralloc.h lib/buffer.h lib/ip4.h lib/uint8.h lib/io.h lib/windoze.h lib/iarray.h lib/taia.h lib/tai.h lib/ndelay.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)http_close.o: lib/http/http_close.c lib/http.h lib/uint16.h lib/uint32.h lib/uint64.h lib/stralloc.h lib/typedefs.h lib/buffer.h lib/ip4.h lib/uint8.h lib/windoze.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)http_writeable.o: lib/http/http_writeable.c lib/http.h lib/uint16.h lib/uint32.h lib/uint64.h lib/stralloc.h lib/typedefs.h lib/buffer.h lib/ip4.h lib/uint8.h lib/scan.h lib/str.h lib/stralloc.h lib/io.h lib/windoze.h lib/iarray.h lib/taia.h lib/tai.h lib/byte.h lib/errmsg.h lib/socket.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)http_read_header.o: lib/http/http_read_header.c lib/http.h lib/uint16.h lib/uint32.h lib/uint64.h lib/stralloc.h lib/typedefs.h lib/buffer.h lib/ip4.h lib/uint8.h lib/scan.h lib/socket_internal.h lib/socket.h lib/str.h lib/stralloc.h lib/io.h lib/windoze.h lib/iarray.h lib/taia.h lib/tai.h lib/byte.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)http_ssl_error.o: lib/http/http_ssl_error.c lib/http.h lib/uint16.h lib/uint32.h lib/uint64.h lib/stralloc.h lib/typedefs.h lib/buffer.h lib/ip4.h lib/uint8.h lib/io.h lib/windoze.h lib/iarray.h lib/taia.h lib/tai.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)http_get.o: lib/http/http_get.c lib/alloc.h lib/typedefs.h lib/socket_internal.h lib/socket.h lib/uint16.h lib/uint32.h lib/socket.h lib/scan.h lib/uint64.h lib/buffer.h lib/byte.h lib/dns.h lib/iopause.h lib/io.h lib/windoze.h lib/iarray.h lib/taia.h lib/tai.h lib/stralloc.h lib/errmsg.h lib/http.h lib/buffer.h lib/ip4.h lib/uint8.h lib/io.h lib/ip4.h lib/str.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)rdir_close.o: lib/rdir/rdir_close.c lib/dir_internal.h lib/dir.h lib/uint64.h lib/rdir.h lib/dir_internal.h lib/stralloc.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)rdir_open.o: lib/rdir/rdir_open.c lib/rdir.h lib/dir.h lib/uint64.h lib/dir_internal.h lib/stralloc.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)rdir_read.o: lib/rdir/rdir_read.c lib/rdir.h lib/dir.h lib/uint64.h lib/dir_internal.h lib/stralloc.h lib/typedefs.h lib/buffer.h lib/byte.h lib/dir_internal.h lib/stralloc.h lib/str.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)pe_offset2rva.o: lib/pe/pe_offset2rva.c lib/pe.h lib/typedefs.h lib/uint64.h lib/uint32.h lib/uint16.h lib/uint8.h lib/uint32.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)pe_header.o: lib/pe/pe_header.c lib/uint16.h lib/pe.h lib/typedefs.h lib/uint64.h lib/uint32.h lib/uint16.h lib/uint8.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)pe_rva2section.o: lib/pe/pe_rva2section.c lib/pe.h lib/typedefs.h lib/uint64.h lib/uint32.h lib/uint16.h lib/uint8.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)pe_get_datadir.o: lib/pe/pe_get_datadir.c lib/pe.h lib/typedefs.h lib/uint64.h lib/uint32.h lib/uint16.h lib/uint8.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)pe_rva2offset.o: lib/pe/pe_rva2offset.c lib/pe.h lib/typedefs.h lib/uint64.h lib/uint32.h lib/uint16.h lib/uint8.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)pe_rva2ptr.o: lib/pe/pe_rva2ptr.c lib/pe.h lib/typedefs.h lib/uint64.h lib/uint32.h lib/uint16.h lib/uint8.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)pe_get_value.o: lib/pe/pe_get_value.c lib/pe.h lib/typedefs.h lib/uint64.h lib/uint32.h lib/uint16.h lib/uint8.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)pe_get_section.o: lib/pe/pe_get_section.c lib/pe.h lib/typedefs.h lib/uint64.h lib/uint32.h lib/uint16.h lib/uint8.h lib/str.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)pe_datadir_name.o: lib/pe/pe_datadir_name.c lib/pe.h lib/typedefs.h lib/uint64.h lib/uint32.h lib/uint16.h lib/uint8.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)pe_thunk.o: lib/pe/pe_thunk.c lib/pe.h lib/typedefs.h lib/uint64.h lib/uint32.h lib/uint16.h lib/uint8.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_free.o: lib/buffer/buffer_free.c lib/alloc.h lib/typedefs.h lib/buffer.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_dummyread.o: lib/buffer/buffer_dummyread.c lib/buffer.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_putlonglong.o: lib/buffer/buffer_putlonglong.c lib/buffer.h lib/typedefs.h lib/fmt.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_init_free.o: lib/buffer/buffer_init_free.c lib/buffer.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_skipn.o: lib/buffer/buffer_skipn.c lib/buffer.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_putlong0.o: lib/buffer/buffer_putlong0.c lib/buffer.h lib/typedefs.h lib/fmt.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_putflush.o: lib/buffer/buffer_putflush.c lib/windoze.h lib/buffer.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_putxlonglong0.o: lib/buffer/buffer_putxlonglong0.c lib/buffer.h lib/typedefs.h lib/fmt.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_flush.o: lib/buffer/buffer_flush.c lib/buffer.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_default.o: lib/buffer/buffer_default.c lib/buffer.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_dummyreadmmap.o: lib/buffer/buffer_dummyreadmmap.c lib/buffer.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_skip_pred.o: lib/buffer/buffer_skip_pred.c lib/buffer.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_putc.o: lib/buffer/buffer_putc.c lib/buffer.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_peekc.o: lib/buffer/buffer_peekc.c lib/buffer.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_puts.o: lib/buffer/buffer_puts.c lib/buffer.h lib/typedefs.h lib/str.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_munmap.o: lib/buffer/buffer_munmap.c lib/windoze.h lib/buffer.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_putptr.o: lib/buffer/buffer_putptr.c lib/buffer.h lib/typedefs.h lib/fmt.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_fromarray.o: lib/buffer/buffer_fromarray.c lib/array.h lib/typedefs.h lib/uint64.h lib/buffer.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_gzip.o: lib/buffer/buffer_gzip.c lib/buffer.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_getline.o: lib/buffer/buffer_getline.c lib/buffer.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_read.o: lib/buffer/buffer_read.c lib/windoze.h lib/buffer.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_puttai.o: lib/buffer/buffer_puttai.c lib/buffer.h lib/typedefs.h lib/tai.h lib/uint64.h lib/fmt.h lib/uint32.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_putnspace.o: lib/buffer/buffer_putnspace.c lib/alloc.h lib/typedefs.h lib/buffer.h lib/byte.h lib/str.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_write_fd.o: lib/buffer/buffer_write_fd.c lib/windoze.h lib/alloc.h lib/typedefs.h lib/buffer.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_frombuf.o: lib/buffer/buffer_frombuf.c lib/buffer.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_deflate.o: lib/buffer/buffer_deflate.c lib/buffer.h lib/typedefs.h lib/byte.h lib/str.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_putulong.o: lib/buffer/buffer_putulong.c lib/buffer.h lib/typedefs.h lib/fmt.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_write.o: lib/buffer/buffer_write.c lib/windoze.h lib/buffer.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_putspace.o: lib/buffer/buffer_putspace.c lib/buffer.h lib/typedefs.h lib/str.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_1.o: lib/buffer/buffer_1.c lib/buffer.h lib/typedefs.h lib/windoze.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_putm_internal.o: lib/buffer/buffer_putm_internal.c lib/buffer.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_get_token.o: lib/buffer/buffer_get_token.c lib/buffer.h lib/typedefs.h lib/byte.h lib/scan.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_0.o: lib/buffer/buffer_0.c lib/windoze.h lib/buffer.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_lzma.o: lib/buffer/buffer_lzma.c lib/buffer.h lib/typedefs.h lib/alloc.h lib/uint8.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_1small.o: lib/buffer/buffer_1small.c lib/windoze.h lib/buffer.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_read_fd.o: lib/buffer/buffer_read_fd.c lib/windoze.h lib/buffer.h lib/typedefs.h lib/alloc.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_putalign.o: lib/buffer/buffer_putalign.c lib/buffer.h lib/typedefs.h lib/byte.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_putsa.o: lib/buffer/buffer_putsa.c lib/buffer.h lib/typedefs.h lib/stralloc.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_putdouble.o: lib/buffer/buffer_putdouble.c lib/buffer.h lib/typedefs.h lib/byte.h lib/fmt.h lib/uint32.h lib/uint64.h lib/str.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_put8long.o: lib/buffer/buffer_put8long.c lib/buffer.h lib/typedefs.h lib/fmt.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_putulonglong.o: lib/buffer/buffer_putulonglong.c lib/buffer.h lib/typedefs.h lib/fmt.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_skipspace.o: lib/buffer/buffer_skipspace.c lib/buffer.h lib/typedefs.h lib/byte.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_put.o: lib/buffer/buffer_put.c lib/buffer.h lib/typedefs.h lib/byte.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_mmapprivate.o: lib/buffer/buffer_mmapprivate.c lib/buffer.h lib/typedefs.h lib/mmap.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_0small.o: lib/buffer/buffer_0small.c lib/windoze.h lib/buffer.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_truncfile.o: lib/buffer/buffer_truncfile.c lib/windoze.h lib/buffer.h lib/typedefs.h lib/open.h lib/windoze.h lib/uint64.h lib/alloc.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_fromstr.o: lib/buffer/buffer_fromstr.c lib/buffer.h lib/typedefs.h lib/str.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_copy.o: lib/buffer/buffer_copy.c lib/buffer.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_skip_until.o: lib/buffer/buffer_skip_until.c lib/buffer.h lib/typedefs.h lib/byte.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_fromsa.o: lib/buffer/buffer_fromsa.c lib/buffer.h lib/typedefs.h lib/stralloc.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_putnlflush.o: lib/buffer/buffer_putnlflush.c lib/buffer.h lib/typedefs.h lib/str.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_2.o: lib/buffer/buffer_2.c lib/buffer.h lib/typedefs.h lib/windoze.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_bzip.o: lib/buffer/buffer_bzip.c lib/buffer.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_inflate.o: lib/buffer/buffer_inflate.c lib/windoze.h lib/buffer.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_feed.o: lib/buffer/buffer_feed.c lib/buffer.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_get_token_pred.o: lib/buffer/buffer_get_token_pred.c lib/buffer.h lib/typedefs.h lib/byte.h lib/scan.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_get_until.o: lib/buffer/buffer_get_until.c lib/buffer.h lib/typedefs.h lib/byte.h lib/scan.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_puterror.o: lib/buffer/buffer_puterror.c lib/buffer.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_putsflush.o: lib/buffer/buffer_putsflush.c lib/buffer.h lib/typedefs.h lib/str.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_dump.o: lib/buffer/buffer_dump.c lib/windoze.h lib/buffer.h lib/typedefs.h lib/fmt.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_putsalign.o: lib/buffer/buffer_putsalign.c lib/buffer.h lib/typedefs.h lib/str.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_mmapshared_fd.o: lib/buffer/buffer_mmapshared_fd.c lib/windoze.h lib/buffer.h lib/typedefs.h lib/mmap.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_bz2.o: lib/buffer/buffer_bz2.c lib/buffer.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_putm_internal_flush.o: lib/buffer/buffer_putm_internal_flush.c lib/buffer.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_peek.o: lib/buffer/buffer_peek.c lib/buffer.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_mmapread_fd.o: lib/buffer/buffer_mmapread_fd.c lib/buffer.h lib/typedefs.h lib/mmap.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_putsaflush.o: lib/buffer/buffer_putsaflush.c lib/buffer.h lib/typedefs.h lib/stralloc.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_putns.o: lib/buffer/buffer_putns.c lib/buffer.h lib/typedefs.h lib/str.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_putnc.o: lib/buffer/buffer_putnc.c lib/buffer.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_mmapprivate_fd.o: lib/buffer/buffer_mmapprivate_fd.c lib/buffer.h lib/typedefs.h lib/mmap.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_putxlonglong.o: lib/buffer/buffer_putxlonglong.c lib/buffer.h lib/typedefs.h lib/fmt.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_close.o: lib/buffer/buffer_close.c lib/windoze.h lib/buffer.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_get.o: lib/buffer/buffer_get.c lib/buffer.h lib/typedefs.h lib/byte.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_putulong0.o: lib/buffer/buffer_putulong0.c lib/buffer.h lib/typedefs.h lib/fmt.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_putspad.o: lib/buffer/buffer_putspad.c lib/buffer.h lib/typedefs.h lib/str.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_prefetch.o: lib/buffer/buffer_prefetch.c lib/buffer.h lib/typedefs.h lib/byte.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_freshen.o: lib/buffer/buffer_freshen.c lib/buffer.h lib/typedefs.h lib/byte.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_putlong.o: lib/buffer/buffer_putlong.c lib/buffer.h lib/typedefs.h lib/fmt.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_puterror2.o: lib/buffer/buffer_puterror2.c lib/buffer.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_stubborn.o: lib/buffer/buffer_stubborn.c lib/windoze.h lib/buffer.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_skipc.o: lib/buffer/buffer_skipc.c lib/buffer.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_putxlong.o: lib/buffer/buffer_putxlong.c lib/buffer.h lib/typedefs.h lib/fmt.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_mmapread.o: lib/buffer/buffer_mmapread.c lib/buffer.h lib/typedefs.h lib/mmap.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_seek.o: lib/buffer/buffer_seek.c lib/buffer.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_getc.o: lib/buffer/buffer_getc.c lib/buffer.h lib/typedefs.h lib/byte.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_getn.o: lib/buffer/buffer_getn.c lib/buffer.h lib/typedefs.h lib/byte.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_putxlong0.o: lib/buffer/buffer_putxlong0.c lib/buffer.h lib/typedefs.h lib/fmt.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_init.o: lib/buffer/buffer_init.c lib/buffer.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_stubborn2.o: lib/buffer/buffer_stubborn2.c lib/windoze.h lib/buffer.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ihex_load_record.o: lib/ihex/ihex_load_record.c lib/ihex.h lib/uint8.h lib/uint16.h lib/uint32.h lib/slist.h lib/alloc.h lib/typedefs.h lib/scan.h lib/uint64.h lib/alloc.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ihex_read_record.o: lib/ihex/ihex_read_record.c lib/ihex.h lib/uint8.h lib/uint16.h lib/uint32.h lib/slist.h lib/alloc.h lib/typedefs.h lib/scan.h lib/uint64.h lib/alloc.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ihex_record_at.o: lib/ihex/ihex_record_at.c lib/ihex.h lib/uint8.h lib/uint16.h lib/uint32.h lib/slist.h lib/alloc.h lib/typedefs.h lib/slist.h lib/uint32.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ihex_read_at.o: lib/ihex/ihex_read_at.c lib/ihex.h lib/uint8.h lib/uint16.h lib/uint32.h lib/slist.h lib/alloc.h lib/typedefs.h lib/slist.h lib/uint32.h lib/byte.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ihex_load_buf.o: lib/ihex/ihex_load_buf.c lib/ihex.h lib/uint8.h lib/uint16.h lib/uint32.h lib/slist.h lib/alloc.h lib/typedefs.h lib/byte.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)map_get.o: lib/map/map_get.c lib/map_internal.h lib/map.h lib/byte.h lib/typedefs.h lib/str.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)map_remove.o: lib/map/map_remove.c lib/map_internal.h lib/map.h lib/byte.h lib/typedefs.h lib/str.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)map_getref.o: lib/map/map_getref.c lib/map_internal.h lib/map.h lib/byte.h lib/typedefs.h lib/str.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)map_iter.o: lib/map/map_iter.c lib/map_internal.h lib/map.h lib/byte.h lib/typedefs.h lib/str.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)map_set.o: lib/map/map_set.c lib/byte.h lib/typedefs.h lib/map_internal.h lib/map.h lib/byte.h lib/str.h lib/alloc.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)map_next.o: lib/map/map_next.c lib/map_internal.h lib/map.h lib/byte.h lib/typedefs.h lib/str.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)map_deinit.o: lib/map/map_deinit.c lib/map_internal.h lib/map.h lib/byte.h lib/typedefs.h lib/str.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)byte_fill.o: lib/byte/byte_fill.c lib/byte.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)byte_replace.o: lib/byte/byte_replace.c lib/byte.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)byte_scan.o: lib/byte/byte_scan.c lib/byte.h lib/typedefs.h lib/stralloc.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)byte_copyr.o: lib/byte/byte_copyr.c lib/byte.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)byte_upper.o: lib/byte/byte_upper.c lib/byte.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)byte_diff.o: lib/byte/byte_diff.c lib/byte.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)byte_case_equal.o: lib/byte/byte_case_equal.c lib/byte.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)byte_lower.o: lib/byte/byte_lower.c lib/byte.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)byte_camelize.o: lib/byte/byte_camelize.c lib/byte.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)byte_rchr.o: lib/byte/byte_rchr.c lib/byte.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)byte_findb.o: lib/byte/byte_findb.c lib/byte.h lib/typedefs.h lib/str.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)byte_equal.o: lib/byte/byte_equal.c lib/byte.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)byte_ccopy.o: lib/byte/byte_ccopy.c lib/byte.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)byte_triml.o: lib/byte/byte_triml.c lib/byte.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)byte_zero.o: lib/byte/byte_zero.c lib/byte.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)byte_count.o: lib/byte/byte_count.c lib/byte.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)byte_copy.o: lib/byte/byte_copy.c lib/byte.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)byte_chr.o: lib/byte/byte_chr.c lib/byte.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)byte_fmt.o: lib/byte/byte_fmt.c lib/byte.h lib/typedefs.h lib/stralloc.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)byte_case_start.o: lib/byte/byte_case_start.c lib/byte.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)byte_trimr.o: lib/byte/byte_trimr.c lib/byte.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)byte_finds.o: lib/byte/byte_finds.c lib/byte.h lib/typedefs.h lib/str.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)byte_case_diff.o: lib/byte/byte_case_diff.c lib/byte.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)byte_chrs.o: lib/byte/byte_chrs.c lib/byte.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)mmap_readat.o: lib/mmap/mmap_readat.c lib/open.h lib/typedefs.h lib/windoze.h lib/uint64.h lib/mmap.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)mmap_shared_fd.o: lib/mmap/mmap_shared_fd.c lib/windoze.h lib/io_internal.h lib/windoze.h lib/array.h lib/typedefs.h lib/uint64.h lib/iarray.h lib/taia.h lib/tai.h lib/uint32.h lib/io.h lib/mmap.h lib/open.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)mmap_unmap.o: lib/mmap/mmap_unmap.c lib/open.h lib/typedefs.h lib/windoze.h lib/uint64.h lib/mmap.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)mmap_private_fd.o: lib/mmap/mmap_private_fd.c lib/windoze.h lib/io_internal.h lib/windoze.h lib/array.h lib/typedefs.h lib/uint64.h lib/iarray.h lib/taia.h lib/tai.h lib/uint32.h lib/io.h lib/mmap.h lib/open.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)mmap_rw_fd.o: lib/mmap/mmap_rw_fd.c lib/windoze.h lib/io_internal.h lib/windoze.h lib/array.h lib/typedefs.h lib/uint64.h lib/iarray.h lib/taia.h lib/tai.h lib/uint32.h lib/io.h lib/mmap.h lib/open.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)mmap_private.o: lib/mmap/mmap_private.c lib/open.h lib/typedefs.h lib/windoze.h lib/uint64.h lib/mmap.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)mmap_map.o: lib/mmap/mmap_map.c lib/windoze.h lib/io_internal.h lib/windoze.h lib/array.h lib/typedefs.h lib/uint64.h lib/iarray.h lib/taia.h lib/tai.h lib/uint32.h lib/io.h lib/mmap.h lib/open.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)mmap_read_fd.o: lib/mmap/mmap_read_fd.c lib/windoze.h lib/io_internal.h lib/windoze.h lib/array.h lib/typedefs.h lib/uint64.h lib/iarray.h lib/taia.h lib/tai.h lib/uint32.h lib/io.h lib/mmap.h lib/open.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)mmap_read.o: lib/mmap/mmap_read.c lib/open.h lib/typedefs.h lib/windoze.h lib/uint64.h lib/mmap.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)mmap_shared.o: lib/mmap/mmap_shared.c lib/open.h lib/typedefs.h lib/windoze.h lib/uint64.h lib/mmap.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)gpio_read.o: lib/gpio/gpio_read.c lib/gpio_internal.h lib/gpio.h lib/mmap.h lib/typedefs.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)gpio_set_input_pull.o: lib/gpio/gpio_set_input_pull.c lib/gpio_internal.h lib/gpio.h lib/mmap.h lib/typedefs.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)gpio_set_pin_mode.o: lib/gpio/gpio_set_pin_mode.c lib/gpio_internal.h lib/gpio.h lib/mmap.h lib/typedefs.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)gpio_set_input_pull_up.o: lib/gpio/gpio_set_input_pull_up.c lib/gpio_internal.h lib/gpio.h lib/mmap.h lib/typedefs.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)gpio_set_output.o: lib/gpio/gpio_set_output.c lib/gpio_internal.h lib/gpio.h lib/mmap.h lib/typedefs.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)gpio_write.o: lib/gpio/gpio_write.c lib/gpio_internal.h lib/gpio.h lib/mmap.h lib/typedefs.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)gpio_set_input_pull_down.o: lib/gpio/gpio_set_input_pull_down.c lib/gpio_internal.h lib/gpio.h lib/mmap.h lib/typedefs.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)gpio_init.o: lib/gpio/gpio_init.c lib/windoze.h lib/buffer.h lib/typedefs.h lib/open.h lib/windoze.h lib/uint64.h lib/gpio_internal.h lib/gpio.h lib/mmap.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)gpio_short_wait.o: lib/gpio/gpio_short_wait.c lib/windoze.h lib/gpio_internal.h lib/gpio.h lib/mmap.h lib/typedefs.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)gpio_set_input_resistor_pull.o: lib/gpio/gpio_set_input_resistor_pull.c lib/gpio_internal.h lib/gpio.h lib/mmap.h lib/typedefs.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)scan_asn1dertag.o: lib/scan/scan_asn1dertag.c lib/scan.h lib/typedefs.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)scan_noncharsetnskip.o: lib/scan/scan_noncharsetnskip.c lib/scan.h lib/typedefs.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)scan_eolskip.o: lib/scan/scan_eolskip.c lib/scan.h lib/typedefs.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)scan_utf8_sem.o: lib/scan/scan_utf8_sem.c lib/scan.h lib/typedefs.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)scan_varint.o: lib/scan/scan_varint.c lib/scan.h lib/typedefs.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)scan_xlonglong.o: lib/scan/scan_xlonglong.c lib/scan.h lib/typedefs.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)scan_double.o: lib/scan/scan_double.c lib/scan.h lib/typedefs.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)scan_longn.o: lib/scan/scan_longn.c lib/scan.h lib/typedefs.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)scan_asn1derlength.o: lib/scan/scan_asn1derlength.c lib/scan.h lib/typedefs.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)scan_whitenskip.o: lib/scan/scan_whitenskip.c lib/scan.h lib/typedefs.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)scan_charsetnskip.o: lib/scan/scan_charsetnskip.c lib/scan.h lib/typedefs.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)scan_xlong.o: lib/scan/scan_xlong.c lib/scan.h lib/typedefs.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)scan_ulongn.o: lib/scan/scan_ulongn.c lib/scan.h lib/typedefs.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)scan_netstring.o: lib/scan/scan_netstring.c lib/scan.h lib/typedefs.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)scan_short.o: lib/scan/scan_short.c lib/scan.h lib/typedefs.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)scan_pb_type1_fixed64.o: lib/scan/scan_pb_type1_fixed64.c lib/scan.h lib/typedefs.h lib/uint32.h lib/uint64.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)scan_nonwhitenskip.o: lib/scan/scan_nonwhitenskip.c lib/scan.h lib/typedefs.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)scan_pb_tag.o: lib/scan/scan_pb_tag.c lib/scan.h lib/typedefs.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)scan_uint.o: lib/scan/scan_uint.c lib/scan.h lib/typedefs.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)scan_xchar.o: lib/scan/scan_xchar.c lib/scan.h lib/typedefs.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)scan_xint.o: lib/scan/scan_xint.c lib/scan.h lib/typedefs.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)scan_pb_type0_sint.o: lib/scan/scan_pb_type0_sint.c lib/scan.h lib/typedefs.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)scan_lineskip.o: lib/scan/scan_lineskip.c lib/scan.h lib/typedefs.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)scan_8int.o: lib/scan/scan_8int.c lib/scan.h lib/typedefs.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)scan_8short.o: lib/scan/scan_8short.c lib/scan.h lib/typedefs.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)scan_int.o: lib/scan/scan_int.c lib/scan.h lib/typedefs.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)scan_8longn.o: lib/scan/scan_8longn.c lib/scan.h lib/typedefs.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)scan_utf8.o: lib/scan/scan_utf8.c lib/fmt.h lib/typedefs.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)scan_xlongn.o: lib/scan/scan_xlongn.c lib/scan.h lib/typedefs.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)scan_fromhex.o: lib/scan/scan_fromhex.c lib/scan.h lib/typedefs.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)scan_httpdate.o: lib/scan/scan_httpdate.c lib/windoze.h lib/byte.h lib/typedefs.h lib/case.h lib/scan.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)scan_line.o: lib/scan/scan_line.c lib/scan.h lib/typedefs.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)scan_8long.o: lib/scan/scan_8long.c lib/scan.h lib/typedefs.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)scan_octal.o: lib/scan/scan_octal.c lib/scan.h lib/typedefs.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)scan_long.o: lib/scan/scan_long.c lib/scan.h lib/typedefs.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)scan_ulong.o: lib/scan/scan_ulong.c lib/scan.h lib/typedefs.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)scan_xmlescape.o: lib/scan/scan_xmlescape.c lib/scan.h lib/typedefs.h lib/uint32.h lib/uint64.h lib/str.h lib/uint8.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)scan_xshort.o: lib/scan/scan_xshort.c lib/scan.h lib/typedefs.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)scan_plusminus.o: lib/scan/scan_plusminus.c lib/scan.h lib/typedefs.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)scan_longlong.o: lib/scan/scan_longlong.c lib/scan.h lib/typedefs.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)scan_pb_type5_fixed32.o: lib/scan/scan_pb_type5_fixed32.c lib/scan.h lib/typedefs.h lib/uint32.h lib/uint64.h lib/uint32.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)scan_ulonglong.o: lib/scan/scan_ulonglong.c lib/scan.h lib/typedefs.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)scan_ushort.o: lib/scan/scan_ushort.c lib/scan.h lib/typedefs.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)iarray_length.o: lib/iarray/iarray_length.c lib/iarray.h lib/uint64.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)iarray_free.o: lib/iarray/iarray_free.c lib/iarray.h lib/uint64.h lib/typedefs.h lib/mmap.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)iarray_get.o: lib/iarray/iarray_get.c lib/iarray.h lib/uint64.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)iarray_init.o: lib/iarray/iarray_init.c lib/iarray.h lib/uint64.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)iarray_allocate.o: lib/iarray/iarray_allocate.c lib/windoze.h lib/byte.h lib/typedefs.h lib/iarray.h lib/uint64.h lib/cas.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)alloc.o: lib/alloc/alloc.c lib/alloc.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)alloc_re.o: lib/alloc/alloc_re.c lib/alloc.h lib/typedefs.h lib/byte.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)alloc_zero.o: lib/alloc/alloc_zero.c lib/alloc.h lib/typedefs.h lib/byte.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)slist_shifts.o: lib/slist/slist_shifts.c lib/slist.h lib/alloc.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)slist_push.o: lib/slist/slist_push.c lib/slist.h lib/alloc.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)slist_pushs.o: lib/slist/slist_pushs.c lib/slist.h lib/alloc.h lib/typedefs.h lib/str.h lib/alloc.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)slist_add_before.o: lib/slist/slist_add_before.c lib/slist.h lib/alloc.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)slist_shift.o: lib/slist/slist_shift.c lib/slist.h lib/alloc.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)slist_find.o: lib/slist/slist_find.c lib/slist.h lib/alloc.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)slist_init.o: lib/slist/slist_init.c lib/slist.h lib/alloc.h lib/typedefs.h lib/byte.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)slist_unshifts.o: lib/slist/slist_unshifts.c lib/slist.h lib/alloc.h lib/typedefs.h lib/str.h lib/alloc.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)slist_remove.o: lib/slist/slist_remove.c lib/slist.h lib/alloc.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)slist_length.o: lib/slist/slist_length.c lib/slist.h lib/alloc.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)slist_add_after.o: lib/slist/slist_add_after.c lib/slist.h lib/alloc.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)slist_find_remove.o: lib/slist/slist_find_remove.c lib/slist.h lib/alloc.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)elf_get_symtab.o: lib/elf/elf_get_symtab.c lib/elf.h lib/uint8.h lib/uint16.h lib/uint32.h lib/uint64.h lib/range.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)elf_section_typename.o: lib/elf/elf_section_typename.c lib/elf.h lib/uint8.h lib/uint16.h lib/uint32.h lib/uint64.h lib/range.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)elf_get_section.o: lib/elf/elf_get_section.c lib/elf.h lib/uint8.h lib/uint16.h lib/uint32.h lib/uint64.h lib/range.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)elf_section.o: lib/elf/elf_section.c lib/elf.h lib/uint8.h lib/uint16.h lib/uint32.h lib/uint64.h lib/range.h lib/typedefs.h lib/range.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)elf_section_headers.o: lib/elf/elf_section_headers.c lib/elf.h lib/uint8.h lib/uint16.h lib/uint32.h lib/uint64.h lib/range.h lib/typedefs.h lib/range.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)elf_get_value.o: lib/elf/elf_get_value.c lib/elf.h lib/uint8.h lib/uint16.h lib/uint32.h lib/uint64.h lib/range.h lib/typedefs.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)elf_section_size.o: lib/elf/elf_section_size.c lib/elf.h lib/uint8.h lib/uint16.h lib/uint32.h lib/uint64.h lib/range.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)elf_section_offset.o: lib/elf/elf_section_offset.c lib/elf.h lib/uint8.h lib/uint16.h lib/uint32.h lib/uint64.h lib/range.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)elf_dynamic_section.o: lib/elf/elf_dynamic_section.c lib/elf.h lib/uint8.h lib/uint16.h lib/uint32.h lib/uint64.h lib/range.h lib/typedefs.h lib/range.h lib/byte.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)elf_header.o: lib/elf/elf_header.c lib/elf.h lib/uint8.h lib/uint16.h lib/uint32.h lib/uint64.h lib/range.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)elf_section_index.o: lib/elf/elf_section_index.c lib/elf.h lib/uint8.h lib/uint16.h lib/uint32.h lib/uint64.h lib/range.h lib/typedefs.h lib/range.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)elf_section_name.o: lib/elf/elf_section_name.c lib/elf.h lib/uint8.h lib/uint16.h lib/uint32.h lib/uint64.h lib/range.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)elf_get_symtab_r.o: lib/elf/elf_get_symtab_r.c lib/elf.h lib/uint8.h lib/uint16.h lib/uint32.h lib/uint64.h lib/range.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)elf_symbol_r.o: lib/elf/elf_symbol_r.c lib/elf.h lib/uint8.h lib/uint16.h lib/uint32.h lib/uint64.h lib/range.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)elf_get_section_r.o: lib/elf/elf_get_section_r.c lib/elf.h lib/uint8.h lib/uint16.h lib/uint32.h lib/uint64.h lib/range.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)elf_program_headers.o: lib/elf/elf_program_headers.c lib/elf.h lib/uint8.h lib/uint16.h lib/uint32.h lib/uint64.h lib/range.h lib/typedefs.h lib/range.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)elf_shstrtab.o: lib/elf/elf_shstrtab.c lib/elf.h lib/uint8.h lib/uint16.h lib/uint32.h lib/uint64.h lib/range.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)elf_section_find.o: lib/elf/elf_section_find.c lib/elf.h lib/uint8.h lib/uint16.h lib/uint32.h lib/uint64.h lib/range.h lib/typedefs.h lib/range.h lib/str.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)process_create.o: lib/process/process_create.c lib/windoze.h lib/byte.h lib/typedefs.h lib/io.h lib/windoze.h lib/iarray.h lib/uint64.h lib/taia.h lib/tai.h lib/uint32.h lib/strlist.h lib/stralloc.h lib/byte.h lib/buffer.h lib/uint64.h lib/env.h lib/errmsg.h lib/wait.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cb_find_prefix.o: lib/cb/cb_find_prefix.c lib/byte.h lib/typedefs.h lib/cb_internal.h lib/byte.h lib/cb.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cb_erase.o: lib/cb/cb_erase.c lib/byte.h lib/typedefs.h lib/cb_internal.h lib/byte.h lib/cb.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cb_get_kv.o: lib/cb/cb_get_kv.c lib/cb_internal.h lib/byte.h lib/typedefs.h lib/cb.h lib/str.h lib/byte.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cb_insert.o: lib/cb/cb_insert.c lib/cb_internal.h lib/byte.h lib/typedefs.h lib/cb.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cb_find.o: lib/cb/cb_find.c lib/cb_internal.h lib/byte.h lib/typedefs.h lib/cb.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cb_clear.o: lib/cb/cb_clear.c lib/cb_internal.h lib/byte.h lib/typedefs.h lib/cb.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cb_new_kv.o: lib/cb/cb_new_kv.c lib/cb_internal.h lib/byte.h lib/typedefs.h lib/cb.h lib/byte.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cb_get_kv_ex.o: lib/cb/cb_get_kv_ex.c lib/cb_internal.h lib/byte.h lib/typedefs.h lib/cb.h lib/str.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cb_foreach.o: lib/cb/cb_foreach.c lib/byte.h lib/typedefs.h lib/cb_internal.h lib/byte.h lib/cb.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)list_find_remove.o: lib/list/list_find_remove.c lib/list.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)list_init.o: lib/list/list_init.c lib/list.h lib/byte.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)list_find.o: lib/list/list_find.c lib/list.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)list_add_after.o: lib/list/list_add_after.c lib/list.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)list_push.o: lib/list/list_push.c lib/list.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)list_move_head.o: lib/list/list_move_head.c lib/list.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)list_unshift.o: lib/list/list_unshift.c lib/list.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)list_move_tail.o: lib/list/list_move_tail.c lib/list.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)list_swap.o: lib/list/list_swap.c lib/list.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)list_remove.o: lib/list/list_remove.c lib/list.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)list_length.o: lib/list/list_length.c lib/list.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)list_add_before.o: lib/list/list_add_before.c lib/list.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)dns_ip6.o: lib/dns/dns_ip6.c lib/byte.h lib/typedefs.h lib/dns.h lib/iopause.h lib/io.h lib/windoze.h lib/iarray.h lib/uint64.h lib/taia.h lib/tai.h lib/uint32.h lib/stralloc.h lib/ip4.h lib/uint8.h lib/uint16.h lib/ip6.h lib/byte.h lib/stralloc.h lib/uint16.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)dns_rcip.o: lib/dns/dns_rcip.c lib/socket_internal.h lib/socket.h lib/typedefs.h lib/uint16.h lib/uint32.h lib/byte.h lib/dns.h lib/iopause.h lib/io.h lib/windoze.h lib/iarray.h lib/uint64.h lib/taia.h lib/tai.h lib/stralloc.h lib/ip4.h lib/uint8.h lib/ip6.h lib/byte.h lib/open.h lib/taia.h lib/stralloc.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)dns_txt.o: lib/dns/dns_txt.c lib/byte.h lib/typedefs.h lib/dns.h lib/iopause.h lib/io.h lib/windoze.h lib/iarray.h lib/uint64.h lib/taia.h lib/tai.h lib/uint32.h lib/stralloc.h lib/stralloc.h lib/uint16.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)dns_mx.o: lib/dns/dns_mx.c lib/byte.h lib/typedefs.h lib/dns.h lib/iopause.h lib/io.h lib/windoze.h lib/iarray.h lib/uint64.h lib/taia.h lib/tai.h lib/uint32.h lib/stralloc.h lib/stralloc.h lib/uint16.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)dns_transmit.o: lib/dns/dns_transmit.c lib/socket_internal.h lib/socket.h lib/typedefs.h lib/uint16.h lib/uint32.h lib/windoze.h lib/byte.h lib/dns.h lib/iopause.h lib/io.h lib/windoze.h lib/iarray.h lib/uint64.h lib/taia.h lib/tai.h lib/stralloc.h lib/ip6.h lib/byte.h lib/uint16.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)dns_dtda.o: lib/dns/dns_dtda.c lib/dns.h lib/iopause.h lib/io.h lib/windoze.h lib/iarray.h lib/uint64.h lib/typedefs.h lib/taia.h lib/tai.h lib/uint32.h lib/stralloc.h lib/stralloc.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)dns_ipq.o: lib/dns/dns_ipq.c lib/byte.h lib/typedefs.h lib/case.h lib/dns.h lib/iopause.h lib/io.h lib/windoze.h lib/iarray.h lib/uint64.h lib/taia.h lib/tai.h lib/uint32.h lib/stralloc.h lib/str.h lib/stralloc.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)dns_domain.o: lib/dns/dns_domain.c lib/byte.h lib/typedefs.h lib/case.h lib/dns.h lib/iopause.h lib/io.h lib/windoze.h lib/iarray.h lib/uint64.h lib/taia.h lib/tai.h lib/uint32.h lib/stralloc.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)dns_dfd.o: lib/dns/dns_dfd.c lib/byte.h lib/typedefs.h lib/dns.h lib/iopause.h lib/io.h lib/windoze.h lib/iarray.h lib/uint64.h lib/taia.h lib/tai.h lib/uint32.h lib/stralloc.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)dns_sortip.o: lib/dns/dns_sortip.c lib/byte.h lib/typedefs.h lib/dns.h lib/iopause.h lib/io.h lib/windoze.h lib/iarray.h lib/uint64.h lib/taia.h lib/tai.h lib/uint32.h lib/stralloc.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)dns_name.o: lib/dns/dns_name.c lib/byte.h lib/typedefs.h lib/dns.h lib/iopause.h lib/io.h lib/windoze.h lib/iarray.h lib/uint64.h lib/taia.h lib/tai.h lib/uint32.h lib/stralloc.h lib/ip6.h lib/byte.h lib/stralloc.h lib/uint16.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)dns_nd.o: lib/dns/dns_nd.c lib/byte.h lib/typedefs.h lib/dns.h lib/iopause.h lib/io.h lib/windoze.h lib/iarray.h lib/uint64.h lib/taia.h lib/tai.h lib/uint32.h lib/stralloc.h lib/fmt.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)dns_nd6.o: lib/dns/dns_nd6.c lib/byte.h lib/typedefs.h lib/dns.h lib/iopause.h lib/io.h lib/windoze.h lib/iarray.h lib/uint64.h lib/taia.h lib/tai.h lib/uint32.h lib/stralloc.h lib/fmt.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)dns_sortip6.o: lib/dns/dns_sortip6.c lib/byte.h lib/typedefs.h lib/dns.h lib/iopause.h lib/io.h lib/windoze.h lib/iarray.h lib/uint64.h lib/taia.h lib/tai.h lib/uint32.h lib/stralloc.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)dns_ip4.o: lib/dns/dns_ip4.c lib/byte.h lib/typedefs.h lib/dns.h lib/iopause.h lib/io.h lib/windoze.h lib/iarray.h lib/uint64.h lib/taia.h lib/tai.h lib/uint32.h lib/stralloc.h lib/uint16.h lib/stralloc.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)dns_resolve.o: lib/dns/dns_resolve.c lib/byte.h lib/typedefs.h lib/dns.h lib/iopause.h lib/io.h lib/windoze.h lib/iarray.h lib/uint64.h lib/taia.h lib/tai.h lib/uint32.h lib/stralloc.h lib/io.h lib/iopause.h lib/ip6.h lib/byte.h lib/taia.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)dns_ipq6.o: lib/dns/dns_ipq6.c lib/byte.h lib/typedefs.h lib/case.h lib/dns.h lib/iopause.h lib/io.h lib/windoze.h lib/iarray.h lib/uint64.h lib/taia.h lib/tai.h lib/uint32.h lib/stralloc.h lib/str.h lib/stralloc.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)dns_packet.o: lib/dns/dns_packet.c lib/dns.h lib/iopause.h lib/io.h lib/windoze.h lib/iarray.h lib/uint64.h lib/typedefs.h lib/taia.h lib/tai.h lib/uint32.h lib/stralloc.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)dns_rcrw.o: lib/dns/dns_rcrw.c lib/windoze.h lib/byte.h lib/typedefs.h lib/dns.h lib/iopause.h lib/io.h lib/windoze.h lib/iarray.h lib/uint64.h lib/taia.h lib/tai.h lib/uint32.h lib/stralloc.h lib/open.h lib/str.h lib/stralloc.h lib/taia.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)dns_random.o: lib/dns/dns_random.c lib/windoze.h lib/dns.h lib/iopause.h lib/io.h lib/windoze.h lib/iarray.h lib/uint64.h lib/typedefs.h lib/taia.h lib/tai.h lib/uint32.h lib/stralloc.h lib/taia.h lib/uint32.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)dir_path.o: lib/dir/dir_path.c lib/stralloc.h lib/typedefs.h lib/dir_internal.h lib/dir.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)dir_type.o: lib/dir/dir_type.c lib/windoze.h lib/dir_internal.h lib/dir.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)dir_time.o: lib/dir/dir_time.c lib/dir.h lib/uint64.h lib/dir_internal.h lib/dir.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)dir_open.o: lib/dir/dir_open.c lib/windoze.h lib/dir_internal.h lib/dir.h lib/uint64.h lib/str.h lib/typedefs.h lib/stralloc.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)dir_close.o: lib/dir/dir_close.c lib/windoze.h lib/dir_internal.h lib/dir.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)dir_read.o: lib/dir/dir_read.c lib/dir_internal.h lib/dir.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)dir_name.o: lib/dir/dir_name.c lib/dir_internal.h lib/dir.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)dir_size.o: lib/dir/dir_size.c lib/windoze.h lib/uint64.h lib/dir_internal.h lib/dir.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)omf_open.o: lib/omf/omf_open.c lib/omf.h lib/typedefs.h lib/uint8.h lib/uint16.h lib/range.h lib/mmap.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)omf_get_name.o: lib/omf/omf_get_name.c lib/omf.h lib/typedefs.h lib/uint8.h lib/uint16.h lib/range.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)omf_close.o: lib/omf/omf_close.c lib/omf.h lib/typedefs.h lib/uint8.h lib/uint16.h lib/range.h lib/mmap.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)omf_record_next.o: lib/omf/omf_record_next.c lib/omf.h lib/typedefs.h lib/uint8.h lib/uint16.h lib/range.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)omf_begin.o: lib/omf/omf_begin.c lib/omf.h lib/typedefs.h lib/uint8.h lib/uint16.h lib/range.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)omf_name.o: lib/omf/omf_name.c lib/omf.h lib/typedefs.h lib/uint8.h lib/uint16.h lib/range.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)omf_end.o: lib/omf/omf_end.c lib/omf.h lib/typedefs.h lib/uint8.h lib/uint16.h lib/range.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)omf_record_begin.o: lib/omf/omf_record_begin.c lib/omf.h lib/typedefs.h lib/uint8.h lib/uint16.h lib/range.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)omf_record_end.o: lib/omf/omf_record_end.c lib/omf.h lib/typedefs.h lib/uint8.h lib/uint16.h lib/range.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)omf_next.o: lib/omf/omf_next.c lib/omf.h lib/typedefs.h lib/uint8.h lib/uint16.h lib/range.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)omf_data.o: lib/omf/omf_data.c lib/omf.h lib/typedefs.h lib/uint8.h lib/uint16.h lib/range.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)sig_pause.o: lib/sig/sig_pause.c lib/sig.h lib/windoze.h lib/windoze.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)sig_dfl.o: lib/sig/sig_dfl.c lib/sig.h lib/windoze.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)sig_number.o: lib/sig/sig_number.c lib/sig.h lib/windoze.h lib/sig/sig-internal.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)sigsegv.o: lib/sig/sigsegv.c lib/sig.h lib/windoze.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)sig_restoreto.o: lib/sig/sig_restoreto.c lib/sig.h lib/windoze.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)sig_unblock.o: lib/sig/sig_unblock.c lib/windoze.h lib/sig.h lib/windoze.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)sig_table.o: lib/sig/sig_table.c lib/sig.h lib/windoze.h lib/sig/sig-internal.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)sig_push.o: lib/sig/sig_push.c lib/sig.h lib/windoze.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)sig_unshield.o: lib/sig/sig_unshield.c lib/sig.h lib/windoze.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)sig_catch.o: lib/sig/sig_catch.c lib/sig.h lib/windoze.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)sig_blocknone.o: lib/sig/sig_blocknone.c lib/sig.h lib/windoze.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)sig_name.o: lib/sig/sig_name.c lib/sig.h lib/windoze.h lib/sig/sig-internal.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)sig_shield.o: lib/sig/sig_shield.c lib/sig.h lib/windoze.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)sig_action.o: lib/sig/sig_action.c lib/sig.h lib/windoze.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)sig_block.o: lib/sig/sig_block.c lib/windoze.h lib/sig.h lib/windoze.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)sigfpe.o: lib/sig/sigfpe.c lib/sig.h lib/windoze.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)sig_blockset.o: lib/sig/sig_blockset.c lib/sig.h lib/windoze.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)sig_stack.o: lib/sig/sig_stack.c lib/sig.h lib/windoze.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)range_init.o: lib/range/range_init.c lib/range.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)linked_list_free.o: lib/hashmap/linked_list_free.c lib/linked_list.h lib/typedefs.h lib/alloc.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)hashmap_init.o: lib/hashmap/hashmap_init.c lib/byte.h lib/typedefs.h lib/linked_list.h lib/alloc.h lib/hashmap.h lib/linked_list.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)hashmap_free.o: lib/hashmap/hashmap_free.c lib/linked_list.h lib/typedefs.h lib/alloc.h lib/hashmap.h lib/linked_list.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)hashmap_remove.o: lib/hashmap/hashmap_remove.c lib/linked_list.h lib/typedefs.h lib/alloc.h lib/hashmap.h lib/linked_list.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)hashmap_default_comparator.o: lib/hashmap/hashmap_default_comparator.c lib/linked_list.h lib/typedefs.h lib/alloc.h lib/hashmap.h lib/linked_list.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)hashmap_size.o: lib/hashmap/hashmap_size.c lib/linked_list.h lib/typedefs.h lib/alloc.h lib/hashmap.h lib/linked_list.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)linked_list_append.o: lib/hashmap/linked_list_append.c lib/linked_list.h lib/typedefs.h lib/alloc.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)hashmap_put.o: lib/hashmap/hashmap_put.c lib/linked_list.h lib/typedefs.h lib/alloc.h lib/hashmap.h lib/linked_list.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)linked_list_size.o: lib/hashmap/linked_list_size.c lib/linked_list.h lib/typedefs.h lib/alloc.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)hashmap_contains_key.o: lib/hashmap/hashmap_contains_key.c lib/linked_list.h lib/typedefs.h lib/alloc.h lib/hashmap.h lib/linked_list.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)linked_list_prepend.o: lib/hashmap/linked_list_prepend.c lib/linked_list.h lib/typedefs.h lib/alloc.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)linked_list_init.o: lib/hashmap/linked_list_init.c lib/linked_list.h lib/typedefs.h lib/alloc.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)hashmap_default_hash_func.o: lib/hashmap/hashmap_default_hash_func.c lib/linked_list.h lib/typedefs.h lib/alloc.h lib/hashmap.h lib/linked_list.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)linked_list_head.o: lib/hashmap/linked_list_head.c lib/linked_list.h lib/typedefs.h lib/alloc.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)hashmap_get.o: lib/hashmap/hashmap_get.c lib/linked_list.h lib/typedefs.h lib/alloc.h lib/hashmap.h lib/linked_list.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)hashmap_clear.o: lib/hashmap/hashmap_clear.c lib/linked_list.h lib/typedefs.h lib/alloc.h lib/hashmap.h lib/linked_list.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)hashmap_keys.o: lib/hashmap/hashmap_keys.c lib/linked_list.h lib/typedefs.h lib/alloc.h lib/hashmap.h lib/linked_list.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)uint16_pack_big.o: lib/uint16/uint16_pack_big.c lib/uint16.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)uint16_pack.o: lib/uint16/uint16_pack.c lib/uint16.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)uint16_unpack_big.o: lib/uint16/uint16_unpack_big.c lib/uint16.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)uint16_unpack.o: lib/uint16/uint16_unpack.c lib/uint16.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)uint16_read_big.o: lib/uint16/uint16_read_big.c lib/uint16.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)uint16_read.o: lib/uint16/uint16_read.c lib/uint16.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)errmsg_infosys.o: lib/errmsg/errmsg_infosys.c lib/errmsg.h lib/errmsg_internal.h lib/uint64.h lib/windoze.h lib/errmsg.h lib/str.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)errmsg_warnsys.o: lib/errmsg/errmsg_warnsys.c lib/errmsg.h lib/errmsg_internal.h lib/uint64.h lib/windoze.h lib/errmsg.h lib/str.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)errmsg_write.o: lib/errmsg/errmsg_write.c lib/errmsg.h lib/errmsg_internal.h lib/uint64.h lib/windoze.h lib/errmsg.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)errmsg_puts.o: lib/errmsg/errmsg_puts.c lib/errmsg_internal.h lib/uint64.h lib/windoze.h lib/errmsg.h lib/str.h lib/typedefs.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)errmsg_warn.o: lib/errmsg/errmsg_warn.c lib/errmsg.h lib/errmsg_internal.h lib/uint64.h lib/windoze.h lib/errmsg.h lib/str.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)errmsg_iam.o: lib/errmsg/errmsg_iam.c lib/errmsg.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)errmsg_info.o: lib/errmsg/errmsg_info.c lib/errmsg.h lib/errmsg_internal.h lib/uint64.h lib/windoze.h lib/errmsg.h lib/str.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)fmt_longlong.o: lib/fmt/fmt_longlong.c lib/fmt.h lib/typedefs.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)fmt_escapecharshell.o: lib/fmt/fmt_escapecharshell.c lib/fmt.h lib/typedefs.h lib/uint32.h lib/uint64.h lib/uint32.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)fmt_iso8601.o: lib/fmt/fmt_iso8601.c lib/fmt.h lib/typedefs.h lib/uint32.h lib/uint64.h lib/byte.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)fmt_escapecharhtml.o: lib/fmt/fmt_escapecharhtml.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)fmt_strm_internal.o: lib/fmt/fmt_strm_internal.c lib/fmt.h lib/typedefs.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)fmt_hexb.o: lib/fmt/fmt_hexb.c lib/fmt.h lib/typedefs.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)fmt_escapecharc.o: lib/fmt/fmt_escapecharc.c lib/fmt.h lib/typedefs.h lib/uint32.h lib/uint64.h lib/uint32.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)fmt_fill.o: lib/fmt/fmt_fill.c lib/fmt.h lib/typedefs.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)fmt_asn1derlength.o: lib/fmt/fmt_asn1derlength.c lib/fmt.h lib/typedefs.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)fmt_ulong.o: lib/fmt/fmt_ulong.c lib/fmt.h lib/typedefs.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)fmt_asn1dertag.o: lib/fmt/fmt_asn1dertag.c lib/fmt.h lib/typedefs.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)fmt_octal.o: lib/fmt/fmt_octal.c lib/fmt.h lib/typedefs.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)fmt_ulong0.o: lib/fmt/fmt_ulong0.c lib/fmt.h lib/typedefs.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)fmt_tohex.o: lib/fmt/fmt_tohex.c lib/fmt.h lib/typedefs.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)fmt_humank.o: lib/fmt/fmt_humank.c lib/fmt.h lib/typedefs.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)fmt_escapecharquotedprintableutf8.o: lib/fmt/fmt_escapecharquotedprintableutf8.c lib/fmt.h lib/typedefs.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)fmt_strn.o: lib/fmt/fmt_strn.c lib/fmt.h lib/typedefs.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)fmt_tai.o: lib/fmt/fmt_tai.c lib/fmt.h lib/typedefs.h lib/uint32.h lib/uint64.h lib/tai.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)fmt_escapecharjson.o: lib/fmt/fmt_escapecharjson.c lib/fmt.h lib/typedefs.h lib/uint32.h lib/uint64.h lib/uint16.h lib/uint32.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)fmt_utf8.o: lib/fmt/fmt_utf8.c lib/fmt.h lib/typedefs.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)fmt_human.o: lib/fmt/fmt_human.c lib/fmt.h lib/typedefs.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)fmt_httpdate.o: lib/fmt/fmt_httpdate.c lib/byte.h lib/typedefs.h lib/fmt.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)fmt_repeat.o: lib/fmt/fmt_repeat.c lib/fmt.h lib/typedefs.h lib/uint32.h lib/uint64.h lib/byte.h lib/str.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)fmt_escapecharxml.o: lib/fmt/fmt_escapecharxml.c lib/byte.h lib/typedefs.h lib/fmt.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)fmt_8long.o: lib/fmt/fmt_8long.c lib/fmt.h lib/typedefs.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)fmt_xlonglong.o: lib/fmt/fmt_xlonglong.c lib/fmt.h lib/typedefs.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)fmt_ulonglong.o: lib/fmt/fmt_ulonglong.c lib/fmt.h lib/typedefs.h lib/uint32.h lib/uint64.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)fmt_long.o: lib/fmt/fmt_long.c lib/fmt.h lib/typedefs.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)fmt_xlong.o: lib/fmt/fmt_xlong.c lib/fmt.h lib/typedefs.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)fmt_pad.o: lib/fmt/fmt_pad.c lib/fmt.h lib/typedefs.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)fmt_minus.o: lib/fmt/fmt_minus.c lib/fmt.h lib/typedefs.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)fmt_str.o: lib/fmt/fmt_str.c lib/fmt.h lib/typedefs.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)fmt_plusminus.o: lib/fmt/fmt_plusminus.c lib/fmt.h lib/typedefs.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)fmt_escapecharquotedprintable.o: lib/fmt/fmt_escapecharquotedprintable.c lib/fmt.h lib/typedefs.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)fmt_double.o: lib/fmt/fmt_double.c lib/fmt.h lib/typedefs.h lib/uint32.h lib/uint64.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)fmt_xmlescape.o: lib/fmt/fmt_xmlescape.c lib/fmt.h lib/typedefs.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ucs_latin1.o: lib/ucs/ucs_latin1.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)str_utf8_latin.o: lib/ucs/str_utf8_latin.c lib/ucs.h lib/typedefs.h lib/str.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)fmt_latin1_utf8.o: lib/ucs/fmt_latin1_utf8.c lib/ucs.h lib/typedefs.h lib/fmt.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)stralloc_utf8_latin1.o: lib/ucs/stralloc_utf8_latin1.c lib/stralloc.h lib/typedefs.h lib/ucs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)scan_latin1_utf8.o: lib/ucs/scan_latin1_utf8.c lib/ucs.h lib/typedefs.h lib/scan.h lib/uint32.h lib/uint64.h lib/buffer.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)stralloc_utf8_to_latin1.o: lib/ucs/stralloc_utf8_to_latin1.c lib/stralloc.h lib/typedefs.h lib/ucs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)stralloc_latin1_to_utf8.o: lib/ucs/stralloc_latin1_to_utf8.c lib/stralloc.h lib/typedefs.h lib/ucs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)setenv.o: lib/setenv.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)taia_approx.o: lib/taia/taia_approx.c lib/taia.h lib/tai.h lib/uint64.h lib/uint32.h lib/tai.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)taia_frac.o: lib/taia/taia_frac.c lib/taia.h lib/tai.h lib/uint64.h lib/uint32.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)taia_pack.o: lib/taia/taia_pack.c lib/taia.h lib/tai.h lib/uint64.h lib/uint32.h lib/tai.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)taia_tai.o: lib/taia/taia_tai.c lib/taia.h lib/tai.h lib/uint64.h lib/uint32.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)taia_less.o: lib/taia/taia_less.c lib/taia.h lib/tai.h lib/uint64.h lib/uint32.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)taia_half.o: lib/taia/taia_half.c lib/taia.h lib/tai.h lib/uint64.h lib/uint32.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)taia_now.o: lib/taia/taia_now.c lib/windoze.h lib/taia.h lib/tai.h lib/uint64.h lib/uint32.h lib/tai.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)taia_unpack.o: lib/taia/taia_unpack.c lib/taia.h lib/tai.h lib/uint64.h lib/uint32.h lib/tai.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)taia_addsec.o: lib/taia/taia_addsec.c lib/taia.h lib/tai.h lib/uint64.h lib/uint32.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)taia_sub.o: lib/taia/taia_sub.c lib/taia.h lib/tai.h lib/uint64.h lib/uint32.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)taia_uint.o: lib/taia/taia_uint.c lib/taia.h lib/tai.h lib/uint64.h lib/uint32.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)taia_add.o: lib/taia/taia_add.c lib/taia.h lib/tai.h lib/uint64.h lib/uint32.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cbmap_get.o: lib/cbmap/cbmap_get.c lib/byte.h lib/typedefs.h lib/cbmap_internal.h lib/cbmap.h lib/uint8.h lib/cbmap/cbmap_alloc.h lib/str.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cbmap_data_node_destroy.o: lib/cbmap/cbmap_data_node_destroy.c lib/cbmap_internal.h lib/cbmap.h lib/uint8.h lib/cbmap/cbmap_alloc.h lib/str.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cbmap_delete.o: lib/cbmap/cbmap_delete.c lib/byte.h lib/typedefs.h lib/cbmap_internal.h lib/cbmap.h lib/uint8.h lib/cbmap/cbmap_alloc.h lib/str.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cbmap_visit.o: lib/cbmap/cbmap_visit.c lib/cbmap_internal.h lib/cbmap.h lib/uint8.h lib/cbmap/cbmap_alloc.h lib/str.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cbmap_insert.o: lib/cbmap/cbmap_insert.c lib/byte.h lib/typedefs.h lib/cbmap/cbmap_alloc.h lib/str.h lib/cbmap_internal.h lib/cbmap.h lib/uint8.h lib/cbmap/cbmap_alloc.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cbmap_internal_node.o: lib/cbmap/cbmap_internal_node.c lib/cbmap/cbmap_alloc.h lib/str.h lib/typedefs.h lib/cbmap_internal.h lib/cbmap.h lib/uint8.h lib/cbmap/cbmap_alloc.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cbmap_alloc.o: lib/cbmap/cbmap_alloc.c lib/windoze.h lib/cbmap/cbmap_alloc.h lib/str.h lib/typedefs.h lib/memalign.h lib/byte.h lib/cbmap_internal.h lib/cbmap.h lib/uint8.h lib/cbmap/cbmap_alloc.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cbmap_visit_all.o: lib/cbmap/cbmap_visit_all.c lib/cbmap_internal.h lib/cbmap.h lib/uint8.h lib/cbmap/cbmap_alloc.h lib/str.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)memalign.o: lib/cbmap/memalign.c lib/memalign.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cbmap_destroy.o: lib/cbmap/cbmap_destroy.c lib/cbmap_internal.h lib/cbmap.h lib/uint8.h lib/cbmap/cbmap_alloc.h lib/str.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cbmap_count.o: lib/cbmap/cbmap_count.c lib/cbmap_internal.h lib/cbmap.h lib/uint8.h lib/cbmap/cbmap_alloc.h lib/str.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cbmap_new.o: lib/cbmap/cbmap_new.c lib/byte.h lib/typedefs.h lib/cbmap_internal.h lib/cbmap.h lib/uint8.h lib/cbmap/cbmap_alloc.h lib/str.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)xml_child_element_text.o: lib/xml/xml_child_element_text.c lib/xml.h lib/buffer.h lib/typedefs.h lib/hmap.h lib/stralloc.h lib/textbuf.h lib/uint64.h lib/str.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)xml_get_document.o: lib/xml/xml_get_document.c lib/xml.h lib/buffer.h lib/typedefs.h lib/hmap.h lib/stralloc.h lib/textbuf.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)xml_find_predicates.o: lib/xml/xml_find_predicates.c lib/str.h lib/typedefs.h lib/strlist.h lib/stralloc.h lib/byte.h lib/uint64.h lib/buffer.h lib/xml.h lib/hmap.h lib/textbuf.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)xml_get_attribute_sa.o: lib/xml/xml_get_attribute_sa.c lib/xml.h lib/buffer.h lib/typedefs.h lib/hmap.h lib/stralloc.h lib/textbuf.h lib/uint64.h lib/stralloc.h lib/hmap.h lib/str.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)xml_textnode.o: lib/xml/xml_textnode.c lib/xml.h lib/buffer.h lib/typedefs.h lib/hmap.h lib/stralloc.h lib/textbuf.h lib/uint64.h lib/str.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)xml_element.o: lib/xml/xml_element.c lib/xml.h lib/buffer.h lib/typedefs.h lib/hmap.h lib/stralloc.h lib/textbuf.h lib/uint64.h lib/str.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)xml_attributes.o: lib/xml/xml_attributes.c lib/xml.h lib/buffer.h lib/typedefs.h lib/hmap.h lib/stralloc.h lib/textbuf.h lib/uint64.h lib/str.h lib/buffer.h lib/hmap.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)xml_element_attrs.o: lib/xml/xml_element_attrs.c lib/xml.h lib/buffer.h lib/typedefs.h lib/hmap.h lib/stralloc.h lib/textbuf.h lib/uint64.h lib/str.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)xml_child_element_attrs.o: lib/xml/xml_child_element_attrs.c lib/xml.h lib/buffer.h lib/typedefs.h lib/hmap.h lib/stralloc.h lib/textbuf.h lib/uint64.h lib/str.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)xml_read_callback.o: lib/xml/xml_read_callback.c lib/byte.h lib/typedefs.h lib/stralloc.h lib/xml.h lib/buffer.h lib/hmap.h lib/stralloc.h lib/textbuf.h lib/uint64.h lib/str.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)xml_path.o: lib/xml/xml_path.c lib/xml.h lib/buffer.h lib/typedefs.h lib/hmap.h lib/stralloc.h lib/textbuf.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)xml_set_attribute_double.o: lib/xml/xml_set_attribute_double.c lib/xml.h lib/buffer.h lib/typedefs.h lib/hmap.h lib/stralloc.h lib/textbuf.h lib/uint64.h lib/str.h lib/fmt.h lib/uint32.h lib/byte.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)xml_attrnode.o: lib/xml/xml_attrnode.c lib/xml.h lib/buffer.h lib/typedefs.h lib/hmap.h lib/stralloc.h lib/textbuf.h lib/uint64.h lib/str.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)xml_read_tree.o: lib/xml/xml_read_tree.c lib/byte.h lib/typedefs.h lib/fmt.h lib/uint32.h lib/uint64.h lib/buffer.h lib/scan.h lib/str.h lib/xml.h lib/buffer.h lib/hmap.h lib/stralloc.h lib/textbuf.h lib/alloc.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)xml_find_parent.o: lib/xml/xml_find_parent.c lib/buffer.h lib/typedefs.h lib/strlist.h lib/stralloc.h lib/byte.h lib/uint64.h lib/buffer.h lib/xml.h lib/hmap.h lib/textbuf.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)xml_delete.o: lib/xml/xml_delete.c lib/xml.h lib/buffer.h lib/typedefs.h lib/hmap.h lib/stralloc.h lib/textbuf.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)xml_get_attribute_double.o: lib/xml/xml_get_attribute_double.c lib/xml.h lib/buffer.h lib/typedefs.h lib/hmap.h lib/stralloc.h lib/textbuf.h lib/uint64.h lib/scan.h lib/uint32.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)xml_find_element_attr.o: lib/xml/xml_find_element_attr.c lib/strlist.h lib/stralloc.h lib/typedefs.h lib/byte.h lib/uint64.h lib/buffer.h lib/xml.h lib/hmap.h lib/textbuf.h lib/str.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)xml_print_attributes.o: lib/xml/xml_print_attributes.c lib/uint64.h lib/buffer.h lib/typedefs.h lib/hmap.h lib/stralloc.h lib/buffer.h lib/xml.h lib/hmap.h lib/textbuf.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)xml_escape.o: lib/xml/xml_escape.c lib/byte.h lib/typedefs.h lib/fmt.h lib/uint32.h lib/uint64.h lib/stralloc.h lib/xml.h lib/buffer.h lib/hmap.h lib/stralloc.h lib/textbuf.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)xml_set_attribute.o: lib/xml/xml_set_attribute.c lib/xml.h lib/buffer.h lib/typedefs.h lib/hmap.h lib/stralloc.h lib/textbuf.h lib/uint64.h lib/hmap.h lib/str.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)xml_newnode.o: lib/xml/xml_newnode.c lib/alloc.h lib/typedefs.h lib/xml.h lib/buffer.h lib/hmap.h lib/stralloc.h lib/textbuf.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)xml_child_element.o: lib/xml/xml_child_element.c lib/xml.h lib/buffer.h lib/typedefs.h lib/hmap.h lib/stralloc.h lib/textbuf.h lib/uint64.h lib/str.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)xml_debug.o: lib/xml/xml_debug.c lib/fmt.h lib/typedefs.h lib/uint32.h lib/uint64.h lib/xml.h lib/buffer.h lib/hmap.h lib/stralloc.h lib/textbuf.h lib/str.h lib/stralloc.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)xml_print.o: lib/xml/xml_print.c lib/xml.h lib/buffer.h lib/typedefs.h lib/hmap.h lib/stralloc.h lib/textbuf.h lib/uint64.h lib/str.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)xml_find_element.o: lib/xml/xml_find_element.c lib/buffer.h lib/typedefs.h lib/strlist.h lib/stralloc.h lib/byte.h lib/uint64.h lib/buffer.h lib/xml.h lib/hmap.h lib/textbuf.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)xml_add_child.o: lib/xml/xml_add_child.c lib/xml.h lib/buffer.h lib/typedefs.h lib/hmap.h lib/stralloc.h lib/textbuf.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)xml_remove.o: lib/xml/xml_remove.c lib/xml.h lib/buffer.h lib/typedefs.h lib/hmap.h lib/stralloc.h lib/textbuf.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)xml_reader_init.o: lib/xml/xml_reader_init.c lib/byte.h lib/typedefs.h lib/xml.h lib/buffer.h lib/hmap.h lib/stralloc.h lib/textbuf.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)xml_get_text.o: lib/xml/xml_get_text.c lib/xml.h lib/buffer.h lib/typedefs.h lib/hmap.h lib/stralloc.h lib/textbuf.h lib/uint64.h lib/str.h lib/scan.h lib/uint32.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)xml_walk.o: lib/xml/xml_walk.c lib/xml.h lib/buffer.h lib/typedefs.h lib/hmap.h lib/stralloc.h lib/textbuf.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)xml_find_pred.o: lib/xml/xml_find_pred.c lib/xml.h lib/buffer.h lib/typedefs.h lib/hmap.h lib/stralloc.h lib/textbuf.h lib/uint64.h lib/strlist.h lib/byte.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)xml_set_attribute_long.o: lib/xml/xml_set_attribute_long.c lib/xml.h lib/buffer.h lib/typedefs.h lib/hmap.h lib/stralloc.h lib/textbuf.h lib/uint64.h lib/scan.h lib/uint32.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)xml_print_nodeset.o: lib/xml/xml_print_nodeset.c lib/xml.h lib/buffer.h lib/typedefs.h lib/hmap.h lib/stralloc.h lib/textbuf.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)xml_find_all_pred.o: lib/xml/xml_find_all_pred.c lib/array.h lib/typedefs.h lib/uint64.h lib/byte.h lib/strlist.h lib/stralloc.h lib/byte.h lib/buffer.h lib/xml.h lib/hmap.h lib/textbuf.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)xml_textnodeb.o: lib/xml/xml_textnodeb.c lib/xml.h lib/buffer.h lib/typedefs.h lib/hmap.h lib/stralloc.h lib/textbuf.h lib/uint64.h lib/str.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)xml_has_attribute.o: lib/xml/xml_has_attribute.c lib/xml.h lib/buffer.h lib/typedefs.h lib/hmap.h lib/stralloc.h lib/textbuf.h lib/uint64.h lib/str.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)xml_free.o: lib/xml/xml_free.c lib/xml.h lib/buffer.h lib/typedefs.h lib/hmap.h lib/stralloc.h lib/textbuf.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)xml_content_sa.o: lib/xml/xml_content_sa.c lib/xml.h lib/buffer.h lib/typedefs.h lib/hmap.h lib/stralloc.h lib/textbuf.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)xml_set_attributes.o: lib/xml/xml_set_attributes.c lib/xml.h lib/buffer.h lib/typedefs.h lib/hmap.h lib/stralloc.h lib/textbuf.h lib/uint64.h lib/str.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)xml_insert.o: lib/xml/xml_insert.c lib/xml.h lib/buffer.h lib/typedefs.h lib/hmap.h lib/stralloc.h lib/textbuf.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)xml_root_element.o: lib/xml/xml_root_element.c lib/xml.h lib/buffer.h lib/typedefs.h lib/hmap.h lib/stralloc.h lib/textbuf.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)xml_get_attribute.o: lib/xml/xml_get_attribute.c lib/xml.h lib/buffer.h lib/typedefs.h lib/hmap.h lib/stralloc.h lib/textbuf.h lib/uint64.h lib/str.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)xml_content.o: lib/xml/xml_content.c lib/xml.h lib/buffer.h lib/typedefs.h lib/hmap.h lib/stralloc.h lib/textbuf.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)strlist_shift.o: lib/strlist/strlist_shift.c lib/byte.h lib/typedefs.h lib/str.h lib/strlist.h lib/stralloc.h lib/byte.h lib/uint64.h lib/buffer.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)strlist_at_n.o: lib/strlist/strlist_at_n.c lib/strlist.h lib/stralloc.h lib/typedefs.h lib/byte.h lib/uint64.h lib/buffer.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)strlist_to_argv.o: lib/strlist/strlist_to_argv.c lib/alloc.h lib/typedefs.h lib/strlist.h lib/stralloc.h lib/byte.h lib/uint64.h lib/buffer.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)strlist_indexofb.o: lib/strlist/strlist_indexofb.c lib/byte.h lib/typedefs.h lib/strlist.h lib/stralloc.h lib/byte.h lib/uint64.h lib/buffer.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)strlist_fromv.o: lib/strlist/strlist_fromv.c lib/str.h lib/typedefs.h lib/strlist.h lib/stralloc.h lib/byte.h lib/uint64.h lib/buffer.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)strlist_fromb.o: lib/strlist/strlist_fromb.c lib/byte.h lib/typedefs.h lib/strlist.h lib/stralloc.h lib/byte.h lib/uint64.h lib/buffer.h lib/str.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)strlist_push_unique_sa.o: lib/strlist/strlist_push_unique_sa.c lib/strlist.h lib/stralloc.h lib/typedefs.h lib/byte.h lib/uint64.h lib/buffer.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)strlist_push.o: lib/strlist/strlist_push.c lib/byte.h lib/typedefs.h lib/str.h lib/strlist.h lib/stralloc.h lib/byte.h lib/uint64.h lib/buffer.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)strlist_prepend_sa.o: lib/strlist/strlist_prepend_sa.c lib/byte.h lib/typedefs.h lib/strlist.h lib/stralloc.h lib/byte.h lib/uint64.h lib/buffer.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)strlist_sub.o: lib/strlist/strlist_sub.c lib/strlist.h lib/stralloc.h lib/typedefs.h lib/byte.h lib/uint64.h lib/buffer.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)strlist_sort.o: lib/strlist/strlist_sort.c lib/str.h lib/typedefs.h lib/strlist.h lib/stralloc.h lib/byte.h lib/uint64.h lib/buffer.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)strlist_at.o: lib/strlist/strlist_at.c lib/strlist.h lib/stralloc.h lib/typedefs.h lib/byte.h lib/uint64.h lib/buffer.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)strlist_pushb.o: lib/strlist/strlist_pushb.c lib/byte.h lib/typedefs.h lib/strlist.h lib/stralloc.h lib/byte.h lib/uint64.h lib/buffer.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)strlist_pushm_internal.o: lib/strlist/strlist_pushm_internal.c lib/strlist.h lib/stralloc.h lib/typedefs.h lib/byte.h lib/uint64.h lib/buffer.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)strlist_index_of.o: lib/strlist/strlist_index_of.c lib/byte.h lib/typedefs.h lib/str.h lib/strlist.h lib/stralloc.h lib/byte.h lib/uint64.h lib/buffer.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)strlist_join.o: lib/strlist/strlist_join.c lib/byte.h lib/typedefs.h lib/stralloc.h lib/strlist.h lib/stralloc.h lib/byte.h lib/uint64.h lib/buffer.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)strlist_at_sa.o: lib/strlist/strlist_at_sa.c lib/byte.h lib/typedefs.h lib/strlist.h lib/stralloc.h lib/byte.h lib/uint64.h lib/buffer.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)strlist_dump.o: lib/strlist/strlist_dump.c lib/buffer.h lib/typedefs.h lib/fmt.h lib/uint32.h lib/uint64.h lib/strlist.h lib/stralloc.h lib/byte.h lib/buffer.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)strlist_push_tokens.o: lib/strlist/strlist_push_tokens.c lib/str.h lib/typedefs.h lib/strlist.h lib/stralloc.h lib/byte.h lib/uint64.h lib/buffer.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)strlist_pop.o: lib/strlist/strlist_pop.c lib/strlist.h lib/stralloc.h lib/typedefs.h lib/byte.h lib/uint64.h lib/buffer.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)strlist_copyat.o: lib/strlist/strlist_copyat.c lib/strlist.h lib/stralloc.h lib/typedefs.h lib/byte.h lib/uint64.h lib/buffer.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)strlist_count.o: lib/strlist/strlist_count.c lib/strlist.h lib/stralloc.h lib/typedefs.h lib/byte.h lib/uint64.h lib/buffer.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)strlist_range.o: lib/strlist/strlist_range.c lib/stralloc.h lib/typedefs.h lib/strlist.h lib/stralloc.h lib/byte.h lib/uint64.h lib/buffer.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)strlist_cat.o: lib/strlist/strlist_cat.c lib/strlist.h lib/stralloc.h lib/typedefs.h lib/byte.h lib/uint64.h lib/buffer.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)strlist_containsb.o: lib/strlist/strlist_containsb.c lib/byte.h lib/typedefs.h lib/strlist.h lib/stralloc.h lib/byte.h lib/uint64.h lib/buffer.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)strlist_removeb.o: lib/strlist/strlist_removeb.c lib/strlist.h lib/stralloc.h lib/typedefs.h lib/byte.h lib/uint64.h lib/buffer.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)strlist_cat_unique.o: lib/strlist/strlist_cat_unique.c lib/strlist.h lib/stralloc.h lib/typedefs.h lib/byte.h lib/uint64.h lib/buffer.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)strlist_shift_n.o: lib/strlist/strlist_shift_n.c lib/strlist.h lib/stralloc.h lib/typedefs.h lib/byte.h lib/uint64.h lib/buffer.h lib/byte.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)strlist_remove_at.o: lib/strlist/strlist_remove_at.c lib/strlist.h lib/stralloc.h lib/typedefs.h lib/byte.h lib/uint64.h lib/buffer.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)strlist_push_unique.o: lib/strlist/strlist_push_unique.c lib/strlist.h lib/stralloc.h lib/typedefs.h lib/byte.h lib/uint64.h lib/buffer.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)strlist_unshift.o: lib/strlist/strlist_unshift.c lib/str.h lib/typedefs.h lib/strlist.h lib/stralloc.h lib/byte.h lib/uint64.h lib/buffer.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)strlist_append_sa.o: lib/strlist/strlist_append_sa.c lib/byte.h lib/typedefs.h lib/str.h lib/strlist.h lib/stralloc.h lib/byte.h lib/uint64.h lib/buffer.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)strlist_prependb.o: lib/strlist/strlist_prependb.c lib/byte.h lib/typedefs.h lib/strlist.h lib/stralloc.h lib/byte.h lib/uint64.h lib/buffer.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)strlist_pushb_unique.o: lib/strlist/strlist_pushb_unique.c lib/strlist.h lib/stralloc.h lib/typedefs.h lib/byte.h lib/uint64.h lib/buffer.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)strlist_joins.o: lib/strlist/strlist_joins.c lib/byte.h lib/typedefs.h lib/strlist.h lib/stralloc.h lib/byte.h lib/uint64.h lib/buffer.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)strlist_froms.o: lib/strlist/strlist_froms.c lib/str.h lib/typedefs.h lib/strlist.h lib/stralloc.h lib/byte.h lib/uint64.h lib/buffer.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)strlist_trunc.o: lib/strlist/strlist_trunc.c lib/strlist.h lib/stralloc.h lib/typedefs.h lib/byte.h lib/uint64.h lib/buffer.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)strlist_prepends.o: lib/strlist/strlist_prepends.c lib/str.h lib/typedefs.h lib/strlist.h lib/stralloc.h lib/byte.h lib/uint64.h lib/buffer.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)strlist_contains_sa.o: lib/strlist/strlist_contains_sa.c lib/byte.h lib/typedefs.h lib/stralloc.h lib/strlist.h lib/stralloc.h lib/byte.h lib/uint64.h lib/buffer.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)strlist_push_sa.o: lib/strlist/strlist_push_sa.c lib/byte.h lib/typedefs.h lib/str.h lib/strlist.h lib/stralloc.h lib/byte.h lib/uint64.h lib/buffer.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)strlist_removes.o: lib/strlist/strlist_removes.c lib/strlist.h lib/stralloc.h lib/typedefs.h lib/byte.h lib/uint64.h lib/buffer.h lib/str.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)strlist_contains.o: lib/strlist/strlist_contains.c lib/byte.h lib/typedefs.h lib/str.h lib/strlist.h lib/stralloc.h lib/byte.h lib/uint64.h lib/buffer.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)fmt_tofrom_array.o: lib/textcode/fmt_tofrom_array.c lib/array.h lib/typedefs.h lib/uint64.h lib/textcode.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)scan_uuencoded.o: lib/textcode/scan_uuencoded.c lib/textcode.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)fmt_xml.o: lib/textcode/fmt_xml.c lib/fmt.h lib/typedefs.h lib/uint32.h lib/uint64.h lib/textcode.h lib/str.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)fmt_yenc.o: lib/textcode/fmt_yenc.c lib/fmt.h lib/typedefs.h lib/uint32.h lib/uint64.h lib/textcode.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)scan_to_array.o: lib/textcode/scan_to_array.c lib/str.h lib/typedefs.h lib/array.h lib/uint64.h lib/textcode.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)fmt_to_array.o: lib/textcode/fmt_to_array.c lib/array.h lib/typedefs.h lib/uint64.h lib/textcode.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)scan_base64.o: lib/textcode/scan_base64.c lib/textcode.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)fmt_cescape.o: lib/textcode/fmt_cescape.c lib/fmt.h lib/typedefs.h lib/uint32.h lib/uint64.h lib/textcode.h lib/str.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)fmt_quotedprintable.o: lib/textcode/fmt_quotedprintable.c lib/fmt.h lib/typedefs.h lib/uint32.h lib/uint64.h lib/textcode.h lib/str.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)scan_html.o: lib/textcode/scan_html.c lib/typedefs.h lib/entities.h lib/uint32.h lib/uint8.h lib/fmt.h lib/typedefs.h lib/uint64.h lib/textcode.h lib/scan.h lib/case.h lib/str.h lib/byte.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)fmt_ldapescape2.o: lib/textcode/fmt_ldapescape2.c lib/fmt.h lib/typedefs.h lib/uint32.h lib/uint64.h lib/textcode.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)fmt_jsonescape.o: lib/textcode/fmt_jsonescape.c lib/byte.h lib/typedefs.h lib/str.h lib/fmt.h lib/uint32.h lib/uint64.h lib/textcode.h lib/scan.h lib/uint32.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)scan_quotedprintable.o: lib/textcode/scan_quotedprintable.c lib/fmt.h lib/typedefs.h lib/uint32.h lib/uint64.h lib/textcode.h lib/scan.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)fmt_stripwhitespace.o: lib/textcode/fmt_stripwhitespace.c lib/fmt.h lib/typedefs.h lib/uint32.h lib/uint64.h lib/scan.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)fmt_base64.o: lib/textcode/fmt_base64.c lib/fmt.h lib/typedefs.h lib/uint32.h lib/uint64.h lib/textcode.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)scan_yenc.o: lib/textcode/scan_yenc.c lib/fmt.h lib/typedefs.h lib/uint32.h lib/uint64.h lib/textcode.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)scan_jsonescape.o: lib/textcode/scan_jsonescape.c lib/fmt.h lib/typedefs.h lib/uint32.h lib/uint64.h lib/textcode.h lib/scan.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)scan_tofrom_array.o: lib/textcode/scan_tofrom_array.c lib/str.h lib/typedefs.h lib/array.h lib/uint64.h lib/textcode.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)fmt_foldwhitespace.o: lib/textcode/fmt_foldwhitespace.c lib/fmt.h lib/typedefs.h lib/uint32.h lib/uint64.h lib/textcode.h lib/str.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)fmt_ldapescape.o: lib/textcode/fmt_ldapescape.c lib/fmt.h lib/typedefs.h lib/uint32.h lib/uint64.h lib/textcode.h lib/str.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)fmt_urlencoded.o: lib/textcode/fmt_urlencoded.c lib/fmt.h lib/typedefs.h lib/uint32.h lib/uint64.h lib/textcode.h lib/str.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)scan_urlencoded.o: lib/textcode/scan_urlencoded.c lib/fmt.h lib/typedefs.h lib/uint32.h lib/uint64.h lib/textcode.h lib/scan.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)fmt_to_sa.o: lib/textcode/fmt_to_sa.c lib/stralloc.h lib/typedefs.h lib/textcode.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)fmt_uuencoded.o: lib/textcode/fmt_uuencoded.c lib/fmt.h lib/typedefs.h lib/uint32.h lib/uint64.h lib/textcode.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)scan_hexdump.o: lib/textcode/scan_hexdump.c lib/fmt.h lib/typedefs.h lib/uint32.h lib/uint64.h lib/textcode.h lib/scan.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)scan_ldapescape.o: lib/textcode/scan_ldapescape.c lib/fmt.h lib/typedefs.h lib/uint32.h lib/uint64.h lib/textcode.h lib/scan.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)scan_base64url.o: lib/textcode/scan_base64url.c lib/textcode.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)base64url.o: lib/textcode/base64url.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)fmt_html.o: lib/textcode/fmt_html.c lib/fmt.h lib/typedefs.h lib/uint32.h lib/uint64.h lib/textcode.h lib/str.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)scan_cescape.o: lib/textcode/scan_cescape.c lib/fmt.h lib/typedefs.h lib/uint32.h lib/uint64.h lib/textcode.h lib/scan.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)fmt_hexdump.o: lib/textcode/fmt_hexdump.c lib/fmt.h lib/typedefs.h lib/uint32.h lib/uint64.h lib/textcode.h lib/str.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)base64.o: lib/textcode/base64.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)fmt_html_tagarg.o: lib/textcode/fmt_html_tagarg.c lib/fmt.h lib/typedefs.h lib/uint32.h lib/uint64.h lib/textcode.h lib/str.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)scan_to_sa.o: lib/textcode/scan_to_sa.c lib/str.h lib/typedefs.h lib/stralloc.h lib/textcode.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)fmt_base64url.o: lib/textcode/fmt_base64url.c lib/fmt.h lib/typedefs.h lib/uint32.h lib/uint64.h lib/textcode.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)range_str2inbuf.o: lib/safemult/range_str2inbuf.c lib/likely.h lib/rangecheck.h lib/typedefs.h lib/uint32.h lib/typedefs.h lib/uint16.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)range_strinbuf.o: lib/safemult/range_strinbuf.c lib/likely.h lib/rangecheck.h lib/typedefs.h lib/uint32.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)umult64.o: lib/safemult/umult64.c lib/safemult.h lib/uint16.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)imult64.o: lib/safemult/imult64.c lib/safemult.h lib/uint16.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)umult32.o: lib/safemult/umult32.c lib/safemult.h lib/uint16.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)imult16.o: lib/safemult/imult16.c lib/safemult.h lib/uint16.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)imult32.o: lib/safemult/imult32.c lib/safemult.h lib/uint16.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)range_str4inbuf.o: lib/safemult/range_str4inbuf.c lib/likely.h lib/rangecheck.h lib/typedefs.h lib/uint32.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)umult16.o: lib/safemult/umult16.c lib/safemult.h lib/uint16.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)range_arrayinbuf.o: lib/safemult/range_arrayinbuf.c lib/likely.h lib/rangecheck.h lib/typedefs.h lib/uint32.h lib/safemult.h lib/uint16.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)tai_pack.o: lib/tai/tai_pack.c lib/tai.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)tai_now.o: lib/tai/tai_now.c lib/tai.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)tai_sub.o: lib/tai/tai_sub.c lib/tai.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)tai_add.o: lib/tai/tai_add.c lib/tai.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)tai_uint.o: lib/tai/tai_uint.c lib/tai.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)tai_unpack.o: lib/tai/tai_unpack.c lib/tai.h lib/uint64.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)round.o: lib/round.c lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)json_toint.o: lib/json/json_toint.c lib/stralloc.h lib/typedefs.h lib/scan.h lib/uint32.h lib/uint64.h lib/json.h lib/charbuf.h lib/byte.h lib/hmap.h lib/stralloc.h lib/buffer.h lib/slist.h lib/alloc.h lib/str.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)json_print.o: lib/json/json_print.c lib/uint64.h lib/buffer.h lib/typedefs.h lib/hmap.h lib/stralloc.h lib/buffer.h lib/json.h lib/charbuf.h lib/byte.h lib/hmap.h lib/slist.h lib/alloc.h lib/uint64.h lib/str.h lib/slist.h lib/stralloc.h lib/fmt.h lib/uint32.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)json_push.o: lib/json/json_push.c lib/slist.h lib/alloc.h lib/typedefs.h lib/json.h lib/charbuf.h lib/byte.h lib/hmap.h lib/stralloc.h lib/buffer.h lib/slist.h lib/uint64.h lib/str.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)json_todouble.o: lib/json/json_todouble.c lib/stralloc.h lib/typedefs.h lib/scan.h lib/uint32.h lib/uint64.h lib/json.h lib/charbuf.h lib/byte.h lib/hmap.h lib/stralloc.h lib/buffer.h lib/slist.h lib/alloc.h lib/str.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)json_tosa.o: lib/json/json_tosa.c lib/buffer.h lib/typedefs.h lib/stralloc.h lib/json.h lib/charbuf.h lib/byte.h lib/hmap.h lib/stralloc.h lib/buffer.h lib/slist.h lib/alloc.h lib/uint64.h lib/str.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)json_parse.o: lib/json/json_parse.c lib/hmap.h lib/stralloc.h lib/typedefs.h lib/buffer.h lib/byte.h lib/charbuf.h lib/byte.h lib/json.h lib/charbuf.h lib/hmap.h lib/slist.h lib/alloc.h lib/uint64.h lib/str.h lib/scan.h lib/uint32.h lib/stralloc.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)json_recurse.o: lib/json/json_recurse.c lib/hmap.h lib/stralloc.h lib/typedefs.h lib/buffer.h lib/json.h lib/charbuf.h lib/byte.h lib/hmap.h lib/slist.h lib/alloc.h lib/uint64.h lib/str.h lib/slist.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)json_length.o: lib/json/json_length.c lib/hmap.h lib/stralloc.h lib/typedefs.h lib/buffer.h lib/json.h lib/charbuf.h lib/byte.h lib/hmap.h lib/slist.h lib/alloc.h lib/uint64.h lib/str.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)json_get_property.o: lib/json/json_get_property.c lib/hmap.h lib/stralloc.h lib/typedefs.h lib/buffer.h lib/json.h lib/charbuf.h lib/byte.h lib/hmap.h lib/slist.h lib/alloc.h lib/uint64.h lib/str.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)json_newnode.o: lib/json/json_newnode.c lib/alloc.h lib/typedefs.h lib/byte.h lib/json.h lib/charbuf.h lib/byte.h lib/hmap.h lib/stralloc.h lib/buffer.h lib/slist.h lib/alloc.h lib/uint64.h lib/str.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)json_read_tree.o: lib/json/json_read_tree.c lib/hmap.h lib/stralloc.h lib/typedefs.h lib/buffer.h lib/byte.h lib/fmt.h lib/uint32.h lib/uint64.h lib/json.h lib/charbuf.h lib/byte.h lib/hmap.h lib/slist.h lib/alloc.h lib/str.h lib/scan.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)json_reader_init.o: lib/json/json_reader_init.c lib/byte.h lib/typedefs.h lib/json.h lib/charbuf.h lib/byte.h lib/hmap.h lib/stralloc.h lib/buffer.h lib/slist.h lib/alloc.h lib/uint64.h lib/str.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)json_read_callback.o: lib/json/json_read_callback.c lib/byte.h lib/typedefs.h lib/json.h lib/charbuf.h lib/byte.h lib/hmap.h lib/stralloc.h lib/buffer.h lib/slist.h lib/alloc.h lib/uint64.h lib/str.h lib/stralloc.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)json_tostring.o: lib/json/json_tostring.c lib/stralloc.h lib/typedefs.h lib/fmt.h lib/uint32.h lib/uint64.h lib/json.h lib/charbuf.h lib/byte.h lib/hmap.h lib/stralloc.h lib/buffer.h lib/slist.h lib/alloc.h lib/str.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)json_free.o: lib/json/json_free.c lib/hmap.h lib/stralloc.h lib/typedefs.h lib/buffer.h lib/alloc.h lib/json.h lib/charbuf.h lib/byte.h lib/hmap.h lib/slist.h lib/alloc.h lib/uint64.h lib/str.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)json_set_property.o: lib/json/json_set_property.c lib/hmap.h lib/stralloc.h lib/typedefs.h lib/buffer.h lib/json.h lib/charbuf.h lib/byte.h lib/hmap.h lib/slist.h lib/alloc.h lib/uint64.h lib/str.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)array_cat.o: lib/array/array_cat.c lib/array.h lib/typedefs.h lib/uint64.h lib/byte.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)array_get.o: lib/array/array_get.c lib/array.h lib/typedefs.h lib/uint64.h lib/likely.h lib/safemult.h lib/uint16.h lib/uint32.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)array_equal.o: lib/array/array_equal.c lib/array.h lib/typedefs.h lib/uint64.h lib/byte.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)array_allocate.o: lib/array/array_allocate.c lib/likely.h lib/typedefs.h lib/array.h lib/typedefs.h lib/uint64.h lib/byte.h lib/safemult.h lib/uint16.h lib/uint32.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)array_start.o: lib/array/array_start.c lib/array.h lib/typedefs.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)array_trunc.o: lib/array/array_trunc.c lib/array.h lib/typedefs.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)array_cat0.o: lib/array/array_cat0.c lib/array.h lib/typedefs.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)array_cate.o: lib/array/array_cate.c lib/array.h lib/typedefs.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)array_catb.o: lib/array/array_catb.c lib/array.h lib/typedefs.h lib/uint64.h lib/byte.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)array_fail.o: lib/array/array_fail.c lib/array.h lib/typedefs.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)array_cats0.o: lib/array/array_cats0.c lib/array.h lib/typedefs.h lib/uint64.h lib/str.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)array_cats.o: lib/array/array_cats.c lib/array.h lib/typedefs.h lib/uint64.h lib/str.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)array_splice.o: lib/array/array_splice.c lib/array.h lib/typedefs.h lib/uint64.h lib/byte.h lib/uint64.h lib/safemult.h lib/uint16.h lib/uint32.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)array_reset.o: lib/array/array_reset.c lib/array.h lib/typedefs.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)array_truncate.o: lib/array/array_truncate.c lib/array.h lib/typedefs.h lib/uint64.h lib/safemult.h lib/uint16.h lib/uint32.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)array_bytes.o: lib/array/array_bytes.c lib/array.h lib/typedefs.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)array_length.o: lib/array/array_length.c lib/array.h lib/typedefs.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)array_indexof.o: lib/array/array_indexof.c lib/array.h lib/typedefs.h lib/uint64.h lib/byte.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)array_end.o: lib/array/array_end.c lib/array.h lib/typedefs.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)array_find.o: lib/array/array_find.c lib/array.h lib/typedefs.h lib/uint64.h lib/byte.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)charbuf_getc.o: lib/charbuf/charbuf_getc.c lib/charbuf.h lib/typedefs.h lib/byte.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)charbuf_froms.o: lib/charbuf/charbuf_froms.c lib/charbuf.h lib/typedefs.h lib/byte.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)charbuf_nextc.o: lib/charbuf/charbuf_nextc.c lib/charbuf.h lib/typedefs.h lib/byte.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)charbuf_init.o: lib/charbuf/charbuf_init.c lib/charbuf.h lib/typedefs.h lib/byte.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)charbuf_skip_pred.o: lib/charbuf/charbuf_skip_pred.c lib/charbuf.h lib/typedefs.h lib/byte.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)charbuf_close.o: lib/charbuf/charbuf_close.c lib/windoze.h lib/charbuf.h lib/typedefs.h lib/byte.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)charbuf_skip_until.o: lib/charbuf/charbuf_skip_until.c lib/charbuf.h lib/typedefs.h lib/byte.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)charbuf_skip.o: lib/charbuf/charbuf_skip.c lib/charbuf.h lib/typedefs.h lib/byte.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)charbuf_peekc.o: lib/charbuf/charbuf_peekc.c lib/charbuf.h lib/typedefs.h lib/byte.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)charbuf_get.o: lib/charbuf/charbuf_get.c lib/charbuf.h lib/typedefs.h lib/byte.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)charbuf_peek.o: lib/charbuf/charbuf_peek.c lib/charbuf.h lib/typedefs.h lib/byte.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)str_case_diff.o: lib/str/str_case_diff.c lib/str.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)str_dup.o: lib/str/str_dup.c lib/byte.h lib/typedefs.h lib/str.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)str_case_equal.o: lib/str/str_case_equal.c lib/str.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)str_rfindb.o: lib/str/str_rfindb.c lib/str.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)str_cat.o: lib/str/str_cat.c lib/str.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)str_equal.o: lib/str/str_equal.c lib/str.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)str_basename.o: lib/str/str_basename.c lib/windoze.h lib/str.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)str_istr.o: lib/str/str_istr.c lib/str.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)str_rchr.o: lib/str/str_rchr.c lib/str.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)str_is.o: lib/str/str_is.c lib/str.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)str_copyb.o: lib/str/str_copyb.c lib/str.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)str_end.o: lib/str/str_end.c lib/str.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)str_find.o: lib/str/str_find.c lib/str.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)str_diffn.o: lib/str/str_diffn.c lib/byte.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)str_chrs.o: lib/str/str_chrs.c lib/str.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)str_chr.o: lib/str/str_chr.c lib/str.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)str_case_diffn.o: lib/str/str_case_diffn.c lib/str.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)str_tok.o: lib/str/str_tok.c lib/str.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)str_ptime.o: lib/str/str_ptime.c lib/str.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)isleap.o: lib/str/isleap.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)time_table_spd.o: lib/str/time_table_spd.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)str_case_start.o: lib/str/str_case_start.c lib/str.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)str_triml.o: lib/str/str_triml.c lib/str.h lib/typedefs.h lib/byte.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)str_len.o: lib/str/str_len.c lib/str.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)str_diff.o: lib/str/str_diff.c lib/byte.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)str_findb.o: lib/str/str_findb.c lib/str.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)str_rchrs.o: lib/str/str_rchrs.c lib/str.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)str_rfind.o: lib/str/str_rfind.c lib/str.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)str_copyn.o: lib/str/str_copyn.c lib/str.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)str_ndup.o: lib/str/str_ndup.c lib/byte.h lib/typedefs.h lib/str.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)str_start.o: lib/str/str_start.c lib/str.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)str_endb.o: lib/str/str_endb.c lib/str.h lib/typedefs.h lib/byte.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)str_lower.o: lib/str/str_lower.c lib/str.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)str_copy.o: lib/str/str_copy.c lib/str.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)str_camelize.o: lib/str/str_camelize.c lib/str.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)getenv.o: lib/getenv.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)textbuf_free.o: lib/textbuf/textbuf_free.c lib/textbuf.h lib/buffer.h lib/typedefs.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)textbuf_read.o: lib/textbuf/textbuf_read.c lib/textbuf.h lib/buffer.h lib/typedefs.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)textbuf_init.o: lib/textbuf/textbuf_init.c lib/textbuf.h lib/buffer.h lib/typedefs.h lib/uint64.h lib/byte.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)is_textbuf.o: lib/textbuf/is_textbuf.c lib/textbuf.h lib/buffer.h lib/typedefs.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)textbuf_line.o: lib/textbuf/textbuf_line.c lib/textbuf.h lib/buffer.h lib/typedefs.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)textbuf_column.o: lib/textbuf/textbuf_column.c lib/textbuf.h lib/buffer.h lib/typedefs.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)open_trunc.o: lib/open/open_trunc.c lib/windoze.h lib/open.h lib/typedefs.h lib/windoze.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)open_rw.o: lib/open/open_rw.c lib/windoze.h lib/open.h lib/typedefs.h lib/windoze.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)open_temp.o: lib/open/open_temp.c lib/windoze.h lib/open.h lib/typedefs.h lib/windoze.h lib/uint64.h lib/str.h lib/uint32.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)open_read.o: lib/open/open_read.c lib/windoze.h lib/open.h lib/typedefs.h lib/windoze.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)open_append.o: lib/open/open_append.c lib/windoze.h lib/open.h lib/typedefs.h lib/windoze.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)open_write.o: lib/open/open_write.c lib/windoze.h lib/open.h lib/typedefs.h lib/windoze.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)open_excl.o: lib/open/open_excl.c lib/windoze.h lib/open.h lib/typedefs.h lib/windoze.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)playlist_init.o: lib/playlist/playlist_init.c lib/playlist.h lib/stralloc.h lib/typedefs.h lib/uint32.h lib/buffer.h lib/byte.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)playlist_write_entry.o: lib/playlist/playlist_write_entry.c lib/windoze.h lib/playlist.h lib/stralloc.h lib/typedefs.h lib/uint32.h lib/buffer.h lib/str.h lib/byte.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)playlist_m3u.o: lib/playlist/playlist_m3u.c lib/buffer.h lib/typedefs.h lib/playlist.h lib/stralloc.h lib/uint32.h lib/buffer.h lib/scan.h lib/uint64.h lib/str.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)playlist_xspf.o: lib/playlist/playlist_xspf.c lib/buffer.h lib/typedefs.h lib/byte.h lib/playlist.h lib/stralloc.h lib/uint32.h lib/buffer.h lib/scan.h lib/uint64.h lib/xml.h lib/hmap.h lib/textbuf.h lib/str.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)playlist_pls.o: lib/playlist/playlist_pls.c lib/buffer.h lib/typedefs.h lib/playlist.h lib/stralloc.h lib/uint32.h lib/buffer.h lib/scan.h lib/uint64.h lib/str.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)playlist_write_finish.o: lib/playlist/playlist_write_finish.c lib/windoze.h lib/io_internal.h lib/windoze.h lib/array.h lib/typedefs.h lib/uint64.h lib/iarray.h lib/taia.h lib/tai.h lib/uint32.h lib/io.h lib/playlist.h lib/stralloc.h lib/buffer.h lib/byte.h lib/fmt.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)playlist_write_start.o: lib/playlist/playlist_write_start.c lib/windoze.h lib/io_internal.h lib/windoze.h lib/array.h lib/typedefs.h lib/uint64.h lib/iarray.h lib/taia.h lib/tai.h lib/uint32.h lib/io.h lib/playlist.h lib/stralloc.h lib/buffer.h lib/byte.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)playlist_read.o: lib/playlist/playlist_read.c lib/playlist.h lib/stralloc.h lib/typedefs.h lib/uint32.h lib/buffer.h lib/byte.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)socket_mcloop6.o: lib/socket/socket_mcloop6.c lib/socket_internal.h lib/socket.h lib/typedefs.h lib/uint16.h lib/uint32.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)socket_tcp6.o: lib/socket/socket_tcp6.c lib/socket_internal.h lib/socket.h lib/typedefs.h lib/uint16.h lib/uint32.h lib/ndelay.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)socket_accept4.o: lib/socket/socket_accept4.c lib/socket_internal.h lib/socket.h lib/typedefs.h lib/uint16.h lib/uint32.h lib/io_internal.h lib/windoze.h lib/array.h lib/uint64.h lib/iarray.h lib/taia.h lib/tai.h lib/io.h lib/ip4.h lib/uint8.h lib/uint64.h lib/byte.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)socket_noipv6.o: lib/socket/socket_noipv6.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)socket_deferaccept.o: lib/socket/socket_deferaccept.c lib/socket_internal.h lib/socket.h lib/typedefs.h lib/uint16.h lib/uint32.h lib/byte.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)socket_getifidx.o: lib/socket/socket_getifidx.c lib/socket_internal.h lib/socket.h lib/typedefs.h lib/uint16.h lib/uint32.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)socket_error.o: lib/socket/socket_error.c lib/socket_internal.h lib/socket.h lib/typedefs.h lib/uint16.h lib/uint32.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)socket_bind6_reuse.o: lib/socket/socket_bind6_reuse.c lib/socket_internal.h lib/socket.h lib/typedefs.h lib/uint16.h lib/uint32.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)socket_mcleave6.o: lib/socket/socket_mcleave6.c lib/socket_internal.h lib/socket.h lib/typedefs.h lib/uint16.h lib/uint32.h lib/byte.h lib/ip6.h lib/byte.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)socket_mcjoin4.o: lib/socket/socket_mcjoin4.c lib/socket_internal.h lib/socket.h lib/typedefs.h lib/uint16.h lib/uint32.h lib/ip4.h lib/uint8.h lib/byte.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)socket_local6.o: lib/socket/socket_local6.c lib/socket_internal.h lib/socket.h lib/typedefs.h lib/uint16.h lib/uint32.h lib/byte.h lib/ip6.h lib/byte.h lib/uint32.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)socket_recv4.o: lib/socket/socket_recv4.c lib/socket_internal.h lib/socket.h lib/typedefs.h lib/uint16.h lib/uint32.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)socket_fastopen_connect4.o: lib/socket/socket_fastopen_connect4.c lib/socket_internal.h lib/socket.h lib/typedefs.h lib/uint16.h lib/uint32.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)fmt_ip4.o: lib/socket/fmt_ip4.c lib/fmt.h lib/typedefs.h lib/uint32.h lib/uint64.h lib/ip4.h lib/uint8.h lib/uint16.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)socket_send6.o: lib/socket/socket_send6.c lib/socket_internal.h lib/socket.h lib/typedefs.h lib/uint16.h lib/uint32.h lib/byte.h lib/ip4.h lib/uint8.h lib/ip6.h lib/byte.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)socket_connect6.o: lib/socket/socket_connect6.c lib/socket_internal.h lib/socket.h lib/typedefs.h lib/uint16.h lib/uint32.h lib/ip6.h lib/byte.h lib/uint16.h lib/uint32.h lib/byte.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)socket_listen.o: lib/socket/socket_listen.c lib/socket_internal.h lib/socket.h lib/typedefs.h lib/uint16.h lib/uint32.h lib/io_internal.h lib/windoze.h lib/array.h lib/uint64.h lib/iarray.h lib/taia.h lib/tai.h lib/io.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)socket_local4.o: lib/socket/socket_local4.c lib/socket_internal.h lib/socket.h lib/typedefs.h lib/uint16.h lib/uint32.h lib/byte.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)fmt_ip6if.o: lib/socket/fmt_ip6if.c lib/socket_internal.h lib/socket.h lib/typedefs.h lib/uint16.h lib/uint32.h lib/fmt.h lib/uint64.h lib/ip6.h lib/byte.h lib/str.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)socket_mcleave4.o: lib/socket/socket_mcleave4.c lib/socket_internal.h lib/socket.h lib/typedefs.h lib/uint16.h lib/uint32.h lib/byte.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)socket_mchopcount6.o: lib/socket/socket_mchopcount6.c lib/socket_internal.h lib/socket.h lib/typedefs.h lib/uint16.h lib/uint32.h lib/ip6.h lib/byte.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)fmt_ip6.o: lib/socket/fmt_ip6.c lib/byte.h lib/typedefs.h lib/fmt.h lib/uint32.h lib/uint64.h lib/ip4.h lib/uint8.h lib/uint16.h lib/ip6.h lib/byte.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)socket_tcp4.o: lib/socket/socket_tcp4.c lib/socket_internal.h lib/socket.h lib/typedefs.h lib/uint16.h lib/uint32.h lib/ndelay.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)socket_sctp4.o: lib/socket/socket_sctp4.c lib/socket_internal.h lib/socket.h lib/typedefs.h lib/uint16.h lib/uint32.h lib/ndelay.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)socket_connected.o: lib/socket/socket_connected.c lib/socket_internal.h lib/socket.h lib/typedefs.h lib/uint16.h lib/uint32.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)socket_mcjoin6.o: lib/socket/socket_mcjoin6.c lib/socket_internal.h lib/socket.h lib/typedefs.h lib/uint16.h lib/uint32.h lib/byte.h lib/ip6.h lib/byte.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)scan_ip4.o: lib/socket/scan_ip4.c lib/ip4.h lib/uint8.h lib/uint16.h lib/uint32.h lib/scan.h lib/typedefs.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)socket_tcp6b.o: lib/socket/socket_tcp6b.c lib/socket_internal.h lib/socket.h lib/typedefs.h lib/uint16.h lib/uint32.h lib/ndelay.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)socket_remote4.o: lib/socket/socket_remote4.c lib/socket_internal.h lib/socket.h lib/typedefs.h lib/uint16.h lib/uint32.h lib/byte.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)scan_ip6if.o: lib/socket/scan_ip6if.c lib/socket_internal.h lib/socket.h lib/typedefs.h lib/uint16.h lib/uint32.h lib/byte.h lib/ip6.h lib/byte.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)fmt_ip6c.o: lib/socket/fmt_ip6c.c lib/byte.h lib/typedefs.h lib/fmt.h lib/uint32.h lib/uint64.h lib/ip4.h lib/uint8.h lib/uint16.h lib/ip6.h lib/byte.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)socket_bind4.o: lib/socket/socket_bind4.c lib/socket_internal.h lib/socket.h lib/typedefs.h lib/uint16.h lib/uint32.h lib/byte.h lib/uint16.h lib/uint32.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)socket_mcloop4.o: lib/socket/socket_mcloop4.c lib/socket_internal.h lib/socket.h lib/typedefs.h lib/uint16.h lib/uint32.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)socket_v4mappedprefix.o: lib/socket/socket_v4mappedprefix.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)scan_ip6.o: lib/socket/scan_ip6.c lib/ip4.h lib/uint8.h lib/uint16.h lib/uint32.h lib/ip6.h lib/byte.h lib/typedefs.h lib/scan.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)socket_sctp4b.o: lib/socket/socket_sctp4b.c lib/socket_internal.h lib/socket.h lib/typedefs.h lib/uint16.h lib/uint32.h lib/ndelay.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)socket_recv6.o: lib/socket/socket_recv6.c lib/socket_internal.h lib/socket.h lib/typedefs.h lib/uint16.h lib/uint32.h lib/byte.h lib/ip6.h lib/byte.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)socket_accept6.o: lib/socket/socket_accept6.c lib/socket_internal.h lib/socket.h lib/typedefs.h lib/uint16.h lib/uint32.h lib/io_internal.h lib/windoze.h lib/array.h lib/uint64.h lib/iarray.h lib/taia.h lib/tai.h lib/io.h lib/ip6.h lib/byte.h lib/uint64.h lib/byte.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)socket_sctp6b.o: lib/socket/socket_sctp6b.c lib/socket_internal.h lib/socket.h lib/typedefs.h lib/uint16.h lib/uint32.h lib/ndelay.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)socket_v6loopback.o: lib/socket/socket_v6loopback.c lib/socket_internal.h lib/socket.h lib/typedefs.h lib/uint16.h lib/uint32.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)scan_ip6_flat.o: lib/socket/scan_ip6_flat.c lib/scan.h lib/typedefs.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)socket_quickack.o: lib/socket/socket_quickack.c lib/socket_internal.h lib/socket.h lib/typedefs.h lib/uint16.h lib/uint32.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)socket_broadcast.o: lib/socket/socket_broadcast.c lib/socket_internal.h lib/socket.h lib/typedefs.h lib/uint16.h lib/uint32.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)socket_sctp6.o: lib/socket/socket_sctp6.c lib/socket_internal.h lib/socket.h lib/typedefs.h lib/uint16.h lib/uint32.h lib/ndelay.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)socket_fastopen.o: lib/socket/socket_fastopen.c lib/socket_internal.h lib/socket.h lib/typedefs.h lib/uint16.h lib/uint32.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)socket_mcttl4.o: lib/socket/socket_mcttl4.c lib/socket_internal.h lib/socket.h lib/typedefs.h lib/uint16.h lib/uint32.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)socket_bind6.o: lib/socket/socket_bind6.c lib/socket_internal.h lib/socket.h lib/typedefs.h lib/uint16.h lib/uint32.h lib/byte.h lib/ip6.h lib/byte.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)socket_getifname.o: lib/socket/socket_getifname.c lib/socket_internal.h lib/socket.h lib/typedefs.h lib/uint16.h lib/uint32.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)socket_bind4_reuse.o: lib/socket/socket_bind4_reuse.c lib/socket_internal.h lib/socket.h lib/typedefs.h lib/uint16.h lib/uint32.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)socket_connect4.o: lib/socket/socket_connect4.c lib/socket_internal.h lib/socket.h lib/typedefs.h lib/uint16.h lib/uint32.h lib/byte.h lib/uint16.h lib/uint32.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)socket_tryreservein.o: lib/socket/socket_tryreservein.c lib/socket_internal.h lib/socket.h lib/typedefs.h lib/uint16.h lib/uint32.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)socket_tcp4b.o: lib/socket/socket_tcp4b.c lib/socket_internal.h lib/socket.h lib/typedefs.h lib/uint16.h lib/uint32.h lib/ndelay.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)socket_v6any.o: lib/socket/socket_v6any.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)socket_udp4.o: lib/socket/socket_udp4.c lib/socket_internal.h lib/socket.h lib/typedefs.h lib/uint16.h lib/uint32.h lib/ndelay.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)socket_fastopen_connect6.o: lib/socket/socket_fastopen_connect6.c lib/socket_internal.h lib/socket.h lib/typedefs.h lib/uint16.h lib/uint32.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)socket_remote6.o: lib/socket/socket_remote6.c lib/socket_internal.h lib/socket.h lib/typedefs.h lib/uint16.h lib/uint32.h lib/byte.h lib/ip6.h lib/byte.h lib/uint32.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)socket_send4.o: lib/socket/socket_send4.c lib/socket_internal.h lib/socket.h lib/typedefs.h lib/uint16.h lib/uint32.h lib/byte.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)fmt_ip6ifc.o: lib/socket/fmt_ip6ifc.c lib/byte.h lib/typedefs.h lib/fmt.h lib/uint32.h lib/uint64.h lib/ip4.h lib/uint8.h lib/uint16.h lib/ip6.h lib/byte.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)fmt_ip6_flat.o: lib/socket/fmt_ip6_flat.c lib/fmt.h lib/typedefs.h lib/uint32.h lib/uint64.h lib/ip6.h lib/byte.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)winsock2errno.o: lib/socket/winsock2errno.c lib/windoze.h lib/socket_internal.h lib/socket.h lib/typedefs.h lib/uint16.h lib/uint32.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)winsock_init.o: lib/socket/winsock_init.c lib/socket_internal.h lib/socket.h lib/typedefs.h lib/uint16.h lib/uint32.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)socket_udp6.o: lib/socket/socket_udp6.c lib/socket_internal.h lib/socket.h lib/typedefs.h lib/uint16.h lib/uint32.h lib/ndelay.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)socket_ip4loopback.o: lib/socket/socket_ip4loopback.c lib/socket_internal.h lib/socket.h lib/typedefs.h lib/uint16.h lib/uint32.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)winsock_socketpair.o: lib/socket/winsock_socketpair.c lib/socket_internal.h lib/socket.h lib/typedefs.h lib/uint16.h lib/uint32.h lib/byte.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)coff_storage_class.o: lib/coff/coff_storage_class.c lib/coff.h lib/uint8.h lib/uint16.h lib/uint32.h lib/range.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)coff_symbol_table.o: lib/coff/coff_symbol_table.c lib/coff.h lib/uint8.h lib/uint16.h lib/uint32.h lib/range.h lib/typedefs.h lib/range.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)coff_symbol_name.o: lib/coff/coff_symbol_name.c lib/coff.h lib/uint8.h lib/uint16.h lib/uint32.h lib/range.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)coff_get_strtab.o: lib/coff/coff_get_strtab.c lib/coff.h lib/uint8.h lib/uint16.h lib/uint32.h lib/range.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)coff_header.o: lib/coff/coff_header.c lib/coff.h lib/uint8.h lib/uint16.h lib/uint32.h lib/range.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)coff_get_symtab.o: lib/coff/coff_get_symtab.c lib/coff.h lib/uint8.h lib/uint16.h lib/uint32.h lib/range.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)coff_get_section.o: lib/coff/coff_get_section.c lib/coff.h lib/uint8.h lib/uint16.h lib/uint32.h lib/range.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)coff_sclass_name.o: lib/coff/coff_sclass_name.c lib/coff.h lib/uint8.h lib/uint16.h lib/uint32.h lib/range.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)coff_index_symtab.o: lib/coff/coff_index_symtab.c lib/coff.h lib/uint8.h lib/uint16.h lib/uint32.h lib/range.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)coff_line_numbers.o: lib/coff/coff_line_numbers.c lib/coff.h lib/uint8.h lib/uint16.h lib/uint32.h lib/range.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)stralloc_endsb.o: lib/stralloc/stralloc_endsb.c lib/byte.h lib/typedefs.h lib/stralloc.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)stralloc_copywcb.o: lib/stralloc/stralloc_copywcb.c lib/byte.h lib/typedefs.h lib/stralloc.h lib/utf8.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)stralloc_cathexb.o: lib/stralloc/stralloc_cathexb.c lib/fmt.h lib/typedefs.h lib/uint32.h lib/uint64.h lib/stralloc.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)stralloc_0.o: lib/stralloc/stralloc_0.c lib/stralloc.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)str_decamelize.o: lib/stralloc/str_decamelize.c lib/str.h lib/typedefs.h lib/stralloc.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)stralloc_find.o: lib/stralloc/stralloc_find.c lib/stralloc.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)stralloc_catm_internal.o: lib/stralloc/stralloc_catm_internal.c lib/str.h lib/typedefs.h lib/stralloc.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)stralloc_count.o: lib/stralloc/stralloc_count.c lib/byte.h lib/typedefs.h lib/stralloc.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)stralloc_replace.o: lib/stralloc/stralloc_replace.c lib/stralloc.h lib/typedefs.h lib/byte.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)stralloc_copys.o: lib/stralloc/stralloc_copys.c lib/str.h lib/typedefs.h lib/stralloc.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)stralloc_catb.o: lib/stralloc/stralloc_catb.c lib/byte.h lib/typedefs.h lib/stralloc.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_getline_sa.o: lib/stralloc/buffer_getline_sa.c lib/buffer.h lib/typedefs.h lib/stralloc.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)stralloc_case_equals.o: lib/stralloc/stralloc_case_equals.c lib/stralloc.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)readclose.o: lib/stralloc/readclose.c lib/stralloc.h lib/typedefs.h lib/open.h lib/windoze.h lib/uint64.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)stralloc_equalb.o: lib/stralloc/stralloc_equalb.c lib/stralloc.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)stralloc_readyplus.o: lib/stralloc/stralloc_readyplus.c lib/stralloc.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)stralloc_replace_non_printable.o: lib/stralloc/stralloc_replace_non_printable.c lib/stralloc.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)stralloc_fmt_pred.o: lib/stralloc/stralloc_fmt_pred.c lib/stralloc.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)stralloc_catint0.o: lib/stralloc/stralloc_catint0.c lib/stralloc.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)stralloc_catxlong.o: lib/stralloc/stralloc_catxlong.c lib/fmt.h lib/typedefs.h lib/uint32.h lib/uint64.h lib/stralloc.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)stralloc_case_end.o: lib/stralloc/stralloc_case_end.c lib/byte.h lib/typedefs.h lib/stralloc.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)stralloc_cat.o: lib/stralloc/stralloc_cat.c lib/str.h lib/typedefs.h lib/stralloc.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)stralloc_case_start.o: lib/stralloc/stralloc_case_start.c lib/byte.h lib/typedefs.h lib/stralloc.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)stralloc_chr.o: lib/stralloc/stralloc_chr.c lib/byte.h lib/typedefs.h lib/stralloc.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)stralloc_copyb.o: lib/stralloc/stralloc_copyb.c lib/byte.h lib/typedefs.h lib/stralloc.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)stralloc_catwcs.o: lib/stralloc/stralloc_catwcs.c lib/stralloc.h lib/typedefs.h lib/utf8.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)stralloc_scan.o: lib/stralloc/stralloc_scan.c lib/stralloc.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)stralloc_free.o: lib/stralloc/stralloc_free.c lib/stralloc.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)stralloc_diff.o: lib/stralloc/stralloc_diff.c lib/stralloc.h lib/typedefs.h lib/byte.h lib/str.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)stralloc_remove_all.o: lib/stralloc/stralloc_remove_all.c lib/byte.h lib/typedefs.h lib/stralloc.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)stralloc_removesuffixs.o: lib/stralloc/stralloc_removesuffixs.c lib/stralloc.h lib/typedefs.h lib/str.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)mmap_filename.o: lib/stralloc/mmap_filename.c lib/buffer.h lib/typedefs.h lib/scan.h lib/uint32.h lib/uint64.h lib/stralloc.h lib/windoze.h lib/str.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)stralloc_findb.o: lib/stralloc/stralloc_findb.c lib/byte.h lib/typedefs.h lib/stralloc.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)stralloc_catwcb.o: lib/stralloc/stralloc_catwcb.c lib/byte.h lib/typedefs.h lib/stralloc.h lib/utf8.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)stralloc_ready.o: lib/stralloc/stralloc_ready.c lib/byte.h lib/typedefs.h lib/stralloc.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)stralloc_insertb.o: lib/stralloc/stralloc_insertb.c lib/byte.h lib/typedefs.h lib/stralloc.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_gettok_sa.o: lib/stralloc/buffer_gettok_sa.c lib/buffer.h lib/typedefs.h lib/byte.h lib/stralloc.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)stralloc_case_diff.o: lib/stralloc/stralloc_case_diff.c lib/byte.h lib/typedefs.h lib/stralloc.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)stralloc_trunc.o: lib/stralloc/stralloc_trunc.c lib/stralloc.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_puts_escaped.o: lib/stralloc/buffer_puts_escaped.c lib/buffer.h lib/typedefs.h lib/str.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)stralloc_insert.o: lib/stralloc/stralloc_insert.c lib/stralloc.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)stralloc_ready_tuned.o: lib/stralloc/stralloc_ready_tuned.c lib/stralloc.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)stralloc_prependb.o: lib/stralloc/stralloc_prependb.c lib/byte.h lib/typedefs.h lib/stralloc.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)stralloc_fmt.o: lib/stralloc/stralloc_fmt.c lib/stralloc.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)stralloc_chop.o: lib/stralloc/stralloc_chop.c lib/stralloc.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)stralloc_shrink.o: lib/stralloc/stralloc_shrink.c lib/stralloc.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)stralloc_diffs.o: lib/stralloc/stralloc_diffs.c lib/stralloc.h lib/typedefs.h lib/byte.h lib/str.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)stralloc_case_starts.o: lib/stralloc/stralloc_case_starts.c lib/byte.h lib/typedefs.h lib/str.h lib/stralloc.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)open_filename.o: lib/stralloc/open_filename.c lib/windoze.h lib/stralloc.h lib/typedefs.h lib/typedefs.h lib/mmap.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)stralloc_ends.o: lib/stralloc/stralloc_ends.c lib/byte.h lib/typedefs.h lib/stralloc.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)stralloc_remove.o: lib/stralloc/stralloc_remove.c lib/byte.h lib/typedefs.h lib/stralloc.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_getnewline_sa.o: lib/stralloc/buffer_getnewline_sa.c lib/buffer.h lib/typedefs.h lib/stralloc.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)openreadclose.o: lib/stralloc/openreadclose.c lib/stralloc.h lib/typedefs.h lib/open.h lib/windoze.h lib/uint64.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)stralloc_catuint0.o: lib/stralloc/stralloc_catuint0.c lib/stralloc.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)stralloc_copy.o: lib/stralloc/stralloc_copy.c lib/str.h lib/typedefs.h lib/stralloc.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)stralloc_finds.o: lib/stralloc/stralloc_finds.c lib/str.h lib/typedefs.h lib/stralloc.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)stralloc_catlong.o: lib/stralloc/stralloc_catlong.c lib/stralloc.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)stralloc_erase.o: lib/stralloc/stralloc_erase.c lib/stralloc.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)stralloc_startb.o: lib/stralloc/stralloc_startb.c lib/byte.h lib/typedefs.h lib/stralloc.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)stralloc_move.o: lib/stralloc/stralloc_move.c lib/byte.h lib/typedefs.h lib/stralloc.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)stralloc_start.o: lib/stralloc/stralloc_start.c lib/byte.h lib/typedefs.h lib/stralloc.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)stralloc_removesuffixb.o: lib/stralloc/stralloc_removesuffixb.c lib/stralloc.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)stralloc_replacec.o: lib/stralloc/stralloc_replacec.c lib/stralloc.h lib/typedefs.h lib/byte.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)stralloc_lower.o: lib/stralloc/stralloc_lower.c lib/byte.h lib/typedefs.h lib/stralloc.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)stralloc_append.o: lib/stralloc/stralloc_append.c lib/stralloc.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)stralloc_contains.o: lib/stralloc/stralloc_contains.c lib/stralloc.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_get_new_token_sa.o: lib/stralloc/buffer_get_new_token_sa.c lib/buffer.h lib/typedefs.h lib/stralloc.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)utf8.o: lib/stralloc/utf8.c lib/utf8.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)stralloc_catdouble.o: lib/stralloc/stralloc_catdouble.c lib/fmt.h lib/typedefs.h lib/uint32.h lib/uint64.h lib/stralloc.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_get_token_sa.o: lib/stralloc/buffer_get_token_sa.c lib/buffer.h lib/typedefs.h lib/byte.h lib/stralloc.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)stralloc_catuint.o: lib/stralloc/stralloc_catuint.c lib/stralloc.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)stralloc_reverse.o: lib/stralloc/stralloc_reverse.c lib/stralloc.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_tosa.o: lib/stralloc/buffer_tosa.c lib/buffer.h lib/typedefs.h lib/stralloc.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)stralloc_catc.o: lib/stralloc/stralloc_catc.c lib/stralloc.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)stralloc_case_equal.o: lib/stralloc/stralloc_case_equal.c lib/stralloc.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)stralloc_equals.o: lib/stralloc/stralloc_equals.c lib/stralloc.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)stralloc_write.o: lib/stralloc/stralloc_write.c lib/buffer.h lib/typedefs.h lib/stralloc.h lib/byte.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)stralloc_nul.o: lib/stralloc/stralloc_nul.c lib/stralloc.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)stralloc_catlong0.o: lib/stralloc/stralloc_catlong0.c lib/stralloc.h lib/typedefs.h lib/fmt.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)stralloc_equal.o: lib/stralloc/stralloc_equal.c lib/stralloc.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)stralloc_upper.o: lib/stralloc/stralloc_upper.c lib/byte.h lib/typedefs.h lib/stralloc.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)stralloc_chomp.o: lib/stralloc/stralloc_chomp.c lib/stralloc.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)stralloc_copywcs.o: lib/stralloc/stralloc_copywcs.c lib/stralloc.h lib/typedefs.h lib/utf8.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)stralloc_trimr.o: lib/stralloc/stralloc_trimr.c lib/byte.h lib/typedefs.h lib/stralloc.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)stralloc_init.o: lib/stralloc/stralloc_init.c lib/stralloc.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)stralloc_replaces.o: lib/stralloc/stralloc_replaces.c lib/stralloc.h lib/typedefs.h lib/byte.h lib/str.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)stralloc_catulong.o: lib/stralloc/stralloc_catulong.c lib/stralloc.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)stralloc_catulong0.o: lib/stralloc/stralloc_catulong0.c lib/stralloc.h lib/typedefs.h lib/fmt.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)stralloc_rchr.o: lib/stralloc/stralloc_rchr.c lib/byte.h lib/typedefs.h lib/stralloc.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)stralloc_reverse_blocks.o: lib/stralloc/stralloc_reverse_blocks.c lib/byte.h lib/typedefs.h lib/stralloc.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)stralloc_diffb.o: lib/stralloc/stralloc_diffb.c lib/byte.h lib/typedefs.h lib/stralloc.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)stralloc_fmt_call.o: lib/stralloc/stralloc_fmt_call.c lib/windoze.h lib/stralloc.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_get_token_sa_pred.o: lib/stralloc/buffer_get_token_sa_pred.c lib/buffer.h lib/typedefs.h lib/byte.h lib/stralloc.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_put_escaped.o: lib/stralloc/buffer_put_escaped.c lib/buffer.h lib/typedefs.h lib/byte.h lib/fmt.h lib/uint32.h lib/uint64.h lib/stralloc.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)stralloc_prepends.o: lib/stralloc/stralloc_prepends.c lib/str.h lib/typedefs.h lib/stralloc.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)stralloc_inserts.o: lib/stralloc/stralloc_inserts.c lib/str.h lib/typedefs.h lib/stralloc.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)stralloc_prepend.o: lib/stralloc/stralloc_prepend.c lib/stralloc.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)stralloc_catint.o: lib/stralloc/stralloc_catint.c lib/stralloc.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)stralloc_starts.o: lib/stralloc/stralloc_starts.c lib/byte.h lib/typedefs.h lib/str.h lib/stralloc.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)stralloc_zero.o: lib/stralloc/stralloc_zero.c lib/stralloc.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)stralloc_cats.o: lib/stralloc/stralloc_cats.c lib/str.h lib/typedefs.h lib/stralloc.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)stralloc_endb.o: lib/stralloc/stralloc_endb.c lib/byte.h lib/typedefs.h lib/stralloc.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)stralloc_expand.o: lib/stralloc/stralloc_expand.c lib/windoze.h lib/stralloc.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_get_new_token_sa_pred.o: lib/stralloc/buffer_get_new_token_sa_pred.c lib/buffer.h lib/typedefs.h lib/stralloc.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)stralloc_decamelize.o: lib/stralloc/stralloc_decamelize.c lib/stralloc.h lib/typedefs.h lib/byte.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)stralloc_case_diffs.o: lib/stralloc/stralloc_case_diffs.c lib/byte.h lib/typedefs.h lib/str.h lib/stralloc.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)stralloc_subst.o: lib/stralloc/stralloc_subst.c lib/byte.h lib/typedefs.h lib/str.h lib/stralloc.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)case_starts.o: lib/case/case_starts.c lib/case.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)case_findb.o: lib/case/case_findb.c lib/case.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)case_diffs.o: lib/case/case_diffs.c lib/case.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)case_find.o: lib/case/case_find.c lib/case.h lib/typedefs.h lib/str.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)case_lowerc.o: lib/case/case_lowerc.c lib/case.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)case_lowers.o: lib/case/case_lowers.c lib/case.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)case_finds.o: lib/case/case_finds.c lib/case.h lib/typedefs.h lib/str.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)case_lowerb.o: lib/case/case_lowerb.c lib/case.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)case_diffb.o: lib/case/case_diffb.c lib/case.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ndelay_off.o: lib/ndelay/ndelay_off.c lib/socket_internal.h lib/socket.h lib/typedefs.h lib/uint16.h lib/uint32.h lib/ndelay.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ndelay_on.o: lib/ndelay/ndelay_on.c lib/socket_internal.h lib/socket.h lib/typedefs.h lib/uint16.h lib/uint32.h lib/ndelay.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)glob.o: lib/unix/glob.c lib/windoze.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)getpagesize.o: lib/unix/getpagesize.c lib/unix.h lib/typedefs.h lib/windoze.h lib/getopt.h lib/readlink.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)readlink.o: lib/unix/readlink.c lib/windoze.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)win32_loadprog.o: lib/unix/win32_loadprog.c lib/windoze.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)fork.o: lib/unix/fork.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)pipe2.o: lib/unix/pipe2.c lib/windoze.h lib/io_internal.h lib/windoze.h lib/array.h lib/typedefs.h lib/uint64.h lib/iarray.h lib/taia.h lib/tai.h lib/uint32.h lib/io.h lib/ndelay.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)wordexp.o: lib/unix/wordexp.c lib/wordexp.h lib/typedefs.h lib/windoze.h lib/windoze.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)fnmatch.o: lib/unix/fnmatch.c lib/fnmatch.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)fnmatch_b.o: lib/unix/fnmatch_b.c lib/fnmatch.h lib/str.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)win32_glob.o: lib/unix/win32_glob.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)getdelim.o: lib/unix/getdelim.c lib/typedefs.h lib/windoze.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)win32_popen.o: lib/unix/win32_popen.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)win32_borland_popen.o: lib/unix/win32_borland_popen.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)getopt.o: lib/unix/getopt.c lib/getopt.h lib/buffer.h lib/typedefs.h lib/str.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)popen.o: lib/unix/popen.c lib/windoze.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)uint32_seed.o: lib/uint32/uint32_seed.c lib/windoze.h lib/uint32.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)uint32_unpack.o: lib/uint32/uint32_unpack.c lib/uint32.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)uint32_pack.o: lib/uint32/uint32_pack.c lib/uint32.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)uint32_prng.o: lib/uint32/uint32_prng.c lib/uint32.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)uint32_read.o: lib/uint32/uint32_read.c lib/uint32.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)uint32_unpack_big.o: lib/uint32/uint32_unpack_big.c lib/uint32.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)uint32_random.o: lib/uint32/uint32_random.c lib/uint32.h lib/taia.h lib/tai.h lib/uint64.h lib/uint32.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)uint32_read_big.o: lib/uint32/uint32_read_big.c lib/uint32.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)uint32_pack_big.o: lib/uint32/uint32_pack_big.c lib/uint32.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)strptime.o: lib/strptime.c lib/str.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)waitn.o: lib/wait/waitn.c lib/wait.h lib/uint64.h lib/windoze.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)waitn_reap.o: lib/wait/waitn_reap.c lib/wait.h lib/uint64.h lib/windoze.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)wait_pids_nohang.o: lib/wait/wait_pids_nohang.c lib/wait.h lib/uint64.h lib/windoze.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)wait_reap.o: lib/wait/wait_reap.c lib/wait.h lib/uint64.h lib/windoze.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)waitpid_nointr.o: lib/wait/waitpid_nointr.c lib/wait.h lib/uint64.h lib/windoze.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)wait_nointr.o: lib/wait/wait_nointr.c lib/wait.h lib/uint64.h lib/windoze.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)wait_pid.o: lib/wait/wait_pid.c lib/wait.h lib/uint64.h lib/uint64.h lib/windoze.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)wait_pid_nohang.o: lib/wait/wait_pid_nohang.c lib/wait.h lib/uint64.h lib/windoze.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)wait_nohang.o: lib/wait/wait_nohang.c lib/wait.h lib/uint64.h lib/windoze.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)eagle-to-circuit.o: eagle-to-circuit.c lib/windoze.h lib/buffer.h lib/typedefs.h lib/byte.h lib/cb.h lib/cbmap.h lib/uint8.h lib/fmt.h lib/uint32.h lib/uint64.h lib/hmap.h lib/stralloc.h lib/buffer.h lib/iterator.h lib/mmap.h lib/scan.h lib/str.h lib/stralloc.h lib/strlist.h lib/byte.h lib/xml.h lib/hmap.h lib/textbuf.h lib/array.h lib/open.h lib/windoze.h lib/slist.h lib/alloc.h map.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)genmakefile.o: genmakefile.c genmakefile.h lib/windoze.h lib/unix.h lib/typedefs.h lib/windoze.h lib/getopt.h lib/readlink.h lib/mmap.h lib/uint64.h lib/open.h lib/path.h lib/stralloc.h lib/str.h lib/rdir.h lib/dir.h lib/dir_internal.h lib/scan.h lib/uint32.h lib/slist.h lib/alloc.h lib/str.h lib/stralloc.h lib/strarray.h lib/array.h lib/byte.h lib/strlist.h lib/buffer.h lib/uint32.h lib/errmsg.h lib/array.h lib/byte.h lib/fmt.h lib/dir.h lib/range.h lib/case.h map.h lib/hmap.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)parse.o: parse.c lib/windoze.h lib/strlist.h lib/stralloc.h lib/typedefs.h lib/byte.h lib/uint64.h lib/buffer.h lib/strarray.h lib/array.h lib/stralloc.h lib/buffer.h lib/path.h lib/windoze.h lib/str.h lib/process.h lib/env.h lib/errmsg.h lib/byte.h lib/mmap.h lib/scan.h lib/uint32.h lib/getopt.h lib/open.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)elflist.o: elflist.c lib/uint64.h lib/buffer.h lib/typedefs.h lib/elf.h lib/uint8.h lib/uint16.h lib/uint32.h lib/uint64.h lib/range.h lib/mmap.h lib/str.h lib/unix.h lib/windoze.h lib/getopt.h lib/readlink.h lib/getopt.h lib/range.h lib/rangecheck.h lib/byte.h lib/fmt.h lib/strlist.h lib/stralloc.h lib/byte.h lib/buffer.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ini.o: ini.c ini.h map.h lib/hmap.h lib/stralloc.h lib/typedefs.h lib/buffer.h lib/byte.h lib/str.h lib/fmt.h lib/uint32.h lib/uint64.h lib/alloc.h lib/stralloc.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)http_ssl.o: http_ssl.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)eagle-init-brd.o: eagle-init-brd.c lib/windoze.h lib/xml.h lib/buffer.h lib/typedefs.h lib/hmap.h lib/stralloc.h lib/textbuf.h lib/uint64.h lib/hmap.h lib/fmt.h lib/uint32.h lib/byte.h lib/mmap.h lib/scan.h lib/str.h lib/stralloc.h lib/strlist.h lib/byte.h lib/round.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)jsontest.o: jsontest.c lib/windoze.h lib/charbuf.h lib/typedefs.h lib/byte.h lib/mmap.h lib/uint64.h lib/byte.h lib/fmt.h lib/uint32.h lib/hmap.h lib/stralloc.h lib/buffer.h lib/iterator.h lib/stralloc.h lib/json.h lib/charbuf.h lib/hmap.h lib/slist.h lib/alloc.h lib/str.h lib/open.h lib/windoze.h lib/io.h lib/iarray.h lib/taia.h lib/tai.h lib/iarray.h lib/str.h lib/scan.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)mediathek-list.o: mediathek-list.c lib/buffer.h lib/typedefs.h lib/byte.h lib/fmt.h lib/uint32.h lib/uint64.h lib/unix.h lib/windoze.h lib/getopt.h lib/readlink.h lib/http.h lib/uint16.h lib/stralloc.h lib/buffer.h lib/ip4.h lib/uint8.h lib/io.h lib/iarray.h lib/taia.h lib/tai.h lib/iarray.h lib/scan.h lib/slist.h lib/alloc.h lib/str.h lib/stralloc.h lib/strarray.h lib/array.h lib/byte.h lib/strlist.h lib/windoze.h lib/errmsg.h lib/case.h lib/hmap.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)testihex.o: testihex.c lib/ihex.h lib/uint8.h lib/uint16.h lib/uint32.h lib/slist.h lib/alloc.h lib/typedefs.h lib/buffer.h lib/stralloc.h lib/mmap.h lib/uint64.h lib/open.h lib/windoze.h lib/byte.h lib/slist.h lib/uint8.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)crc.o: crc.c lib/buffer.h lib/typedefs.h lib/errmsg.h lib/uint32.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)decode-ls-lR.o: decode-ls-lR.c lib/stralloc.h lib/typedefs.h lib/buffer.h lib/open.h lib/windoze.h lib/uint64.h lib/fmt.h lib/uint32.h lib/byte.h lib/str.h lib/io.h lib/iarray.h lib/taia.h lib/tai.h lib/iarray.h lib/path.h lib/stralloc.h lib/str.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)hexedit.o: hexedit.c lib/windoze.h lib/unix.h lib/typedefs.h lib/windoze.h lib/getopt.h lib/readlink.h lib/io.h lib/iarray.h lib/uint64.h lib/taia.h lib/tai.h lib/uint32.h lib/iarray.h lib/array.h lib/buffer.h lib/stralloc.h lib/mmap.h lib/scan.h lib/uint64.h lib/uint8.h lib/errmsg.h lib/open.h lib/path.h lib/stralloc.h lib/str.h lib/str.h lib/byte.h lib/fmt.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)dnstest.o: dnstest.c lib/socket.h lib/typedefs.h lib/uint16.h lib/uint32.h lib/io.h lib/windoze.h lib/iarray.h lib/uint64.h lib/taia.h lib/tai.h lib/iarray.h lib/case.h lib/open.h lib/stralloc.h lib/scan.h lib/fmt.h lib/str.h lib/tai.h lib/taia.h lib/buffer.h lib/dns.h lib/iopause.h lib/io.h lib/stralloc.h lib/errmsg.h lib/ip4.h lib/uint8.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)dnsname.o: dnsname.c lib/socket.h lib/typedefs.h lib/uint16.h lib/uint32.h lib/io.h lib/windoze.h lib/iarray.h lib/uint64.h lib/taia.h lib/tai.h lib/iarray.h lib/case.h lib/open.h lib/stralloc.h lib/scan.h lib/fmt.h lib/str.h lib/tai.h lib/taia.h lib/buffer.h lib/dns.h lib/iopause.h lib/io.h lib/stralloc.h lib/errmsg.h lib/ip4.h lib/uint8.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)eagle-to-svg.o: eagle-to-svg.c lib/scan.h lib/typedefs.h lib/uint32.h lib/uint64.h lib/open.h lib/windoze.h lib/buffer.h lib/str.h lib/strlist.h lib/stralloc.h lib/byte.h lib/buffer.h lib/xml.h lib/hmap.h lib/textbuf.h map.h lib/hmap.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)xml2json.o: xml2json.c lib/buffer.h lib/typedefs.h lib/byte.h lib/fmt.h lib/uint32.h lib/uint64.h lib/hmap.h lib/stralloc.h lib/buffer.h lib/iterator.h lib/stralloc.h lib/xml.h lib/hmap.h lib/textbuf.h lib/str.h lib/mmap.h lib/scan.h lib/strlist.h lib/byte.h lib/json.h lib/charbuf.h lib/slist.h lib/alloc.h lib/str.h lib/array.h lib/textcode.h lib/errmsg.h lib/open.h lib/windoze.h lib/getopt.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)elf64list.o: elf64list.c lib/elf.h lib/uint8.h lib/uint16.h lib/uint32.h lib/uint64.h lib/range.h lib/typedefs.h lib/mmap.h lib/uint16.h lib/uint32.h lib/uint64.h lib/str.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)httpproxy.o: httpproxy.c lib/windoze.h lib/tai.h lib/uint64.h lib/buffer.h lib/typedefs.h lib/socket.h lib/uint16.h lib/uint32.h lib/socket_internal.h lib/socket.h lib/ndelay.h lib/unix.h lib/windoze.h lib/getopt.h lib/readlink.h lib/errmsg.h lib/stralloc.h lib/dns.h lib/iopause.h lib/io.h lib/iarray.h lib/taia.h lib/tai.h lib/stralloc.h lib/ip4.h lib/uint8.h lib/map.h lib/byte.h lib/safemult.h lib/str.h lib/mmap.h lib/scan.h lib/uint32.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)piccfg.o: piccfg.c lib/uint8.h lib/uint16.h lib/uint32.h lib/slist.h lib/alloc.h lib/typedefs.h lib/mmap.h lib/uint64.h lib/alloc.h lib/byte.h lib/str.h lib/scan.h lib/uint32.h lib/ihex.h lib/uint8.h lib/uint16.h lib/slist.h lib/stralloc.h lib/buffer.h lib/map.h lib/byte.h lib/strlist.h lib/stralloc.h lib/buffer.h lib/getopt.h lib/dir.h lib/path.h lib/windoze.h lib/str.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)tcping.o: tcping.c lib/windoze.h lib/tai.h lib/uint64.h lib/buffer.h lib/typedefs.h lib/socket.h lib/uint16.h lib/uint32.h lib/socket_internal.h lib/socket.h lib/ndelay.h lib/unix.h lib/windoze.h lib/getopt.h lib/readlink.h lib/errmsg.h lib/stralloc.h lib/dns.h lib/iopause.h lib/io.h lib/iarray.h lib/taia.h lib/tai.h lib/stralloc.h lib/ip4.h lib/uint8.h lib/map.h lib/byte.h lib/safemult.h lib/str.h lib/mmap.h lib/scan.h lib/uint32.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)xmltest3.o: xmltest3.c lib/scan.h lib/typedefs.h lib/uint32.h lib/uint64.h lib/byte.h lib/xml.h lib/buffer.h lib/hmap.h lib/stralloc.h lib/textbuf.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)mplab.o: mplab.c genmakefile.h lib/windoze.h lib/unix.h lib/typedefs.h lib/windoze.h lib/getopt.h lib/readlink.h lib/mmap.h lib/uint64.h lib/open.h lib/path.h lib/stralloc.h lib/str.h lib/rdir.h lib/dir.h lib/dir_internal.h lib/scan.h lib/uint32.h lib/slist.h lib/alloc.h lib/str.h lib/stralloc.h lib/strarray.h lib/array.h lib/byte.h lib/strlist.h lib/buffer.h lib/uint32.h lib/errmsg.h lib/array.h lib/byte.h lib/fmt.h lib/dir.h lib/range.h lib/case.h map.h lib/hmap.h ini.h mplab.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)serial.o: serial.c serial.h lib/uint64.h lib/io.h lib/windoze.h lib/iarray.h lib/uint64.h lib/typedefs.h lib/taia.h lib/tai.h lib/uint32.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)rdir-test.o: rdir-test.c lib/rdir.h lib/dir.h lib/uint64.h lib/dir_internal.h lib/stralloc.h lib/typedefs.h lib/dir.h lib/buffer.h lib/stralloc.h lib/byte.h lib/str.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)xml2moon.o: xml2moon.c lib/uint64.h lib/buffer.h lib/typedefs.h lib/byte.h lib/fmt.h lib/uint32.h lib/uint64.h lib/hmap.h lib/stralloc.h lib/buffer.h lib/iterator.h lib/stralloc.h lib/xml.h lib/hmap.h lib/textbuf.h lib/str.h lib/mmap.h lib/scan.h lib/strlist.h lib/byte.h lib/array.h lib/textcode.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)macho32list.o: macho32list.c lib/uint32.h lib/macho.h lib/uint8.h lib/uint16.h lib/uint32.h lib/uint64.h lib/mmap.h lib/typedefs.h lib/str.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)tinyproxy.o: tinyproxy.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)opensearch-dump.o: opensearch-dump.c lib/buffer.h lib/typedefs.h lib/stralloc.h lib/xml.h lib/buffer.h lib/hmap.h lib/stralloc.h lib/textbuf.h lib/uint64.h lib/str.h lib/byte.h lib/fmt.h lib/uint32.h lib/scan.h lib/mmap.h lib/hmap.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)torrent-progress.o: torrent-progress.c lib/windoze.h lib/uint64.h lib/buffer.h lib/typedefs.h lib/io.h lib/windoze.h lib/iarray.h lib/uint64.h lib/taia.h lib/tai.h lib/uint32.h lib/iarray.h lib/mmap.h lib/open.h lib/stralloc.h lib/byte.h lib/fmt.h lib/str.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)sln.o: sln.c lib/windoze.h lib/buffer.h lib/typedefs.h lib/byte.h lib/errmsg.h lib/path.h lib/windoze.h lib/stralloc.h lib/str.h lib/str.h lib/stralloc.h lib/strarray.h lib/uint64.h lib/array.h lib/byte.h lib/mmap.h lib/getopt.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)logserial.o: logserial.c serial.h serial.c lib/uint64.h lib/io.h lib/windoze.h lib/iarray.h lib/uint64.h lib/typedefs.h lib/taia.h lib/tai.h lib/uint32.h lib/str.h lib/alloc.h lib/strlist.h lib/stralloc.h lib/byte.h lib/buffer.h lib/strarray.h lib/array.h lib/buffer.h lib/errmsg.h lib/ndelay.h lib/tai.h lib/taia.h lib/dir.h lib/rdir.h lib/dir.h lib/dir_internal.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)test_tuple.o: tests/test_tuple.c tests/unit_test.h tests/../lib/buffer.h tests/../lib/typedefs.h tests/../lib/taia.h tests/../lib/tai.h tests/../lib/uint64.h tests/../lib/uint32.h tests/../lib/windoze.h tests/../lib/path_internal.h tests/../lib/str.h tests/../lib/path.h tests/../lib/windoze.h tests/../lib/stralloc.h tests/../lib/tuple.h tests/../lib/stralloc.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)unit_test_runner.o: tests/unit_test_runner.c tests/unit_test.h tests/../lib/buffer.h tests/../lib/typedefs.h tests/../lib/taia.h tests/../lib/tai.h tests/../lib/uint64.h tests/../lib/uint32.h tests/../lib/windoze.h tests/../lib/path_internal.h tests/../lib/str.h tests/../lib/path.h tests/../lib/windoze.h tests/../lib/stralloc.h tests/../lib/path.h tests/../lib/strlist.h tests/../lib/byte.h tests/../lib/buffer.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)test_stralloc.o: tests/test_stralloc.c tests/unit_test.h tests/../lib/buffer.h tests/../lib/typedefs.h tests/../lib/taia.h tests/../lib/tai.h tests/../lib/uint64.h tests/../lib/uint32.h tests/../lib/windoze.h tests/../lib/path_internal.h tests/../lib/str.h tests/../lib/path.h tests/../lib/windoze.h tests/../lib/stralloc.h tests/../lib/stralloc.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)test_path.o: tests/test_path.c tests/unit_test.h tests/../lib/buffer.h tests/../lib/typedefs.h tests/../lib/taia.h tests/../lib/tai.h tests/../lib/uint64.h tests/../lib/uint32.h tests/../lib/windoze.h tests/../lib/path_internal.h tests/../lib/str.h tests/../lib/path.h tests/../lib/windoze.h tests/../lib/stralloc.h tests/../lib/path.h tests/../lib/stralloc.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)test_strlist.o: tests/test_strlist.c tests/unit_test.h tests/../lib/buffer.h tests/../lib/typedefs.h tests/../lib/taia.h tests/../lib/tai.h tests/../lib/uint64.h tests/../lib/uint32.h tests/../lib/windoze.h tests/../lib/path_internal.h tests/../lib/str.h tests/../lib/path.h tests/../lib/windoze.h tests/../lib/stralloc.h tests/../lib/strlist.h tests/../lib/byte.h tests/../lib/buffer.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)all_tests.o: tests/all_tests.c tests/unit_test.h tests/../lib/buffer.h tests/../lib/typedefs.h tests/../lib/taia.h tests/../lib/tai.h tests/../lib/uint64.h tests/../lib/uint32.h tests/../lib/windoze.h tests/../lib/path_internal.h tests/../lib/str.h tests/../lib/path.h tests/../lib/windoze.h tests/../lib/stralloc.h tests/../lib/errmsg.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)unit_test.o: tests/unit_test.c tests/../lib/windoze.h tests/../lib/stralloc.h tests/../lib/typedefs.h tests/../lib/buffer.h tests/../lib/open.h tests/../lib/windoze.h tests/../lib/uint64.h tests/../lib/io_internal.h tests/../lib/array.h tests/../lib/iarray.h tests/../lib/taia.h tests/../lib/tai.h tests/../lib/uint32.h tests/../lib/io.h tests/../lib/errmsg.h tests/unit_test.h tests/../lib/taia.h tests/../lib/path_internal.h tests/../lib/str.h tests/../lib/path.h tests/../lib/stralloc.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)montty.o: montty.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)count-depth.o: count-depth.c lib/byte.h lib/typedefs.h lib/buffer.h lib/fmt.h lib/uint32.h lib/uint64.h lib/str.h lib/io.h lib/windoze.h lib/iarray.h lib/taia.h lib/tai.h lib/iarray.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ccat.o: ccat.c lib/buffer.h lib/typedefs.h lib/open.h lib/windoze.h lib/uint64.h lib/unix.h lib/getopt.h lib/readlink.h lib/str.h lib/mmap.h lib/byte.h lib/iarray.h lib/io.h lib/iarray.h lib/taia.h lib/tai.h lib/uint32.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)tcpproxy.o: tcpproxy.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)jsonpp.o: jsonpp.c lib/json.h lib/charbuf.h lib/typedefs.h lib/byte.h lib/hmap.h lib/stralloc.h lib/buffer.h lib/slist.h lib/alloc.h lib/uint64.h lib/str.h lib/open.h lib/windoze.h lib/fmt.h lib/uint32.h lib/errmsg.h lib/str.h lib/scan.h lib/getopt.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)pelist.o: pelist.c lib/unix.h lib/typedefs.h lib/windoze.h lib/getopt.h lib/readlink.h lib/uint64.h lib/buffer.h lib/mmap.h lib/uint64.h lib/pe.h lib/uint32.h lib/uint16.h lib/uint8.h lib/str.h lib/fmt.h lib/byte.h lib/errmsg.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cparser.o: cparser/cparser.c cparser/ansi-c.tab.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)lex.yy.o: cparser/lex.yy.c cparser/ansi-c.tab.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ansi-c.tab.o: cparser/ansi-c.tab.c cparser/../lib/buffer.h cparser/../lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)xmltest4.o: xmltest4.c lib/buffer.h lib/typedefs.h lib/byte.h lib/fmt.h lib/uint32.h lib/uint64.h lib/hmap.h lib/stralloc.h lib/buffer.h lib/iterator.h lib/stralloc.h lib/xml.h lib/hmap.h lib/textbuf.h lib/str.h lib/mmap.h lib/scan.h lib/strlist.h lib/byte.h lib/array.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)impgen.o: impgen.c lib/byte.h lib/typedefs.h lib/buffer.h lib/mmap.h lib/uint64.h lib/open.h lib/windoze.h lib/pe.h lib/uint32.h lib/uint16.h lib/uint8.h lib/uint32.h lib/str.h lib/fmt.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)omflist.o: omflist.c lib/buffer.h lib/typedefs.h lib/unix.h lib/windoze.h lib/getopt.h lib/readlink.h lib/str.h lib/errmsg.h lib/omf.h lib/uint8.h lib/uint16.h lib/range.h lib/fmt.h lib/uint32.h lib/uint64.h lib/mmap.h lib/byte.h lib/stralloc.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)httptest.o: httptest.c lib/windoze.h lib/http.h lib/uint16.h lib/uint32.h lib/uint64.h lib/stralloc.h lib/typedefs.h lib/buffer.h lib/ip4.h lib/uint8.h lib/byte.h lib/io.h lib/windoze.h lib/iarray.h lib/taia.h lib/tai.h lib/iopause.h lib/io.h lib/socket.h lib/taia.h lib/errmsg.h lib/open.h lib/dns.h lib/iopause.h lib/fmt.h lib/iarray.h lib/scan.h lib/str.h lib/stralloc.h lib/tai.h lib/case.h lib/buffer.h lib/getopt.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)redir.o: redir.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)lex.yy.o: lex.yy.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)fdpassing.o: examples/fdpassing.c examples/../lib/io.h examples/../lib/windoze.h examples/../lib/iarray.h examples/../lib/uint64.h examples/../lib/typedefs.h examples/../lib/taia.h examples/../lib/tai.h examples/../lib/uint32.h examples/../lib/buffer.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)b64encode.o: examples/b64encode.c examples/../lib/str.h examples/../lib/typedefs.h examples/../lib/buffer.h examples/../lib/textcode.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)scan.o: examples/scan.c examples/../lib/scan.h examples/../lib/typedefs.h examples/../lib/uint32.h examples/../lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)readhttp.o: examples/readhttp.c examples/../lib/stralloc.h examples/../lib/typedefs.h examples/../lib/buffer.h examples/../lib/byte.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)longlong.o: examples/longlong.c examples/../lib/uint64.h examples/../lib/fmt.h examples/../lib/typedefs.h examples/../lib/uint32.h examples/../lib/uint64.h examples/../lib/str.h examples/../lib/scan.h examples/../lib/byte.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_tosa_example.o: examples/buffer_tosa_example.c examples/../lib/byte.h examples/../lib/typedefs.h examples/../lib/stralloc.h examples/../lib/buffer.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)io2.o: examples/io2.c examples/../lib/io.h examples/../lib/windoze.h examples/../lib/iarray.h examples/../lib/uint64.h examples/../lib/typedefs.h examples/../lib/taia.h examples/../lib/tai.h examples/../lib/uint32.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)marshal.o: examples/marshal.c examples/../lib/fmt.h examples/../lib/typedefs.h examples/../lib/uint32.h examples/../lib/uint64.h examples/../lib/stralloc.h examples/../lib/buffer.h examples/../lib/scan.h examples/../lib/textcode.h examples/../lib/byte.h examples/../lib/uint16.h examples/../lib/uint32.h examples/../lib/uint64.h examples/../lib/open.h examples/../lib/windoze.h examples/../lib/mmap.h examples/../lib/str.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)protobuf.o: examples/protobuf.c examples/../lib/fmt.h examples/../lib/typedefs.h examples/../lib/uint32.h examples/../lib/uint64.h examples/../lib/scan.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)server.o: examples/server.c examples/../lib/scan.h examples/../lib/typedefs.h examples/../lib/uint32.h examples/../lib/uint64.h examples/../lib/ip6.h examples/../lib/byte.h examples/../lib/str.h examples/../lib/socket.h examples/../lib/uint16.h examples/../lib/ndelay.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)uint.o: examples/uint.c examples/../lib/uint16.h examples/../lib/uint32.h examples/../lib/uint64.h examples/../lib/byte.h examples/../lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)range.o: examples/range.c examples/../lib/rangecheck.h examples/../lib/typedefs.h examples/../lib/uint32.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)proxy.o: examples/proxy.c examples/../lib/socket.h examples/../lib/typedefs.h examples/../lib/uint16.h examples/../lib/uint32.h examples/../lib/io.h examples/../lib/windoze.h examples/../lib/iarray.h examples/../lib/uint64.h examples/../lib/taia.h examples/../lib/tai.h examples/../lib/buffer.h examples/../lib/ip6.h examples/../lib/byte.h examples/../lib/str.h examples/../lib/dns.h examples/../lib/iopause.h examples/../lib/io.h examples/../lib/stralloc.h examples/../lib/getopt.h examples/../lib/scan.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_fromsa_example.o: examples/buffer_fromsa_example.c examples/../lib/stralloc.h examples/../lib/typedefs.h examples/../lib/buffer.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)mult.o: examples/mult.c examples/../lib/safemult.h examples/../lib/uint16.h examples/../lib/uint32.h examples/../lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)fmt_iso8691.o: examples/fmt_iso8691.c examples/../lib/fmt.h examples/../lib/typedefs.h examples/../lib/uint32.h examples/../lib/uint64.h examples/../lib/scan.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)fmt_long_example.o: examples/fmt_long_example.c examples/../lib/fmt.h examples/../lib/typedefs.h examples/../lib/uint32.h examples/../lib/uint64.h examples/../lib/str.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)iom.o: examples/iom.c examples/../lib/io.h examples/../lib/windoze.h examples/../lib/iarray.h examples/../lib/uint64.h examples/../lib/typedefs.h examples/../lib/taia.h examples/../lib/tai.h examples/../lib/uint32.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)fmt_ip6_example.o: examples/fmt_ip6_example.c examples/../lib/ip6.h examples/../lib/byte.h examples/../lib/typedefs.h examples/../lib/uint32.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)urlencode.o: examples/urlencode.c examples/../lib/buffer.h examples/../lib/typedefs.h examples/../lib/textcode.h examples/../lib/str.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)unurl.o: examples/unurl.c examples/../lib/str.h examples/../lib/typedefs.h examples/../lib/buffer.h examples/../lib/textcode.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cescape.o: examples/cescape.c examples/../lib/buffer.h examples/../lib/typedefs.h examples/../lib/textcode.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)utf8_example.o: examples/utf8_example.c examples/../lib/scan.h examples/../lib/typedefs.h examples/../lib/uint32.h examples/../lib/uint64.h examples/../lib/fmt.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)stralloc_buffer.o: examples/stralloc_buffer.c examples/../lib/stralloc.h examples/../lib/typedefs.h examples/../lib/buffer.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)fmt.o: examples/fmt.c examples/../lib/fmt.h examples/../lib/typedefs.h examples/../lib/uint32.h examples/../lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)io5.o: examples/io5.c examples/../lib/socket.h examples/../lib/typedefs.h examples/../lib/uint16.h examples/../lib/uint32.h examples/../lib/io.h examples/../lib/windoze.h examples/../lib/iarray.h examples/../lib/uint64.h examples/../lib/taia.h examples/../lib/tai.h examples/../lib/buffer.h examples/../lib/ip6.h examples/../lib/byte.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)fmt_human_example.o: examples/fmt_human_example.c examples/../lib/fmt.h examples/../lib/typedefs.h examples/../lib/uint32.h examples/../lib/uint64.h examples/../lib/buffer.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)io4.o: examples/io4.c examples/../lib/io.h examples/../lib/windoze.h examples/../lib/iarray.h examples/../lib/uint64.h examples/../lib/typedefs.h examples/../lib/taia.h examples/../lib/tai.h examples/../lib/uint32.h examples/../lib/uint64.h examples/../lib/buffer.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)uudecode.o: examples/uudecode.c examples/../lib/textcode.h examples/../lib/str.h examples/../lib/typedefs.h examples/../lib/buffer.h examples/../lib/open.h examples/../lib/windoze.h examples/../lib/uint64.h examples/../lib/stralloc.h examples/../lib/scan.h examples/../lib/uint32.h examples/../lib/case.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)httpd.o: examples/httpd.c examples/../lib/socket.h examples/../lib/typedefs.h examples/../lib/uint16.h examples/../lib/uint32.h examples/../lib/io.h examples/../lib/windoze.h examples/../lib/iarray.h examples/../lib/uint64.h examples/../lib/taia.h examples/../lib/tai.h examples/../lib/buffer.h examples/../lib/ip6.h examples/../lib/byte.h examples/../lib/array.h examples/../lib/case.h examples/../lib/fmt.h examples/../lib/iob.h examples/../lib/io.h examples/../lib/array.h examples/../lib/str.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)fmt_strm_alloca.o: examples/fmt_strm_alloca.c examples/../lib/fmt.h examples/../lib/typedefs.h examples/../lib/uint32.h examples/../lib/uint64.h examples/../lib/byte.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_mmap.o: examples/buffer_mmap.c examples/../lib/buffer.h examples/../lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)vd.o: examples/vd.c examples/../lib/socket.h examples/../lib/typedefs.h examples/../lib/uint16.h examples/../lib/uint32.h examples/../lib/buffer.h examples/../lib/ndelay.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)json.o: examples/json.c examples/../lib/textcode.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)dnsip.o: examples/dnsip.c examples/../lib/dns.h examples/../lib/iopause.h examples/../lib/io.h examples/../lib/windoze.h examples/../lib/iarray.h examples/../lib/uint64.h examples/../lib/typedefs.h examples/../lib/taia.h examples/../lib/tai.h examples/../lib/uint32.h examples/../lib/stralloc.h examples/../lib/ip4.h examples/../lib/uint8.h examples/../lib/uint16.h examples/../lib/buffer.h examples/../lib/errmsg.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)iob.o: examples/iob.c examples/../lib/iob.h examples/../lib/io.h examples/../lib/windoze.h examples/../lib/iarray.h examples/../lib/uint64.h examples/../lib/typedefs.h examples/../lib/taia.h examples/../lib/tai.h examples/../lib/uint32.h examples/../lib/array.h examples/../lib/buffer.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)dllink.o: examples/dllink.c examples/../lib/socket.h examples/../lib/typedefs.h examples/../lib/uint16.h examples/../lib/uint32.h examples/../lib/buffer.h examples/../lib/case.h examples/../lib/ndelay.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cas.o: examples/cas.c examples/../lib/cas.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)array.o: examples/array.c examples/../lib/array.h examples/../lib/typedefs.h examples/../lib/uint64.h examples/../lib/byte.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)io3.o: examples/io3.c examples/../lib/uint64.h examples/../lib/buffer.h examples/../lib/typedefs.h examples/../lib/io.h examples/../lib/windoze.h examples/../lib/iarray.h examples/../lib/uint64.h examples/../lib/taia.h examples/../lib/tai.h examples/../lib/uint32.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)textcode.o: examples/textcode.c examples/../lib/array.h examples/../lib/typedefs.h examples/../lib/uint64.h examples/../lib/textcode.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)fmt_httpdate_example.o: examples/fmt_httpdate_example.c examples/../lib/scan.h examples/../lib/typedefs.h examples/../lib/uint32.h examples/../lib/uint64.h examples/../lib/buffer.h examples/../lib/fmt.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_1_example.o: examples/buffer_1_example.c examples/../lib/buffer.h examples/../lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)io.o: examples/io.c examples/../lib/io.h examples/../lib/windoze.h examples/../lib/iarray.h examples/../lib/uint64.h examples/../lib/typedefs.h examples/../lib/taia.h examples/../lib/tai.h examples/../lib/uint32.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)stralloc_chomp_example.o: examples/stralloc_chomp_example.c examples/../lib/stralloc.h examples/../lib/typedefs.h examples/../lib/buffer.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)client.o: examples/client.c examples/../lib/scan.h examples/../lib/typedefs.h examples/../lib/uint32.h examples/../lib/uint64.h examples/../lib/ip6.h examples/../lib/byte.h examples/../lib/str.h examples/../lib/socket.h examples/../lib/uint16.h examples/../lib/ndelay.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)byte_copy_example.o: examples/byte_copy_example.c examples/../lib/byte.h examples/../lib/typedefs.h examples/../lib/errmsg.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)scan_long_example.o: examples/scan_long_example.c examples/../lib/scan.h examples/../lib/typedefs.h examples/../lib/uint32.h examples/../lib/uint64.h examples/../lib/fmt.h examples/../lib/buffer.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)netstring.o: examples/netstring.c examples/../lib/fmt.h examples/../lib/typedefs.h examples/../lib/uint32.h examples/../lib/uint64.h examples/../lib/scan.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cdbget2.o: examples/cdbget2.c examples/../lib/buffer.h examples/../lib/typedefs.h examples/../lib/errmsg.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ansi-c.tab.o: ansi-c.tab.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)xmltest.o: xmltest.c lib/xml.h lib/buffer.h lib/typedefs.h lib/hmap.h lib/stralloc.h lib/textbuf.h lib/uint64.h lib/buffer.h lib/byte.h lib/fmt.h lib/uint32.h lib/hmap.h lib/iterator.h lib/stralloc.h lib/strlist.h lib/byte.h lib/str.h lib/mmap.h lib/map.h lib/scan.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)xmltest2.o: xmltest2.c lib/buffer.h lib/typedefs.h lib/byte.h lib/fmt.h lib/uint32.h lib/uint64.h lib/hmap.h lib/stralloc.h lib/buffer.h lib/iterator.h lib/stralloc.h lib/strlist.h lib/byte.h lib/xml.h lib/hmap.h lib/textbuf.h lib/str.h lib/mmap.h lib/scan.h lib/array.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)strip-comments.o: strip-comments.c lib/open.h lib/typedefs.h lib/windoze.h lib/uint64.h lib/fmt.h lib/uint32.h lib/errmsg.h lib/str.h lib/scan.h lib/getopt.h lib/charbuf.h lib/byte.h lib/stralloc.h lib/buffer.h lib/mmap.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffertest.o: buffertest.c lib/stralloc.h lib/typedefs.h lib/buffer.h lib/open.h lib/windoze.h lib/uint64.h lib/mmap.h lib/byte.h lib/str.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)dnsip.o: dnsip.c lib/socket.h lib/typedefs.h lib/uint16.h lib/uint32.h lib/io.h lib/windoze.h lib/iarray.h lib/uint64.h lib/taia.h lib/tai.h lib/iarray.h lib/case.h lib/open.h lib/stralloc.h lib/scan.h lib/fmt.h lib/str.h lib/tai.h lib/taia.h lib/buffer.h lib/dns.h lib/iopause.h lib/io.h lib/stralloc.h lib/errmsg.h lib/ip4.h lib/uint8.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cmake-run.o: cmake-run.c lib/windoze.h lib/strlist.h lib/stralloc.h lib/typedefs.h lib/byte.h lib/uint64.h lib/buffer.h lib/strarray.h lib/array.h lib/open.h lib/windoze.h lib/stralloc.h lib/buffer.h lib/path.h lib/str.h lib/process.h lib/env.h lib/errmsg.h lib/byte.h lib/mmap.h lib/scan.h lib/uint32.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)regfilter.o: regfilter.c lib/windoze.h lib/uint64.h lib/buffer.h lib/typedefs.h lib/byte.h lib/fmt.h lib/uint32.h lib/uint64.h lib/open.h lib/windoze.h lib/scan.h lib/str.h lib/stralloc.h lib/io.h lib/iarray.h lib/taia.h lib/tai.h lib/iarray.h lib/path.h lib/stralloc.h lib/str.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)binfmttest.o: binfmttest.c lib/byte.h lib/typedefs.h lib/binfmt.h lib/uint8.h lib/uint32.h lib/uint64.h lib/range.h lib/mmap.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)list-r.o: list-r.c lib/windoze.h lib/getopt.h lib/buffer.h lib/typedefs.h lib/dir.h lib/uint64.h lib/fmt.h lib/uint32.h lib/fnmatch.h lib/io.h lib/windoze.h lib/iarray.h lib/taia.h lib/tai.h lib/iarray.h lib/open.h lib/str.h lib/stralloc.h lib/uint64.h lib/ioctlcmd.h lib/byte.h lib/array.h lib/unix.h lib/getopt.h lib/readlink.h lib/path.h lib/stralloc.h lib/str.h lib/scan.h lib/mmap.h lib/strlist.h lib/byte.h lib/buffer.h lib/strarray.h lib/array.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)mediathek-parser.o: mediathek-parser.c lib/unix.h lib/typedefs.h lib/windoze.h lib/getopt.h lib/readlink.h lib/array.h lib/uint64.h lib/buffer.h lib/byte.h lib/io.h lib/iarray.h lib/taia.h lib/tai.h lib/uint32.h lib/iarray.h lib/open.h lib/str.h lib/stralloc.h lib/strlist.h lib/stralloc.h lib/byte.h lib/buffer.h lib/fmt.h lib/mmap.h lib/ucs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)elfwrsec.o: elfwrsec.c lib/buffer.h lib/typedefs.h lib/elf.h lib/uint8.h lib/uint16.h lib/uint32.h lib/uint64.h lib/range.h lib/mmap.h lib/str.h lib/byte.h lib/fmt.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ntldd.o: ntldd.c lib/windoze.h lib/uint64.h lib/buffer.h lib/typedefs.h lib/byte.h lib/unix.h lib/windoze.h lib/getopt.h lib/readlink.h lib/path.h lib/stralloc.h lib/str.h lib/str.h lib/pe.h lib/uint64.h lib/uint32.h lib/uint16.h lib/uint8.h lib/mmap.h lib/fmt.h lib/stralloc.h lib/strlist.h lib/byte.h lib/buffer.h libntldd.h lib/uint16.h lib/uint32.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)conf.o: openssl-1.1.1d/fuzz/conf.c openssl-1.1.1d/fuzz/fuzzer.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)crl.o: openssl-1.1.1d/fuzz/crl.c openssl-1.1.1d/fuzz/fuzzer.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bignum.o: openssl-1.1.1d/fuzz/bignum.c openssl-1.1.1d/fuzz/fuzzer.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)asn1.o: openssl-1.1.1d/fuzz/asn1.c openssl-1.1.1d/fuzz/fuzzer.h openssl-1.1.1d/fuzz/rand.inc
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)asn1parse.o: openssl-1.1.1d/fuzz/asn1parse.c openssl-1.1.1d/fuzz/fuzzer.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bndiv.o: openssl-1.1.1d/fuzz/bndiv.c openssl-1.1.1d/fuzz/fuzzer.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)server.o: openssl-1.1.1d/fuzz/server.c openssl-1.1.1d/fuzz/fuzzer.h openssl-1.1.1d/fuzz/rand.inc
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ct.o: openssl-1.1.1d/fuzz/ct.c openssl-1.1.1d/fuzz/fuzzer.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cms.o: openssl-1.1.1d/fuzz/cms.c openssl-1.1.1d/fuzz/fuzzer.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)test-corpus.o: openssl-1.1.1d/fuzz/test-corpus.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)driver.o: openssl-1.1.1d/fuzz/driver.c openssl-1.1.1d/fuzz/fuzzer.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)x509.o: openssl-1.1.1d/fuzz/x509.c openssl-1.1.1d/fuzz/fuzzer.h openssl-1.1.1d/fuzz/rand.inc
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)client.o: openssl-1.1.1d/fuzz/client.c openssl-1.1.1d/fuzz/fuzzer.h openssl-1.1.1d/fuzz/rand.inc
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)applink.o: openssl-1.1.1d/ms/applink.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)uplink.o: openssl-1.1.1d/ms/uplink.c openssl-1.1.1d/ms/uplink.h openssl-1.1.1d/ms/applink.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)crl.o: openssl-1.1.1d/apps/crl.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)pkcs12.o: openssl-1.1.1d/apps/pkcs12.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ocsp.o: openssl-1.1.1d/apps/ocsp.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)vms_term_sock.o: openssl-1.1.1d/apps/vms_term_sock.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)dsa.o: openssl-1.1.1d/apps/dsa.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)opt.o: openssl-1.1.1d/apps/opt.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)genrsa.o: openssl-1.1.1d/apps/genrsa.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)pkey.o: openssl-1.1.1d/apps/pkey.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)crl2p7.o: openssl-1.1.1d/apps/crl2p7.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)req.o: openssl-1.1.1d/apps/req.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)rsa.o: openssl-1.1.1d/apps/rsa.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ec.o: openssl-1.1.1d/apps/ec.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)nseq.o: openssl-1.1.1d/apps/nseq.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)storeutl.o: openssl-1.1.1d/apps/storeutl.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)sess_id.o: openssl-1.1.1d/apps/sess_id.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)gendsa.o: openssl-1.1.1d/apps/gendsa.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)s_socket.o: openssl-1.1.1d/apps/s_socket.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)engine.o: openssl-1.1.1d/apps/engine.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)version.o: openssl-1.1.1d/apps/version.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cms.o: openssl-1.1.1d/apps/cms.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)speed.o: openssl-1.1.1d/apps/speed.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bf_prefix.o: openssl-1.1.1d/apps/bf_prefix.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)openssl.o: openssl-1.1.1d/apps/openssl.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)win32_init.o: openssl-1.1.1d/apps/win32_init.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ecparam.o: openssl-1.1.1d/apps/ecparam.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)vms_decc_init.o: openssl-1.1.1d/apps/vms_decc_init.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)apps.o: openssl-1.1.1d/apps/apps.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)app_rand.o: openssl-1.1.1d/apps/app_rand.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)prime.o: openssl-1.1.1d/apps/prime.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)pkcs7.o: openssl-1.1.1d/apps/pkcs7.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)dsaparam.o: openssl-1.1.1d/apps/dsaparam.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)dgst.o: openssl-1.1.1d/apps/dgst.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)errstr.o: openssl-1.1.1d/apps/errstr.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)s_server.o: openssl-1.1.1d/apps/s_server.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)rsautl.o: openssl-1.1.1d/apps/rsautl.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ts.o: openssl-1.1.1d/apps/ts.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)dhparam.o: openssl-1.1.1d/apps/dhparam.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)srp.o: openssl-1.1.1d/apps/srp.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)s_client.o: openssl-1.1.1d/apps/s_client.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)rehash.o: openssl-1.1.1d/apps/rehash.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)enc.o: openssl-1.1.1d/apps/enc.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)asn1pars.o: openssl-1.1.1d/apps/asn1pars.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)passwd.o: openssl-1.1.1d/apps/passwd.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)pkcs8.o: openssl-1.1.1d/apps/pkcs8.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)pkeyutl.o: openssl-1.1.1d/apps/pkeyutl.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)rand.o: openssl-1.1.1d/apps/rand.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)spkac.o: openssl-1.1.1d/apps/spkac.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)smime.o: openssl-1.1.1d/apps/smime.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ciphers.o: openssl-1.1.1d/apps/ciphers.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)x509.o: openssl-1.1.1d/apps/x509.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)s_time.o: openssl-1.1.1d/apps/s_time.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)verify.o: openssl-1.1.1d/apps/verify.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ca.o: openssl-1.1.1d/apps/ca.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)s_cb.o: openssl-1.1.1d/apps/s_cb.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)pkeyparam.o: openssl-1.1.1d/apps/pkeyparam.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)genpkey.o: openssl-1.1.1d/apps/genpkey.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ecstresstest.o: openssl-1.1.1d/test/ecstresstest.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)sm4_internal_test.o: openssl-1.1.1d/test/sm4_internal_test.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)chacha_internal_test.o: openssl-1.1.1d/test/chacha_internal_test.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)exptest.o: openssl-1.1.1d/test/exptest.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)test_test.o: openssl-1.1.1d/test/test_test.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)pkey_meth_test.o: openssl-1.1.1d/test/pkey_meth_test.c openssl-1.1.1d/test/testutil.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bntest.o: openssl-1.1.1d/test/bntest.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)errtest.o: openssl-1.1.1d/test/errtest.c openssl-1.1.1d/test/testutil.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)srptest.o: openssl-1.1.1d/test/srptest.c openssl-1.1.1d/test/testutil.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)uitest.o: openssl-1.1.1d/test/uitest.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ectest.o: openssl-1.1.1d/test/ectest.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)asn1_string_table_test.o: openssl-1.1.1d/test/asn1_string_table_test.c openssl-1.1.1d/test/testutil.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)asynctest.o: openssl-1.1.1d/test/asynctest.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ecdsatest.o: openssl-1.1.1d/test/ecdsatest.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)v3ext.o: openssl-1.1.1d/test/v3ext.c openssl-1.1.1d/test/testutil.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)aborttest.o: openssl-1.1.1d/test/aborttest.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)gosttest.o: openssl-1.1.1d/test/gosttest.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bio_memleak_test.o: openssl-1.1.1d/test/bio_memleak_test.c openssl-1.1.1d/test/testutil.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)dhtest.o: openssl-1.1.1d/test/dhtest.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bftest.o: openssl-1.1.1d/test/bftest.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)x509aux.o: openssl-1.1.1d/test/x509aux.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cipherlist_test.o: openssl-1.1.1d/test/cipherlist_test.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)afalgtest.o: openssl-1.1.1d/test/afalgtest.c openssl-1.1.1d/test/testutil.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)pkey_meth_kdf_test.o: openssl-1.1.1d/test/pkey_meth_kdf_test.c openssl-1.1.1d/test/testutil.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ocspapitest.o: openssl-1.1.1d/test/ocspapitest.c openssl-1.1.1d/test/testutil.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ideatest.o: openssl-1.1.1d/test/ideatest.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)threadstest.o: openssl-1.1.1d/test/threadstest.c openssl-1.1.1d/test/testutil.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)sslbuffertest.o: openssl-1.1.1d/test/sslbuffertest.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ssltest_old.o: openssl-1.1.1d/test/ssltest_old.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)d2i_test.o: openssl-1.1.1d/test/d2i_test.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)versions.o: openssl-1.1.1d/test/versions.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)exdatatest.o: openssl-1.1.1d/test/exdatatest.c openssl-1.1.1d/test/testutil.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)mdc2test.o: openssl-1.1.1d/test/mdc2test.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)sslapitest.o: openssl-1.1.1d/test/sslapitest.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ec_internal_test.o: openssl-1.1.1d/test/ec_internal_test.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)evp_extra_test.o: openssl-1.1.1d/test/evp_extra_test.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)shlibloadtest.o: openssl-1.1.1d/test/shlibloadtest.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)dsatest.o: openssl-1.1.1d/test/dsatest.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ssl_cert_table_internal_test.o: openssl-1.1.1d/test/ssl_cert_table_internal_test.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)fatalerrtest.o: openssl-1.1.1d/test/fatalerrtest.c openssl-1.1.1d/test/ssltestlib.h openssl-1.1.1d/test/testutil.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)siphash_internal_test.o: openssl-1.1.1d/test/siphash_internal_test.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)dsa_no_digest_size_test.o: openssl-1.1.1d/test/dsa_no_digest_size_test.c openssl-1.1.1d/test/testutil.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bioprinttest.o: openssl-1.1.1d/test/bioprinttest.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)handshake_helper.o: openssl-1.1.1d/test/handshake_helper.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)stack_test.o: openssl-1.1.1d/test/stack_test.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)curve448_internal_test.o: openssl-1.1.1d/test/curve448_internal_test.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)x509_internal_test.o: openssl-1.1.1d/test/x509_internal_test.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)gmdifftest.o: openssl-1.1.1d/test/gmdifftest.c openssl-1.1.1d/test/testutil.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bio_enc_test.o: openssl-1.1.1d/test/bio_enc_test.c openssl-1.1.1d/test/testutil.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)crltest.o: openssl-1.1.1d/test/crltest.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)rc5test.o: openssl-1.1.1d/test/rc5test.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)x509_dup_cert_test.o: openssl-1.1.1d/test/x509_dup_cert_test.c openssl-1.1.1d/test/testutil.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)mdc2_internal_test.o: openssl-1.1.1d/test/mdc2_internal_test.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)recordlentest.o: openssl-1.1.1d/test/recordlentest.c openssl-1.1.1d/test/ssltestlib.h openssl-1.1.1d/test/testutil.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)evp_test.o: openssl-1.1.1d/test/evp_test.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)asn1_encode_test.o: openssl-1.1.1d/test/asn1_encode_test.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)clienthellotest.o: openssl-1.1.1d/test/clienthellotest.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)danetest.o: openssl-1.1.1d/test/danetest.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)rsa_test.o: openssl-1.1.1d/test/rsa_test.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)pbelutest.o: openssl-1.1.1d/test/pbelutest.c openssl-1.1.1d/test/testutil.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)servername_test.o: openssl-1.1.1d/test/servername_test.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)sslcorrupttest.o: openssl-1.1.1d/test/sslcorrupttest.c openssl-1.1.1d/test/ssltestlib.h openssl-1.1.1d/test/testutil.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)lhash_test.o: openssl-1.1.1d/test/lhash_test.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)asynciotest.o: openssl-1.1.1d/test/asynciotest.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)rsa_complex.o: openssl-1.1.1d/test/rsa_complex.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cipher_overhead_test.o: openssl-1.1.1d/test/cipher_overhead_test.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)constant_time_test.o: openssl-1.1.1d/test/constant_time_test.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)wpackettest.o: openssl-1.1.1d/test/wpackettest.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ssl_test.o: openssl-1.1.1d/test/ssl_test.c openssl-1.1.1d/test/handshake_helper.h openssl-1.1.1d/test/ssl_test_ctx.h openssl-1.1.1d/test/testutil.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ssl_test_ctx.o: openssl-1.1.1d/test/ssl_test_ctx.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)rdrand_sanitytest.o: openssl-1.1.1d/test/rdrand_sanitytest.c openssl-1.1.1d/test/testutil.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)drbgtest.o: openssl-1.1.1d/test/drbgtest.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cipherbytes_test.o: openssl-1.1.1d/test/cipherbytes_test.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)tls13secretstest.o: openssl-1.1.1d/test/tls13secretstest.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)x509_time_test.o: openssl-1.1.1d/test/x509_time_test.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)sm2_internal_test.o: openssl-1.1.1d/test/sm2_internal_test.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)tls13encryptiontest.o: openssl-1.1.1d/test/tls13encryptiontest.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)memleaktest.o: openssl-1.1.1d/test/memleaktest.c openssl-1.1.1d/test/testutil.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)init.o: openssl-1.1.1d/test/testutil/init.c openssl-1.1.1d/testutil.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)basic_output.o: openssl-1.1.1d/test/testutil/basic_output.c openssl-1.1.1d/testutil.h openssl-1.1.1d/test/testutil/output.h openssl-1.1.1d/test/testutil/tu_local.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)format_output.o: openssl-1.1.1d/test/testutil/format_output.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)random.o: openssl-1.1.1d/test/testutil/random.c openssl-1.1.1d/testutil.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)driver.o: openssl-1.1.1d/test/testutil/driver.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)stanza.o: openssl-1.1.1d/test/testutil/stanza.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cb.o: openssl-1.1.1d/test/testutil/cb.c openssl-1.1.1d/test/testutil/output.h openssl-1.1.1d/test/testutil/tu_local.h openssl-1.1.1d/testutil.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)tap_bio.o: openssl-1.1.1d/test/testutil/tap_bio.c openssl-1.1.1d/test/testutil/tu_local.h openssl-1.1.1d/testutil.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)main.o: openssl-1.1.1d/test/testutil/main.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)output_helpers.o: openssl-1.1.1d/test/testutil/output_helpers.c openssl-1.1.1d/test/testutil/output.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)tests.o: openssl-1.1.1d/test/testutil/tests.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)test_cleanup.o: openssl-1.1.1d/test/testutil/test_cleanup.c openssl-1.1.1d/testutil.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)rc2test.o: openssl-1.1.1d/test/rc2test.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ctype_internal_test.o: openssl-1.1.1d/test/ctype_internal_test.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)casttest.o: openssl-1.1.1d/test/casttest.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)packettest.o: openssl-1.1.1d/test/packettest.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)v3nametest.o: openssl-1.1.1d/test/v3nametest.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)sysdefaulttest.o: openssl-1.1.1d/test/sysdefaulttest.c openssl-1.1.1d/test/testutil.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)enginetest.o: openssl-1.1.1d/test/enginetest.c openssl-1.1.1d/test/testutil.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)drbg_cavs_test.o: openssl-1.1.1d/test/drbg_cavs_test.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)poly1305_internal_test.o: openssl-1.1.1d/test/poly1305_internal_test.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)conf_include_test.o: openssl-1.1.1d/test/conf_include_test.c openssl-1.1.1d/test/testutil.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)secmemtest.o: openssl-1.1.1d/test/secmemtest.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bad_dtls_test.o: openssl-1.1.1d/test/bad_dtls_test.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)modes_internal_test.o: openssl-1.1.1d/test/modes_internal_test.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)asn1_decode_test.o: openssl-1.1.1d/test/asn1_decode_test.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ssltestlib.o: openssl-1.1.1d/test/ssltestlib.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ssl_test_ctx_test.o: openssl-1.1.1d/test/ssl_test_ctx_test.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)destest.o: openssl-1.1.1d/test/destest.c openssl-1.1.1d/test/testutil.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)rsa_mp_test.o: openssl-1.1.1d/test/rsa_mp_test.c openssl-1.1.1d/test/testutil.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)x509_check_cert_pkey_test.o: openssl-1.1.1d/test/x509_check_cert_pkey_test.c openssl-1.1.1d/test/testutil.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)asn1_time_test.o: openssl-1.1.1d/test/asn1_time_test.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ciphername_test.o: openssl-1.1.1d/test/ciphername_test.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)igetest.o: openssl-1.1.1d/test/igetest.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)dtls_mtu_test.o: openssl-1.1.1d/test/dtls_mtu_test.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)md2test.o: openssl-1.1.1d/test/md2test.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bio_callback_test.o: openssl-1.1.1d/test/bio_callback_test.c openssl-1.1.1d/test/testutil.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ct_test.o: openssl-1.1.1d/test/ct_test.c openssl-1.1.1d/test/testutil.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)dtlstest.o: openssl-1.1.1d/test/dtlstest.c openssl-1.1.1d/test/ssltestlib.h openssl-1.1.1d/test/testutil.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)rc4test.o: openssl-1.1.1d/test/rc4test.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)dtlsv1listentest.o: openssl-1.1.1d/test/dtlsv1listentest.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)sanitytest.o: openssl-1.1.1d/test/sanitytest.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)asn1_internal_test.o: openssl-1.1.1d/test/asn1_internal_test.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)tls13ccstest.o: openssl-1.1.1d/test/tls13ccstest.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)verify_extra_test.o: openssl-1.1.1d/test/verify_extra_test.c openssl-1.1.1d/test/testutil.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cmsapitest.o: openssl-1.1.1d/test/cmsapitest.c openssl-1.1.1d/test/testutil.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)time_offset_test.o: openssl-1.1.1d/test/time_offset_test.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)hmactest.o: openssl-1.1.1d/test/hmactest.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)drbg_cavs_data.o: openssl-1.1.1d/test/drbg_cavs_data.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)pemtest.o: openssl-1.1.1d/test/pemtest.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)s3_msg.o: openssl-1.1.1d/ssl/s3_msg.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)t1_enc.o: openssl-1.1.1d/ssl/t1_enc.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ssl_stat.o: openssl-1.1.1d/ssl/ssl_stat.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)d1_srtp.o: openssl-1.1.1d/ssl/d1_srtp.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ssl_txt.o: openssl-1.1.1d/ssl/ssl_txt.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ssl_mcnf.o: openssl-1.1.1d/ssl/ssl_mcnf.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bio_ssl.o: openssl-1.1.1d/ssl/bio_ssl.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ssl_asn1.o: openssl-1.1.1d/ssl/ssl_asn1.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)t1_lib.o: openssl-1.1.1d/ssl/t1_lib.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)rec_layer_d1.o: openssl-1.1.1d/ssl/record/rec_layer_d1.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)dtls1_bitmap.o: openssl-1.1.1d/ssl/record/dtls1_bitmap.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ssl3_record_tls13.o: openssl-1.1.1d/ssl/record/ssl3_record_tls13.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)rec_layer_s3.o: openssl-1.1.1d/ssl/record/rec_layer_s3.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ssl3_buffer.o: openssl-1.1.1d/ssl/record/ssl3_buffer.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ssl3_record.o: openssl-1.1.1d/ssl/record/ssl3_record.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)statem_lib.o: openssl-1.1.1d/ssl/statem/statem_lib.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)statem.o: openssl-1.1.1d/ssl/statem/statem.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)statem_srvr.o: openssl-1.1.1d/ssl/statem/statem_srvr.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)statem_clnt.o: openssl-1.1.1d/ssl/statem/statem_clnt.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)extensions_srvr.o: openssl-1.1.1d/ssl/statem/extensions_srvr.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)statem_dtls.o: openssl-1.1.1d/ssl/statem/statem_dtls.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)extensions_cust.o: openssl-1.1.1d/ssl/statem/extensions_cust.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)extensions_clnt.o: openssl-1.1.1d/ssl/statem/extensions_clnt.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)extensions.o: openssl-1.1.1d/ssl/statem/extensions.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ssl_ciph.o: openssl-1.1.1d/ssl/ssl_ciph.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)s3_enc.o: openssl-1.1.1d/ssl/s3_enc.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)tls_srp.o: openssl-1.1.1d/ssl/tls_srp.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ssl_lib.o: openssl-1.1.1d/ssl/ssl_lib.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ssl_err.o: openssl-1.1.1d/ssl/ssl_err.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ssl_rsa.o: openssl-1.1.1d/ssl/ssl_rsa.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)packet.o: openssl-1.1.1d/ssl/packet.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)s3_cbc.o: openssl-1.1.1d/ssl/s3_cbc.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)pqueue.o: openssl-1.1.1d/ssl/pqueue.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ssl_utst.o: openssl-1.1.1d/ssl/ssl_utst.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ssl_conf.o: openssl-1.1.1d/ssl/ssl_conf.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)s3_lib.o: openssl-1.1.1d/ssl/s3_lib.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)methods.o: openssl-1.1.1d/ssl/methods.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)tls13_enc.o: openssl-1.1.1d/ssl/tls13_enc.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)d1_msg.o: openssl-1.1.1d/ssl/d1_msg.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ssl_sess.o: openssl-1.1.1d/ssl/ssl_sess.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ssl_cert.o: openssl-1.1.1d/ssl/ssl_cert.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)d1_lib.o: openssl-1.1.1d/ssl/d1_lib.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ssl_init.o: openssl-1.1.1d/ssl/ssl_init.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)t1_trce.o: openssl-1.1.1d/ssl/t1_trce.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)mem_clr.o: openssl-1.1.1d/crypto/mem_clr.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)rc5_ecb.o: openssl-1.1.1d/crypto/rc5/rc5_ecb.c openssl-1.1.1d/crypto/rc5/rc5_locl.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)rc5_skey.o: openssl-1.1.1d/crypto/rc5/rc5_skey.c openssl-1.1.1d/crypto/rc5/rc5_locl.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)rc5cfb64.o: openssl-1.1.1d/crypto/rc5/rc5cfb64.c openssl-1.1.1d/crypto/rc5/rc5_locl.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)rc5ofb64.o: openssl-1.1.1d/crypto/rc5/rc5ofb64.c openssl-1.1.1d/crypto/rc5/rc5_locl.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)rc5_enc.o: openssl-1.1.1d/crypto/rc5/rc5_enc.c openssl-1.1.1d/crypto/rc5/rc5_locl.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)o_time.o: openssl-1.1.1d/crypto/o_time.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)sparcv9cap.o: openssl-1.1.1d/crypto/sparcv9cap.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)md2_one.o: openssl-1.1.1d/crypto/md2/md2_one.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)md2_dgst.o: openssl-1.1.1d/crypto/md2/md2_dgst.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)eng_openssl.o: openssl-1.1.1d/crypto/engine/eng_openssl.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)eng_cnf.o: openssl-1.1.1d/crypto/engine/eng_cnf.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)eng_init.o: openssl-1.1.1d/crypto/engine/eng_init.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)eng_err.o: openssl-1.1.1d/crypto/engine/eng_err.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)tb_dh.o: openssl-1.1.1d/crypto/engine/tb_dh.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)eng_fat.o: openssl-1.1.1d/crypto/engine/eng_fat.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)eng_dyn.o: openssl-1.1.1d/crypto/engine/eng_dyn.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)tb_asnmth.o: openssl-1.1.1d/crypto/engine/tb_asnmth.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)tb_cipher.o: openssl-1.1.1d/crypto/engine/tb_cipher.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)tb_eckey.o: openssl-1.1.1d/crypto/engine/tb_eckey.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)eng_rdrand.o: openssl-1.1.1d/crypto/engine/eng_rdrand.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)tb_rsa.o: openssl-1.1.1d/crypto/engine/tb_rsa.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)tb_rand.o: openssl-1.1.1d/crypto/engine/tb_rand.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)tb_pkmeth.o: openssl-1.1.1d/crypto/engine/tb_pkmeth.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)eng_devcrypto.o: openssl-1.1.1d/crypto/engine/eng_devcrypto.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)tb_dsa.o: openssl-1.1.1d/crypto/engine/tb_dsa.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)eng_pkey.o: openssl-1.1.1d/crypto/engine/eng_pkey.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)eng_ctrl.o: openssl-1.1.1d/crypto/engine/eng_ctrl.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)eng_table.o: openssl-1.1.1d/crypto/engine/eng_table.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)eng_list.o: openssl-1.1.1d/crypto/engine/eng_list.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)eng_lib.o: openssl-1.1.1d/crypto/engine/eng_lib.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)tb_digest.o: openssl-1.1.1d/crypto/engine/tb_digest.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)eng_all.o: openssl-1.1.1d/crypto/engine/eng_all.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)sha256.o: openssl-1.1.1d/crypto/sha/sha256.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)sha1dgst.o: openssl-1.1.1d/crypto/sha/sha1dgst.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)sha1_one.o: openssl-1.1.1d/crypto/sha/sha1_one.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)sha512.o: openssl-1.1.1d/crypto/sha/sha512.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)keccak1600.o: openssl-1.1.1d/crypto/sha/keccak1600.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)pem_pkey.o: openssl-1.1.1d/crypto/pem/pem_pkey.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)pem_err.o: openssl-1.1.1d/crypto/pem/pem_err.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)pem_lib.o: openssl-1.1.1d/crypto/pem/pem_lib.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)pem_oth.o: openssl-1.1.1d/crypto/pem/pem_oth.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)pem_x509.o: openssl-1.1.1d/crypto/pem/pem_x509.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)pvkfmt.o: openssl-1.1.1d/crypto/pem/pvkfmt.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)pem_info.o: openssl-1.1.1d/crypto/pem/pem_info.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)pem_sign.o: openssl-1.1.1d/crypto/pem/pem_sign.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)pem_all.o: openssl-1.1.1d/crypto/pem/pem_all.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)pem_pk8.o: openssl-1.1.1d/crypto/pem/pem_pk8.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)pem_xaux.o: openssl-1.1.1d/crypto/pem/pem_xaux.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cm_pmeth.o: openssl-1.1.1d/crypto/cmac/cm_pmeth.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cmac.o: openssl-1.1.1d/crypto/cmac/cmac.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cm_ameth.o: openssl-1.1.1d/crypto/cmac/cm_ameth.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cms_smime.o: openssl-1.1.1d/crypto/cms/cms_smime.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cms_dd.o: openssl-1.1.1d/crypto/cms/cms_dd.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cms_env.o: openssl-1.1.1d/crypto/cms/cms_env.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cms_cd.o: openssl-1.1.1d/crypto/cms/cms_cd.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cms_lib.o: openssl-1.1.1d/crypto/cms/cms_lib.c openssl-1.1.1d/crypto/cms/cms_lcl.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cms_asn1.o: openssl-1.1.1d/crypto/cms/cms_asn1.c openssl-1.1.1d/crypto/cms/cms_lcl.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cms_att.o: openssl-1.1.1d/crypto/cms/cms_att.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cms_err.o: openssl-1.1.1d/crypto/cms/cms_err.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cms_ess.o: openssl-1.1.1d/crypto/cms/cms_ess.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cms_kari.o: openssl-1.1.1d/crypto/cms/cms_kari.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cms_sd.o: openssl-1.1.1d/crypto/cms/cms_sd.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cms_enc.o: openssl-1.1.1d/crypto/cms/cms_enc.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cms_io.o: openssl-1.1.1d/crypto/cms/cms_io.c openssl-1.1.1d/crypto/cms/cms_lcl.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cms_pwri.o: openssl-1.1.1d/crypto/cms/cms_pwri.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)aria.o: openssl-1.1.1d/crypto/aria/aria.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)aes_core.o: openssl-1.1.1d/crypto/aes/aes_core.c openssl-1.1.1d/crypto/aes/aes_locl.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)aes_wrap.o: openssl-1.1.1d/crypto/aes/aes_wrap.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)aes_ecb.o: openssl-1.1.1d/crypto/aes/aes_ecb.c openssl-1.1.1d/crypto/aes/aes_locl.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)aes_cfb.o: openssl-1.1.1d/crypto/aes/aes_cfb.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)aes_ofb.o: openssl-1.1.1d/crypto/aes/aes_ofb.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)aes_ige.o: openssl-1.1.1d/crypto/aes/aes_ige.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)aes_cbc.o: openssl-1.1.1d/crypto/aes/aes_cbc.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)aes_misc.o: openssl-1.1.1d/crypto/aes/aes_misc.c openssl-1.1.1d/crypto/aes/aes_locl.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)aes_x86core.o: openssl-1.1.1d/crypto/aes/aes_x86core.c openssl-1.1.1d/crypto/aes/aes_locl.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)dllmain.o: openssl-1.1.1d/crypto/dllmain.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)uid.o: openssl-1.1.1d/crypto/uid.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cversion.o: openssl-1.1.1d/crypto/cversion.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)md5_one.o: openssl-1.1.1d/crypto/md5/md5_one.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)md5_dgst.o: openssl-1.1.1d/crypto/md5/md5_dgst.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)conf_api.o: openssl-1.1.1d/crypto/conf/conf_api.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)conf_def.o: openssl-1.1.1d/crypto/conf/conf_def.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)conf_sap.o: openssl-1.1.1d/crypto/conf/conf_sap.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)conf_mall.o: openssl-1.1.1d/crypto/conf/conf_mall.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)conf_mod.o: openssl-1.1.1d/crypto/conf/conf_mod.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)conf_lib.o: openssl-1.1.1d/crypto/conf/conf_lib.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)conf_ssl.o: openssl-1.1.1d/crypto/conf/conf_ssl.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)conf_err.o: openssl-1.1.1d/crypto/conf/conf_err.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buf_err.o: openssl-1.1.1d/crypto/buffer/buf_err.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer.o: openssl-1.1.1d/crypto/buffer/buffer.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)rc2ofb64.o: openssl-1.1.1d/crypto/rc2/rc2ofb64.c openssl-1.1.1d/crypto/rc2/rc2_locl.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)rc2_ecb.o: openssl-1.1.1d/crypto/rc2/rc2_ecb.c openssl-1.1.1d/crypto/rc2/rc2_locl.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)rc2_cbc.o: openssl-1.1.1d/crypto/rc2/rc2_cbc.c openssl-1.1.1d/crypto/rc2/rc2_locl.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)rc2cfb64.o: openssl-1.1.1d/crypto/rc2/rc2cfb64.c openssl-1.1.1d/crypto/rc2/rc2_locl.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)rc2_skey.o: openssl-1.1.1d/crypto/rc2/rc2_skey.c openssl-1.1.1d/crypto/rc2/rc2_locl.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)sm2_pmeth.o: openssl-1.1.1d/crypto/sm2/sm2_pmeth.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)sm2_err.o: openssl-1.1.1d/crypto/sm2/sm2_err.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)sm2_crypt.o: openssl-1.1.1d/crypto/sm2/sm2_crypt.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)sm2_sign.o: openssl-1.1.1d/crypto/sm2/sm2_sign.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)mem.o: openssl-1.1.1d/crypto/mem.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)async.o: openssl-1.1.1d/crypto/async/async.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)async_null.o: openssl-1.1.1d/crypto/async/arch/async_null.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)async_posix.o: openssl-1.1.1d/crypto/async/arch/async_posix.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)async_win.o: openssl-1.1.1d/crypto/async/arch/async_win.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)async_wait.o: openssl-1.1.1d/crypto/async/async_wait.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)async_err.o: openssl-1.1.1d/crypto/async/async_err.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)o_names.o: openssl-1.1.1d/crypto/objects/o_names.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)obj_xref.o: openssl-1.1.1d/crypto/objects/obj_xref.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)obj_err.o: openssl-1.1.1d/crypto/objects/obj_err.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)obj_dat.o: openssl-1.1.1d/crypto/objects/obj_dat.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)obj_lib.o: openssl-1.1.1d/crypto/objects/obj_lib.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)sm4.o: openssl-1.1.1d/crypto/sm4/sm4.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cbc128.o: openssl-1.1.1d/crypto/modes/cbc128.c openssl-1.1.1d/crypto/modes/modes_lcl.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cfb128.o: openssl-1.1.1d/crypto/modes/cfb128.c openssl-1.1.1d/crypto/modes/modes_lcl.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)wrap128.o: openssl-1.1.1d/crypto/modes/wrap128.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ocb128.o: openssl-1.1.1d/crypto/modes/ocb128.c openssl-1.1.1d/crypto/modes/modes_lcl.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ctr128.o: openssl-1.1.1d/crypto/modes/ctr128.c openssl-1.1.1d/crypto/modes/modes_lcl.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ofb128.o: openssl-1.1.1d/crypto/modes/ofb128.c openssl-1.1.1d/crypto/modes/modes_lcl.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ccm128.o: openssl-1.1.1d/crypto/modes/ccm128.c openssl-1.1.1d/crypto/modes/modes_lcl.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)xts128.o: openssl-1.1.1d/crypto/modes/xts128.c openssl-1.1.1d/crypto/modes/modes_lcl.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cts128.o: openssl-1.1.1d/crypto/modes/cts128.c openssl-1.1.1d/crypto/modes/modes_lcl.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)gcm128.o: openssl-1.1.1d/crypto/modes/gcm128.c openssl-1.1.1d/crypto/modes/modes_lcl.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)hm_ameth.o: openssl-1.1.1d/crypto/hmac/hm_ameth.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)hm_pmeth.o: openssl-1.1.1d/crypto/hmac/hm_pmeth.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)hmac.o: openssl-1.1.1d/crypto/hmac/hmac.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)init.o: openssl-1.1.1d/crypto/init.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ct_vfy.o: openssl-1.1.1d/crypto/ct/ct_vfy.c openssl-1.1.1d/crypto/ct/ct_locl.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ct_err.o: openssl-1.1.1d/crypto/ct/ct_err.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ct_sct_ctx.o: openssl-1.1.1d/crypto/ct/ct_sct_ctx.c openssl-1.1.1d/crypto/ct/ct_locl.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ct_sct.o: openssl-1.1.1d/crypto/ct/ct_sct.c openssl-1.1.1d/crypto/ct/ct_locl.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ct_x509v3.o: openssl-1.1.1d/crypto/ct/ct_x509v3.c openssl-1.1.1d/crypto/ct/ct_locl.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ct_prn.o: openssl-1.1.1d/crypto/ct/ct_prn.c openssl-1.1.1d/crypto/ct/ct_locl.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ct_log.o: openssl-1.1.1d/crypto/ct/ct_log.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ct_b64.o: openssl-1.1.1d/crypto/ct/ct_b64.c openssl-1.1.1d/crypto/ct/ct_locl.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ct_policy.o: openssl-1.1.1d/crypto/ct/ct_policy.c openssl-1.1.1d/crypto/ct/ct_locl.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ct_oct.o: openssl-1.1.1d/crypto/ct/ct_oct.c openssl-1.1.1d/crypto/ct/ct_locl.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)e_camellia.o: openssl-1.1.1d/crypto/evp/e_camellia.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)e_idea.o: openssl-1.1.1d/crypto/evp/e_idea.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)m_md4.o: openssl-1.1.1d/crypto/evp/m_md4.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)names.o: openssl-1.1.1d/crypto/evp/names.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)evp_err.o: openssl-1.1.1d/crypto/evp/evp_err.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)m_sha1.o: openssl-1.1.1d/crypto/evp/m_sha1.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bio_md.o: openssl-1.1.1d/crypto/evp/bio_md.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)m_ripemd.o: openssl-1.1.1d/crypto/evp/m_ripemd.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)evp_lib.o: openssl-1.1.1d/crypto/evp/evp_lib.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bio_enc.o: openssl-1.1.1d/crypto/evp/bio_enc.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)pmeth_gn.o: openssl-1.1.1d/crypto/evp/pmeth_gn.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)c_alld.o: openssl-1.1.1d/crypto/evp/c_alld.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)m_sha3.o: openssl-1.1.1d/crypto/evp/m_sha3.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)e_rc2.o: openssl-1.1.1d/crypto/evp/e_rc2.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bio_b64.o: openssl-1.1.1d/crypto/evp/bio_b64.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)p_enc.o: openssl-1.1.1d/crypto/evp/p_enc.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)e_null.o: openssl-1.1.1d/crypto/evp/e_null.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)e_aes_cbc_hmac_sha256.o: openssl-1.1.1d/crypto/evp/e_aes_cbc_hmac_sha256.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)e_rc4_hmac_md5.o: openssl-1.1.1d/crypto/evp/e_rc4_hmac_md5.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)p_open.o: openssl-1.1.1d/crypto/evp/p_open.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)p_lib.o: openssl-1.1.1d/crypto/evp/p_lib.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)e_bf.o: openssl-1.1.1d/crypto/evp/e_bf.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)e_sm4.o: openssl-1.1.1d/crypto/evp/e_sm4.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)pmeth_lib.o: openssl-1.1.1d/crypto/evp/pmeth_lib.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)encode.o: openssl-1.1.1d/crypto/evp/encode.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)p5_crpt2.o: openssl-1.1.1d/crypto/evp/p5_crpt2.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)e_seed.o: openssl-1.1.1d/crypto/evp/e_seed.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)e_aes_cbc_hmac_sha1.o: openssl-1.1.1d/crypto/evp/e_aes_cbc_hmac_sha1.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)evp_pkey.o: openssl-1.1.1d/crypto/evp/evp_pkey.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)e_des3.o: openssl-1.1.1d/crypto/evp/e_des3.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)e_rc5.o: openssl-1.1.1d/crypto/evp/e_rc5.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)m_md5.o: openssl-1.1.1d/crypto/evp/m_md5.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)p_dec.o: openssl-1.1.1d/crypto/evp/p_dec.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)m_md2.o: openssl-1.1.1d/crypto/evp/m_md2.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)e_aria.o: openssl-1.1.1d/crypto/evp/e_aria.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bio_ok.o: openssl-1.1.1d/crypto/evp/bio_ok.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)p5_crpt.o: openssl-1.1.1d/crypto/evp/p5_crpt.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)p_seal.o: openssl-1.1.1d/crypto/evp/p_seal.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)evp_pbe.o: openssl-1.1.1d/crypto/evp/evp_pbe.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)e_rc4.o: openssl-1.1.1d/crypto/evp/e_rc4.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)p_verify.o: openssl-1.1.1d/crypto/evp/p_verify.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)evp_enc.o: openssl-1.1.1d/crypto/evp/evp_enc.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cmeth_lib.o: openssl-1.1.1d/crypto/evp/cmeth_lib.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)digest.o: openssl-1.1.1d/crypto/evp/digest.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)pbe_scrypt.o: openssl-1.1.1d/crypto/evp/pbe_scrypt.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)e_chacha20_poly1305.o: openssl-1.1.1d/crypto/evp/e_chacha20_poly1305.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)e_des.o: openssl-1.1.1d/crypto/evp/e_des.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)m_mdc2.o: openssl-1.1.1d/crypto/evp/m_mdc2.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)evp_cnf.o: openssl-1.1.1d/crypto/evp/evp_cnf.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)e_old.o: openssl-1.1.1d/crypto/evp/e_old.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)c_allc.o: openssl-1.1.1d/crypto/evp/c_allc.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)m_md5_sha1.o: openssl-1.1.1d/crypto/evp/m_md5_sha1.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)evp_key.o: openssl-1.1.1d/crypto/evp/evp_key.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)m_wp.o: openssl-1.1.1d/crypto/evp/m_wp.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)pmeth_fn.o: openssl-1.1.1d/crypto/evp/pmeth_fn.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)m_sigver.o: openssl-1.1.1d/crypto/evp/m_sigver.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)e_cast.o: openssl-1.1.1d/crypto/evp/e_cast.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)e_aes.o: openssl-1.1.1d/crypto/evp/e_aes.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)p_sign.o: openssl-1.1.1d/crypto/evp/p_sign.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)e_xcbc_d.o: openssl-1.1.1d/crypto/evp/e_xcbc_d.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)m_null.o: openssl-1.1.1d/crypto/evp/m_null.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)seed_cbc.o: openssl-1.1.1d/crypto/seed/seed_cbc.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)seed_cfb.o: openssl-1.1.1d/crypto/seed/seed_cfb.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)seed_ofb.o: openssl-1.1.1d/crypto/seed/seed_ofb.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)seed_ecb.o: openssl-1.1.1d/crypto/seed/seed_ecb.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)seed.o: openssl-1.1.1d/crypto/seed/seed.c openssl-1.1.1d/crypto/seed/seed_locl.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ts_rsp_print.o: openssl-1.1.1d/crypto/ts/ts_rsp_print.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ts_err.o: openssl-1.1.1d/crypto/ts/ts_err.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ts_asn1.o: openssl-1.1.1d/crypto/ts/ts_asn1.c openssl-1.1.1d/crypto/ts/ts_lcl.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ts_req_print.o: openssl-1.1.1d/crypto/ts/ts_req_print.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ts_rsp_verify.o: openssl-1.1.1d/crypto/ts/ts_rsp_verify.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ts_req_utils.o: openssl-1.1.1d/crypto/ts/ts_req_utils.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ts_rsp_sign.o: openssl-1.1.1d/crypto/ts/ts_rsp_sign.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ts_verify_ctx.o: openssl-1.1.1d/crypto/ts/ts_verify_ctx.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ts_lib.o: openssl-1.1.1d/crypto/ts/ts_lib.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ts_rsp_utils.o: openssl-1.1.1d/crypto/ts/ts_rsp_utils.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ts_conf.o: openssl-1.1.1d/crypto/ts/ts_conf.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)dso_err.o: openssl-1.1.1d/crypto/dso/dso_err.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)dso_dl.o: openssl-1.1.1d/crypto/dso/dso_dl.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)dso_win32.o: openssl-1.1.1d/crypto/dso/dso_win32.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)dso_dlfcn.o: openssl-1.1.1d/crypto/dso/dso_dlfcn.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)dso_openssl.o: openssl-1.1.1d/crypto/dso/dso_openssl.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)dso_vms.o: openssl-1.1.1d/crypto/dso/dso_vms.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)dso_lib.o: openssl-1.1.1d/crypto/dso/dso_lib.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)rmd_dgst.o: openssl-1.1.1d/crypto/ripemd/rmd_dgst.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)rmd_one.o: openssl-1.1.1d/crypto/ripemd/rmd_one.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)threads_pthread.o: openssl-1.1.1d/crypto/threads_pthread.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bf_skey.o: openssl-1.1.1d/crypto/bf/bf_skey.c openssl-1.1.1d/crypto/bf/bf_locl.h openssl-1.1.1d/crypto/bf/bf_pi.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bf_cfb64.o: openssl-1.1.1d/crypto/bf/bf_cfb64.c openssl-1.1.1d/crypto/bf/bf_locl.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bf_enc.o: openssl-1.1.1d/crypto/bf/bf_enc.c openssl-1.1.1d/crypto/bf/bf_locl.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bf_ofb64.o: openssl-1.1.1d/crypto/bf/bf_ofb64.c openssl-1.1.1d/crypto/bf/bf_locl.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bf_ecb.o: openssl-1.1.1d/crypto/bf/bf_ecb.c openssl-1.1.1d/crypto/bf/bf_locl.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)i_ecb.o: openssl-1.1.1d/crypto/idea/i_ecb.c openssl-1.1.1d/crypto/idea/idea_lcl.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)i_ofb64.o: openssl-1.1.1d/crypto/idea/i_ofb64.c openssl-1.1.1d/crypto/idea/idea_lcl.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)i_cbc.o: openssl-1.1.1d/crypto/idea/i_cbc.c openssl-1.1.1d/crypto/idea/idea_lcl.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)i_skey.o: openssl-1.1.1d/crypto/idea/i_skey.c openssl-1.1.1d/crypto/idea/idea_lcl.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)i_cfb64.o: openssl-1.1.1d/crypto/idea/i_cfb64.c openssl-1.1.1d/crypto/idea/idea_lcl.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)txt_db.o: openssl-1.1.1d/crypto/txt_db/txt_db.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ameth_lib.o: openssl-1.1.1d/crypto/asn1/ameth_lib.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)a_utctm.o: openssl-1.1.1d/crypto/asn1/a_utctm.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)p5_pbe.o: openssl-1.1.1d/crypto/asn1/p5_pbe.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)asn1_gen.o: openssl-1.1.1d/crypto/asn1/asn1_gen.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)asn_mstbl.o: openssl-1.1.1d/crypto/asn1/asn_mstbl.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)asn1_par.o: openssl-1.1.1d/crypto/asn1/asn1_par.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bio_asn1.o: openssl-1.1.1d/crypto/asn1/bio_asn1.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)a_strnid.o: openssl-1.1.1d/crypto/asn1/a_strnid.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)i2d_pu.o: openssl-1.1.1d/crypto/asn1/i2d_pu.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)t_pkey.o: openssl-1.1.1d/crypto/asn1/t_pkey.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)d2i_pr.o: openssl-1.1.1d/crypto/asn1/d2i_pr.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)a_gentm.o: openssl-1.1.1d/crypto/asn1/a_gentm.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)a_verify.o: openssl-1.1.1d/crypto/asn1/a_verify.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)a_strex.o: openssl-1.1.1d/crypto/asn1/a_strex.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)tasn_prn.o: openssl-1.1.1d/crypto/asn1/tasn_prn.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)tasn_fre.o: openssl-1.1.1d/crypto/asn1/tasn_fre.c openssl-1.1.1d/crypto/asn1/asn1_locl.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)f_string.o: openssl-1.1.1d/crypto/asn1/f_string.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)a_bitstr.o: openssl-1.1.1d/crypto/asn1/a_bitstr.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)x_sig.o: openssl-1.1.1d/crypto/asn1/x_sig.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)x_long.o: openssl-1.1.1d/crypto/asn1/x_long.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)a_print.o: openssl-1.1.1d/crypto/asn1/a_print.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)x_spki.o: openssl-1.1.1d/crypto/asn1/x_spki.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)asn1_err.o: openssl-1.1.1d/crypto/asn1/asn1_err.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)a_int.o: openssl-1.1.1d/crypto/asn1/a_int.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)asn_mime.o: openssl-1.1.1d/crypto/asn1/asn_mime.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)a_i2d_fp.o: openssl-1.1.1d/crypto/asn1/a_i2d_fp.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)p5_pbev2.o: openssl-1.1.1d/crypto/asn1/p5_pbev2.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bio_ndef.o: openssl-1.1.1d/crypto/asn1/bio_ndef.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)a_type.o: openssl-1.1.1d/crypto/asn1/a_type.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)n_pkey.o: openssl-1.1.1d/crypto/asn1/n_pkey.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)x_int64.o: openssl-1.1.1d/crypto/asn1/x_int64.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)a_octet.o: openssl-1.1.1d/crypto/asn1/a_octet.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)p5_scrypt.o: openssl-1.1.1d/crypto/asn1/p5_scrypt.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)nsseq.o: openssl-1.1.1d/crypto/asn1/nsseq.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)a_time.o: openssl-1.1.1d/crypto/asn1/a_time.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)asn1_lib.o: openssl-1.1.1d/crypto/asn1/asn1_lib.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)x_algor.o: openssl-1.1.1d/crypto/asn1/x_algor.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)i2d_pr.o: openssl-1.1.1d/crypto/asn1/i2d_pr.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)f_int.o: openssl-1.1.1d/crypto/asn1/f_int.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)x_bignum.o: openssl-1.1.1d/crypto/asn1/x_bignum.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)a_sign.o: openssl-1.1.1d/crypto/asn1/a_sign.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)asn_moid.o: openssl-1.1.1d/crypto/asn1/asn_moid.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)a_d2i_fp.o: openssl-1.1.1d/crypto/asn1/a_d2i_fp.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)tasn_new.o: openssl-1.1.1d/crypto/asn1/tasn_new.c openssl-1.1.1d/crypto/asn1/asn1_locl.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)a_object.o: openssl-1.1.1d/crypto/asn1/a_object.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)d2i_pu.o: openssl-1.1.1d/crypto/asn1/d2i_pu.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)t_bitst.o: openssl-1.1.1d/crypto/asn1/t_bitst.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)evp_asn1.o: openssl-1.1.1d/crypto/asn1/evp_asn1.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)a_utf8.o: openssl-1.1.1d/crypto/asn1/a_utf8.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)tasn_scn.o: openssl-1.1.1d/crypto/asn1/tasn_scn.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)a_dup.o: openssl-1.1.1d/crypto/asn1/a_dup.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)p8_pkey.o: openssl-1.1.1d/crypto/asn1/p8_pkey.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)asn1_item_list.o: openssl-1.1.1d/crypto/asn1/asn1_item_list.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)tasn_utl.o: openssl-1.1.1d/crypto/asn1/tasn_utl.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)tasn_dec.o: openssl-1.1.1d/crypto/asn1/tasn_dec.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)t_spki.o: openssl-1.1.1d/crypto/asn1/t_spki.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)a_digest.o: openssl-1.1.1d/crypto/asn1/a_digest.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)a_mbstr.o: openssl-1.1.1d/crypto/asn1/a_mbstr.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)tasn_typ.o: openssl-1.1.1d/crypto/asn1/tasn_typ.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)asn_pack.o: openssl-1.1.1d/crypto/asn1/asn_pack.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)tasn_enc.o: openssl-1.1.1d/crypto/asn1/tasn_enc.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)x_info.o: openssl-1.1.1d/crypto/asn1/x_info.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)x_val.o: openssl-1.1.1d/crypto/asn1/x_val.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)x_pkey.o: openssl-1.1.1d/crypto/asn1/x_pkey.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)siphash_pmeth.o: openssl-1.1.1d/crypto/siphash/siphash_pmeth.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)siphash_ameth.o: openssl-1.1.1d/crypto/siphash/siphash_ameth.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)siphash.o: openssl-1.1.1d/crypto/siphash/siphash.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)store_register.o: openssl-1.1.1d/crypto/store/store_register.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)store_lib.o: openssl-1.1.1d/crypto/store/store_lib.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)store_err.o: openssl-1.1.1d/crypto/store/store_err.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)loader_file.o: openssl-1.1.1d/crypto/store/loader_file.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)store_strings.o: openssl-1.1.1d/crypto/store/store_strings.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)store_init.o: openssl-1.1.1d/crypto/store/store_init.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)dh_rfc7919.o: openssl-1.1.1d/crypto/dh/dh_rfc7919.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)dh_kdf.o: openssl-1.1.1d/crypto/dh/dh_kdf.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)dh_lib.o: openssl-1.1.1d/crypto/dh/dh_lib.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)dh_rfc5114.o: openssl-1.1.1d/crypto/dh/dh_rfc5114.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)dh_prn.o: openssl-1.1.1d/crypto/dh/dh_prn.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)dh_pmeth.o: openssl-1.1.1d/crypto/dh/dh_pmeth.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)dh_asn1.o: openssl-1.1.1d/crypto/dh/dh_asn1.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)dh_err.o: openssl-1.1.1d/crypto/dh/dh_err.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)dh_meth.o: openssl-1.1.1d/crypto/dh/dh_meth.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)dh_depr.o: openssl-1.1.1d/crypto/dh/dh_depr.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)dh_check.o: openssl-1.1.1d/crypto/dh/dh_check.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)dh_ameth.o: openssl-1.1.1d/crypto/dh/dh_ameth.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)dh_gen.o: openssl-1.1.1d/crypto/dh/dh_gen.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)dh_key.o: openssl-1.1.1d/crypto/dh/dh_key.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ebcdic.o: openssl-1.1.1d/crypto/ebcdic.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)mdc2dgst.o: openssl-1.1.1d/crypto/mdc2/mdc2dgst.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)mdc2_one.o: openssl-1.1.1d/crypto/mdc2/mdc2_one.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cmll_ofb.o: openssl-1.1.1d/crypto/camellia/cmll_ofb.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cmll_cbc.o: openssl-1.1.1d/crypto/camellia/cmll_cbc.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cmll_misc.o: openssl-1.1.1d/crypto/camellia/cmll_misc.c openssl-1.1.1d/crypto/camellia/cmll_locl.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)camellia.o: openssl-1.1.1d/crypto/camellia/camellia.c openssl-1.1.1d/crypto/camellia/cmll_locl.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cmll_cfb.o: openssl-1.1.1d/crypto/camellia/cmll_cfb.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cmll_ctr.o: openssl-1.1.1d/crypto/camellia/cmll_ctr.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cmll_ecb.o: openssl-1.1.1d/crypto/camellia/cmll_ecb.c openssl-1.1.1d/crypto/camellia/cmll_locl.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)LPdir_wince.o: openssl-1.1.1d/crypto/LPdir_wince.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)o_init.o: openssl-1.1.1d/crypto/o_init.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)m_sm3.o: openssl-1.1.1d/crypto/sm3/m_sm3.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)sm3.o: openssl-1.1.1d/crypto/sm3/sm3.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)chacha_enc.o: openssl-1.1.1d/crypto/chacha/chacha_enc.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)c_ecb.o: openssl-1.1.1d/crypto/cast/c_ecb.c openssl-1.1.1d/crypto/cast/cast_lcl.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)c_ofb64.o: openssl-1.1.1d/crypto/cast/c_ofb64.c openssl-1.1.1d/crypto/cast/cast_lcl.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)c_enc.o: openssl-1.1.1d/crypto/cast/c_enc.c openssl-1.1.1d/crypto/cast/cast_lcl.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)c_skey.o: openssl-1.1.1d/crypto/cast/c_skey.c openssl-1.1.1d/crypto/cast/cast_lcl.h openssl-1.1.1d/crypto/cast/cast_s.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)c_cfb64.o: openssl-1.1.1d/crypto/cast/c_cfb64.c openssl-1.1.1d/crypto/cast/cast_lcl.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)LPdir_unix.o: openssl-1.1.1d/crypto/LPdir_unix.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)s390xcap.o: openssl-1.1.1d/crypto/s390xcap.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)LPdir_vms.o: openssl-1.1.1d/crypto/LPdir_vms.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)wp_block.o: openssl-1.1.1d/crypto/whrlpool/wp_block.c openssl-1.1.1d/crypto/whrlpool/wp_locl.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)wp_dgst.o: openssl-1.1.1d/crypto/whrlpool/wp_dgst.c openssl-1.1.1d/crypto/whrlpool/wp_locl.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)mem_sec.o: openssl-1.1.1d/crypto/mem_sec.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)pcbc_enc.o: openssl-1.1.1d/crypto/des/pcbc_enc.c openssl-1.1.1d/crypto/des/des_locl.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ecb3_enc.o: openssl-1.1.1d/crypto/des/ecb3_enc.c openssl-1.1.1d/crypto/des/des_locl.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)str2key.o: openssl-1.1.1d/crypto/des/str2key.c openssl-1.1.1d/crypto/des/des_locl.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)xcbc_enc.o: openssl-1.1.1d/crypto/des/xcbc_enc.c openssl-1.1.1d/crypto/des/des_locl.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)fcrypt.o: openssl-1.1.1d/crypto/des/fcrypt.c openssl-1.1.1d/crypto/des/des_locl.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ncbc_enc.o: openssl-1.1.1d/crypto/des/ncbc_enc.c openssl-1.1.1d/crypto/des/des_locl.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)des_enc.o: openssl-1.1.1d/crypto/des/des_enc.c openssl-1.1.1d/crypto/des/des_locl.h openssl-1.1.1d/crypto/des/spr.h openssl-1.1.1d/crypto/des/ncbc_enc.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ofb64ede.o: openssl-1.1.1d/crypto/des/ofb64ede.c openssl-1.1.1d/crypto/des/des_locl.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)qud_cksm.o: openssl-1.1.1d/crypto/des/qud_cksm.c openssl-1.1.1d/crypto/des/des_locl.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cfb_enc.o: openssl-1.1.1d/crypto/des/cfb_enc.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)rand_key.o: openssl-1.1.1d/crypto/des/rand_key.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cbc_cksm.o: openssl-1.1.1d/crypto/des/cbc_cksm.c openssl-1.1.1d/crypto/des/des_locl.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)fcrypt_b.o: openssl-1.1.1d/crypto/des/fcrypt_b.c openssl-1.1.1d/crypto/des/des_locl.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ecb_enc.o: openssl-1.1.1d/crypto/des/ecb_enc.c openssl-1.1.1d/crypto/des/des_locl.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cfb64ede.o: openssl-1.1.1d/crypto/des/cfb64ede.c openssl-1.1.1d/crypto/des/des_locl.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)set_key.o: openssl-1.1.1d/crypto/des/set_key.c openssl-1.1.1d/crypto/des/des_locl.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cfb64enc.o: openssl-1.1.1d/crypto/des/cfb64enc.c openssl-1.1.1d/crypto/des/des_locl.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ofb_enc.o: openssl-1.1.1d/crypto/des/ofb_enc.c openssl-1.1.1d/crypto/des/des_locl.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ofb64enc.o: openssl-1.1.1d/crypto/des/ofb64enc.c openssl-1.1.1d/crypto/des/des_locl.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cbc_enc.o: openssl-1.1.1d/crypto/des/cbc_enc.c openssl-1.1.1d/crypto/des/ncbc_enc.c openssl-1.1.1d/crypto/des/des_locl.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ui_err.o: openssl-1.1.1d/crypto/ui/ui_err.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ui_openssl.o: openssl-1.1.1d/crypto/ui/ui_openssl.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ui_util.o: openssl-1.1.1d/crypto/ui/ui_util.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ui_null.o: openssl-1.1.1d/crypto/ui/ui_null.c openssl-1.1.1d/crypto/ui/ui_locl.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ui_lib.o: openssl-1.1.1d/crypto/ui/ui_lib.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)mem_dbg.o: openssl-1.1.1d/crypto/mem_dbg.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)md4_dgst.o: openssl-1.1.1d/crypto/md4/md4_dgst.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)md4_one.o: openssl-1.1.1d/crypto/md4/md4_one.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cryptlib.o: openssl-1.1.1d/crypto/cryptlib.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)lhash.o: openssl-1.1.1d/crypto/lhash/lhash.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)lh_stats.o: openssl-1.1.1d/crypto/lhash/lh_stats.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)o_fopen.o: openssl-1.1.1d/crypto/o_fopen.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ocsp_srv.o: openssl-1.1.1d/crypto/ocsp/ocsp_srv.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ocsp_prn.o: openssl-1.1.1d/crypto/ocsp/ocsp_prn.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ocsp_vfy.o: openssl-1.1.1d/crypto/ocsp/ocsp_vfy.c openssl-1.1.1d/crypto/ocsp/ocsp_lcl.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ocsp_lib.o: openssl-1.1.1d/crypto/ocsp/ocsp_lib.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ocsp_asn.o: openssl-1.1.1d/crypto/ocsp/ocsp_asn.c openssl-1.1.1d/crypto/ocsp/ocsp_lcl.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ocsp_ht.o: openssl-1.1.1d/crypto/ocsp/ocsp_ht.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)v3_ocsp.o: openssl-1.1.1d/crypto/ocsp/v3_ocsp.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ocsp_err.o: openssl-1.1.1d/crypto/ocsp/ocsp_err.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ocsp_ext.o: openssl-1.1.1d/crypto/ocsp/ocsp_ext.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ocsp_cl.o: openssl-1.1.1d/crypto/ocsp/ocsp_cl.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)srp_lib.o: openssl-1.1.1d/crypto/srp/srp_lib.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)srp_vfy.o: openssl-1.1.1d/crypto/srp/srp_vfy.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)comp_err.o: openssl-1.1.1d/crypto/comp/comp_err.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)comp_lib.o: openssl-1.1.1d/crypto/comp/comp_lib.c openssl-1.1.1d/crypto/comp/comp_lcl.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)c_zlib.o: openssl-1.1.1d/crypto/comp/c_zlib.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)pk7_lib.o: openssl-1.1.1d/crypto/pkcs7/pk7_lib.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)pk7_asn1.o: openssl-1.1.1d/crypto/pkcs7/pk7_asn1.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)pk7_doit.o: openssl-1.1.1d/crypto/pkcs7/pk7_doit.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bio_pk7.o: openssl-1.1.1d/crypto/pkcs7/bio_pk7.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)pkcs7err.o: openssl-1.1.1d/crypto/pkcs7/pkcs7err.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)pk7_mime.o: openssl-1.1.1d/crypto/pkcs7/pk7_mime.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)pk7_attr.o: openssl-1.1.1d/crypto/pkcs7/pk7_attr.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)pk7_smime.o: openssl-1.1.1d/crypto/pkcs7/pk7_smime.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)hkdf.o: openssl-1.1.1d/crypto/kdf/hkdf.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)kdf_err.o: openssl-1.1.1d/crypto/kdf/kdf_err.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)scrypt.o: openssl-1.1.1d/crypto/kdf/scrypt.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)tls1_prf.o: openssl-1.1.1d/crypto/kdf/tls1_prf.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)dsa_depr.o: openssl-1.1.1d/crypto/dsa/dsa_depr.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)dsa_key.o: openssl-1.1.1d/crypto/dsa/dsa_key.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)dsa_ossl.o: openssl-1.1.1d/crypto/dsa/dsa_ossl.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)dsa_lib.o: openssl-1.1.1d/crypto/dsa/dsa_lib.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)dsa_ameth.o: openssl-1.1.1d/crypto/dsa/dsa_ameth.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)dsa_prn.o: openssl-1.1.1d/crypto/dsa/dsa_prn.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)dsa_vrf.o: openssl-1.1.1d/crypto/dsa/dsa_vrf.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)dsa_pmeth.o: openssl-1.1.1d/crypto/dsa/dsa_pmeth.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)dsa_asn1.o: openssl-1.1.1d/crypto/dsa/dsa_asn1.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)dsa_err.o: openssl-1.1.1d/crypto/dsa/dsa_err.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)dsa_sign.o: openssl-1.1.1d/crypto/dsa/dsa_sign.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)dsa_gen.o: openssl-1.1.1d/crypto/dsa/dsa_gen.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)dsa_meth.o: openssl-1.1.1d/crypto/dsa/dsa_meth.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ctype.o: openssl-1.1.1d/crypto/ctype.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)o_fips.o: openssl-1.1.1d/crypto/o_fips.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ppccap.o: openssl-1.1.1d/crypto/ppccap.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)rc4_skey.o: openssl-1.1.1d/crypto/rc4/rc4_skey.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)rc4_enc.o: openssl-1.1.1d/crypto/rc4/rc4_enc.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)threads_none.o: openssl-1.1.1d/crypto/threads_none.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)o_str.o: openssl-1.1.1d/crypto/o_str.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)err_all.o: openssl-1.1.1d/crypto/err/err_all.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)err.o: openssl-1.1.1d/crypto/err/err.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)err_prn.o: openssl-1.1.1d/crypto/err/err_prn.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)armcap.o: openssl-1.1.1d/crypto/armcap.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bf_lbuf.o: openssl-1.1.1d/crypto/bio/bf_lbuf.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)b_sock2.o: openssl-1.1.1d/crypto/bio/b_sock2.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bss_null.o: openssl-1.1.1d/crypto/bio/bss_null.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bss_acpt.o: openssl-1.1.1d/crypto/bio/bss_acpt.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bss_mem.o: openssl-1.1.1d/crypto/bio/bss_mem.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bf_buff.o: openssl-1.1.1d/crypto/bio/bf_buff.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)b_sock.o: openssl-1.1.1d/crypto/bio/b_sock.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)b_print.o: openssl-1.1.1d/crypto/bio/b_print.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bss_dgram.o: openssl-1.1.1d/crypto/bio/bss_dgram.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bio_err.o: openssl-1.1.1d/crypto/bio/bio_err.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)b_addr.o: openssl-1.1.1d/crypto/bio/b_addr.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bss_sock.o: openssl-1.1.1d/crypto/bio/bss_sock.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bio_cb.o: openssl-1.1.1d/crypto/bio/bio_cb.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bss_bio.o: openssl-1.1.1d/crypto/bio/bss_bio.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bio_lib.o: openssl-1.1.1d/crypto/bio/bio_lib.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bf_null.o: openssl-1.1.1d/crypto/bio/bf_null.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bss_log.o: openssl-1.1.1d/crypto/bio/bss_log.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bss_conn.o: openssl-1.1.1d/crypto/bio/bss_conn.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bss_fd.o: openssl-1.1.1d/crypto/bio/bss_fd.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bf_nbio.o: openssl-1.1.1d/crypto/bio/bf_nbio.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)b_dump.o: openssl-1.1.1d/crypto/bio/b_dump.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bio_meth.o: openssl-1.1.1d/crypto/bio/bio_meth.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bss_file.o: openssl-1.1.1d/crypto/bio/bss_file.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)stack.o: openssl-1.1.1d/crypto/stack/stack.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cpt_err.o: openssl-1.1.1d/crypto/cpt_err.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)getenv.o: openssl-1.1.1d/crypto/getenv.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)m_blake2s.o: openssl-1.1.1d/crypto/blake2/m_blake2s.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)m_blake2b.o: openssl-1.1.1d/crypto/blake2/m_blake2b.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)blake2s.o: openssl-1.1.1d/crypto/blake2/blake2s.c openssl-1.1.1d/crypto/blake2/blake2_locl.h openssl-1.1.1d/crypto/blake2/blake2_impl.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)blake2b.o: openssl-1.1.1d/crypto/blake2/blake2b.c openssl-1.1.1d/crypto/blake2/blake2_locl.h openssl-1.1.1d/crypto/blake2/blake2_impl.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)threads_win.o: openssl-1.1.1d/crypto/threads_win.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)rsa_none.o: openssl-1.1.1d/crypto/rsa/rsa_none.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)rsa_x931g.o: openssl-1.1.1d/crypto/rsa/rsa_x931g.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)rsa_prn.o: openssl-1.1.1d/crypto/rsa/rsa_prn.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)rsa_meth.o: openssl-1.1.1d/crypto/rsa/rsa_meth.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)rsa_pmeth.o: openssl-1.1.1d/crypto/rsa/rsa_pmeth.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)rsa_oaep.o: openssl-1.1.1d/crypto/rsa/rsa_oaep.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)rsa_crpt.o: openssl-1.1.1d/crypto/rsa/rsa_crpt.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)rsa_mp.o: openssl-1.1.1d/crypto/rsa/rsa_mp.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)rsa_saos.o: openssl-1.1.1d/crypto/rsa/rsa_saos.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)rsa_ossl.o: openssl-1.1.1d/crypto/rsa/rsa_ossl.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)rsa_asn1.o: openssl-1.1.1d/crypto/rsa/rsa_asn1.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)rsa_sign.o: openssl-1.1.1d/crypto/rsa/rsa_sign.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)rsa_ssl.o: openssl-1.1.1d/crypto/rsa/rsa_ssl.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)rsa_gen.o: openssl-1.1.1d/crypto/rsa/rsa_gen.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)rsa_ameth.o: openssl-1.1.1d/crypto/rsa/rsa_ameth.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)rsa_err.o: openssl-1.1.1d/crypto/rsa/rsa_err.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)rsa_pk1.o: openssl-1.1.1d/crypto/rsa/rsa_pk1.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)rsa_lib.o: openssl-1.1.1d/crypto/rsa/rsa_lib.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)rsa_x931.o: openssl-1.1.1d/crypto/rsa/rsa_x931.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)rsa_pss.o: openssl-1.1.1d/crypto/rsa/rsa_pss.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)rsa_depr.o: openssl-1.1.1d/crypto/rsa/rsa_depr.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)rsa_chk.o: openssl-1.1.1d/crypto/rsa/rsa_chk.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)v3_conf.o: openssl-1.1.1d/crypto/x509v3/v3_conf.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)pcy_lib.o: openssl-1.1.1d/crypto/x509v3/pcy_lib.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)v3err.o: openssl-1.1.1d/crypto/x509v3/v3err.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)v3_akey.o: openssl-1.1.1d/crypto/x509v3/v3_akey.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)v3_pcia.o: openssl-1.1.1d/crypto/x509v3/v3_pcia.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)v3_sxnet.o: openssl-1.1.1d/crypto/x509v3/v3_sxnet.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)v3_ncons.o: openssl-1.1.1d/crypto/x509v3/v3_ncons.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)v3_alt.o: openssl-1.1.1d/crypto/x509v3/v3_alt.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)pcy_data.o: openssl-1.1.1d/crypto/x509v3/pcy_data.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)v3_pci.o: openssl-1.1.1d/crypto/x509v3/v3_pci.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)v3_utl.o: openssl-1.1.1d/crypto/x509v3/v3_utl.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)v3_enum.o: openssl-1.1.1d/crypto/x509v3/v3_enum.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)v3_bcons.o: openssl-1.1.1d/crypto/x509v3/v3_bcons.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)v3_akeya.o: openssl-1.1.1d/crypto/x509v3/v3_akeya.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)v3_genn.o: openssl-1.1.1d/crypto/x509v3/v3_genn.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)v3_skey.o: openssl-1.1.1d/crypto/x509v3/v3_skey.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)v3_extku.o: openssl-1.1.1d/crypto/x509v3/v3_extku.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)v3_pmaps.o: openssl-1.1.1d/crypto/x509v3/v3_pmaps.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)v3_tlsf.o: openssl-1.1.1d/crypto/x509v3/v3_tlsf.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)v3_prn.o: openssl-1.1.1d/crypto/x509v3/v3_prn.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)pcy_tree.o: openssl-1.1.1d/crypto/x509v3/pcy_tree.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)v3_addr.o: openssl-1.1.1d/crypto/x509v3/v3_addr.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)v3_bitst.o: openssl-1.1.1d/crypto/x509v3/v3_bitst.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)v3_int.o: openssl-1.1.1d/crypto/x509v3/v3_int.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)pcy_cache.o: openssl-1.1.1d/crypto/x509v3/pcy_cache.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)v3_info.o: openssl-1.1.1d/crypto/x509v3/v3_info.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)v3_admis.o: openssl-1.1.1d/crypto/x509v3/v3_admis.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)v3_ia5.o: openssl-1.1.1d/crypto/x509v3/v3_ia5.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)pcy_map.o: openssl-1.1.1d/crypto/x509v3/pcy_map.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)v3_purp.o: openssl-1.1.1d/crypto/x509v3/v3_purp.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)v3_cpols.o: openssl-1.1.1d/crypto/x509v3/v3_cpols.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)v3_pcons.o: openssl-1.1.1d/crypto/x509v3/v3_pcons.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)v3_asid.o: openssl-1.1.1d/crypto/x509v3/v3_asid.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)v3_lib.o: openssl-1.1.1d/crypto/x509v3/v3_lib.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)pcy_node.o: openssl-1.1.1d/crypto/x509v3/pcy_node.c openssl-1.1.1d/crypto/x509v3/pcy_int.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)v3_pku.o: openssl-1.1.1d/crypto/x509v3/v3_pku.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)v3_crld.o: openssl-1.1.1d/crypto/x509v3/v3_crld.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ex_data.o: openssl-1.1.1d/crypto/ex_data.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)LPdir_win32.o: openssl-1.1.1d/crypto/LPdir_win32.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)o_dir.o: openssl-1.1.1d/crypto/o_dir.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)LPdir_win.o: openssl-1.1.1d/crypto/LPdir_win.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)LPdir_nyi.o: openssl-1.1.1d/crypto/LPdir_nyi.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ecp_smpl.o: openssl-1.1.1d/crypto/ec/ecp_smpl.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ecdh_kdf.o: openssl-1.1.1d/crypto/ec/ecdh_kdf.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ecx_meth.o: openssl-1.1.1d/crypto/ec/ecx_meth.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ec_mult.o: openssl-1.1.1d/crypto/ec/ec_mult.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ecp_oct.o: openssl-1.1.1d/crypto/ec/ecp_oct.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)curve25519.o: openssl-1.1.1d/crypto/ec/curve25519.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)curve448.o: openssl-1.1.1d/crypto/ec/curve448/curve448.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)f_generic.o: openssl-1.1.1d/crypto/ec/curve448/f_generic.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)f_impl.o: openssl-1.1.1d/crypto/ec/curve448/arch_32/f_impl.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)scalar.o: openssl-1.1.1d/crypto/ec/curve448/scalar.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)eddsa.o: openssl-1.1.1d/crypto/ec/curve448/eddsa.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)curve448_tables.o: openssl-1.1.1d/crypto/ec/curve448/curve448_tables.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ecdsa_ossl.o: openssl-1.1.1d/crypto/ec/ecdsa_ossl.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ecp_mont.o: openssl-1.1.1d/crypto/ec/ecp_mont.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ec_curve.o: openssl-1.1.1d/crypto/ec/ec_curve.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ec_key.o: openssl-1.1.1d/crypto/ec/ec_key.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ec_asn1.o: openssl-1.1.1d/crypto/ec/ec_asn1.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ec_lib.o: openssl-1.1.1d/crypto/ec/ec_lib.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ecdsa_vrf.o: openssl-1.1.1d/crypto/ec/ecdsa_vrf.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ecp_nistputil.o: openssl-1.1.1d/crypto/ec/ecp_nistputil.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ec_oct.o: openssl-1.1.1d/crypto/ec/ec_oct.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ecdsa_sign.o: openssl-1.1.1d/crypto/ec/ecdsa_sign.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)eck_prn.o: openssl-1.1.1d/crypto/ec/eck_prn.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ecp_nistp256.o: openssl-1.1.1d/crypto/ec/ecp_nistp256.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ec_kmeth.o: openssl-1.1.1d/crypto/ec/ec_kmeth.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ec2_oct.o: openssl-1.1.1d/crypto/ec/ec2_oct.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ecp_nistz256_table.o: openssl-1.1.1d/crypto/ec/ecp_nistz256_table.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ecp_nistp224.o: openssl-1.1.1d/crypto/ec/ecp_nistp224.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ec_cvt.o: openssl-1.1.1d/crypto/ec/ec_cvt.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ec2_smpl.o: openssl-1.1.1d/crypto/ec/ec2_smpl.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ecdh_ossl.o: openssl-1.1.1d/crypto/ec/ecdh_ossl.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ec_check.o: openssl-1.1.1d/crypto/ec/ec_check.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ecp_nistp521.o: openssl-1.1.1d/crypto/ec/ecp_nistp521.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ec_err.o: openssl-1.1.1d/crypto/ec/ec_err.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ec_print.o: openssl-1.1.1d/crypto/ec/ec_print.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ecp_nistz256.o: openssl-1.1.1d/crypto/ec/ecp_nistz256.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ec_pmeth.o: openssl-1.1.1d/crypto/ec/ec_pmeth.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ec_ameth.o: openssl-1.1.1d/crypto/ec/ec_ameth.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ecp_nist.o: openssl-1.1.1d/crypto/ec/ecp_nist.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)x509_def.o: openssl-1.1.1d/crypto/x509/x509_def.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)x509_r2x.o: openssl-1.1.1d/crypto/x509/x509_r2x.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)t_req.o: openssl-1.1.1d/crypto/x509/t_req.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)x509spki.o: openssl-1.1.1d/crypto/x509/x509spki.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)t_crl.o: openssl-1.1.1d/crypto/x509/t_crl.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)x_attrib.o: openssl-1.1.1d/crypto/x509/x_attrib.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)x_all.o: openssl-1.1.1d/crypto/x509/x_all.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)x509_vfy.o: openssl-1.1.1d/crypto/x509/x509_vfy.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)x509_set.o: openssl-1.1.1d/crypto/x509/x509_set.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)x509type.o: openssl-1.1.1d/crypto/x509/x509type.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)x_pubkey.o: openssl-1.1.1d/crypto/x509/x_pubkey.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)x509_trs.o: openssl-1.1.1d/crypto/x509/x509_trs.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)x509rset.o: openssl-1.1.1d/crypto/x509/x509rset.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)x509name.o: openssl-1.1.1d/crypto/x509/x509name.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)x_req.o: openssl-1.1.1d/crypto/x509/x_req.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)x509_meth.o: openssl-1.1.1d/crypto/x509/x509_meth.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)t_x509.o: openssl-1.1.1d/crypto/x509/t_x509.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)x_x509a.o: openssl-1.1.1d/crypto/x509/x_x509a.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)x509_obj.o: openssl-1.1.1d/crypto/x509/x509_obj.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)x509_ext.o: openssl-1.1.1d/crypto/x509/x509_ext.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)x509_lu.o: openssl-1.1.1d/crypto/x509/x509_lu.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)x_x509.o: openssl-1.1.1d/crypto/x509/x_x509.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)x_crl.o: openssl-1.1.1d/crypto/x509/x_crl.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)x_name.o: openssl-1.1.1d/crypto/x509/x_name.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)x509_req.o: openssl-1.1.1d/crypto/x509/x509_req.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)x509_att.o: openssl-1.1.1d/crypto/x509/x509_att.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)x509_v3.o: openssl-1.1.1d/crypto/x509/x509_v3.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)x509cset.o: openssl-1.1.1d/crypto/x509/x509cset.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)x509_err.o: openssl-1.1.1d/crypto/x509/x509_err.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)by_file.o: openssl-1.1.1d/crypto/x509/by_file.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)x_exten.o: openssl-1.1.1d/crypto/x509/x_exten.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)x509_txt.o: openssl-1.1.1d/crypto/x509/x509_txt.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)x509_cmp.o: openssl-1.1.1d/crypto/x509/x509_cmp.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)x509_vpm.o: openssl-1.1.1d/crypto/x509/x509_vpm.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)x509_d2.o: openssl-1.1.1d/crypto/x509/x509_d2.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)by_dir.o: openssl-1.1.1d/crypto/x509/by_dir.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)p12_attr.o: openssl-1.1.1d/crypto/pkcs12/p12_attr.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)p12_mutl.o: openssl-1.1.1d/crypto/pkcs12/p12_mutl.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)p12_add.o: openssl-1.1.1d/crypto/pkcs12/p12_add.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)p12_kiss.o: openssl-1.1.1d/crypto/pkcs12/p12_kiss.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)p12_asn.o: openssl-1.1.1d/crypto/pkcs12/p12_asn.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)p12_crt.o: openssl-1.1.1d/crypto/pkcs12/p12_crt.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)pk12err.o: openssl-1.1.1d/crypto/pkcs12/pk12err.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)p12_sbag.o: openssl-1.1.1d/crypto/pkcs12/p12_sbag.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)p12_init.o: openssl-1.1.1d/crypto/pkcs12/p12_init.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)p12_key.o: openssl-1.1.1d/crypto/pkcs12/p12_key.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)p12_decr.o: openssl-1.1.1d/crypto/pkcs12/p12_decr.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)p12_p8d.o: openssl-1.1.1d/crypto/pkcs12/p12_p8d.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)p12_crpt.o: openssl-1.1.1d/crypto/pkcs12/p12_crpt.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)p12_npas.o: openssl-1.1.1d/crypto/pkcs12/p12_npas.c openssl-1.1.1d/crypto/pkcs12/p12_lcl.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)p12_p8e.o: openssl-1.1.1d/crypto/pkcs12/p12_p8e.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)p12_utl.o: openssl-1.1.1d/crypto/pkcs12/p12_utl.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bn_lib.o: openssl-1.1.1d/crypto/bn/bn_lib.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bn_gf2m.o: openssl-1.1.1d/crypto/bn/bn_gf2m.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bn_mpi.o: openssl-1.1.1d/crypto/bn/bn_mpi.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bn_word.o: openssl-1.1.1d/crypto/bn/bn_word.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bn_blind.o: openssl-1.1.1d/crypto/bn/bn_blind.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bn_print.o: openssl-1.1.1d/crypto/bn/bn_print.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bn_rand.o: openssl-1.1.1d/crypto/bn/bn_rand.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bn_intern.o: openssl-1.1.1d/crypto/bn/bn_intern.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bn_add.o: openssl-1.1.1d/crypto/bn/bn_add.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bn_sqrt.o: openssl-1.1.1d/crypto/bn/bn_sqrt.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bn_exp2.o: openssl-1.1.1d/crypto/bn/bn_exp2.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bn_recp.o: openssl-1.1.1d/crypto/bn/bn_recp.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bn_div.o: openssl-1.1.1d/crypto/bn/bn_div.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)rsaz_exp.o: openssl-1.1.1d/crypto/bn/rsaz_exp.c openssl-1.1.1d/crypto/bn/rsaz_exp.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bn_sqr.o: openssl-1.1.1d/crypto/bn/bn_sqr.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bn_mont.o: openssl-1.1.1d/crypto/bn/bn_mont.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bn_kron.o: openssl-1.1.1d/crypto/bn/bn_kron.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)x86_64-gcc.o: openssl-1.1.1d/crypto/bn/asm/x86_64-gcc.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bn_nist.o: openssl-1.1.1d/crypto/bn/bn_nist.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bn_x931p.o: openssl-1.1.1d/crypto/bn/bn_x931p.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bn_exp.o: openssl-1.1.1d/crypto/bn/bn_exp.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bn_asm.o: openssl-1.1.1d/crypto/bn/bn_asm.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bn_prime.o: openssl-1.1.1d/crypto/bn/bn_prime.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bn_ctx.o: openssl-1.1.1d/crypto/bn/bn_ctx.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bn_gcd.o: openssl-1.1.1d/crypto/bn/bn_gcd.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bn_err.o: openssl-1.1.1d/crypto/bn/bn_err.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bn_srp.o: openssl-1.1.1d/crypto/bn/bn_srp.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bn_depr.o: openssl-1.1.1d/crypto/bn/bn_depr.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bn_dh.o: openssl-1.1.1d/crypto/bn/bn_dh.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bn_shift.o: openssl-1.1.1d/crypto/bn/bn_shift.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bn_mul.o: openssl-1.1.1d/crypto/bn/bn_mul.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bn_mod.o: openssl-1.1.1d/crypto/bn/bn_mod.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bn_const.o: openssl-1.1.1d/crypto/bn/bn_const.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)poly1305_ieee754.o: openssl-1.1.1d/crypto/poly1305/poly1305_ieee754.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)poly1305_ameth.o: openssl-1.1.1d/crypto/poly1305/poly1305_ameth.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)poly1305_pmeth.o: openssl-1.1.1d/crypto/poly1305/poly1305_pmeth.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)poly1305.o: openssl-1.1.1d/crypto/poly1305/poly1305.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)poly1305_base2_44.o: openssl-1.1.1d/crypto/poly1305/poly1305_base2_44.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)randfile.o: openssl-1.1.1d/crypto/rand/randfile.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)rand_err.o: openssl-1.1.1d/crypto/rand/rand_err.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)rand_win.o: openssl-1.1.1d/crypto/rand/rand_win.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)drbg_lib.o: openssl-1.1.1d/crypto/rand/drbg_lib.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)rand_egd.o: openssl-1.1.1d/crypto/rand/rand_egd.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)drbg_ctr.o: openssl-1.1.1d/crypto/rand/drbg_ctr.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)rand_vms.o: openssl-1.1.1d/crypto/rand/rand_vms.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)rand_lib.o: openssl-1.1.1d/crypto/rand/rand_lib.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)rand_unix.o: openssl-1.1.1d/crypto/rand/rand_unix.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cms_ver.o: openssl-1.1.1d/demos/cms/cms_ver.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cms_dec.o: openssl-1.1.1d/demos/cms/cms_dec.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cms_uncomp.o: openssl-1.1.1d/demos/cms/cms_uncomp.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cms_sign2.o: openssl-1.1.1d/demos/cms/cms_sign2.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cms_sign.o: openssl-1.1.1d/demos/cms/cms_sign.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cms_denc.o: openssl-1.1.1d/demos/cms/cms_denc.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cms_comp.o: openssl-1.1.1d/demos/cms/cms_comp.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cms_ddec.o: openssl-1.1.1d/demos/cms/cms_ddec.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cms_enc.o: openssl-1.1.1d/demos/cms/cms_enc.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)aesccm.o: openssl-1.1.1d/demos/evp/aesccm.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)aesgcm.o: openssl-1.1.1d/demos/evp/aesgcm.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)smdec.o: openssl-1.1.1d/demos/smime/smdec.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)smsign.o: openssl-1.1.1d/demos/smime/smsign.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)smsign2.o: openssl-1.1.1d/demos/smime/smsign2.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)smver.o: openssl-1.1.1d/demos/smime/smver.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)smenc.o: openssl-1.1.1d/demos/smime/smenc.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)client-conf.o: openssl-1.1.1d/demos/bio/client-conf.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)saccept.o: openssl-1.1.1d/demos/bio/saccept.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)sconnect.o: openssl-1.1.1d/demos/bio/sconnect.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)server-conf.o: openssl-1.1.1d/demos/bio/server-conf.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)server-arg.o: openssl-1.1.1d/demos/bio/server-arg.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)server-cmod.o: openssl-1.1.1d/demos/bio/server-cmod.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)client-arg.o: openssl-1.1.1d/demos/bio/client-arg.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)pkwrite.o: openssl-1.1.1d/demos/pkcs12/pkwrite.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)pkread.o: openssl-1.1.1d/demos/pkcs12/pkread.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)e_afalg.o: openssl-1.1.1d/engines/e_afalg.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)e_dasync.o: openssl-1.1.1d/engines/e_dasync.c openssl-1.1.1d/engines/e_dasync_err.c openssl-1.1.1d/engines/e_dasync_err.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)e_afalg_err.o: openssl-1.1.1d/engines/e_afalg_err.c openssl-1.1.1d/engines/e_afalg_err.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)e_dasync_err.o: openssl-1.1.1d/engines/e_dasync_err.c openssl-1.1.1d/engines/e_dasync_err.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)e_ossltest.o: openssl-1.1.1d/engines/e_ossltest.c openssl-1.1.1d/engines/e_ossltest_err.c openssl-1.1.1d/engines/e_ossltest_err.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)e_ossltest_err.o: openssl-1.1.1d/engines/e_ossltest_err.c openssl-1.1.1d/engines/e_ossltest_err.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)e_capi_err.o: openssl-1.1.1d/engines/e_capi_err.c openssl-1.1.1d/engines/e_capi_err.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)e_capi.o: openssl-1.1.1d/engines/e_capi.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)e_padlock.o: openssl-1.1.1d/engines/e_padlock.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cc-wrap.o: cc-wrap.c lib/windoze.h lib/strlist.h lib/stralloc.h lib/typedefs.h lib/byte.h lib/uint64.h lib/buffer.h lib/strarray.h lib/array.h lib/stralloc.h lib/buffer.h lib/path.h lib/windoze.h lib/str.h lib/process.h lib/env.h lib/errmsg.h lib/byte.h lib/mmap.h lib/scan.h lib/uint32.h lib/open.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bsdiffcat.o: bsdiffcat.c lib/windoze.h lib/array.h lib/typedefs.h lib/uint64.h lib/uint64.h lib/buffer.h lib/fmt.h lib/uint32.h lib/io.h lib/windoze.h lib/iarray.h lib/taia.h lib/tai.h lib/iarray.h lib/mmap.h lib/open.h lib/byte.h lib/str.h lib/path.h lib/stralloc.h lib/str.h lib/errmsg.h lib/uint32.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)xmlpp.o: xmlpp.c lib/buffer.h lib/typedefs.h lib/hmap.h lib/stralloc.h lib/buffer.h lib/io.h lib/windoze.h lib/iarray.h lib/uint64.h lib/taia.h lib/tai.h lib/uint32.h lib/iarray.h lib/stralloc.h lib/xml.h lib/hmap.h lib/textbuf.h lib/byte.h lib/fmt.h lib/mmap.h lib/str.h lib/getopt.h lib/errmsg.h lib/scan.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ziptest.o: ziptest.c lib/unix.h lib/typedefs.h lib/windoze.h lib/getopt.h lib/readlink.h lib/buffer.h lib/io.h lib/iarray.h lib/uint64.h lib/taia.h lib/tai.h lib/uint32.h lib/iarray.h lib/str.h lib/byte.h lib/mmap.h lib/open.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)pathtool.o: pathtool.c lib/unix.h lib/typedefs.h lib/windoze.h lib/getopt.h lib/readlink.h lib/buffer.h lib/errmsg.h lib/path.h lib/stralloc.h lib/str.h lib/stralloc.h lib/strlist.h lib/byte.h lib/uint64.h lib/buffer.h lib/windoze.h lib/str.h lib/byte.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)msys-shell.o: msys-shell.c lib/windoze.h lib/strlist.h lib/stralloc.h lib/typedefs.h lib/byte.h lib/uint64.h lib/buffer.h lib/array.h lib/strarray.h lib/array.h lib/stralloc.h lib/buffer.h lib/path.h lib/windoze.h lib/str.h lib/process.h lib/env.h lib/errmsg.h lib/byte.h lib/unix.h lib/getopt.h lib/readlink.h lib/getopt.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)t.o: t.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)compiler-wrapper.o: compiler-wrapper.c lib/windoze.h lib/stralloc.h lib/typedefs.h lib/strlist.h lib/stralloc.h lib/byte.h lib/uint64.h lib/buffer.h lib/str.h lib/byte.h lib/fmt.h lib/uint32.h lib/buffer.h lib/dir.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)plsconv.o: plsconv.c lib/windoze.h lib/unix.h lib/typedefs.h lib/windoze.h lib/getopt.h lib/readlink.h lib/buffer.h lib/byte.h lib/io.h lib/iarray.h lib/uint64.h lib/taia.h lib/tai.h lib/uint32.h lib/iarray.h lib/open.h lib/playlist.h lib/stralloc.h lib/buffer.h lib/str.h lib/stralloc.h lib/fmt.h lib/scan.h lib/hmap.h lib/xml.h lib/hmap.h lib/textbuf.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
