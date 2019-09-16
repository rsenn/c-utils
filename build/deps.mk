$(BUILDDIR)reg2cmd.o: reg2cmd.c lib/windoze.h lib/io.h lib/windoze.h lib/iarray.h lib/uint64.h lib/typedefs.h lib/taia.h lib/tai.h lib/uint32.h lib/iarray.h lib/stralloc.h lib/uint64.h lib/buffer.h lib/open.h lib/fmt.h lib/byte.h lib/str.h lib/scan.h lib/path.h lib/stralloc.h lib/str.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)strarraytest.o: strarraytest.c lib/buffer.h lib/typedefs.h lib/mmap.h lib/uint64.h lib/stralloc.h lib/strarray.h lib/array.h lib/stralloc.h lib/byte.h lib/str.h lib/byte.h lib/array.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)eagle-gen-cmds.o: eagle-gen-cmds.c lib/unix.h lib/typedefs.h lib/windoze.h lib/getopt.h lib/readlink.h lib/byte.h lib/array.h lib/uint64.h lib/open.h lib/buffer.h lib/cb.h lib/cbmap.h lib/uint8.h lib/errmsg.h lib/fmt.h lib/uint32.h lib/hmap.h lib/stralloc.h lib/buffer.h lib/iterator.h lib/mmap.h lib/scan.h lib/str.h lib/stralloc.h lib/strarray.h lib/array.h lib/byte.h lib/strlist.h lib/xml.h lib/hmap.h lib/textbuf.h lib/round.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cofflist.o: cofflist.c lib/unix.h lib/typedefs.h lib/windoze.h lib/getopt.h lib/readlink.h lib/uint64.h lib/buffer.h lib/mmap.h lib/uint64.h lib/coff.h lib/uint8.h lib/uint16.h lib/uint32.h lib/range.h lib/str.h lib/stralloc.h lib/byte.h lib/fmt.h lib/errmsg.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ar-wrap.o: ar-wrap.c lib/windoze.h lib/strlist.h lib/stralloc.h lib/typedefs.h lib/byte.h lib/uint64.h lib/buffer.h lib/strarray.h lib/array.h lib/open.h lib/windoze.h lib/stralloc.h lib/buffer.h lib/path.h lib/str.h lib/process.h lib/env.h lib/errmsg.h lib/byte.h lib/mmap.h lib/scan.h lib/uint32.h lib/wait.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)pkgcfg.o: pkgcfg.c lib/unix.h lib/typedefs.h lib/windoze.h lib/getopt.h lib/readlink.h lib/algorithm.h lib/iterator.h lib/buffer.h lib/byte.h lib/cbmap.h lib/uint8.h lib/dir.h lib/uint64.h lib/env.h lib/errmsg.h lib/iterator.h lib/path.h lib/stralloc.h lib/str.h lib/slist.h lib/alloc.h lib/str.h lib/stralloc.h lib/strarray.h lib/array.h lib/byte.h lib/strlist.h lib/buffer.h lib/wordexp.h lib/io.h lib/iarray.h lib/taia.h lib/tai.h lib/uint32.h lib/iarray.h lib/ndelay.h lib/mmap.h lib/array.h lib/socket.h lib/uint16.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)io_finishandshutdown.o: lib/io/io_finishandshutdown.c lib/io_internal.h lib/windoze.h lib/array.h lib/typedefs.h lib/uint64.h lib/iarray.h lib/taia.h lib/tai.h lib/uint32.h lib/io.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)io_getcookie.o: lib/io/io_getcookie.c lib/windoze.h lib/io_internal.h lib/windoze.h lib/array.h lib/typedefs.h lib/uint64.h lib/iarray.h lib/taia.h lib/tai.h lib/uint32.h lib/io.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)io_eagain.o: lib/io/io_eagain.c lib/io_internal.h lib/windoze.h lib/array.h lib/typedefs.h lib/uint64.h lib/iarray.h lib/taia.h lib/tai.h lib/uint32.h lib/io.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)io_timeout.o: lib/io/io_timeout.c lib/io_internal.h lib/windoze.h lib/array.h lib/typedefs.h lib/uint64.h lib/iarray.h lib/taia.h lib/tai.h lib/uint32.h lib/io.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)io_eagain_read.o: lib/io/io_eagain_read.c lib/io_internal.h lib/windoze.h lib/array.h lib/typedefs.h lib/uint64.h lib/iarray.h lib/taia.h lib/tai.h lib/uint32.h lib/io.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)io_eagain_write.o: lib/io/io_eagain_write.c lib/io_internal.h lib/windoze.h lib/array.h lib/typedefs.h lib/uint64.h lib/iarray.h lib/taia.h lib/tai.h lib/uint32.h lib/io.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)io_dup.o: lib/io/io_dup.c lib/windoze.h lib/io.h lib/windoze.h lib/iarray.h lib/uint64.h lib/typedefs.h lib/taia.h lib/tai.h lib/uint32.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)iopause.o: lib/io/iopause.c lib/socket_internal.h lib/socket.h lib/typedefs.h lib/uint16.h lib/uint32.h lib/select.h lib/io_internal.h lib/windoze.h lib/array.h lib/uint64.h lib/iarray.h lib/taia.h lib/tai.h lib/io.h lib/iopause.h lib/taia.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)io_timeouted.o: lib/io/io_timeouted.c lib/io_internal.h lib/windoze.h lib/array.h lib/typedefs.h lib/uint64.h lib/iarray.h lib/taia.h lib/tai.h lib/uint32.h lib/io.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)io_wait.o: lib/io/io_wait.c lib/io_internal.h lib/windoze.h lib/array.h lib/typedefs.h lib/uint64.h lib/iarray.h lib/taia.h lib/tai.h lib/uint32.h lib/io.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)io_passfd.o: lib/io/io_passfd.c lib/windoze.h lib/io_internal.h lib/windoze.h lib/array.h lib/typedefs.h lib/uint64.h lib/iarray.h lib/taia.h lib/tai.h lib/uint32.h lib/io.h lib/typedefs.h lib/byte.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)io_setcookie.o: lib/io/io_setcookie.c lib/windoze.h lib/io_internal.h lib/windoze.h lib/array.h lib/typedefs.h lib/uint64.h lib/iarray.h lib/taia.h lib/tai.h lib/uint32.h lib/io.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)io_block.o: lib/io/io_block.c lib/windoze.h lib/socket_internal.h lib/socket.h lib/typedefs.h lib/uint16.h lib/uint32.h lib/io_internal.h lib/windoze.h lib/array.h lib/uint64.h lib/iarray.h lib/taia.h lib/tai.h lib/io.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)io_canread.o: lib/io/io_canread.c lib/windoze.h lib/io_internal.h lib/windoze.h lib/array.h lib/typedefs.h lib/uint64.h lib/iarray.h lib/taia.h lib/tai.h lib/uint32.h lib/io.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)io_closeonexec.o: lib/io/io_closeonexec.c lib/windoze.h lib/io_internal.h lib/windoze.h lib/array.h lib/typedefs.h lib/uint64.h lib/iarray.h lib/taia.h lib/tai.h lib/uint32.h lib/io.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)io_trywritetimeout.o: lib/io/io_trywritetimeout.c lib/socket_internal.h lib/socket.h lib/typedefs.h lib/uint16.h lib/uint32.h lib/io_internal.h lib/windoze.h lib/array.h lib/uint64.h lib/iarray.h lib/taia.h lib/tai.h lib/io.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)io_debugstring.o: lib/io/io_debugstring.c lib/windoze.h lib/socket_internal.h lib/socket.h lib/typedefs.h lib/uint16.h lib/uint32.h lib/fmt.h lib/uint64.h lib/iarray.h lib/io_internal.h lib/windoze.h lib/array.h lib/iarray.h lib/taia.h lib/tai.h lib/io.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)io_waitwrite.o: lib/io/io_waitwrite.c lib/socket_internal.h lib/socket.h lib/typedefs.h lib/uint16.h lib/uint32.h lib/io_internal.h lib/windoze.h lib/array.h lib/uint64.h lib/iarray.h lib/taia.h lib/tai.h lib/io.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)io_sendfile.o: lib/io/io_sendfile.c lib/windoze.h lib/socket_internal.h lib/socket.h lib/typedefs.h lib/uint16.h lib/uint32.h lib/io_internal.h lib/windoze.h lib/array.h lib/uint64.h lib/iarray.h lib/taia.h lib/tai.h lib/io.h lib/uint8.h lib/iob.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)io_dontwantwrite.o: lib/io/io_dontwantwrite.c lib/socket_internal.h lib/socket.h lib/typedefs.h lib/uint16.h lib/uint32.h lib/io_internal.h lib/windoze.h lib/array.h lib/uint64.h lib/iarray.h lib/taia.h lib/tai.h lib/io.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)io_fd.o: lib/io/io_fd.c lib/windoze.h lib/socket_internal.h lib/socket.h lib/typedefs.h lib/uint16.h lib/uint32.h lib/array.h lib/uint64.h lib/io_internal.h lib/windoze.h lib/array.h lib/iarray.h lib/taia.h lib/tai.h lib/io.h lib/byte.h lib/cas.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)io_waitread.o: lib/io/io_waitread.c lib/windoze.h lib/uint64.h lib/socket_internal.h lib/socket.h lib/typedefs.h lib/uint16.h lib/uint32.h lib/io_internal.h lib/windoze.h lib/array.h lib/uint64.h lib/iarray.h lib/taia.h lib/tai.h lib/io.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)io_wantwrite.o: lib/io/io_wantwrite.c lib/socket_internal.h lib/socket.h lib/typedefs.h lib/uint16.h lib/uint32.h lib/io_internal.h lib/windoze.h lib/array.h lib/uint64.h lib/iarray.h lib/taia.h lib/tai.h lib/io.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)io_tryread.o: lib/io/io_tryread.c lib/windoze.h lib/byte.h lib/typedefs.h lib/io_internal.h lib/windoze.h lib/array.h lib/uint64.h lib/iarray.h lib/taia.h lib/tai.h lib/uint32.h lib/io.h lib/socket_internal.h lib/socket.h lib/uint16.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)io_appendfile.o: lib/io/io_appendfile.c lib/windoze.h lib/io_internal.h lib/windoze.h lib/array.h lib/typedefs.h lib/uint64.h lib/iarray.h lib/taia.h lib/tai.h lib/uint32.h lib/io.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)io_sigpipe.o: lib/io/io_sigpipe.c lib/io_internal.h lib/windoze.h lib/array.h lib/typedefs.h lib/uint64.h lib/iarray.h lib/taia.h lib/tai.h lib/uint32.h lib/io.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)io_tryreadtimeout.o: lib/io/io_tryreadtimeout.c lib/windoze.h lib/socket_internal.h lib/socket.h lib/typedefs.h lib/uint16.h lib/uint32.h lib/io_internal.h lib/windoze.h lib/array.h lib/uint64.h lib/iarray.h lib/taia.h lib/tai.h lib/io.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)io_pipe.o: lib/io/io_pipe.c lib/windoze.h lib/io_internal.h lib/windoze.h lib/array.h lib/typedefs.h lib/uint64.h lib/iarray.h lib/taia.h lib/tai.h lib/uint32.h lib/io.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)io_trywrite.o: lib/io/io_trywrite.c lib/socket_internal.h lib/socket.h lib/typedefs.h lib/uint16.h lib/uint32.h lib/io_internal.h lib/windoze.h lib/array.h lib/uint64.h lib/iarray.h lib/taia.h lib/tai.h lib/io.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)io_canwrite.o: lib/io/io_canwrite.c lib/windoze.h lib/io_internal.h lib/windoze.h lib/array.h lib/typedefs.h lib/uint64.h lib/iarray.h lib/taia.h lib/tai.h lib/uint32.h lib/io.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)io_createfile.o: lib/io/io_createfile.c lib/windoze.h lib/io_internal.h lib/windoze.h lib/array.h lib/typedefs.h lib/uint64.h lib/iarray.h lib/taia.h lib/tai.h lib/uint32.h lib/io.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)io_mmapwritefile.o: lib/io/io_mmapwritefile.c lib/windoze.h lib/io_internal.h lib/windoze.h lib/array.h lib/typedefs.h lib/uint64.h lib/iarray.h lib/taia.h lib/tai.h lib/uint32.h lib/io.h lib/iob.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)io_timedout.o: lib/io/io_timedout.c lib/io_internal.h lib/windoze.h lib/array.h lib/typedefs.h lib/uint64.h lib/iarray.h lib/taia.h lib/tai.h lib/uint32.h lib/io.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)io_close.o: lib/io/io_close.c lib/windoze.h lib/io_internal.h lib/windoze.h lib/array.h lib/typedefs.h lib/uint64.h lib/iarray.h lib/taia.h lib/tai.h lib/uint32.h lib/io.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)io_waituntil.o: lib/io/io_waituntil.c lib/safemult.h lib/uint16.h lib/uint32.h lib/uint64.h lib/io.h lib/windoze.h lib/iarray.h lib/typedefs.h lib/taia.h lib/tai.h lib/taia.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)io_wantread.o: lib/io/io_wantread.c lib/socket_internal.h lib/socket.h lib/typedefs.h lib/uint16.h lib/uint32.h lib/io_internal.h lib/windoze.h lib/array.h lib/uint64.h lib/iarray.h lib/taia.h lib/tai.h lib/io.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)io_check.o: lib/io/io_check.c lib/io_internal.h lib/windoze.h lib/array.h lib/typedefs.h lib/uint64.h lib/iarray.h lib/taia.h lib/tai.h lib/uint32.h lib/io.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)io_readwritefile.o: lib/io/io_readwritefile.c lib/windoze.h lib/io_internal.h lib/windoze.h lib/array.h lib/typedefs.h lib/uint64.h lib/iarray.h lib/taia.h lib/tai.h lib/uint32.h lib/io.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)io_socketpair.o: lib/io/io_socketpair.c lib/windoze.h lib/socket_internal.h lib/socket.h lib/typedefs.h lib/uint16.h lib/uint32.h lib/io_internal.h lib/windoze.h lib/array.h lib/uint64.h lib/iarray.h lib/taia.h lib/tai.h lib/io.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)io_readfile.o: lib/io/io_readfile.c lib/windoze.h lib/io_internal.h lib/windoze.h lib/array.h lib/typedefs.h lib/uint64.h lib/iarray.h lib/taia.h lib/tai.h lib/uint32.h lib/io.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)io_dontwantread.o: lib/io/io_dontwantread.c lib/socket_internal.h lib/socket.h lib/typedefs.h lib/uint16.h lib/uint32.h lib/io_internal.h lib/windoze.h lib/array.h lib/uint64.h lib/iarray.h lib/taia.h lib/tai.h lib/io.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)io_waituntil2.o: lib/io/io_waituntil2.c lib/socket_internal.h lib/socket.h lib/typedefs.h lib/uint16.h lib/uint32.h lib/io_internal.h lib/windoze.h lib/array.h lib/uint64.h lib/iarray.h lib/taia.h lib/tai.h lib/io.h lib/buffer.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)io_nonblock.o: lib/io/io_nonblock.c lib/windoze.h lib/socket_internal.h lib/socket.h lib/typedefs.h lib/uint16.h lib/uint32.h lib/io_internal.h lib/windoze.h lib/array.h lib/uint64.h lib/iarray.h lib/taia.h lib/tai.h lib/io.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)path_dirname.o: lib/path/path_dirname.c lib/path_internal.h lib/str.h lib/typedefs.h lib/path.h lib/windoze.h lib/stralloc.h lib/str.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)path_canonicalize.o: lib/path/path_canonicalize.c lib/windoze.h lib/path_internal.h lib/str.h lib/typedefs.h lib/path.h lib/windoze.h lib/stralloc.h lib/readlink.h lib/buffer.h lib/byte.h lib/str.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)path_skip.o: lib/path/path_skip.c lib/path_internal.h lib/str.h lib/typedefs.h lib/path.h lib/windoze.h lib/stralloc.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)path_len_s.o: lib/path/path_len_s.c lib/path_internal.h lib/str.h lib/typedefs.h lib/path.h lib/windoze.h lib/stralloc.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)path_skip_separator.o: lib/path/path_skip_separator.c lib/path_internal.h lib/str.h lib/typedefs.h lib/path.h lib/windoze.h lib/stralloc.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)symlink.o: lib/path/symlink.c lib/byte.h lib/typedefs.h lib/windoze.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)path_len.o: lib/path/path_len.c lib/path_internal.h lib/str.h lib/typedefs.h lib/path.h lib/windoze.h lib/stralloc.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)path_getcwd.o: lib/path/path_getcwd.c lib/path_internal.h lib/str.h lib/typedefs.h lib/path.h lib/windoze.h lib/stralloc.h lib/windoze.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)path_absolute.o: lib/path/path_absolute.c lib/path_internal.h lib/str.h lib/typedefs.h lib/path.h lib/windoze.h lib/stralloc.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)path_relative.o: lib/path/path_relative.c lib/path_internal.h lib/str.h lib/typedefs.h lib/path.h lib/windoze.h lib/stralloc.h lib/stralloc.h lib/strlist.h lib/byte.h lib/uint64.h lib/buffer.h lib/windoze.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)path_gethome.o: lib/path/path_gethome.c lib/buffer.h lib/typedefs.h lib/path_internal.h lib/str.h lib/path.h lib/windoze.h lib/stralloc.h lib/scan.h lib/uint32.h lib/uint64.h lib/windoze.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)path_split.o: lib/path/path_split.c lib/path_internal.h lib/str.h lib/typedefs.h lib/path.h lib/windoze.h lib/stralloc.h lib/str.h lib/strlist.h lib/byte.h lib/uint64.h lib/buffer.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)path_exists.o: lib/path/path_exists.c lib/path_internal.h lib/str.h lib/typedefs.h lib/path.h lib/windoze.h lib/stralloc.h lib/windoze.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)path_is_absolute.o: lib/path/path_is_absolute.c lib/path_internal.h lib/str.h lib/typedefs.h lib/path.h lib/windoze.h lib/stralloc.h lib/str.h lib/windoze.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)path_collapse.o: lib/path/path_collapse.c lib/path_internal.h lib/str.h lib/typedefs.h lib/path.h lib/windoze.h lib/stralloc.h lib/strlist.h lib/byte.h lib/uint64.h lib/buffer.h lib/byte.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)path_getsep.o: lib/path/path_getsep.c lib/path_internal.h lib/str.h lib/typedefs.h lib/path.h lib/windoze.h lib/stralloc.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)path_num_sa.o: lib/path/path_num_sa.c lib/path_internal.h lib/str.h lib/typedefs.h lib/path.h lib/windoze.h lib/stralloc.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)path_basename.o: lib/path/path_basename.c lib/path_internal.h lib/str.h lib/typedefs.h lib/path.h lib/windoze.h lib/stralloc.h lib/str.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)path_num.o: lib/path/path_num.c lib/path_internal.h lib/str.h lib/typedefs.h lib/path.h lib/windoze.h lib/stralloc.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)path_fnmatch.o: lib/path/path_fnmatch.c lib/path_internal.h lib/str.h lib/typedefs.h lib/path.h lib/windoze.h lib/stralloc.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)path_skips.o: lib/path/path_skips.c lib/path_internal.h lib/str.h lib/typedefs.h lib/path.h lib/windoze.h lib/stralloc.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)path_right.o: lib/path/path_right.c lib/path_internal.h lib/str.h lib/typedefs.h lib/path.h lib/windoze.h lib/stralloc.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)path_is_separator.o: lib/path/path_is_separator.c lib/path_internal.h lib/str.h lib/typedefs.h lib/path.h lib/windoze.h lib/stralloc.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)path_find.o: lib/path/path_find.c lib/path_internal.h lib/str.h lib/typedefs.h lib/path.h lib/windoze.h lib/stralloc.h lib/dir.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)path_absolute_sa.o: lib/path/path_absolute_sa.c lib/path_internal.h lib/str.h lib/typedefs.h lib/path.h lib/windoze.h lib/stralloc.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)path_canonical.o: lib/path/path_canonical.c lib/path_internal.h lib/str.h lib/typedefs.h lib/path.h lib/windoze.h lib/stralloc.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)path_readlink.o: lib/path/path_readlink.c lib/windoze.h lib/unix.h lib/typedefs.h lib/windoze.h lib/getopt.h lib/readlink.h lib/path_internal.h lib/str.h lib/path.h lib/stralloc.h lib/readlink.h lib/str.h lib/stralloc.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)path_canonical_sa.o: lib/path/path_canonical_sa.c lib/path_internal.h lib/str.h lib/typedefs.h lib/path.h lib/windoze.h lib/stralloc.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)path_realpath.o: lib/path/path_realpath.c lib/byte.h lib/typedefs.h lib/path_internal.h lib/str.h lib/path.h lib/windoze.h lib/stralloc.h lib/windoze.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)strarray_each.o: lib/strarray/strarray_each.c lib/strarray.h lib/uint64.h lib/array.h lib/typedefs.h lib/stralloc.h lib/byte.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)strarray_removeprefixs.o: lib/strarray/strarray_removeprefixs.c lib/str.h lib/typedefs.h lib/stralloc.h lib/strarray.h lib/uint64.h lib/array.h lib/stralloc.h lib/byte.h lib/byte.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)strarray_pop.o: lib/strarray/strarray_pop.c lib/strarray.h lib/uint64.h lib/array.h lib/typedefs.h lib/stralloc.h lib/byte.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)strarray_push.o: lib/strarray/strarray_push.c lib/array.h lib/typedefs.h lib/uint64.h lib/str.h lib/strarray.h lib/array.h lib/stralloc.h lib/byte.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)strarray_sort.o: lib/strarray/strarray_sort.c lib/strarray.h lib/uint64.h lib/array.h lib/typedefs.h lib/stralloc.h lib/byte.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)strarray_set.o: lib/strarray/strarray_set.c lib/array.h lib/typedefs.h lib/uint64.h lib/str.h lib/strarray.h lib/array.h lib/stralloc.h lib/byte.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)strarray_from_argv.o: lib/strarray/strarray_from_argv.c lib/strarray.h lib/uint64.h lib/array.h lib/typedefs.h lib/stralloc.h lib/byte.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)strarray_joins.o: lib/strarray/strarray_joins.c lib/str.h lib/typedefs.h lib/stralloc.h lib/strarray.h lib/uint64.h lib/array.h lib/stralloc.h lib/byte.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)strarray_glob.o: lib/strarray/strarray_glob.c lib/glob.h lib/strarray.h lib/uint64.h lib/array.h lib/typedefs.h lib/stralloc.h lib/byte.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)strarray_push_sa.o: lib/strarray/strarray_push_sa.c lib/stralloc.h lib/typedefs.h lib/strarray.h lib/uint64.h lib/array.h lib/stralloc.h lib/byte.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)strarray_transform.o: lib/strarray/strarray_transform.c lib/str.h lib/typedefs.h lib/stralloc.h lib/strarray.h lib/uint64.h lib/array.h lib/stralloc.h lib/byte.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)strarray_appends.o: lib/strarray/strarray_appends.c lib/str.h lib/typedefs.h lib/stralloc.h lib/strarray.h lib/uint64.h lib/array.h lib/stralloc.h lib/byte.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)strarray_prepends.o: lib/strarray/strarray_prepends.c lib/str.h lib/typedefs.h lib/stralloc.h lib/strarray.h lib/uint64.h lib/array.h lib/stralloc.h lib/byte.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)strarray_removesuffixs.o: lib/strarray/strarray_removesuffixs.c lib/str.h lib/typedefs.h lib/stralloc.h lib/strarray.h lib/uint64.h lib/array.h lib/stralloc.h lib/byte.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)strarray_index_of_sa.o: lib/strarray/strarray_index_of_sa.c lib/stralloc.h lib/typedefs.h lib/strarray.h lib/uint64.h lib/array.h lib/stralloc.h lib/byte.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)strarray_index_of.o: lib/strarray/strarray_index_of.c lib/str.h lib/typedefs.h lib/strarray.h lib/uint64.h lib/array.h lib/stralloc.h lib/byte.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)strarray_pushb.o: lib/strarray/strarray_pushb.c lib/str.h lib/typedefs.h lib/strarray.h lib/uint64.h lib/array.h lib/stralloc.h lib/byte.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)strarray_splice.o: lib/strarray/strarray_splice.c lib/strarray.h lib/uint64.h lib/array.h lib/typedefs.h lib/stralloc.h lib/byte.h lib/array.h lib/byte.h lib/safemult.h lib/uint16.h lib/uint32.h lib/uint64.h lib/str.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)strarray_to_argv.o: lib/strarray/strarray_to_argv.c lib/byte.h lib/typedefs.h lib/strarray.h lib/uint64.h lib/array.h lib/stralloc.h lib/byte.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)strarray_pushd.o: lib/strarray/strarray_pushd.c lib/str.h lib/typedefs.h lib/strarray.h lib/uint64.h lib/array.h lib/stralloc.h lib/byte.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)uint64_read.o: lib/uint64/uint64_read.c lib/uint64.h lib/uint32.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)uint64_unpack.o: lib/uint64/uint64_unpack.c lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)uint64_pack_big.o: lib/uint64/uint64_pack_big.c lib/uint64.h lib/uint32.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)uint64_unpack_big.o: lib/uint64/uint64_unpack_big.c lib/uint64.h lib/uint32.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)uint64_pack.o: lib/uint64/uint64_pack.c lib/uint64.h lib/uint32.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)uint64_read_big.o: lib/uint64/uint64_read_big.c lib/uint64.h lib/uint32.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)hmap_add.o: lib/hmap/hmap_add.c lib/byte.h lib/typedefs.h lib/hmap_internal.h lib/byte.h lib/hmap.h lib/stralloc.h lib/buffer.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)hmap_size.o: lib/hmap/hmap_size.c lib/hmap_internal.h lib/byte.h lib/typedefs.h lib/hmap.h lib/stralloc.h lib/buffer.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)hmap_dump.o: lib/hmap/hmap_dump.c lib/uint64.h lib/buffer.h lib/typedefs.h lib/hmap.h lib/stralloc.h lib/buffer.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)hmap_add_tuple_with_data.o: lib/hmap/hmap_add_tuple_with_data.c lib/byte.h lib/typedefs.h lib/hmap_internal.h lib/byte.h lib/hmap.h lib/stralloc.h lib/buffer.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)hmap_free_data.o: lib/hmap/hmap_free_data.c lib/hmap_internal.h lib/byte.h lib/typedefs.h lib/hmap.h lib/stralloc.h lib/buffer.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)hmap_delete.o: lib/hmap/hmap_delete.c lib/hmap_internal.h lib/byte.h lib/typedefs.h lib/hmap.h lib/stralloc.h lib/buffer.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)hmap_print_table.o: lib/hmap/hmap_print_table.c lib/uint64.h lib/buffer.h lib/typedefs.h lib/hmap_internal.h lib/byte.h lib/hmap.h lib/stralloc.h lib/buffer.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)hmap_truncate.o: lib/hmap/hmap_truncate.c lib/hmap_internal.h lib/byte.h lib/typedefs.h lib/hmap.h lib/stralloc.h lib/buffer.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)hmap_set_stralloc.o: lib/hmap/hmap_set_stralloc.c lib/hmap.h lib/stralloc.h lib/typedefs.h lib/buffer.h lib/buffer.h lib/stralloc.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)hmap_set_chars.o: lib/hmap/hmap_set_chars.c lib/hmap.h lib/stralloc.h lib/typedefs.h lib/buffer.h lib/buffer.h lib/str.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)hmap_init.o: lib/hmap/hmap_init.c lib/hmap_internal.h lib/byte.h lib/typedefs.h lib/hmap.h lib/stralloc.h lib/buffer.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)hmap_print_list.o: lib/hmap/hmap_print_list.c lib/uint64.h lib/buffer.h lib/typedefs.h lib/hmap_internal.h lib/byte.h lib/hmap.h lib/stralloc.h lib/buffer.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)hmap_is_locate.o: lib/hmap/hmap_is_locate.c lib/hmap_internal.h lib/byte.h lib/typedefs.h lib/hmap.h lib/stralloc.h lib/buffer.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)hmap_hash.o: lib/hmap/hmap_hash.c lib/hmap_internal.h lib/byte.h lib/typedefs.h lib/hmap.h lib/stralloc.h lib/buffer.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)hmap_get.o: lib/hmap/hmap_get.c lib/hmap.h lib/stralloc.h lib/typedefs.h lib/buffer.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)hmap_distance.o: lib/hmap/hmap_distance.c lib/hmap_internal.h lib/byte.h lib/typedefs.h lib/hmap.h lib/stralloc.h lib/buffer.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)hmap_print_tree.o: lib/hmap/hmap_print_tree.c lib/uint64.h lib/buffer.h lib/typedefs.h lib/hmap_internal.h lib/byte.h lib/hmap.h lib/stralloc.h lib/buffer.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)hmap_destroy.o: lib/hmap/hmap_destroy.c lib/hmap_internal.h lib/byte.h lib/typedefs.h lib/hmap.h lib/stralloc.h lib/buffer.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)hmap_set.o: lib/hmap/hmap_set.c lib/hmap_internal.h lib/byte.h lib/typedefs.h lib/hmap.h lib/stralloc.h lib/buffer.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)hmap_cmp.o: lib/hmap/hmap_cmp.c lib/hmap_internal.h lib/byte.h lib/typedefs.h lib/hmap.h lib/stralloc.h lib/buffer.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)hmap_search.o: lib/hmap/hmap_search.c lib/hmap_internal.h lib/byte.h lib/typedefs.h lib/hmap.h lib/stralloc.h lib/buffer.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)binfmt_open.o: lib/binfmt/binfmt_open.c lib/binfmt.h lib/typedefs.h lib/uint8.h lib/uint32.h lib/uint64.h lib/range.h lib/mmap.h lib/byte.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)env_put.o: lib/env/env_put.c lib/windoze.h lib/typedefs.h lib/env.h lib/typedefs.h lib/byte.h lib/str.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)env_make.o: lib/env/env_make.c lib/env.h lib/typedefs.h lib/str.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)env_addmodif.o: lib/env/env_addmodif.c lib/env.h lib/typedefs.h lib/stralloc.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)env_set.o: lib/env/env_set.c lib/windoze.h lib/env.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)env_len.o: lib/env/env_len.c lib/env.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)env_get.o: lib/env/env_get.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)env_unset.o: lib/env/env_unset.c lib/windoze.h lib/env.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)env_merge.o: lib/env/env_merge.c lib/byte.h lib/typedefs.h lib/env.h lib/str.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)env_pick.o: lib/env/env_pick.c lib/windoze.h lib/env.h lib/typedefs.h lib/str.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)env_string.o: lib/env/env_string.c lib/env.h lib/typedefs.h lib/stralloc.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)env_get2.o: lib/env/env_get2.c lib/env.h lib/typedefs.h lib/str.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)http_read.o: lib/http/http_read.c lib/http.h lib/uint16.h lib/uint32.h lib/uint64.h lib/stralloc.h lib/typedefs.h lib/buffer.h lib/ip4.h lib/uint8.h lib/scan.h lib/socket_internal.h lib/socket.h lib/str.h lib/stralloc.h lib/io.h lib/windoze.h lib/iarray.h lib/taia.h lib/tai.h lib/byte.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)http_readable.o: lib/http/http_readable.c lib/socket_internal.h lib/socket.h lib/typedefs.h lib/uint16.h lib/uint32.h lib/windoze.h lib/buffer.h lib/byte.h lib/http.h lib/uint64.h lib/stralloc.h lib/buffer.h lib/ip4.h lib/uint8.h lib/io_internal.h lib/windoze.h lib/array.h lib/iarray.h lib/taia.h lib/tai.h lib/io.h lib/scan.h lib/stralloc.h lib/str.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)http_init.o: lib/http/http_init.c lib/socket_internal.h lib/socket.h lib/typedefs.h lib/uint16.h lib/uint32.h lib/http.h lib/uint64.h lib/stralloc.h lib/buffer.h lib/ip4.h lib/uint8.h lib/byte.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)http_sendreq.o: lib/http/http_sendreq.c lib/socket_internal.h lib/socket.h lib/typedefs.h lib/uint16.h lib/uint32.h lib/buffer.h lib/http.h lib/uint64.h lib/stralloc.h lib/buffer.h lib/ip4.h lib/uint8.h lib/io_internal.h lib/windoze.h lib/array.h lib/iarray.h lib/taia.h lib/tai.h lib/io.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)http_socket.o: lib/http/http_socket.c lib/windoze.h lib/socket_internal.h lib/socket.h lib/typedefs.h lib/uint16.h lib/uint32.h lib/buffer.h lib/http.h lib/uint64.h lib/stralloc.h lib/buffer.h lib/ip4.h lib/uint8.h lib/io.h lib/windoze.h lib/iarray.h lib/taia.h lib/tai.h lib/ndelay.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)http_writeable.o: lib/http/http_writeable.c lib/http.h lib/uint16.h lib/uint32.h lib/uint64.h lib/stralloc.h lib/typedefs.h lib/buffer.h lib/ip4.h lib/uint8.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)http_get.o: lib/http/http_get.c lib/socket_internal.h lib/socket.h lib/typedefs.h lib/uint16.h lib/uint32.h lib/socket.h lib/buffer.h lib/byte.h lib/dns.h lib/iopause.h lib/io.h lib/windoze.h lib/iarray.h lib/uint64.h lib/taia.h lib/tai.h lib/stralloc.h lib/errmsg.h lib/http.h lib/buffer.h lib/ip4.h lib/uint8.h lib/io.h lib/ip4.h lib/str.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)rdir_close.o: lib/rdir/rdir_close.c lib/dir.h lib/uint64.h lib/rdir.h lib/dir.h lib/dir_internal.h lib/stralloc.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)rdir_open.o: lib/rdir/rdir_open.c lib/rdir.h lib/dir.h lib/uint64.h lib/dir_internal.h lib/stralloc.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)rdir_read.o: lib/rdir/rdir_read.c lib/rdir.h lib/dir.h lib/uint64.h lib/dir_internal.h lib/stralloc.h lib/typedefs.h lib/buffer.h lib/byte.h lib/dir_internal.h lib/stralloc.h lib/str.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)pe_offset2rva.o: lib/pe/pe_offset2rva.c lib/pe.h lib/typedefs.h lib/uint64.h lib/uint32.h lib/uint16.h lib/uint8.h lib/uint32.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)pe_header.o: lib/pe/pe_header.c lib/pe.h lib/typedefs.h lib/uint64.h lib/uint32.h lib/uint16.h lib/uint8.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)pe_rva2section.o: lib/pe/pe_rva2section.c lib/pe.h lib/typedefs.h lib/uint64.h lib/uint32.h lib/uint16.h lib/uint8.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)pe_get_datadir.o: lib/pe/pe_get_datadir.c lib/pe.h lib/typedefs.h lib/uint64.h lib/uint32.h lib/uint16.h lib/uint8.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)pe_rva2offset.o: lib/pe/pe_rva2offset.c lib/pe.h lib/typedefs.h lib/uint64.h lib/uint32.h lib/uint16.h lib/uint8.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)pe_rva2ptr.o: lib/pe/pe_rva2ptr.c lib/pe.h lib/typedefs.h lib/uint64.h lib/uint32.h lib/uint16.h lib/uint8.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)pe_get_value.o: lib/pe/pe_get_value.c lib/pe.h lib/typedefs.h lib/uint64.h lib/uint32.h lib/uint16.h lib/uint8.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)pe_get_section.o: lib/pe/pe_get_section.c lib/pe.h lib/typedefs.h lib/uint64.h lib/uint32.h lib/uint16.h lib/uint8.h lib/str.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)pe_datadir_name.o: lib/pe/pe_datadir_name.c lib/pe.h lib/typedefs.h lib/uint64.h lib/uint32.h lib/uint16.h lib/uint8.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)pe_thunk.o: lib/pe/pe_thunk.c lib/pe.h lib/typedefs.h lib/uint64.h lib/uint32.h lib/uint16.h lib/uint8.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_free.o: lib/buffer/buffer_free.c lib/buffer.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_dummyread.o: lib/buffer/buffer_dummyread.c lib/buffer.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_putlonglong.o: lib/buffer/buffer_putlonglong.c lib/buffer.h lib/typedefs.h lib/fmt.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_init_free.o: lib/buffer/buffer_init_free.c lib/buffer.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_skipn.o: lib/buffer/buffer_skipn.c lib/buffer.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_putlong0.o: lib/buffer/buffer_putlong0.c lib/buffer.h lib/typedefs.h lib/fmt.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_putflush.o: lib/buffer/buffer_putflush.c lib/windoze.h lib/buffer.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_putxlonglong0.o: lib/buffer/buffer_putxlonglong0.c lib/buffer.h lib/typedefs.h lib/fmt.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_flush.o: lib/buffer/buffer_flush.c lib/buffer.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_default.o: lib/buffer/buffer_default.c lib/buffer.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_dummyreadmmap.o: lib/buffer/buffer_dummyreadmmap.c lib/buffer.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_skip_pred.o: lib/buffer/buffer_skip_pred.c lib/buffer.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_putc.o: lib/buffer/buffer_putc.c lib/buffer.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_peekc.o: lib/buffer/buffer_peekc.c lib/buffer.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_puts.o: lib/buffer/buffer_puts.c lib/buffer.h lib/typedefs.h lib/str.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_munmap.o: lib/buffer/buffer_munmap.c lib/windoze.h lib/buffer.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_putptr.o: lib/buffer/buffer_putptr.c lib/buffer.h lib/typedefs.h lib/fmt.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_fromarray.o: lib/buffer/buffer_fromarray.c lib/array.h lib/typedefs.h lib/uint64.h lib/buffer.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_gzip.o: lib/buffer/buffer_gzip.c lib/buffer.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_getline.o: lib/buffer/buffer_getline.c lib/buffer.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_read.o: lib/buffer/buffer_read.c lib/windoze.h lib/buffer.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_puttai.o: lib/buffer/buffer_puttai.c lib/buffer.h lib/typedefs.h lib/fmt.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_putnspace.o: lib/buffer/buffer_putnspace.c lib/buffer.h lib/typedefs.h lib/byte.h lib/str.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_write_fd.o: lib/buffer/buffer_write_fd.c lib/windoze.h lib/buffer.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_frombuf.o: lib/buffer/buffer_frombuf.c lib/buffer.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_deflate.o: lib/buffer/buffer_deflate.c lib/buffer.h lib/typedefs.h lib/byte.h lib/str.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_putulong.o: lib/buffer/buffer_putulong.c lib/buffer.h lib/typedefs.h lib/fmt.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_write.o: lib/buffer/buffer_write.c lib/windoze.h lib/buffer.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_putspace.o: lib/buffer/buffer_putspace.c lib/buffer.h lib/typedefs.h lib/str.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_1.o: lib/buffer/buffer_1.c lib/buffer.h lib/typedefs.h lib/windoze.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_putm_internal.o: lib/buffer/buffer_putm_internal.c lib/buffer.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_get_token.o: lib/buffer/buffer_get_token.c lib/buffer.h lib/typedefs.h lib/byte.h lib/scan.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_0.o: lib/buffer/buffer_0.c lib/windoze.h lib/buffer.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_lzma.o: lib/buffer/buffer_lzma.c lib/buffer.h lib/typedefs.h lib/uint8.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_1small.o: lib/buffer/buffer_1small.c lib/windoze.h lib/buffer.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_read_fd.o: lib/buffer/buffer_read_fd.c lib/windoze.h lib/buffer.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_putalign.o: lib/buffer/buffer_putalign.c lib/buffer.h lib/typedefs.h lib/byte.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_putsa.o: lib/buffer/buffer_putsa.c lib/buffer.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_putdouble.o: lib/buffer/buffer_putdouble.c lib/buffer.h lib/typedefs.h lib/byte.h lib/fmt.h lib/uint32.h lib/uint64.h lib/str.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_put8long.o: lib/buffer/buffer_put8long.c lib/buffer.h lib/typedefs.h lib/fmt.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_putulonglong.o: lib/buffer/buffer_putulonglong.c lib/buffer.h lib/typedefs.h lib/fmt.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_skipspace.o: lib/buffer/buffer_skipspace.c lib/buffer.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_put.o: lib/buffer/buffer_put.c lib/buffer.h lib/typedefs.h lib/byte.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_mmapprivate.o: lib/buffer/buffer_mmapprivate.c lib/buffer.h lib/typedefs.h lib/mmap.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_0small.o: lib/buffer/buffer_0small.c lib/windoze.h lib/buffer.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_truncfile.o: lib/buffer/buffer_truncfile.c lib/windoze.h lib/buffer.h lib/typedefs.h lib/open.h lib/windoze.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_fromstr.o: lib/buffer/buffer_fromstr.c lib/buffer.h lib/typedefs.h lib/str.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_copy.o: lib/buffer/buffer_copy.c lib/buffer.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_skip_until.o: lib/buffer/buffer_skip_until.c lib/buffer.h lib/typedefs.h lib/byte.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_fromsa.o: lib/buffer/buffer_fromsa.c lib/buffer.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_putnlflush.o: lib/buffer/buffer_putnlflush.c lib/buffer.h lib/typedefs.h lib/str.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_2.o: lib/buffer/buffer_2.c lib/buffer.h lib/typedefs.h lib/windoze.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_bzip.o: lib/buffer/buffer_bzip.c lib/buffer.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_inflate.o: lib/buffer/buffer_inflate.c lib/windoze.h lib/buffer.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_feed.o: lib/buffer/buffer_feed.c lib/buffer.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_get_token_pred.o: lib/buffer/buffer_get_token_pred.c lib/buffer.h lib/typedefs.h lib/byte.h lib/scan.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_get_until.o: lib/buffer/buffer_get_until.c lib/buffer.h lib/typedefs.h lib/byte.h lib/scan.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_puterror.o: lib/buffer/buffer_puterror.c lib/buffer.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_putsflush.o: lib/buffer/buffer_putsflush.c lib/buffer.h lib/typedefs.h lib/str.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_dump.o: lib/buffer/buffer_dump.c lib/windoze.h lib/buffer.h lib/typedefs.h lib/fmt.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_putsalign.o: lib/buffer/buffer_putsalign.c lib/buffer.h lib/typedefs.h lib/str.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_mmapshared_fd.o: lib/buffer/buffer_mmapshared_fd.c lib/windoze.h lib/buffer.h lib/typedefs.h lib/mmap.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_bz2.o: lib/buffer/buffer_bz2.c lib/buffer.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_putm_internal_flush.o: lib/buffer/buffer_putm_internal_flush.c lib/buffer.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_peek.o: lib/buffer/buffer_peek.c lib/buffer.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_mmapread_fd.o: lib/buffer/buffer_mmapread_fd.c lib/buffer.h lib/typedefs.h lib/mmap.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_putsaflush.o: lib/buffer/buffer_putsaflush.c lib/buffer.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_putns.o: lib/buffer/buffer_putns.c lib/buffer.h lib/typedefs.h lib/str.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_putnc.o: lib/buffer/buffer_putnc.c lib/buffer.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_mmapprivate_fd.o: lib/buffer/buffer_mmapprivate_fd.c lib/buffer.h lib/typedefs.h lib/mmap.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_putxlonglong.o: lib/buffer/buffer_putxlonglong.c lib/buffer.h lib/typedefs.h lib/fmt.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_close.o: lib/buffer/buffer_close.c lib/windoze.h lib/buffer.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_get.o: lib/buffer/buffer_get.c lib/buffer.h lib/typedefs.h lib/byte.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_putulong0.o: lib/buffer/buffer_putulong0.c lib/buffer.h lib/typedefs.h lib/fmt.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_putspad.o: lib/buffer/buffer_putspad.c lib/buffer.h lib/typedefs.h lib/str.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_prefetch.o: lib/buffer/buffer_prefetch.c lib/buffer.h lib/typedefs.h lib/byte.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_freshen.o: lib/buffer/buffer_freshen.c lib/buffer.h lib/typedefs.h lib/byte.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_putlong.o: lib/buffer/buffer_putlong.c lib/buffer.h lib/typedefs.h lib/fmt.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_puterror2.o: lib/buffer/buffer_puterror2.c lib/buffer.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_stubborn.o: lib/buffer/buffer_stubborn.c lib/windoze.h lib/buffer.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_skipc.o: lib/buffer/buffer_skipc.c lib/buffer.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_putxlong.o: lib/buffer/buffer_putxlong.c lib/buffer.h lib/typedefs.h lib/fmt.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_mmapread.o: lib/buffer/buffer_mmapread.c lib/buffer.h lib/typedefs.h lib/mmap.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_seek.o: lib/buffer/buffer_seek.c lib/buffer.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_getc.o: lib/buffer/buffer_getc.c lib/buffer.h lib/typedefs.h lib/byte.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_getn.o: lib/buffer/buffer_getn.c lib/buffer.h lib/typedefs.h lib/byte.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_putxlong0.o: lib/buffer/buffer_putxlong0.c lib/buffer.h lib/typedefs.h lib/fmt.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_init.o: lib/buffer/buffer_init.c lib/buffer.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer_stubborn2.o: lib/buffer/buffer_stubborn2.c lib/windoze.h lib/buffer.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)expand_arith.o: lib/expand/expand_arith.c lib/tree.h lib/stralloc.h lib/typedefs.h lib/uint64.h lib/expand.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)expand_argv.o: lib/expand/expand_argv.c lib/tree.h lib/stralloc.h lib/typedefs.h lib/uint64.h lib/expand.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)expand_catsa.o: lib/expand/expand_catsa.c lib/vartab.h lib/uint64.h lib/var.h lib/stralloc.h lib/typedefs.h lib/vartab.h lib/byte.h lib/tree.h lib/expand.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)expand_glob.o: lib/expand/expand_glob.c lib/vartab.h lib/uint64.h lib/var.h lib/stralloc.h lib/typedefs.h lib/vartab.h lib/tree.h lib/glob.h lib/var.h lib/expand.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)expand_vars.o: lib/expand/expand_vars.c lib/vartab.h lib/uint64.h lib/var.h lib/stralloc.h lib/typedefs.h lib/vartab.h lib/tree.h lib/expand.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)expand_command.o: lib/expand/expand_command.c lib/stralloc.h lib/typedefs.h lib/buffer.h lib/tree.h lib/stralloc.h lib/uint64.h lib/vartab.h lib/var.h lib/vartab.h lib/expand.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)expand_cat.o: lib/expand/expand_cat.c lib/vartab.h lib/uint64.h lib/var.h lib/stralloc.h lib/typedefs.h lib/vartab.h lib/stralloc.h lib/str.h lib/var.h lib/tree.h lib/expand.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)expand_param.o: lib/expand/expand_param.c lib/windoze.h lib/vartab.h lib/uint64.h lib/var.h lib/stralloc.h lib/typedefs.h lib/vartab.h lib/stralloc.h lib/fmt.h lib/uint32.h lib/str.h lib/var.h lib/fnmatch.h lib/tree.h lib/expand.h lib/errmsg.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)expand_copysa.o: lib/expand/expand_copysa.c lib/vartab.h lib/uint64.h lib/var.h lib/stralloc.h lib/typedefs.h lib/vartab.h lib/tree.h lib/byte.h lib/expand.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)expand_escape.o: lib/expand/expand_escape.c lib/str.h lib/typedefs.h lib/stralloc.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)expand_unescape.o: lib/expand/expand_unescape.c lib/tree.h lib/stralloc.h lib/typedefs.h lib/uint64.h lib/stralloc.h lib/expand.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)expand_args.o: lib/expand/expand_args.c lib/vartab.h lib/uint64.h lib/var.h lib/stralloc.h lib/typedefs.h lib/vartab.h lib/stralloc.h lib/tree.h lib/expand.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)expand_arg.o: lib/expand/expand_arg.c lib/vartab.h lib/uint64.h lib/var.h lib/stralloc.h lib/typedefs.h lib/vartab.h lib/stralloc.h lib/tree.h lib/expand.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)vartab_cleanup.o: lib/vartab/vartab_cleanup.c lib/vartab.h lib/uint64.h lib/var.h lib/stralloc.h lib/typedefs.h lib/vartab.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)vartab_root.o: lib/vartab/vartab_root.c lib/vartab.h lib/uint64.h lib/var.h lib/stralloc.h lib/typedefs.h lib/vartab.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)vartab_push.o: lib/vartab/vartab_push.c lib/byte.h lib/typedefs.h lib/vartab.h lib/uint64.h lib/var.h lib/stralloc.h lib/vartab.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)vartab_add.o: lib/vartab/vartab_add.c lib/var.h lib/uint64.h lib/stralloc.h lib/typedefs.h lib/vartab.h lib/var.h lib/vartab.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)vartab_dump.o: lib/vartab/vartab_dump.c lib/var.h lib/uint64.h lib/stralloc.h lib/typedefs.h lib/vartab.h lib/var.h lib/vartab.h lib/buffer.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)vartab_print.o: lib/vartab/vartab_print.c lib/var.h lib/uint64.h lib/stralloc.h lib/typedefs.h lib/vartab.h lib/var.h lib/vartab.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)vartab_hash.o: lib/vartab/vartab_hash.c lib/vartab.h lib/uint64.h lib/var.h lib/stralloc.h lib/typedefs.h lib/vartab.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)vartab_search.o: lib/vartab/vartab_search.c lib/vartab.h lib/uint64.h lib/var.h lib/stralloc.h lib/typedefs.h lib/vartab.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)vartab_pop.o: lib/vartab/vartab_pop.c lib/vartab.h lib/uint64.h lib/var.h lib/stralloc.h lib/typedefs.h lib/vartab.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ihex_read_record.o: lib/ihex/ihex_read_record.c lib/ihex.h lib/uint8.h lib/uint16.h lib/slist.h lib/alloc.h lib/typedefs.h lib/scan.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ihex_read_data.o: lib/ihex/ihex_read_data.c lib/ihex.h lib/uint8.h lib/uint16.h lib/slist.h lib/alloc.h lib/typedefs.h lib/scan.h lib/uint32.h lib/uint64.h lib/alloc.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ihex_read_buf.o: lib/ihex/ihex_read_buf.c lib/ihex.h lib/uint8.h lib/uint16.h lib/slist.h lib/alloc.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)map_get.o: lib/map/map_get.c lib/map_internal.h lib/map.h lib/byte.h lib/typedefs.h lib/str.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)map_remove.o: lib/map/map_remove.c lib/map_internal.h lib/map.h lib/byte.h lib/typedefs.h lib/str.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)map_getref.o: lib/map/map_getref.c lib/map_internal.h lib/map.h lib/byte.h lib/typedefs.h lib/str.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)map_iter.o: lib/map/map_iter.c lib/map_internal.h lib/map.h lib/byte.h lib/typedefs.h lib/str.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)map_set.o: lib/map/map_set.c lib/byte.h lib/typedefs.h lib/map_internal.h lib/map.h lib/byte.h lib/str.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)map_next.o: lib/map/map_next.c lib/map_internal.h lib/map.h lib/byte.h lib/typedefs.h lib/str.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)map_deinit.o: lib/map/map_deinit.c lib/map_internal.h lib/map.h lib/byte.h lib/typedefs.h lib/str.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)byte_fill.o: lib/byte/byte_fill.c lib/byte.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)byte_replace.o: lib/byte/byte_replace.c lib/byte.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)byte_scan.o: lib/byte/byte_scan.c lib/byte.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)byte_copyr.o: lib/byte/byte_copyr.c lib/byte.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)byte_upper.o: lib/byte/byte_upper.c lib/byte.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)byte_diff.o: lib/byte/byte_diff.c lib/byte.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)byte_case_equal.o: lib/byte/byte_case_equal.c lib/byte.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)byte_lower.o: lib/byte/byte_lower.c lib/byte.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)byte_camelize.o: lib/byte/byte_camelize.c lib/byte.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)byte_rchr.o: lib/byte/byte_rchr.c lib/byte.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)byte_findb.o: lib/byte/byte_findb.c lib/byte.h lib/typedefs.h lib/str.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)byte_equal.o: lib/byte/byte_equal.c lib/byte.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)byte_ccopy.o: lib/byte/byte_ccopy.c lib/byte.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)byte_triml.o: lib/byte/byte_triml.c lib/byte.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)byte_zero.o: lib/byte/byte_zero.c lib/byte.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)byte_count.o: lib/byte/byte_count.c lib/byte.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)byte_copy.o: lib/byte/byte_copy.c lib/byte.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)byte_chr.o: lib/byte/byte_chr.c lib/byte.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)byte_fmt.o: lib/byte/byte_fmt.c lib/byte.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)byte_case_start.o: lib/byte/byte_case_start.c lib/byte.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)byte_trimr.o: lib/byte/byte_trimr.c lib/byte.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)byte_finds.o: lib/byte/byte_finds.c lib/byte.h lib/typedefs.h lib/str.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)byte_case_diff.o: lib/byte/byte_case_diff.c lib/byte.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)byte_chrs.o: lib/byte/byte_chrs.c lib/byte.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)mmap_readat.o: lib/mmap/mmap_readat.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)mmap_shared_fd.o: lib/mmap/mmap_shared_fd.c lib/windoze.h lib/io_internal.h lib/windoze.h lib/array.h lib/typedefs.h lib/uint64.h lib/iarray.h lib/taia.h lib/tai.h lib/uint32.h lib/io.h lib/mmap.h lib/open.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)mmap_unmap.o: lib/mmap/mmap_unmap.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)mmap_private_fd.o: lib/mmap/mmap_private_fd.c lib/windoze.h lib/io_internal.h lib/windoze.h lib/array.h lib/typedefs.h lib/uint64.h lib/iarray.h lib/taia.h lib/tai.h lib/uint32.h lib/io.h lib/mmap.h lib/open.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)mmap_rw_fd.o: lib/mmap/mmap_rw_fd.c lib/windoze.h lib/io_internal.h lib/windoze.h lib/array.h lib/typedefs.h lib/uint64.h lib/iarray.h lib/taia.h lib/tai.h lib/uint32.h lib/io.h lib/mmap.h lib/open.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)mmap_private.o: lib/mmap/mmap_private.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)mmap_map.o: lib/mmap/mmap_map.c lib/windoze.h lib/io_internal.h lib/windoze.h lib/array.h lib/typedefs.h lib/uint64.h lib/iarray.h lib/taia.h lib/tai.h lib/uint32.h lib/io.h lib/mmap.h lib/open.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)mmap_read_fd.o: lib/mmap/mmap_read_fd.c lib/windoze.h lib/io_internal.h lib/windoze.h lib/array.h lib/typedefs.h lib/uint64.h lib/iarray.h lib/taia.h lib/tai.h lib/uint32.h lib/io.h lib/mmap.h lib/open.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)mmap_read.o: lib/mmap/mmap_read.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)mmap_shared.o: lib/mmap/mmap_shared.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)gpio_read.o: lib/gpio/gpio_read.c lib/gpio_internal.h lib/gpio.h lib/mmap.h lib/typedefs.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)gpio_set_input_pull.o: lib/gpio/gpio_set_input_pull.c lib/gpio_internal.h lib/gpio.h lib/mmap.h lib/typedefs.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)gpio_set_pin_mode.o: lib/gpio/gpio_set_pin_mode.c lib/gpio_internal.h lib/gpio.h lib/mmap.h lib/typedefs.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)gpio_set_input_pull_up.o: lib/gpio/gpio_set_input_pull_up.c lib/gpio_internal.h lib/gpio.h lib/mmap.h lib/typedefs.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)gpio_set_output.o: lib/gpio/gpio_set_output.c lib/gpio_internal.h lib/gpio.h lib/mmap.h lib/typedefs.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)gpio_write.o: lib/gpio/gpio_write.c lib/gpio_internal.h lib/gpio.h lib/mmap.h lib/typedefs.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)gpio_set_input_pull_down.o: lib/gpio/gpio_set_input_pull_down.c lib/gpio_internal.h lib/gpio.h lib/mmap.h lib/typedefs.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)gpio_init.o: lib/gpio/gpio_init.c lib/windoze.h lib/buffer.h lib/typedefs.h lib/open.h lib/windoze.h lib/uint64.h lib/gpio_internal.h lib/gpio.h lib/mmap.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)gpio_short_wait.o: lib/gpio/gpio_short_wait.c lib/windoze.h lib/gpio_internal.h lib/gpio.h lib/mmap.h lib/typedefs.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)gpio_set_input_resistor_pull.o: lib/gpio/gpio_set_input_resistor_pull.c lib/gpio_internal.h lib/gpio.h lib/mmap.h lib/typedefs.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)scan_asn1dertag.o: lib/scan/scan_asn1dertag.c lib/scan.h lib/typedefs.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)scan_noncharsetnskip.o: lib/scan/scan_noncharsetnskip.c lib/scan.h lib/typedefs.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)scan_eolskip.o: lib/scan/scan_eolskip.c lib/scan.h lib/typedefs.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)scan_utf8_sem.o: lib/scan/scan_utf8_sem.c lib/scan.h lib/typedefs.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)scan_varint.o: lib/scan/scan_varint.c lib/scan.h lib/typedefs.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)scan_xlonglong.o: lib/scan/scan_xlonglong.c lib/scan.h lib/typedefs.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)scan_double.o: lib/scan/scan_double.c lib/scan.h lib/typedefs.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)scan_longn.o: lib/scan/scan_longn.c lib/scan.h lib/typedefs.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)scan_asn1derlength.o: lib/scan/scan_asn1derlength.c lib/scan.h lib/typedefs.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)scan_whitenskip.o: lib/scan/scan_whitenskip.c lib/scan.h lib/typedefs.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)scan_charsetnskip.o: lib/scan/scan_charsetnskip.c lib/scan.h lib/typedefs.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)scan_xlong.o: lib/scan/scan_xlong.c lib/scan.h lib/typedefs.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)scan_ulongn.o: lib/scan/scan_ulongn.c lib/scan.h lib/typedefs.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)scan_netstring.o: lib/scan/scan_netstring.c lib/scan.h lib/typedefs.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)scan_short.o: lib/scan/scan_short.c lib/scan.h lib/typedefs.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)scan_pb_type1_fixed64.o: lib/scan/scan_pb_type1_fixed64.c lib/scan.h lib/typedefs.h lib/uint32.h lib/uint64.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)scan_nonwhitenskip.o: lib/scan/scan_nonwhitenskip.c lib/scan.h lib/typedefs.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)scan_pb_tag.o: lib/scan/scan_pb_tag.c lib/scan.h lib/typedefs.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)scan_uint.o: lib/scan/scan_uint.c lib/scan.h lib/typedefs.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)scan_xchar.o: lib/scan/scan_xchar.c lib/scan.h lib/typedefs.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)scan_xint.o: lib/scan/scan_xint.c lib/scan.h lib/typedefs.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)scan_pb_type0_sint.o: lib/scan/scan_pb_type0_sint.c lib/scan.h lib/typedefs.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)scan_lineskip.o: lib/scan/scan_lineskip.c lib/scan.h lib/typedefs.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)scan_8int.o: lib/scan/scan_8int.c lib/scan.h lib/typedefs.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)scan_8short.o: lib/scan/scan_8short.c lib/scan.h lib/typedefs.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)scan_int.o: lib/scan/scan_int.c lib/scan.h lib/typedefs.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)scan_8longn.o: lib/scan/scan_8longn.c lib/scan.h lib/typedefs.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)scan_utf8.o: lib/scan/scan_utf8.c lib/fmt.h lib/typedefs.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)scan_xlongn.o: lib/scan/scan_xlongn.c lib/scan.h lib/typedefs.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)scan_fromhex.o: lib/scan/scan_fromhex.c lib/scan.h lib/typedefs.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)scan_httpdate.o: lib/scan/scan_httpdate.c lib/windoze.h lib/byte.h lib/typedefs.h lib/case.h lib/scan.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)scan_line.o: lib/scan/scan_line.c lib/scan.h lib/typedefs.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)scan_8long.o: lib/scan/scan_8long.c lib/scan.h lib/typedefs.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)scan_octal.o: lib/scan/scan_octal.c lib/scan.h lib/typedefs.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)scan_long.o: lib/scan/scan_long.c lib/scan.h lib/typedefs.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)scan_ulong.o: lib/scan/scan_ulong.c lib/scan.h lib/typedefs.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)scan_xmlescape.o: lib/scan/scan_xmlescape.c lib/scan.h lib/typedefs.h lib/uint32.h lib/uint64.h lib/str.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)scan_xshort.o: lib/scan/scan_xshort.c lib/scan.h lib/typedefs.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)scan_plusminus.o: lib/scan/scan_plusminus.c lib/scan.h lib/typedefs.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)scan_longlong.o: lib/scan/scan_longlong.c lib/scan.h lib/typedefs.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)scan_pb_type5_fixed32.o: lib/scan/scan_pb_type5_fixed32.c lib/scan.h lib/typedefs.h lib/uint32.h lib/uint64.h lib/uint32.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)scan_ulonglong.o: lib/scan/scan_ulonglong.c lib/scan.h lib/typedefs.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)scan_ushort.o: lib/scan/scan_ushort.c lib/scan.h lib/typedefs.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)iarray_length.o: lib/iarray/iarray_length.c lib/iarray.h lib/uint64.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)iarray_free.o: lib/iarray/iarray_free.c lib/iarray.h lib/uint64.h lib/typedefs.h lib/mmap.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)iarray_get.o: lib/iarray/iarray_get.c lib/iarray.h lib/uint64.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)iarray_init.o: lib/iarray/iarray_init.c lib/iarray.h lib/uint64.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)iarray_allocate.o: lib/iarray/iarray_allocate.c lib/windoze.h lib/byte.h lib/typedefs.h lib/iarray.h lib/uint64.h lib/cas.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)alloc.o: lib/alloc/alloc.c lib/alloc.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)alloc_re.o: lib/alloc/alloc_re.c lib/alloc.h lib/typedefs.h lib/byte.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)alloc_zero.o: lib/alloc/alloc_zero.c lib/alloc.h lib/typedefs.h lib/byte.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)slist_shifts.o: lib/slist/slist_shifts.c lib/slist.h lib/alloc.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)slist_push.o: lib/slist/slist_push.c lib/slist.h lib/alloc.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)slist_pushs.o: lib/slist/slist_pushs.c lib/slist.h lib/alloc.h lib/typedefs.h lib/str.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)slist_add_before.o: lib/slist/slist_add_before.c lib/slist.h lib/alloc.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)slist_shift.o: lib/slist/slist_shift.c lib/slist.h lib/alloc.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)slist_find.o: lib/slist/slist_find.c lib/slist.h lib/alloc.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)slist_init.o: lib/slist/slist_init.c lib/slist.h lib/alloc.h lib/typedefs.h lib/byte.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)slist_unshifts.o: lib/slist/slist_unshifts.c lib/slist.h lib/alloc.h lib/typedefs.h lib/str.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)slist_remove.o: lib/slist/slist_remove.c lib/slist.h lib/alloc.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)slist_length.o: lib/slist/slist_length.c lib/slist.h lib/alloc.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)slist_add_after.o: lib/slist/slist_add_after.c lib/slist.h lib/alloc.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)slist_find_remove.o: lib/slist/slist_find_remove.c lib/slist.h lib/alloc.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)elf_get_symtab.o: lib/elf/elf_get_symtab.c lib/elf.h lib/uint8.h lib/uint16.h lib/uint32.h lib/uint64.h lib/range.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)elf_section_typename.o: lib/elf/elf_section_typename.c lib/elf.h lib/uint8.h lib/uint16.h lib/uint32.h lib/uint64.h lib/range.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)elf_get_section.o: lib/elf/elf_get_section.c lib/elf.h lib/uint8.h lib/uint16.h lib/uint32.h lib/uint64.h lib/range.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)elf_section.o: lib/elf/elf_section.c lib/elf.h lib/uint8.h lib/uint16.h lib/uint32.h lib/uint64.h lib/range.h lib/typedefs.h lib/range.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)elf_section_headers.o: lib/elf/elf_section_headers.c lib/elf.h lib/uint8.h lib/uint16.h lib/uint32.h lib/uint64.h lib/range.h lib/typedefs.h lib/range.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)elf_get_value.o: lib/elf/elf_get_value.c lib/elf.h lib/uint8.h lib/uint16.h lib/uint32.h lib/uint64.h lib/range.h lib/typedefs.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)elf_section_size.o: lib/elf/elf_section_size.c lib/elf.h lib/uint8.h lib/uint16.h lib/uint32.h lib/uint64.h lib/range.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)elf_section_offset.o: lib/elf/elf_section_offset.c lib/elf.h lib/uint8.h lib/uint16.h lib/uint32.h lib/uint64.h lib/range.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)elf_dynamic_section.o: lib/elf/elf_dynamic_section.c lib/elf.h lib/uint8.h lib/uint16.h lib/uint32.h lib/uint64.h lib/range.h lib/typedefs.h lib/range.h lib/byte.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)elf_header.o: lib/elf/elf_header.c lib/elf.h lib/uint8.h lib/uint16.h lib/uint32.h lib/uint64.h lib/range.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)elf_section_index.o: lib/elf/elf_section_index.c lib/elf.h lib/uint8.h lib/uint16.h lib/uint32.h lib/uint64.h lib/range.h lib/typedefs.h lib/range.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)elf_section_name.o: lib/elf/elf_section_name.c lib/elf.h lib/uint8.h lib/uint16.h lib/uint32.h lib/uint64.h lib/range.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)elf_get_symtab_r.o: lib/elf/elf_get_symtab_r.c lib/elf.h lib/uint8.h lib/uint16.h lib/uint32.h lib/uint64.h lib/range.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)elf_symbol_r.o: lib/elf/elf_symbol_r.c lib/elf.h lib/uint8.h lib/uint16.h lib/uint32.h lib/uint64.h lib/range.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)elf_get_section_r.o: lib/elf/elf_get_section_r.c lib/elf.h lib/uint8.h lib/uint16.h lib/uint32.h lib/uint64.h lib/range.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)elf_program_headers.o: lib/elf/elf_program_headers.c lib/elf.h lib/uint8.h lib/uint16.h lib/uint32.h lib/uint64.h lib/range.h lib/typedefs.h lib/range.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)elf_shstrtab.o: lib/elf/elf_shstrtab.c lib/elf.h lib/uint8.h lib/uint16.h lib/uint32.h lib/uint64.h lib/range.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)elf_section_find.o: lib/elf/elf_section_find.c lib/elf.h lib/uint8.h lib/uint16.h lib/uint32.h lib/uint64.h lib/range.h lib/typedefs.h lib/range.h lib/str.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)process_create.o: lib/process/process_create.c lib/windoze.h lib/byte.h lib/typedefs.h lib/io.h lib/windoze.h lib/iarray.h lib/uint64.h lib/taia.h lib/tai.h lib/uint32.h lib/strlist.h lib/stralloc.h lib/byte.h lib/buffer.h lib/uint64.h lib/env.h lib/errmsg.h lib/wait.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cb_find_prefix.o: lib/cb/cb_find_prefix.c lib/byte.h lib/typedefs.h lib/cb_internal.h lib/byte.h lib/cb.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cb_erase.o: lib/cb/cb_erase.c lib/byte.h lib/typedefs.h lib/cb_internal.h lib/byte.h lib/cb.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cb_get_kv.o: lib/cb/cb_get_kv.c lib/cb_internal.h lib/byte.h lib/typedefs.h lib/cb.h lib/str.h lib/byte.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cb_insert.o: lib/cb/cb_insert.c lib/cb_internal.h lib/byte.h lib/typedefs.h lib/cb.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cb_find.o: lib/cb/cb_find.c lib/cb_internal.h lib/byte.h lib/typedefs.h lib/cb.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cb_clear.o: lib/cb/cb_clear.c lib/cb_internal.h lib/byte.h lib/typedefs.h lib/cb.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cb_new_kv.o: lib/cb/cb_new_kv.c lib/cb_internal.h lib/byte.h lib/typedefs.h lib/cb.h lib/byte.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cb_get_kv_ex.o: lib/cb/cb_get_kv_ex.c lib/cb_internal.h lib/byte.h lib/typedefs.h lib/cb.h lib/str.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cb_foreach.o: lib/cb/cb_foreach.c lib/byte.h lib/typedefs.h lib/cb_internal.h lib/byte.h lib/cb.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)list_find_remove.o: lib/list/list_find_remove.c lib/list.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)list_init.o: lib/list/list_init.c lib/list.h lib/byte.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)list_find.o: lib/list/list_find.c lib/list.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)list_add_after.o: lib/list/list_add_after.c lib/list.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)list_push.o: lib/list/list_push.c lib/list.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)list_move_head.o: lib/list/list_move_head.c lib/list.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)list_unshift.o: lib/list/list_unshift.c lib/list.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)list_move_tail.o: lib/list/list_move_tail.c lib/list.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)list_swap.o: lib/list/list_swap.c lib/list.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)list_remove.o: lib/list/list_remove.c lib/list.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)list_length.o: lib/list/list_length.c lib/list.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)list_add_before.o: lib/list/list_add_before.c lib/list.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)dns_ip6.o: lib/dns/dns_ip6.c lib/byte.h lib/typedefs.h lib/dns.h lib/iopause.h lib/io.h lib/windoze.h lib/iarray.h lib/uint64.h lib/taia.h lib/tai.h lib/uint32.h lib/stralloc.h lib/ip4.h lib/uint8.h lib/uint16.h lib/ip6.h lib/byte.h lib/stralloc.h lib/uint16.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)dns_rcip.o: lib/dns/dns_rcip.c lib/socket_internal.h lib/socket.h lib/typedefs.h lib/uint16.h lib/uint32.h lib/byte.h lib/dns.h lib/iopause.h lib/io.h lib/windoze.h lib/iarray.h lib/uint64.h lib/taia.h lib/tai.h lib/stralloc.h lib/ip4.h lib/uint8.h lib/ip6.h lib/byte.h lib/open.h lib/taia.h lib/stralloc.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)dns_txt.o: lib/dns/dns_txt.c lib/byte.h lib/typedefs.h lib/dns.h lib/iopause.h lib/io.h lib/windoze.h lib/iarray.h lib/uint64.h lib/taia.h lib/tai.h lib/uint32.h lib/stralloc.h lib/stralloc.h lib/uint16.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)dns_mx.o: lib/dns/dns_mx.c lib/byte.h lib/typedefs.h lib/dns.h lib/iopause.h lib/io.h lib/windoze.h lib/iarray.h lib/uint64.h lib/taia.h lib/tai.h lib/uint32.h lib/stralloc.h lib/stralloc.h lib/uint16.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)dns_transmit.o: lib/dns/dns_transmit.c lib/socket_internal.h lib/socket.h lib/typedefs.h lib/uint16.h lib/uint32.h lib/windoze.h lib/byte.h lib/dns.h lib/iopause.h lib/io.h lib/windoze.h lib/iarray.h lib/uint64.h lib/taia.h lib/tai.h lib/stralloc.h lib/ip6.h lib/byte.h lib/uint16.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)dns_dtda.o: lib/dns/dns_dtda.c lib/dns.h lib/iopause.h lib/io.h lib/windoze.h lib/iarray.h lib/uint64.h lib/typedefs.h lib/taia.h lib/tai.h lib/uint32.h lib/stralloc.h lib/stralloc.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)dns_ipq.o: lib/dns/dns_ipq.c lib/byte.h lib/typedefs.h lib/case.h lib/dns.h lib/iopause.h lib/io.h lib/windoze.h lib/iarray.h lib/uint64.h lib/taia.h lib/tai.h lib/uint32.h lib/stralloc.h lib/str.h lib/stralloc.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)dns_domain.o: lib/dns/dns_domain.c lib/byte.h lib/typedefs.h lib/case.h lib/dns.h lib/iopause.h lib/io.h lib/windoze.h lib/iarray.h lib/uint64.h lib/taia.h lib/tai.h lib/uint32.h lib/stralloc.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)dns_dfd.o: lib/dns/dns_dfd.c lib/byte.h lib/typedefs.h lib/dns.h lib/iopause.h lib/io.h lib/windoze.h lib/iarray.h lib/uint64.h lib/taia.h lib/tai.h lib/uint32.h lib/stralloc.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)dns_sortip.o: lib/dns/dns_sortip.c lib/byte.h lib/typedefs.h lib/dns.h lib/iopause.h lib/io.h lib/windoze.h lib/iarray.h lib/uint64.h lib/taia.h lib/tai.h lib/uint32.h lib/stralloc.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)dns_name.o: lib/dns/dns_name.c lib/byte.h lib/typedefs.h lib/dns.h lib/iopause.h lib/io.h lib/windoze.h lib/iarray.h lib/uint64.h lib/taia.h lib/tai.h lib/uint32.h lib/stralloc.h lib/ip6.h lib/byte.h lib/stralloc.h lib/uint16.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)dns_nd.o: lib/dns/dns_nd.c lib/byte.h lib/typedefs.h lib/dns.h lib/iopause.h lib/io.h lib/windoze.h lib/iarray.h lib/uint64.h lib/taia.h lib/tai.h lib/uint32.h lib/stralloc.h lib/fmt.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)dns_nd6.o: lib/dns/dns_nd6.c lib/byte.h lib/typedefs.h lib/dns.h lib/iopause.h lib/io.h lib/windoze.h lib/iarray.h lib/uint64.h lib/taia.h lib/tai.h lib/uint32.h lib/stralloc.h lib/fmt.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)dns_sortip6.o: lib/dns/dns_sortip6.c lib/byte.h lib/typedefs.h lib/dns.h lib/iopause.h lib/io.h lib/windoze.h lib/iarray.h lib/uint64.h lib/taia.h lib/tai.h lib/uint32.h lib/stralloc.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)dns_ip4.o: lib/dns/dns_ip4.c lib/byte.h lib/typedefs.h lib/dns.h lib/iopause.h lib/io.h lib/windoze.h lib/iarray.h lib/uint64.h lib/taia.h lib/tai.h lib/uint32.h lib/stralloc.h lib/uint16.h lib/stralloc.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)dns_resolve.o: lib/dns/dns_resolve.c lib/byte.h lib/typedefs.h lib/dns.h lib/iopause.h lib/io.h lib/windoze.h lib/iarray.h lib/uint64.h lib/taia.h lib/tai.h lib/uint32.h lib/stralloc.h lib/io.h lib/iopause.h lib/ip6.h lib/byte.h lib/taia.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)dns_ipq6.o: lib/dns/dns_ipq6.c lib/byte.h lib/typedefs.h lib/case.h lib/dns.h lib/iopause.h lib/io.h lib/windoze.h lib/iarray.h lib/uint64.h lib/taia.h lib/tai.h lib/uint32.h lib/stralloc.h lib/str.h lib/stralloc.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)dns_packet.o: lib/dns/dns_packet.c lib/dns.h lib/iopause.h lib/io.h lib/windoze.h lib/iarray.h lib/uint64.h lib/typedefs.h lib/taia.h lib/tai.h lib/uint32.h lib/stralloc.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)dns_rcrw.o: lib/dns/dns_rcrw.c lib/windoze.h lib/byte.h lib/typedefs.h lib/dns.h lib/iopause.h lib/io.h lib/windoze.h lib/iarray.h lib/uint64.h lib/taia.h lib/tai.h lib/uint32.h lib/stralloc.h lib/open.h lib/str.h lib/stralloc.h lib/taia.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)dns_random.o: lib/dns/dns_random.c lib/windoze.h lib/dns.h lib/iopause.h lib/io.h lib/windoze.h lib/iarray.h lib/uint64.h lib/typedefs.h lib/taia.h lib/tai.h lib/uint32.h lib/stralloc.h lib/taia.h lib/uint32.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)dir_path.o: lib/dir/dir_path.c lib/stralloc.h lib/typedefs.h lib/dir_internal.h lib/dir.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)dir_type.o: lib/dir/dir_type.c lib/dir_internal.h lib/dir.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)dir_time.o: lib/dir/dir_time.c lib/dir.h lib/uint64.h lib/dir_internal.h lib/dir.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)dir_open.o: lib/dir/dir_open.c lib/windoze.h lib/dir_internal.h lib/dir.h lib/uint64.h lib/str.h lib/typedefs.h lib/stralloc.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)dir_close.o: lib/dir/dir_close.c lib/windoze.h lib/dir_internal.h lib/dir.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)dir_read.o: lib/dir/dir_read.c lib/dir_internal.h lib/dir.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)dir_name.o: lib/dir/dir_name.c lib/dir_internal.h lib/dir.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)dir_size.o: lib/dir/dir_size.c lib/windoze.h lib/uint64.h lib/dir_internal.h lib/dir.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)omf_open.o: lib/omf/omf_open.c lib/omf.h lib/typedefs.h lib/uint8.h lib/uint16.h lib/range.h lib/mmap.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)omf_get_name.o: lib/omf/omf_get_name.c lib/omf.h lib/typedefs.h lib/uint8.h lib/uint16.h lib/range.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)omf_close.o: lib/omf/omf_close.c lib/omf.h lib/typedefs.h lib/uint8.h lib/uint16.h lib/range.h lib/mmap.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)omf_record_next.o: lib/omf/omf_record_next.c lib/omf.h lib/typedefs.h lib/uint8.h lib/uint16.h lib/range.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)omf_begin.o: lib/omf/omf_begin.c lib/omf.h lib/typedefs.h lib/uint8.h lib/uint16.h lib/range.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)omf_name.o: lib/omf/omf_name.c lib/omf.h lib/typedefs.h lib/uint8.h lib/uint16.h lib/range.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)omf_end.o: lib/omf/omf_end.c lib/omf.h lib/typedefs.h lib/uint8.h lib/uint16.h lib/range.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)omf_record_begin.o: lib/omf/omf_record_begin.c lib/omf.h lib/typedefs.h lib/uint8.h lib/uint16.h lib/range.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)omf_record_end.o: lib/omf/omf_record_end.c lib/omf.h lib/typedefs.h lib/uint8.h lib/uint16.h lib/range.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)omf_next.o: lib/omf/omf_next.c lib/omf.h lib/typedefs.h lib/uint8.h lib/uint16.h lib/range.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)omf_data.o: lib/omf/omf_data.c lib/omf.h lib/typedefs.h lib/uint8.h lib/uint16.h lib/range.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)sig_unblock.o: lib/sig/sig_unblock.c lib/windoze.h lib/sig.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)sig_block.o: lib/sig/sig_block.c lib/windoze.h lib/sig.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)range_init.o: lib/range/range_init.c lib/range.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)linked_list_free.o: lib/hashmap/linked_list_free.c lib/linked_list.h lib/typedefs.h lib/memory.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)hashmap_init.o: lib/hashmap/hashmap_init.c lib/byte.h lib/typedefs.h lib/linked_list.h lib/memory.h lib/hashmap.h lib/linked_list.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)hashmap_free.o: lib/hashmap/hashmap_free.c lib/linked_list.h lib/typedefs.h lib/memory.h lib/hashmap.h lib/linked_list.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)hashmap_remove.o: lib/hashmap/hashmap_remove.c lib/linked_list.h lib/typedefs.h lib/memory.h lib/hashmap.h lib/linked_list.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)hashmap_default_comparator.o: lib/hashmap/hashmap_default_comparator.c lib/linked_list.h lib/typedefs.h lib/memory.h lib/hashmap.h lib/linked_list.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)hashmap_size.o: lib/hashmap/hashmap_size.c lib/linked_list.h lib/typedefs.h lib/memory.h lib/hashmap.h lib/linked_list.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)linked_list_append.o: lib/hashmap/linked_list_append.c lib/linked_list.h lib/typedefs.h lib/memory.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)hashmap_put.o: lib/hashmap/hashmap_put.c lib/linked_list.h lib/typedefs.h lib/memory.h lib/hashmap.h lib/linked_list.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)linked_list_size.o: lib/hashmap/linked_list_size.c lib/linked_list.h lib/typedefs.h lib/memory.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)hashmap_contains_key.o: lib/hashmap/hashmap_contains_key.c lib/linked_list.h lib/typedefs.h lib/memory.h lib/hashmap.h lib/linked_list.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)linked_list_prepend.o: lib/hashmap/linked_list_prepend.c lib/linked_list.h lib/typedefs.h lib/memory.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)linked_list_init.o: lib/hashmap/linked_list_init.c lib/linked_list.h lib/typedefs.h lib/memory.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)hashmap_default_hash_func.o: lib/hashmap/hashmap_default_hash_func.c lib/linked_list.h lib/typedefs.h lib/memory.h lib/hashmap.h lib/linked_list.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)linked_list_head.o: lib/hashmap/linked_list_head.c lib/linked_list.h lib/typedefs.h lib/memory.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)hashmap_get.o: lib/hashmap/hashmap_get.c lib/linked_list.h lib/typedefs.h lib/memory.h lib/hashmap.h lib/linked_list.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)hashmap_clear.o: lib/hashmap/hashmap_clear.c lib/linked_list.h lib/typedefs.h lib/memory.h lib/hashmap.h lib/linked_list.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)hashmap_keys.o: lib/hashmap/hashmap_keys.c lib/linked_list.h lib/typedefs.h lib/memory.h lib/hashmap.h lib/linked_list.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)uint16_pack_big.o: lib/uint16/uint16_pack_big.c lib/uint16.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)uint16_pack.o: lib/uint16/uint16_pack.c lib/uint16.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)uint16_unpack_big.o: lib/uint16/uint16_unpack_big.c lib/uint16.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)uint16_unpack.o: lib/uint16/uint16_unpack.c lib/uint16.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)uint16_read_big.o: lib/uint16/uint16_read_big.c lib/uint16.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)uint16_read.o: lib/uint16/uint16_read.c lib/uint16.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)errmsg_infosys.o: lib/errmsg/errmsg_infosys.c lib/errmsg.h lib/errmsg_internal.h lib/uint64.h lib/windoze.h lib/errmsg.h lib/str.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)errmsg_warnsys.o: lib/errmsg/errmsg_warnsys.c lib/errmsg.h lib/errmsg_internal.h lib/uint64.h lib/windoze.h lib/errmsg.h lib/str.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)errmsg_write.o: lib/errmsg/errmsg_write.c lib/errmsg.h lib/errmsg_internal.h lib/uint64.h lib/windoze.h lib/errmsg.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)errmsg_puts.o: lib/errmsg/errmsg_puts.c lib/errmsg_internal.h lib/uint64.h lib/windoze.h lib/errmsg.h lib/str.h lib/typedefs.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)errmsg_warn.o: lib/errmsg/errmsg_warn.c lib/errmsg.h lib/errmsg_internal.h lib/uint64.h lib/windoze.h lib/errmsg.h lib/str.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)errmsg_iam.o: lib/errmsg/errmsg_iam.c lib/errmsg.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)errmsg_info.o: lib/errmsg/errmsg_info.c lib/errmsg.h lib/errmsg_internal.h lib/uint64.h lib/windoze.h lib/errmsg.h lib/str.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)fmt_longlong.o: lib/fmt/fmt_longlong.c lib/fmt.h lib/typedefs.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)fmt_escapecharshell.o: lib/fmt/fmt_escapecharshell.c lib/fmt.h lib/typedefs.h lib/uint32.h lib/uint64.h lib/uint32.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)fmt_iso8601.o: lib/fmt/fmt_iso8601.c lib/fmt.h lib/typedefs.h lib/uint32.h lib/uint64.h lib/byte.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)fmt_escapecharhtml.o: lib/fmt/fmt_escapecharhtml.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)fmt_strm_internal.o: lib/fmt/fmt_strm_internal.c lib/fmt.h lib/typedefs.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)fmt_hexb.o: lib/fmt/fmt_hexb.c lib/fmt.h lib/typedefs.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)fmt_escapecharc.o: lib/fmt/fmt_escapecharc.c lib/fmt.h lib/typedefs.h lib/uint32.h lib/uint64.h lib/uint32.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)fmt_fill.o: lib/fmt/fmt_fill.c lib/fmt.h lib/typedefs.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)fmt_asn1derlength.o: lib/fmt/fmt_asn1derlength.c lib/fmt.h lib/typedefs.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)fmt_ulong.o: lib/fmt/fmt_ulong.c lib/fmt.h lib/typedefs.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)fmt_asn1dertag.o: lib/fmt/fmt_asn1dertag.c lib/fmt.h lib/typedefs.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)fmt_octal.o: lib/fmt/fmt_octal.c lib/fmt.h lib/typedefs.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)fmt_ulong0.o: lib/fmt/fmt_ulong0.c lib/fmt.h lib/typedefs.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)fmt_tohex.o: lib/fmt/fmt_tohex.c lib/fmt.h lib/typedefs.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)fmt_humank.o: lib/fmt/fmt_humank.c lib/fmt.h lib/typedefs.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)fmt_escapecharquotedprintableutf8.o: lib/fmt/fmt_escapecharquotedprintableutf8.c lib/fmt.h lib/typedefs.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)fmt_strn.o: lib/fmt/fmt_strn.c lib/fmt.h lib/typedefs.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)fmt_tai.o: lib/fmt/fmt_tai.c lib/fmt.h lib/typedefs.h lib/uint32.h lib/uint64.h lib/tai.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)fmt_escapecharjson.o: lib/fmt/fmt_escapecharjson.c lib/fmt.h lib/typedefs.h lib/uint32.h lib/uint64.h lib/uint16.h lib/uint32.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)fmt_utf8.o: lib/fmt/fmt_utf8.c lib/fmt.h lib/typedefs.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)fmt_human.o: lib/fmt/fmt_human.c lib/fmt.h lib/typedefs.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)fmt_httpdate.o: lib/fmt/fmt_httpdate.c lib/byte.h lib/typedefs.h lib/fmt.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)fmt_repeat.o: lib/fmt/fmt_repeat.c lib/fmt.h lib/typedefs.h lib/uint32.h lib/uint64.h lib/byte.h lib/str.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)fmt_escapecharxml.o: lib/fmt/fmt_escapecharxml.c lib/byte.h lib/typedefs.h lib/fmt.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)fmt_8long.o: lib/fmt/fmt_8long.c lib/fmt.h lib/typedefs.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)fmt_xlonglong.o: lib/fmt/fmt_xlonglong.c lib/fmt.h lib/typedefs.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)fmt_ulonglong.o: lib/fmt/fmt_ulonglong.c lib/fmt.h lib/typedefs.h lib/uint32.h lib/uint64.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)fmt_long.o: lib/fmt/fmt_long.c lib/fmt.h lib/typedefs.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)fmt_xlong.o: lib/fmt/fmt_xlong.c lib/fmt.h lib/typedefs.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)fmt_pad.o: lib/fmt/fmt_pad.c lib/fmt.h lib/typedefs.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)fmt_minus.o: lib/fmt/fmt_minus.c lib/fmt.h lib/typedefs.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)fmt_str.o: lib/fmt/fmt_str.c lib/fmt.h lib/typedefs.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)fmt_plusminus.o: lib/fmt/fmt_plusminus.c lib/fmt.h lib/typedefs.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)fmt_escapecharquotedprintable.o: lib/fmt/fmt_escapecharquotedprintable.c lib/fmt.h lib/typedefs.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)fmt_double.o: lib/fmt/fmt_double.c lib/fmt.h lib/typedefs.h lib/uint32.h lib/uint64.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)fmt_xmlescape.o: lib/fmt/fmt_xmlescape.c lib/fmt.h lib/typedefs.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ucs_latin1.o: lib/ucs/ucs_latin1.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)str_utf8_latin.o: lib/ucs/str_utf8_latin.c lib/ucs.h lib/typedefs.h lib/str.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)fmt_latin1_utf8.o: lib/ucs/fmt_latin1_utf8.c lib/ucs.h lib/typedefs.h lib/fmt.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)stralloc_utf8_latin1.o: lib/ucs/stralloc_utf8_latin1.c lib/stralloc.h lib/typedefs.h lib/ucs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)scan_latin1_utf8.o: lib/ucs/scan_latin1_utf8.c lib/ucs.h lib/typedefs.h lib/scan.h lib/uint32.h lib/uint64.h lib/buffer.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)stralloc_utf8_to_latin1.o: lib/ucs/stralloc_utf8_to_latin1.c lib/stralloc.h lib/typedefs.h lib/ucs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)stralloc_latin1_to_utf8.o: lib/ucs/stralloc_latin1_to_utf8.c lib/stralloc.h lib/typedefs.h lib/ucs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)setenv.o: lib/setenv.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)taia_approx.o: lib/taia/taia_approx.c lib/taia.h lib/tai.h lib/uint64.h lib/uint32.h lib/tai.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)taia_frac.o: lib/taia/taia_frac.c lib/taia.h lib/tai.h lib/uint64.h lib/uint32.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)taia_pack.o: lib/taia/taia_pack.c lib/taia.h lib/tai.h lib/uint64.h lib/uint32.h lib/tai.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)taia_tai.o: lib/taia/taia_tai.c lib/taia.h lib/tai.h lib/uint64.h lib/uint32.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)taia_less.o: lib/taia/taia_less.c lib/taia.h lib/tai.h lib/uint64.h lib/uint32.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)taia_half.o: lib/taia/taia_half.c lib/taia.h lib/tai.h lib/uint64.h lib/uint32.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)taia_now.o: lib/taia/taia_now.c lib/windoze.h lib/taia.h lib/tai.h lib/uint64.h lib/uint32.h lib/tai.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)taia_unpack.o: lib/taia/taia_unpack.c lib/taia.h lib/tai.h lib/uint64.h lib/uint32.h lib/tai.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)taia_addsec.o: lib/taia/taia_addsec.c lib/taia.h lib/tai.h lib/uint64.h lib/uint32.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)taia_sub.o: lib/taia/taia_sub.c lib/taia.h lib/tai.h lib/uint64.h lib/uint32.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)taia_uint.o: lib/taia/taia_uint.c lib/taia.h lib/tai.h lib/uint64.h lib/uint32.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)taia_add.o: lib/taia/taia_add.c lib/taia.h lib/tai.h lib/uint64.h lib/uint32.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cbmap_get.o: lib/cbmap/cbmap_get.c lib/byte.h lib/typedefs.h lib/cbmap_internal.h lib/cbmap.h lib/uint8.h lib/cbmap/cbmap_alloc.h lib/str.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cbmap_data_node_destroy.o: lib/cbmap/cbmap_data_node_destroy.c lib/cbmap_internal.h lib/cbmap.h lib/uint8.h lib/cbmap/cbmap_alloc.h lib/str.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cbmap_delete.o: lib/cbmap/cbmap_delete.c lib/byte.h lib/typedefs.h lib/cbmap_internal.h lib/cbmap.h lib/uint8.h lib/cbmap/cbmap_alloc.h lib/str.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cbmap_visit.o: lib/cbmap/cbmap_visit.c lib/cbmap_internal.h lib/cbmap.h lib/uint8.h lib/cbmap/cbmap_alloc.h lib/str.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cbmap_insert.o: lib/cbmap/cbmap_insert.c lib/byte.h lib/typedefs.h lib/cbmap/cbmap_alloc.h lib/str.h lib/cbmap_internal.h lib/cbmap.h lib/uint8.h lib/cbmap/cbmap_alloc.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cbmap_internal_node.o: lib/cbmap/cbmap_internal_node.c lib/cbmap/cbmap_alloc.h lib/str.h lib/typedefs.h lib/cbmap_internal.h lib/cbmap.h lib/uint8.h lib/cbmap/cbmap_alloc.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cbmap_alloc.o: lib/cbmap/cbmap_alloc.c lib/windoze.h lib/cbmap/cbmap_alloc.h lib/str.h lib/typedefs.h lib/memalign.h lib/byte.h lib/cbmap_internal.h lib/cbmap.h lib/uint8.h lib/cbmap/cbmap_alloc.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cbmap_visit_all.o: lib/cbmap/cbmap_visit_all.c lib/cbmap_internal.h lib/cbmap.h lib/uint8.h lib/cbmap/cbmap_alloc.h lib/str.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)memalign.o: lib/cbmap/memalign.c lib/memalign.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cbmap_destroy.o: lib/cbmap/cbmap_destroy.c lib/cbmap_internal.h lib/cbmap.h lib/uint8.h lib/cbmap/cbmap_alloc.h lib/str.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cbmap_count.o: lib/cbmap/cbmap_count.c lib/cbmap_internal.h lib/cbmap.h lib/uint8.h lib/cbmap/cbmap_alloc.h lib/str.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cbmap_new.o: lib/cbmap/cbmap_new.c lib/byte.h lib/typedefs.h lib/cbmap_internal.h lib/cbmap.h lib/uint8.h lib/cbmap/cbmap_alloc.h lib/str.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)var_export.o: lib/var/var_export.c lib/byte.h lib/typedefs.h lib/var.h lib/uint64.h lib/stralloc.h lib/vartab.h lib/var.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)var_rndhash.o: lib/var/var_rndhash.c lib/uint32.h lib/uint64.h lib/var.h lib/uint64.h lib/stralloc.h lib/typedefs.h lib/vartab.h lib/var.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)var_get.o: lib/var/var_get.c lib/vartab.h lib/uint64.h lib/var.h lib/stralloc.h lib/typedefs.h lib/vartab.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)var_create.o: lib/var/var_create.c lib/vartab.h lib/uint64.h lib/var.h lib/stralloc.h lib/typedefs.h lib/vartab.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)var_setvsa.o: lib/var/var_setvsa.c lib/byte.h lib/typedefs.h lib/var.h lib/uint64.h lib/stralloc.h lib/vartab.h lib/var.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)var_setsa.o: lib/var/var_setsa.c lib/var.h lib/uint64.h lib/stralloc.h lib/typedefs.h lib/vartab.h lib/var.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)var_import.o: lib/var/var_import.c lib/str.h lib/typedefs.h lib/vartab.h lib/uint64.h lib/var.h lib/stralloc.h lib/vartab.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)var_count.o: lib/var/var_count.c lib/var.h lib/uint64.h lib/stralloc.h lib/typedefs.h lib/vartab.h lib/var.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)var_lexhash.o: lib/var/var_lexhash.c lib/fmt.h lib/typedefs.h lib/uint32.h lib/uint64.h lib/var.h lib/stralloc.h lib/vartab.h lib/var.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)var_len.o: lib/var/var_len.c lib/var.h lib/uint64.h lib/stralloc.h lib/typedefs.h lib/vartab.h lib/var.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)var_print.o: lib/var/var_print.c lib/str.h lib/typedefs.h lib/var.h lib/uint64.h lib/stralloc.h lib/vartab.h lib/var.h lib/buffer.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)var_set.o: lib/var/var_set.c lib/byte.h lib/typedefs.h lib/str.h lib/var.h lib/uint64.h lib/stralloc.h lib/vartab.h lib/var.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)var_vlen.o: lib/var/var_vlen.c lib/var.h lib/uint64.h lib/stralloc.h lib/typedefs.h lib/vartab.h lib/var.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)var_copys.o: lib/var/var_copys.c lib/var.h lib/uint64.h lib/stralloc.h lib/typedefs.h lib/vartab.h lib/var.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)var_search.o: lib/var/var_search.c lib/vartab.h lib/uint64.h lib/var.h lib/stralloc.h lib/typedefs.h lib/vartab.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)var_init.o: lib/var/var_init.c lib/byte.h lib/typedefs.h lib/var.h lib/uint64.h lib/stralloc.h lib/vartab.h lib/var.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)var_hsearch.o: lib/var/var_hsearch.c lib/vartab.h lib/uint64.h lib/var.h lib/stralloc.h lib/typedefs.h lib/vartab.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)var_cleanup.o: lib/var/var_cleanup.c lib/var.h lib/uint64.h lib/stralloc.h lib/typedefs.h lib/vartab.h lib/var.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)var_unset.o: lib/var/var_unset.c lib/var.h lib/uint64.h lib/stralloc.h lib/typedefs.h lib/vartab.h lib/var.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)var_dump.o: lib/var/var_dump.c lib/fmt.h lib/typedefs.h lib/uint32.h lib/uint64.h lib/var.h lib/stralloc.h lib/vartab.h lib/var.h lib/buffer.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)var_bsearch.o: lib/var/var_bsearch.c lib/vartab.h lib/uint64.h lib/var.h lib/stralloc.h lib/typedefs.h lib/vartab.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)var_setvint.o: lib/var/var_setvint.c lib/var.h lib/uint64.h lib/stralloc.h lib/typedefs.h lib/vartab.h lib/var.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)var_value.o: lib/var/var_value.c lib/var.h lib/uint64.h lib/stralloc.h lib/typedefs.h lib/vartab.h lib/var.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)var_valid.o: lib/var/var_valid.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)var_chflg.o: lib/var/var_chflg.c lib/vartab.h lib/uint64.h lib/var.h lib/stralloc.h lib/typedefs.h lib/vartab.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)var_vdefault.o: lib/var/var_vdefault.c lib/str.h lib/typedefs.h lib/var.h lib/uint64.h lib/stralloc.h lib/vartab.h lib/var.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)xml_get_document.o: lib/xml/xml_get_document.c lib/xml.h lib/buffer.h lib/typedefs.h lib/hmap.h lib/stralloc.h lib/textbuf.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)xml_find_predicates.o: lib/xml/xml_find_predicates.c lib/str.h lib/typedefs.h lib/strlist.h lib/stralloc.h lib/byte.h lib/uint64.h lib/buffer.h lib/xml.h lib/hmap.h lib/textbuf.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)xml_get_attribute_sa.o: lib/xml/xml_get_attribute_sa.c lib/xml.h lib/buffer.h lib/typedefs.h lib/hmap.h lib/stralloc.h lib/textbuf.h lib/uint64.h lib/stralloc.h lib/hmap.h lib/str.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)xml_textnode.o: lib/xml/xml_textnode.c lib/xml.h lib/buffer.h lib/typedefs.h lib/hmap.h lib/stralloc.h lib/textbuf.h lib/uint64.h lib/str.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)xml_element.o: lib/xml/xml_element.c lib/xml.h lib/buffer.h lib/typedefs.h lib/hmap.h lib/stralloc.h lib/textbuf.h lib/uint64.h lib/str.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)xml_read_callback.o: lib/xml/xml_read_callback.c lib/byte.h lib/typedefs.h lib/stralloc.h lib/xml.h lib/buffer.h lib/hmap.h lib/stralloc.h lib/textbuf.h lib/uint64.h lib/str.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)xml_path.o: lib/xml/xml_path.c lib/xml.h lib/buffer.h lib/typedefs.h lib/hmap.h lib/stralloc.h lib/textbuf.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)xml_set_attribute_double.o: lib/xml/xml_set_attribute_double.c lib/xml.h lib/buffer.h lib/typedefs.h lib/hmap.h lib/stralloc.h lib/textbuf.h lib/uint64.h lib/str.h lib/fmt.h lib/uint32.h lib/byte.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)xml_attrnode.o: lib/xml/xml_attrnode.c lib/xml.h lib/buffer.h lib/typedefs.h lib/hmap.h lib/stralloc.h lib/textbuf.h lib/uint64.h lib/str.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)xml_read_tree.o: lib/xml/xml_read_tree.c lib/byte.h lib/typedefs.h lib/fmt.h lib/uint32.h lib/uint64.h lib/scan.h lib/xml.h lib/buffer.h lib/hmap.h lib/stralloc.h lib/textbuf.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)xml_find_parent.o: lib/xml/xml_find_parent.c lib/buffer.h lib/typedefs.h lib/strlist.h lib/stralloc.h lib/byte.h lib/uint64.h lib/buffer.h lib/xml.h lib/hmap.h lib/textbuf.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)xml_delete.o: lib/xml/xml_delete.c lib/xml.h lib/buffer.h lib/typedefs.h lib/hmap.h lib/stralloc.h lib/textbuf.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)xml_get_attribute_double.o: lib/xml/xml_get_attribute_double.c lib/xml.h lib/buffer.h lib/typedefs.h lib/hmap.h lib/stralloc.h lib/textbuf.h lib/uint64.h lib/scan.h lib/uint32.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)xml_find_element_attr.o: lib/xml/xml_find_element_attr.c lib/strlist.h lib/stralloc.h lib/typedefs.h lib/byte.h lib/uint64.h lib/buffer.h lib/xml.h lib/hmap.h lib/textbuf.h lib/str.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)xml_print_attributes.o: lib/xml/xml_print_attributes.c lib/uint64.h lib/buffer.h lib/typedefs.h lib/hmap.h lib/stralloc.h lib/buffer.h lib/xml.h lib/hmap.h lib/textbuf.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)xml_escape.o: lib/xml/xml_escape.c lib/byte.h lib/typedefs.h lib/fmt.h lib/uint32.h lib/uint64.h lib/stralloc.h lib/xml.h lib/buffer.h lib/hmap.h lib/stralloc.h lib/textbuf.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)xml_set_attribute.o: lib/xml/xml_set_attribute.c lib/xml.h lib/buffer.h lib/typedefs.h lib/hmap.h lib/stralloc.h lib/textbuf.h lib/uint64.h lib/hmap.h lib/str.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)xml_newnode.o: lib/xml/xml_newnode.c lib/alloc.h lib/typedefs.h lib/xml.h lib/buffer.h lib/hmap.h lib/stralloc.h lib/textbuf.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)xml_debug.o: lib/xml/xml_debug.c lib/fmt.h lib/typedefs.h lib/uint32.h lib/uint64.h lib/xml.h lib/buffer.h lib/hmap.h lib/stralloc.h lib/textbuf.h lib/str.h lib/stralloc.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)xml_print.o: lib/xml/xml_print.c lib/xml.h lib/buffer.h lib/typedefs.h lib/hmap.h lib/stralloc.h lib/textbuf.h lib/uint64.h lib/str.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)xml_find_element.o: lib/xml/xml_find_element.c lib/buffer.h lib/typedefs.h lib/strlist.h lib/stralloc.h lib/byte.h lib/uint64.h lib/buffer.h lib/xml.h lib/hmap.h lib/textbuf.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)xml_add_child.o: lib/xml/xml_add_child.c lib/xml.h lib/buffer.h lib/typedefs.h lib/hmap.h lib/stralloc.h lib/textbuf.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)xml_remove.o: lib/xml/xml_remove.c lib/xml.h lib/buffer.h lib/typedefs.h lib/hmap.h lib/stralloc.h lib/textbuf.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)xml_reader_init.o: lib/xml/xml_reader_init.c lib/byte.h lib/typedefs.h lib/xml.h lib/buffer.h lib/hmap.h lib/stralloc.h lib/textbuf.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)xml_walk.o: lib/xml/xml_walk.c lib/xml.h lib/buffer.h lib/typedefs.h lib/hmap.h lib/stralloc.h lib/textbuf.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)xml_find_pred.o: lib/xml/xml_find_pred.c lib/xml.h lib/buffer.h lib/typedefs.h lib/hmap.h lib/stralloc.h lib/textbuf.h lib/uint64.h lib/strlist.h lib/byte.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)xml_print_nodeset.o: lib/xml/xml_print_nodeset.c lib/xml.h lib/buffer.h lib/typedefs.h lib/hmap.h lib/stralloc.h lib/textbuf.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)xml_find_all_pred.o: lib/xml/xml_find_all_pred.c lib/array.h lib/typedefs.h lib/uint64.h lib/byte.h lib/strlist.h lib/stralloc.h lib/byte.h lib/buffer.h lib/xml.h lib/hmap.h lib/textbuf.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)xml_has_attribute.o: lib/xml/xml_has_attribute.c lib/xml.h lib/buffer.h lib/typedefs.h lib/hmap.h lib/stralloc.h lib/textbuf.h lib/uint64.h lib/str.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)xml_free.o: lib/xml/xml_free.c lib/xml.h lib/buffer.h lib/typedefs.h lib/hmap.h lib/stralloc.h lib/textbuf.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)xml_content_sa.o: lib/xml/xml_content_sa.c lib/xml.h lib/buffer.h lib/typedefs.h lib/hmap.h lib/stralloc.h lib/textbuf.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)xml_insert.o: lib/xml/xml_insert.c lib/xml.h lib/buffer.h lib/typedefs.h lib/hmap.h lib/stralloc.h lib/textbuf.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)xml_root_element.o: lib/xml/xml_root_element.c lib/xml.h lib/buffer.h lib/typedefs.h lib/hmap.h lib/stralloc.h lib/textbuf.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)xml_get_attribute.o: lib/xml/xml_get_attribute.c lib/xml.h lib/buffer.h lib/typedefs.h lib/hmap.h lib/stralloc.h lib/textbuf.h lib/uint64.h lib/str.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)xml_content.o: lib/xml/xml_content.c lib/xml.h lib/buffer.h lib/typedefs.h lib/hmap.h lib/stralloc.h lib/textbuf.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)strlist_shift.o: lib/strlist/strlist_shift.c lib/byte.h lib/typedefs.h lib/str.h lib/strlist.h lib/stralloc.h lib/byte.h lib/uint64.h lib/buffer.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)strlist_at_n.o: lib/strlist/strlist_at_n.c lib/strlist.h lib/stralloc.h lib/typedefs.h lib/byte.h lib/uint64.h lib/buffer.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)strlist_to_argv.o: lib/strlist/strlist_to_argv.c lib/strlist.h lib/stralloc.h lib/typedefs.h lib/byte.h lib/uint64.h lib/buffer.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)strlist_indexofb.o: lib/strlist/strlist_indexofb.c lib/byte.h lib/typedefs.h lib/strlist.h lib/stralloc.h lib/byte.h lib/uint64.h lib/buffer.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)strlist_fromv.o: lib/strlist/strlist_fromv.c lib/str.h lib/typedefs.h lib/strlist.h lib/stralloc.h lib/byte.h lib/uint64.h lib/buffer.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)strlist_fromb.o: lib/strlist/strlist_fromb.c lib/byte.h lib/typedefs.h lib/strlist.h lib/stralloc.h lib/byte.h lib/uint64.h lib/buffer.h lib/str.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)strlist_push_unique_sa.o: lib/strlist/strlist_push_unique_sa.c lib/strlist.h lib/stralloc.h lib/typedefs.h lib/byte.h lib/uint64.h lib/buffer.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)strlist_push.o: lib/strlist/strlist_push.c lib/byte.h lib/typedefs.h lib/str.h lib/strlist.h lib/stralloc.h lib/byte.h lib/uint64.h lib/buffer.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)strlist_prepend_sa.o: lib/strlist/strlist_prepend_sa.c lib/byte.h lib/typedefs.h lib/strlist.h lib/stralloc.h lib/byte.h lib/uint64.h lib/buffer.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)strlist_sub.o: lib/strlist/strlist_sub.c lib/strlist.h lib/stralloc.h lib/typedefs.h lib/byte.h lib/uint64.h lib/buffer.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)strlist_sort.o: lib/strlist/strlist_sort.c lib/str.h lib/typedefs.h lib/strlist.h lib/stralloc.h lib/byte.h lib/uint64.h lib/buffer.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)strlist_at.o: lib/strlist/strlist_at.c lib/strlist.h lib/stralloc.h lib/typedefs.h lib/byte.h lib/uint64.h lib/buffer.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)strlist_pushb.o: lib/strlist/strlist_pushb.c lib/byte.h lib/typedefs.h lib/strlist.h lib/stralloc.h lib/byte.h lib/uint64.h lib/buffer.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)strlist_pushm_internal.o: lib/strlist/strlist_pushm_internal.c lib/strlist.h lib/stralloc.h lib/typedefs.h lib/byte.h lib/uint64.h lib/buffer.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)strlist_index_of.o: lib/strlist/strlist_index_of.c lib/byte.h lib/typedefs.h lib/str.h lib/strlist.h lib/stralloc.h lib/byte.h lib/uint64.h lib/buffer.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)strlist_join.o: lib/strlist/strlist_join.c lib/byte.h lib/typedefs.h lib/stralloc.h lib/strlist.h lib/stralloc.h lib/byte.h lib/uint64.h lib/buffer.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)strlist_at_sa.o: lib/strlist/strlist_at_sa.c lib/byte.h lib/typedefs.h lib/strlist.h lib/stralloc.h lib/byte.h lib/uint64.h lib/buffer.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)strlist_dump.o: lib/strlist/strlist_dump.c lib/buffer.h lib/typedefs.h lib/fmt.h lib/uint32.h lib/uint64.h lib/strlist.h lib/stralloc.h lib/byte.h lib/buffer.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)strlist_push_tokens.o: lib/strlist/strlist_push_tokens.c lib/str.h lib/typedefs.h lib/strlist.h lib/stralloc.h lib/byte.h lib/uint64.h lib/buffer.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)strlist_pop.o: lib/strlist/strlist_pop.c lib/strlist.h lib/stralloc.h lib/typedefs.h lib/byte.h lib/uint64.h lib/buffer.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)strlist_count.o: lib/strlist/strlist_count.c lib/strlist.h lib/stralloc.h lib/typedefs.h lib/byte.h lib/uint64.h lib/buffer.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)strlist_range.o: lib/strlist/strlist_range.c lib/stralloc.h lib/typedefs.h lib/strlist.h lib/stralloc.h lib/byte.h lib/uint64.h lib/buffer.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)strlist_cat.o: lib/strlist/strlist_cat.c lib/strlist.h lib/stralloc.h lib/typedefs.h lib/byte.h lib/uint64.h lib/buffer.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)strlist_containsb.o: lib/strlist/strlist_containsb.c lib/byte.h lib/typedefs.h lib/strlist.h lib/stralloc.h lib/byte.h lib/uint64.h lib/buffer.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)strlist_removeb.o: lib/strlist/strlist_removeb.c lib/strlist.h lib/stralloc.h lib/typedefs.h lib/byte.h lib/uint64.h lib/buffer.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)strlist_shift_n.o: lib/strlist/strlist_shift_n.c lib/strlist.h lib/stralloc.h lib/typedefs.h lib/byte.h lib/uint64.h lib/buffer.h lib/byte.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)strlist_push_unique.o: lib/strlist/strlist_push_unique.c lib/strlist.h lib/stralloc.h lib/typedefs.h lib/byte.h lib/uint64.h lib/buffer.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)strlist_unshift.o: lib/strlist/strlist_unshift.c lib/str.h lib/typedefs.h lib/strlist.h lib/stralloc.h lib/byte.h lib/uint64.h lib/buffer.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)strlist_append_sa.o: lib/strlist/strlist_append_sa.c lib/byte.h lib/typedefs.h lib/str.h lib/strlist.h lib/stralloc.h lib/byte.h lib/uint64.h lib/buffer.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)strlist_prependb.o: lib/strlist/strlist_prependb.c lib/byte.h lib/typedefs.h lib/strlist.h lib/stralloc.h lib/byte.h lib/uint64.h lib/buffer.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)strlist_pushb_unique.o: lib/strlist/strlist_pushb_unique.c lib/strlist.h lib/stralloc.h lib/typedefs.h lib/byte.h lib/uint64.h lib/buffer.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)strlist_joins.o: lib/strlist/strlist_joins.c lib/byte.h lib/typedefs.h lib/strlist.h lib/stralloc.h lib/byte.h lib/uint64.h lib/buffer.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)strlist_froms.o: lib/strlist/strlist_froms.c lib/str.h lib/typedefs.h lib/strlist.h lib/stralloc.h lib/byte.h lib/uint64.h lib/buffer.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)strlist_trunc.o: lib/strlist/strlist_trunc.c lib/strlist.h lib/stralloc.h lib/typedefs.h lib/byte.h lib/uint64.h lib/buffer.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)strlist_prepends.o: lib/strlist/strlist_prepends.c lib/str.h lib/typedefs.h lib/strlist.h lib/stralloc.h lib/byte.h lib/uint64.h lib/buffer.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)strlist_contains_sa.o: lib/strlist/strlist_contains_sa.c lib/byte.h lib/typedefs.h lib/stralloc.h lib/strlist.h lib/stralloc.h lib/byte.h lib/uint64.h lib/buffer.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)strlist_push_sa.o: lib/strlist/strlist_push_sa.c lib/byte.h lib/typedefs.h lib/str.h lib/strlist.h lib/stralloc.h lib/byte.h lib/uint64.h lib/buffer.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)strlist_removes.o: lib/strlist/strlist_removes.c lib/strlist.h lib/stralloc.h lib/typedefs.h lib/byte.h lib/uint64.h lib/buffer.h lib/str.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)strlist_contains.o: lib/strlist/strlist_contains.c lib/byte.h lib/typedefs.h lib/str.h lib/strlist.h lib/stralloc.h lib/byte.h lib/uint64.h lib/buffer.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)fmt_tofrom_array.o: lib/textcode/fmt_tofrom_array.c lib/array.h lib/typedefs.h lib/uint64.h lib/textcode.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)scan_uuencoded.o: lib/textcode/scan_uuencoded.c lib/textcode.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)fmt_xml.o: lib/textcode/fmt_xml.c lib/fmt.h lib/typedefs.h lib/uint32.h lib/uint64.h lib/textcode.h lib/str.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)fmt_yenc.o: lib/textcode/fmt_yenc.c lib/fmt.h lib/typedefs.h lib/uint32.h lib/uint64.h lib/textcode.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)scan_to_array.o: lib/textcode/scan_to_array.c lib/str.h lib/typedefs.h lib/array.h lib/uint64.h lib/textcode.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)fmt_to_array.o: lib/textcode/fmt_to_array.c lib/array.h lib/typedefs.h lib/uint64.h lib/textcode.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)scan_base64.o: lib/textcode/scan_base64.c lib/textcode.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)fmt_cescape.o: lib/textcode/fmt_cescape.c lib/fmt.h lib/typedefs.h lib/uint32.h lib/uint64.h lib/textcode.h lib/str.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)fmt_quotedprintable.o: lib/textcode/fmt_quotedprintable.c lib/fmt.h lib/typedefs.h lib/uint32.h lib/uint64.h lib/textcode.h lib/str.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)scan_html.o: lib/textcode/scan_html.c lib/typedefs.h lib/entities.h lib/uint32.h lib/fmt.h lib/typedefs.h lib/uint64.h lib/textcode.h lib/scan.h lib/case.h lib/str.h lib/byte.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)fmt_ldapescape2.o: lib/textcode/fmt_ldapescape2.c lib/fmt.h lib/typedefs.h lib/uint32.h lib/uint64.h lib/textcode.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)fmt_jsonescape.o: lib/textcode/fmt_jsonescape.c lib/byte.h lib/typedefs.h lib/str.h lib/fmt.h lib/uint32.h lib/uint64.h lib/textcode.h lib/scan.h lib/uint32.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)scan_quotedprintable.o: lib/textcode/scan_quotedprintable.c lib/fmt.h lib/typedefs.h lib/uint32.h lib/uint64.h lib/textcode.h lib/scan.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)fmt_stripwhitespace.o: lib/textcode/fmt_stripwhitespace.c lib/fmt.h lib/typedefs.h lib/uint32.h lib/uint64.h lib/scan.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)fmt_base64.o: lib/textcode/fmt_base64.c lib/fmt.h lib/typedefs.h lib/uint32.h lib/uint64.h lib/textcode.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)scan_yenc.o: lib/textcode/scan_yenc.c lib/fmt.h lib/typedefs.h lib/uint32.h lib/uint64.h lib/textcode.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)scan_jsonescape.o: lib/textcode/scan_jsonescape.c lib/fmt.h lib/typedefs.h lib/uint32.h lib/uint64.h lib/textcode.h lib/scan.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)scan_tofrom_array.o: lib/textcode/scan_tofrom_array.c lib/str.h lib/typedefs.h lib/array.h lib/uint64.h lib/textcode.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)fmt_foldwhitespace.o: lib/textcode/fmt_foldwhitespace.c lib/fmt.h lib/typedefs.h lib/uint32.h lib/uint64.h lib/textcode.h lib/str.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)fmt_ldapescape.o: lib/textcode/fmt_ldapescape.c lib/fmt.h lib/typedefs.h lib/uint32.h lib/uint64.h lib/textcode.h lib/str.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)fmt_urlencoded.o: lib/textcode/fmt_urlencoded.c lib/fmt.h lib/typedefs.h lib/uint32.h lib/uint64.h lib/textcode.h lib/str.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)scan_urlencoded.o: lib/textcode/scan_urlencoded.c lib/fmt.h lib/typedefs.h lib/uint32.h lib/uint64.h lib/textcode.h lib/scan.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)fmt_to_sa.o: lib/textcode/fmt_to_sa.c lib/stralloc.h lib/typedefs.h lib/textcode.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)fmt_uuencoded.o: lib/textcode/fmt_uuencoded.c lib/fmt.h lib/typedefs.h lib/uint32.h lib/uint64.h lib/textcode.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)scan_hexdump.o: lib/textcode/scan_hexdump.c lib/fmt.h lib/typedefs.h lib/uint32.h lib/uint64.h lib/textcode.h lib/scan.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)scan_ldapescape.o: lib/textcode/scan_ldapescape.c lib/fmt.h lib/typedefs.h lib/uint32.h lib/uint64.h lib/textcode.h lib/scan.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)scan_base64url.o: lib/textcode/scan_base64url.c lib/textcode.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)base64url.o: lib/textcode/base64url.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)fmt_html.o: lib/textcode/fmt_html.c lib/fmt.h lib/typedefs.h lib/uint32.h lib/uint64.h lib/textcode.h lib/str.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)scan_cescape.o: lib/textcode/scan_cescape.c lib/fmt.h lib/typedefs.h lib/uint32.h lib/uint64.h lib/textcode.h lib/scan.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)fmt_hexdump.o: lib/textcode/fmt_hexdump.c lib/fmt.h lib/typedefs.h lib/uint32.h lib/uint64.h lib/textcode.h lib/str.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)base64.o: lib/textcode/base64.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)fmt_html_tagarg.o: lib/textcode/fmt_html_tagarg.c lib/fmt.h lib/typedefs.h lib/uint32.h lib/uint64.h lib/textcode.h lib/str.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)scan_to_sa.o: lib/textcode/scan_to_sa.c lib/str.h lib/typedefs.h lib/stralloc.h lib/textcode.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)fmt_base64url.o: lib/textcode/fmt_base64url.c lib/fmt.h lib/typedefs.h lib/uint32.h lib/uint64.h lib/textcode.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)range_str2inbuf.o: lib/safemult/range_str2inbuf.c lib/likely.h lib/rangecheck.h lib/typedefs.h lib/uint32.h lib/typedefs.h lib/uint16.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)range_strinbuf.o: lib/safemult/range_strinbuf.c lib/likely.h lib/rangecheck.h lib/typedefs.h lib/uint32.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)umult64.o: lib/safemult/umult64.c lib/safemult.h lib/uint16.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)imult64.o: lib/safemult/imult64.c lib/safemult.h lib/uint16.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)umult32.o: lib/safemult/umult32.c lib/safemult.h lib/uint16.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)imult16.o: lib/safemult/imult16.c lib/safemult.h lib/uint16.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)imult32.o: lib/safemult/imult32.c lib/safemult.h lib/uint16.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)range_str4inbuf.o: lib/safemult/range_str4inbuf.c lib/likely.h lib/rangecheck.h lib/typedefs.h lib/uint32.h lib/typedefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)umult16.o: lib/safemult/umult16.c lib/safemult.h lib/uint16.h lib/uint32.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)range_arrayinbuf.o: lib/safemult/range_arrayinbuf.c lib/likely.h lib/rangecheck.h lib/typedefs.h lib/uint32.h lib/safemult.h lib/uint16.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)tai_pack.o: lib/tai/tai_pack.c lib/tai.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)tai_now.o: lib/tai/tai_now.c lib/tai.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)tai_sub.o: lib/tai/tai_sub.c lib/tai.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)tai_add.o: lib/tai/tai_add.c lib/tai.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)tai_uint.o: lib/tai/tai_uint.c lib/tai.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)tai_unpack.o: lib/tai/tai_unpack.c lib/tai.h lib/uint64.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)round.o: lib/round.c lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)json_toint.o: lib/json/json_toint.c lib/stralloc.h lib/typedefs.h lib/scan.h lib/uint32.h lib/uint64.h lib/json.h lib/charbuf.h lib/byte.h lib/hmap.h lib/stralloc.h lib/buffer.h lib/slist.h lib/alloc.h lib/str.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)json_print.o: lib/json/json_print.c lib/uint64.h lib/buffer.h lib/typedefs.h lib/hmap.h lib/stralloc.h lib/buffer.h lib/json.h lib/charbuf.h lib/byte.h lib/hmap.h lib/slist.h lib/alloc.h lib/uint64.h lib/str.h lib/slist.h lib/stralloc.h lib/fmt.h lib/uint32.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)json_push.o: lib/json/json_push.c lib/slist.h lib/alloc.h lib/typedefs.h lib/json.h lib/charbuf.h lib/byte.h lib/hmap.h lib/stralloc.h lib/buffer.h lib/slist.h lib/uint64.h lib/str.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)json_todouble.o: lib/json/json_todouble.c lib/stralloc.h lib/typedefs.h lib/scan.h lib/uint32.h lib/uint64.h lib/json.h lib/charbuf.h lib/byte.h lib/hmap.h lib/stralloc.h lib/buffer.h lib/slist.h lib/alloc.h lib/str.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)json_tosa.o: lib/json/json_tosa.c lib/buffer.h lib/typedefs.h lib/stralloc.h lib/json.h lib/charbuf.h lib/byte.h lib/hmap.h lib/stralloc.h lib/buffer.h lib/slist.h lib/alloc.h lib/uint64.h lib/str.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)json_parse.o: lib/json/json_parse.c lib/hmap.h lib/stralloc.h lib/typedefs.h lib/buffer.h lib/byte.h lib/charbuf.h lib/byte.h lib/json.h lib/charbuf.h lib/hmap.h lib/slist.h lib/alloc.h lib/uint64.h lib/str.h lib/scan.h lib/uint32.h lib/stralloc.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)json_recurse.o: lib/json/json_recurse.c lib/hmap.h lib/stralloc.h lib/typedefs.h lib/buffer.h lib/json.h lib/charbuf.h lib/byte.h lib/hmap.h lib/slist.h lib/alloc.h lib/uint64.h lib/str.h lib/slist.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)json_length.o: lib/json/json_length.c lib/hmap.h lib/stralloc.h lib/typedefs.h lib/buffer.h lib/json.h lib/charbuf.h lib/byte.h lib/hmap.h lib/slist.h lib/alloc.h lib/uint64.h lib/str.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)json_get_property.o: lib/json/json_get_property.c lib/hmap.h lib/stralloc.h lib/typedefs.h lib/buffer.h lib/json.h lib/charbuf.h lib/byte.h lib/hmap.h lib/slist.h lib/alloc.h lib/uint64.h lib/str.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)json_newnode.o: lib/json/json_newnode.c lib/alloc.h lib/typedefs.h lib/byte.h lib/json.h lib/charbuf.h lib/byte.h lib/hmap.h lib/stralloc.h lib/buffer.h lib/slist.h lib/alloc.h lib/uint64.h lib/str.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)json_read_tree.o: lib/json/json_read_tree.c lib/hmap.h lib/stralloc.h lib/typedefs.h lib/buffer.h lib/byte.h lib/fmt.h lib/uint32.h lib/uint64.h lib/json.h lib/charbuf.h lib/byte.h lib/hmap.h lib/slist.h lib/alloc.h lib/str.h lib/scan.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)json_reader_init.o: lib/json/json_reader_init.c lib/byte.h lib/typedefs.h lib/json.h lib/charbuf.h lib/byte.h lib/hmap.h lib/stralloc.h lib/buffer.h lib/slist.h lib/alloc.h lib/uint64.h lib/str.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)json_read_callback.o: lib/json/json_read_callback.c lib/byte.h lib/typedefs.h lib/json.h lib/charbuf.h lib/byte.h lib/hmap.h lib/stralloc.h lib/buffer.h lib/slist.h lib/alloc.h lib/uint64.h lib/str.h lib/stralloc.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)json_tostring.o: lib/json/json_tostring.c lib/stralloc.h lib/typedefs.h lib/fmt.h lib/uint32.h lib/uint64.h lib/json.h lib/charbuf.h lib/byte.h lib/hmap.h lib/stralloc.h lib/buffer.h lib/slist.h lib/alloc.h lib/str.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)json_free.o: lib/json/json_free.c lib/hmap.h lib/stralloc.h lib/typedefs.h lib/buffer.h lib/alloc.h lib/json.h lib/charbuf.h lib/byte.h lib/hmap.h lib/slist.h lib/alloc.h lib/uint64.h lib/str.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)json_set_property.o: lib/json/json_set_property.c lib/hmap.h lib/stralloc.h lib/typedefs.h lib/buffer.h lib/json.h lib/charbuf.h lib/byte.h lib/hmap.h lib/slist.h lib/alloc.h lib/uint64.h lib/str.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)array_cat.o: lib/array/array_cat.c lib/array.h lib/typedefs.h lib/uint64.h lib/byte.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)array_get.o: lib/array/array_get.c lib/array.h lib/typedefs.h lib/uint64.h lib/likely.h lib/safemult.h lib/uint16.h lib/uint32.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)array_equal.o: lib/array/array_equal.c lib/array.h lib/typedefs.h lib/uint64.h lib/byte.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)array_allocate.o: lib/array/array_allocate.c lib/likely.h lib/typedefs.h lib/array.h lib/typedefs.h lib/uint64.h lib/byte.h lib/safemult.h lib/uint16.h lib/uint32.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)array_start.o: lib/array/array_start.c lib/array.h lib/typedefs.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)array_trunc.o: lib/array/array_trunc.c lib/array.h lib/typedefs.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)array_cat0.o: lib/array/array_cat0.c lib/array.h lib/typedefs.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)array_cate.o: lib/array/array_cate.c lib/array.h lib/typedefs.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)array_catb.o: lib/array/array_catb.c lib/array.h lib/typedefs.h lib/uint64.h lib/byte.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)array_fail.o: lib/array/array_fail.c lib/array.h lib/typedefs.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)array_cats0.o: lib/array/array_cats0.c lib/array.h lib/typedefs.h lib/uint64.h lib/str.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)array_cats.o: lib/array/array_cats.c lib/array.h lib/typedefs.h lib/uint64.h lib/str.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)array_splice.o: lib/array/array_splice.c lib/array.h lib/typedefs.h lib/uint64.h lib/byte.h lib/uint64.h lib/safemult.h lib/uint16.h lib/uint32.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)array_reset.o: lib/array/array_reset.c lib/array.h lib/typedefs.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)array_truncate.o: lib/array/array_truncate.c lib/array.h lib/typedefs.h lib/uint64.h lib/safemult.h lib/uint16.h lib/uint32.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)array_bytes.o: lib/array/array_bytes.c lib/array.h lib/typedefs.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)array_length.o: lib/array/array_length.c lib/array.h lib/typedefs.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)array_indexof.o: lib/array/array_indexof.c lib/array.h lib/typedefs.h lib/uint64.h lib/byte.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)array_end.o: lib/array/array_end.c lib/array.h lib/typedefs.h lib/uint64.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)array_find.o: lib/array/array_find.c lib/array.h lib/typedefs.h lib/uint64.h lib/byte.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)charbuf_getc.o: lib/charbuf/charbuf_getc.c lib/charbuf.h lib/typedefs.h lib/byte.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
