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
$(BUILDDIR)io_finishandshutdown.o: lib/io/io_finishandshutdown.c lib/io_internal.h lib/windoze.h lib/array.h lib/typedefs.h lib/uint64.h lib/iarray.h lib/taia.h lib/tai.h lib/uint32.h lib/io.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)io_getcookie.o: lib/io/io_getcookie.c lib/windoze.h lib/io_internal.h lib/windoze.h lib/array.h lib/typedefs.h lib/uint64.h lib/iarray.h lib/taia.h lib/tai.h lib/uint32.h lib/io.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)io_eagain.o: lib/io/io_eagain.c lib/io_internal.h lib/windoze.h lib/array.h lib/typedefs.h lib/uint64.h lib/iarray.h lib/taia.h lib/tai.h lib/uint32.h lib/io.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)io_timeout.o: lib/io/io_timeout.c lib/io_internal.h lib/windoze.h lib/array.h lib/typedefs.h lib/uint64.h lib/iarray.h lib/taia.h lib/tai.h lib/uint32.h lib/io.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)io_eagain_read.o: lib/io/io_eagain_read.c lib/io_internal.h lib/windoze.h lib/array.h lib/typedefs.h lib/uint64.h lib/iarray.h lib/taia.h lib/tai.h lib/uint32.h lib/io.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)io_eagain_write.o: lib/io/io_eagain_write.c lib/io_internal.h lib/windoze.h lib/array.h lib/typedefs.h lib/uint64.h lib/iarray.h lib/taia.h lib/tai.h lib/uint32.h lib/io.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)io_dup.o: lib/io/io_dup.c lib/windoze.h lib/io.h lib/windoze.h lib/iarray.h lib/uint64.h lib/typedefs.h lib/taia.h lib/tai.h lib/uint32.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)iopause.o: lib/io/iopause.c lib/socket_internal.h lib/socket.h lib/typedefs.h lib/uint16.h lib/uint32.h lib/select.h lib/io_internal.h lib/windoze.h lib/array.h lib/uint64.h lib/iarray.h lib/taia.h lib/tai.h lib/io.h lib/iopause.h lib/taia.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)io_timeouted.o: lib/io/io_timeouted.c lib/io_internal.h lib/windoze.h lib/array.h lib/typedefs.h lib/uint64.h lib/iarray.h lib/taia.h lib/tai.h lib/uint32.h lib/io.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)io_wait.o: lib/io/io_wait.c lib/io_internal.h lib/windoze.h lib/array.h lib/typedefs.h lib/uint64.h lib/iarray.h lib/taia.h lib/tai.h lib/uint32.h lib/io.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)io_passfd.o: lib/io/io_passfd.c lib/windoze.h lib/socket_internal.h lib/socket.h lib/typedefs.h lib/uint16.h lib/uint32.h lib/io_internal.h lib/windoze.h lib/array.h lib/uint64.h lib/iarray.h lib/taia.h lib/tai.h lib/io.h lib/typedefs.h lib/byte.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)io_setcookie.o: lib/io/io_setcookie.c lib/windoze.h lib/io_internal.h lib/windoze.h lib/array.h lib/typedefs.h lib/uint64.h lib/iarray.h lib/taia.h lib/tai.h lib/uint32.h lib/io.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)io_block.o: lib/io/io_block.c lib/windoze.h lib/socket_internal.h lib/socket.h lib/typedefs.h lib/uint16.h lib/uint32.h lib/io_internal.h lib/windoze.h lib/array.h lib/uint64.h lib/iarray.h lib/taia.h lib/tai.h lib/io.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)io_canread.o: lib/io/io_canread.c lib/windoze.h lib/io_internal.h lib/windoze.h lib/array.h lib/typedefs.h lib/uint64.h lib/iarray.h lib/taia.h lib/tai.h lib/uint32.h lib/io.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)io_closeonexec.o: lib/io/io_closeonexec.c lib/windoze.h lib/io_internal.h lib/windoze.h lib/array.h lib/typedefs.h lib/uint64.h lib/iarray.h lib/taia.h lib/tai.h lib/uint32.h lib/io.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)io_trywritetimeout.o: lib/io/io_trywritetimeout.c lib/socket_internal.h lib/socket.h lib/typedefs.h lib/uint16.h lib/uint32.h lib/io_internal.h lib/windoze.h lib/array.h lib/uint64.h lib/iarray.h lib/taia.h lib/tai.h lib/io.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)io_debugstring.o: lib/io/io_debugstring.c lib/windoze.h lib/socket_internal.h lib/socket.h lib/typedefs.h lib/uint16.h lib/uint32.h lib/fmt.h lib/uint64.h lib/iarray.h lib/io_internal.h lib/windoze.h lib/array.h lib/iarray.h lib/taia.h lib/tai.h lib/io.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)io_waitwrite.o: lib/io/io_waitwrite.c lib/socket_internal.h lib/socket.h lib/typedefs.h lib/uint16.h lib/uint32.h lib/io_internal.h lib/windoze.h lib/array.h lib/uint64.h lib/iarray.h lib/taia.h lib/tai.h lib/io.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)io_sendfile.o: lib/io/io_sendfile.c lib/windoze.h lib/socket_internal.h lib/socket.h lib/typedefs.h lib/uint16.h lib/uint32.h lib/io_internal.h lib/windoze.h lib/array.h lib/uint64.h lib/iarray.h lib/taia.h lib/tai.h lib/io.h lib/uint8.h lib/iob.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)io_dontwantwrite.o: lib/io/io_dontwantwrite.c lib/socket_internal.h lib/socket.h lib/typedefs.h lib/uint16.h lib/uint32.h lib/io_internal.h lib/windoze.h lib/array.h lib/uint64.h lib/iarray.h lib/taia.h lib/tai.h lib/io.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)io_fd.o: lib/io/io_fd.c lib/windoze.h lib/socket_internal.h lib/socket.h lib/typedefs.h lib/uint16.h lib/uint32.h lib/array.h lib/uint64.h lib/io_internal.h lib/windoze.h lib/array.h lib/iarray.h lib/taia.h lib/tai.h lib/io.h lib/byte.h lib/cas.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)io_waitread.o: lib/io/io_waitread.c lib/windoze.h lib/uint64.h lib/socket_internal.h lib/socket.h lib/typedefs.h lib/uint16.h lib/uint32.h lib/io_internal.h lib/windoze.h lib/array.h lib/uint64.h lib/iarray.h lib/taia.h lib/tai.h lib/io.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)io_wantwrite.o: lib/io/io_wantwrite.c lib/socket_internal.h lib/socket.h lib/typedefs.h lib/uint16.h lib/uint32.h lib/io_internal.h lib/windoze.h lib/array.h lib/uint64.h lib/iarray.h lib/taia.h lib/tai.h lib/io.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)io_tryread.o: lib/io/io_tryread.c lib/windoze.h lib/byte.h lib/typedefs.h lib/io_internal.h lib/windoze.h lib/array.h lib/uint64.h lib/iarray.h lib/taia.h lib/tai.h lib/uint32.h lib/io.h lib/socket_internal.h lib/socket.h lib/uint16.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)io_appendfile.o: lib/io/io_appendfile.c lib/windoze.h lib/io_internal.h lib/windoze.h lib/array.h lib/typedefs.h lib/uint64.h lib/iarray.h lib/taia.h lib/tai.h lib/uint32.h lib/io.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)io_sigpipe.o: lib/io/io_sigpipe.c lib/io_internal.h lib/windoze.h lib/array.h lib/typedefs.h lib/uint64.h lib/iarray.h lib/taia.h lib/tai.h lib/uint32.h lib/io.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)io_tryreadtimeout.o: lib/io/io_tryreadtimeout.c lib/windoze.h lib/socket_internal.h lib/socket.h lib/typedefs.h lib/uint16.h lib/uint32.h lib/io_internal.h lib/windoze.h lib/array.h lib/uint64.h lib/iarray.h lib/taia.h lib/tai.h lib/io.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)io_pipe.o: lib/io/io_pipe.c lib/windoze.h lib/io_internal.h lib/windoze.h lib/array.h lib/typedefs.h lib/uint64.h lib/iarray.h lib/taia.h lib/tai.h lib/uint32.h lib/io.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)io_trywrite.o: lib/io/io_trywrite.c lib/socket_internal.h lib/socket.h lib/typedefs.h lib/uint16.h lib/uint32.h lib/io_internal.h lib/windoze.h lib/array.h lib/uint64.h lib/iarray.h lib/taia.h lib/tai.h lib/io.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)io_canwrite.o: lib/io/io_canwrite.c lib/windoze.h lib/io_internal.h lib/windoze.h lib/array.h lib/typedefs.h lib/uint64.h lib/iarray.h lib/taia.h lib/tai.h lib/uint32.h lib/io.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)io_createfile.o: lib/io/io_createfile.c lib/windoze.h lib/io_internal.h lib/windoze.h lib/array.h lib/typedefs.h lib/uint64.h lib/iarray.h lib/taia.h lib/tai.h lib/uint32.h lib/io.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)io_mmapwritefile.o: lib/io/io_mmapwritefile.c lib/windoze.h lib/io_internal.h lib/windoze.h lib/array.h lib/typedefs.h lib/uint64.h lib/iarray.h lib/taia.h lib/tai.h lib/uint32.h lib/io.h lib/iob.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)io_timedout.o: lib/io/io_timedout.c lib/io_internal.h lib/windoze.h lib/array.h lib/typedefs.h lib/uint64.h lib/iarray.h lib/taia.h lib/tai.h lib/uint32.h lib/io.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)io_close.o: lib/io/io_close.c lib/windoze.h lib/io_internal.h lib/windoze.h lib/array.h lib/typedefs.h lib/uint64.h lib/iarray.h lib/taia.h lib/tai.h lib/uint32.h lib/io.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)io_waituntil.o: lib/io/io_waituntil.c lib/safemult.h lib/uint16.h lib/uint32.h lib/uint64.h lib/io.h lib/windoze.h lib/iarray.h lib/typedefs.h lib/taia.h lib/tai.h lib/taia.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)io_wantread.o: lib/io/io_wantread.c lib/socket_internal.h lib/socket.h lib/typedefs.h lib/uint16.h lib/uint32.h lib/io_internal.h lib/windoze.h lib/array.h lib/uint64.h lib/iarray.h lib/taia.h lib/tai.h lib/io.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)io_check.o: lib/io/io_check.c lib/io_internal.h lib/windoze.h lib/array.h lib/typedefs.h lib/uint64.h lib/iarray.h lib/taia.h lib/tai.h lib/uint32.h lib/io.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)io_readwritefile.o: lib/io/io_readwritefile.c lib/windoze.h lib/io_internal.h lib/windoze.h lib/array.h lib/typedefs.h lib/uint64.h lib/iarray.h lib/taia.h lib/tai.h lib/uint32.h lib/io.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)io_socketpair.o: lib/io/io_socketpair.c lib/windoze.h lib/socket_internal.h lib/socket.h lib/typedefs.h lib/uint16.h lib/uint32.h lib/io_internal.h lib/windoze.h lib/array.h lib/uint64.h lib/iarray.h lib/taia.h lib/tai.h lib/io.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)io_readfile.o: lib/io/io_readfile.c lib/windoze.h lib/io_internal.h lib/windoze.h lib/array.h lib/typedefs.h lib/uint64.h lib/iarray.h lib/taia.h lib/tai.h lib/uint32.h lib/io.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)io_dontwantread.o: lib/io/io_dontwantread.c lib/socket_internal.h lib/socket.h lib/typedefs.h lib/uint16.h lib/uint32.h lib/io_internal.h lib/windoze.h lib/array.h lib/uint64.h lib/iarray.h lib/taia.h lib/tai.h lib/io.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)io_waituntil2.o: lib/io/io_waituntil2.c lib/socket_internal.h lib/socket.h lib/typedefs.h lib/uint16.h lib/uint32.h lib/io_internal.h lib/windoze.h lib/array.h lib/uint64.h lib/iarray.h lib/taia.h lib/tai.h lib/io.h lib/buffer.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)io_nonblock.o: lib/io/io_nonblock.c lib/windoze.h lib/socket_internal.h lib/socket.h lib/typedefs.h lib/uint16.h lib/uint32.h lib/io_internal.h lib/windoze.h lib/array.h lib/uint64.h lib/iarray.h lib/taia.h lib/tai.h lib/io.h
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
$(BUILDDIR)path_collapse.o: lib/path/path_collapse.c lib/path_internal.h lib/str.h lib/typedefs.h lib/path.h lib/windoze.h lib/stralloc.h lib/strlist.h lib/byte.h lib/uint64.h lib/buffer.h lib/byte.h
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
$(BUILDDIR)strarray_appends.o: lib/strarray/strarray_appends.c lib/str.h lib/typedefs.h lib/stralloc.h lib/strarray.h lib/uint64.h lib/array.h lib/stralloc.h lib/byte.h
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
$(BUILDDIR)strarray_free.o: lib/strarray/strarray_free.c lib/strarray.h lib/uint64.h lib/array.h lib/typedefs.h lib/stralloc.h lib/byte.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)strarray_splice.o: lib/strarray/strarray_splice.c lib/strarray.h lib/uint64.h lib/array.h lib/typedefs.h lib/stralloc.h lib/byte.h lib/array.h lib/byte.h lib/safemult.h lib/uint16.h lib/uint32.h lib/uint64.h lib/str.h lib/alloc.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)strarray_to_argv.o: lib/strarray/strarray_to_argv.c lib/byte.h lib/typedefs.h lib/strarray.h lib/uint64.h lib/array.h lib/stralloc.h lib/byte.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)strarray_pushd.o: lib/strarray/strarray_pushd.c lib/str.h lib/typedefs.h lib/strarray.h lib/uint64.h lib/array.h lib/stralloc.h lib/byte.h
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
$(BUILDDIR)hmap_set.o: lib/hmap/hmap_set.c lib/hmap_internal.h lib/byte.h lib/typedefs.h lib/hmap.h lib/stralloc.h lib/buffer.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)hmap_cmp.o: lib/hmap/hmap_cmp.c lib/hmap_internal.h lib/byte.h lib/typedefs.h lib/hmap.h lib/stralloc.h lib/buffer.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)hmap_search.o: lib/hmap/hmap_search.c lib/hmap_internal.h lib/byte.h lib/typedefs.h lib/hmap.h lib/stralloc.h lib/buffer.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)binfmt_open.o: lib/binfmt/binfmt_open.c lib/binfmt.h lib/typedefs.h lib/uint8.h lib/uint32.h lib/uint64.h lib/range.h lib/mmap.h lib/byte.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)env_put.o: lib/env/env_put.c lib/windoze.h lib/typedefs.h lib/env.h lib/typedefs.h lib/byte.h lib/str.h
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
$(BUILDDIR)http_get.o: lib/http/http_get.c lib/socket_internal.h lib/socket.h lib/typedefs.h lib/uint16.h lib/uint32.h lib/socket.h lib/scan.h lib/uint64.h lib/buffer.h lib/byte.h lib/dns.h lib/iopause.h lib/io.h lib/windoze.h lib/iarray.h lib/taia.h lib/tai.h lib/stralloc.h lib/errmsg.h lib/http.h lib/buffer.h lib/ip4.h lib/uint8.h lib/io.h lib/ip4.h lib/str.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)rdir_close.o: lib/rdir/rdir_close.c lib/dir.h lib/uint64.h lib/rdir.h lib/dir.h lib/dir_internal.h lib/stralloc.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)rdir_open.o: lib/rdir/rdir_open.c lib/rdir.h lib/dir.h lib/uint64.h lib/dir_internal.h lib/stralloc.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)rdir_read.o: lib/rdir/rdir_read.c lib/rdir.h lib/dir.h lib/uint64.h lib/dir_internal.h lib/stralloc.h lib/typedefs.h lib/buffer.h lib/byte.h lib/dir_internal.h lib/stralloc.h lib/str.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)pe_offset2rva.o: lib/pe/pe_offset2rva.c lib/pe.h lib/typedefs.h lib/uint64.h lib/uint32.h lib/uint16.h lib/uint8.h lib/uint32.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)pe_header.o: lib/pe/pe_header.c lib/pe.h lib/typedefs.h lib/uint64.h lib/uint32.h lib/uint16.h lib/uint8.h
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
$(BUILDDIR)buffer_free.o: lib/buffer/buffer_free.c lib/buffer.h lib/typedefs.h
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
$(BUILDDIR)buffer_putnspace.o: lib/buffer/buffer_putnspace.c lib/buffer.h lib/typedefs.h lib/byte.h lib/str.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_write_fd.o: lib/buffer/buffer_write_fd.c lib/windoze.h lib/buffer.h lib/typedefs.h
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
$(BUILDDIR)buffer_lzma.o: lib/buffer/buffer_lzma.c lib/buffer.h lib/typedefs.h lib/uint8.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_1small.o: lib/buffer/buffer_1small.c lib/windoze.h lib/buffer.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_read_fd.o: lib/buffer/buffer_read_fd.c lib/windoze.h lib/buffer.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_putalign.o: lib/buffer/buffer_putalign.c lib/buffer.h lib/typedefs.h lib/byte.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_putsa.o: lib/buffer/buffer_putsa.c lib/buffer.h lib/typedefs.h
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
$(BUILDDIR)buffer_truncfile.o: lib/buffer/buffer_truncfile.c lib/windoze.h lib/buffer.h lib/typedefs.h lib/open.h lib/windoze.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_fromstr.o: lib/buffer/buffer_fromstr.c lib/buffer.h lib/typedefs.h lib/str.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_copy.o: lib/buffer/buffer_copy.c lib/buffer.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_skip_until.o: lib/buffer/buffer_skip_until.c lib/buffer.h lib/typedefs.h lib/byte.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_fromsa.o: lib/buffer/buffer_fromsa.c lib/buffer.h lib/typedefs.h
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
