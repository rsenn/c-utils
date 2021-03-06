$(BUILDDIR)array$(EXEEXT): examples/array.c io.a iarray.a ndelay.a buffer.a array.a byte.a str.a fmt.a safemult.a
	$(CROSS_COMPILE)$(CC) $(LDFLAGS) -o $@ $^ $(LIBS)
$(BUILDDIR)b64encode$(EXEEXT): examples/b64encode.c io.a iarray.a ndelay.a buffer.a byte.a fmt.a textcode.a str.a
	$(CROSS_COMPILE)$(CC) $(LDFLAGS) -o $@ $^ $(LIBS)
$(BUILDDIR)buffer_1$(EXEEXT): examples/buffer_1.c io.a iarray.a ndelay.a buffer.a byte.a fmt.a
	$(CROSS_COMPILE)$(CC) $(LDFLAGS) -o $@ $^ $(LIBS)
$(BUILDDIR)buffer_fromsa$(EXEEXT): examples/buffer_fromsa.c io.a iarray.a ndelay.a buffer.a stralloc.a byte.a fmt.a
	$(CROSS_COMPILE)$(CC) $(LDFLAGS) -o $@ $^ $(LIBS)
$(BUILDDIR)buffer_mmap$(EXEEXT): examples/buffer_mmap.c io.a iarray.a ndelay.a buffer.a mmap.a open.a byte.a fmt.a
	$(CROSS_COMPILE)$(CC) $(LDFLAGS) -o $@ $^ $(LIBS)
$(BUILDDIR)buffer_tosa$(EXEEXT): examples/buffer_tosa.c io.a iarray.a ndelay.a buffer.a byte.a stralloc.a fmt.a
	$(CROSS_COMPILE)$(CC) $(LDFLAGS) -o $@ $^ $(LIBS)
$(BUILDDIR)byte_copy$(EXEEXT): examples/byte_copy.c io.a iarray.a ndelay.a buffer.a byte.a fmt.a
	$(CROSS_COMPILE)$(CC) $(LDFLAGS) -o $@ $^ $(LIBS)
$(BUILDDIR)cas$(EXEEXT): examples/cas.c io.a iarray.a ndelay.a buffer.a byte.a fmt.a
	$(CROSS_COMPILE)$(CC) $(LDFLAGS) -o $@ $^ $(LIBS)
$(BUILDDIR)cdbget2$(EXEEXT): examples/cdbget2.c io.a iarray.a ndelay.a buffer.a byte.a fmt.a errmsg.a
	$(CROSS_COMPILE)$(CC) $(LDFLAGS) -o $@ $^ $(LIBS)
$(BUILDDIR)cescape$(EXEEXT): examples/cescape.c io.a iarray.a ndelay.a buffer.a fmt.a byte.a
	$(CROSS_COMPILE)$(CC) $(LDFLAGS) -o $@ $^ $(LIBS)
$(BUILDDIR)client$(EXEEXT): examples/client.c io.a iarray.a ndelay.a buffer.a byte.a scan.a str.a fmt.a ndelay.a pe.a socket.a
	$(CROSS_COMPILE)$(CC) $(LDFLAGS) -o $@ $^ $(LIBS) $(WINSOCK_LIB)
$(BUILDDIR)dllink$(EXEEXT): examples/dllink.c io.a iarray.a ndelay.a buffer.a byte.a fmt.a case.a ndelay.a socket.a
	$(CROSS_COMPILE)$(CC) $(LDFLAGS) -o $@ $^ $(LIBS) $(WINSOCK_LIB)
$(BUILDDIR)dnsip_example$(EXEEXT): examples/dnsip.c dns.a io.a iarray.a ndelay.a stralloc.a buffer.a open.a byte.a fmt.a
	$(CROSS_COMPILE)$(CC) $(LDFLAGS) -o $@ $^ $(LIBS)
$(BUILDDIR)fdpassing$(EXEEXT): examples/fdpassing.c io.a iarray.a ndelay.a buffer.a byte.a fmt.a
	$(CROSS_COMPILE)$(CC) $(LDFLAGS) -o $@ $^ $(LIBS)
$(BUILDDIR)fmt$(EXEEXT): examples/fmt.c io.a iarray.a ndelay.a buffer.a fmt.a byte.a
	$(CROSS_COMPILE)$(CC) $(LDFLAGS) -o $@ $^ $(LIBS)
$(BUILDDIR)fmt_httpdate$(EXEEXT): examples/fmt_httpdate.c io.a iarray.a ndelay.a buffer.a fmt.a scan.a byte.a
	$(CROSS_COMPILE)$(CC) $(LDFLAGS) -o $@ $^ $(LIBS)
$(BUILDDIR)fmt_human$(EXEEXT): examples/fmt_human.c io.a iarray.a ndelay.a buffer.a fmt.a byte.a
	$(CROSS_COMPILE)$(CC) $(LDFLAGS) -o $@ $^ $(LIBS)
$(BUILDDIR)fmt_ip6$(EXEEXT): examples/fmt_ip6.c io.a iarray.a ndelay.a buffer.a fmt.a byte.a
	$(CROSS_COMPILE)$(CC) $(LDFLAGS) -o $@ $^ $(LIBS)
$(BUILDDIR)fmt_iso8691$(EXEEXT): examples/fmt_iso8691.c io.a iarray.a ndelay.a buffer.a fmt.a scan.a byte.a
	$(CROSS_COMPILE)$(CC) $(LDFLAGS) -o $@ $^ $(LIBS)
$(BUILDDIR)fmt_long$(EXEEXT): examples/fmt_long.c io.a iarray.a ndelay.a buffer.a fmt.a str.a byte.a
	$(CROSS_COMPILE)$(CC) $(LDFLAGS) -o $@ $^ $(LIBS)
$(BUILDDIR)fmt_longlong$(EXEEXT): examples/fmt_longlong.c io.a iarray.a ndelay.a buffer.a byte.a fmt.a scan.a str.a str.a
	$(CROSS_COMPILE)$(CC) $(LDFLAGS) -o $@ $^ $(LIBS)
$(BUILDDIR)fmt_strm_alloca$(EXEEXT): examples/fmt_strm_alloca.c io.a iarray.a ndelay.a buffer.a byte.a fmt.a fmt.a
	$(CROSS_COMPILE)$(CC) $(LDFLAGS) -o $@ $^ $(LIBS)
$(BUILDDIR)httpd$(EXEEXT): examples/httpd.c io.a iarray.a buffer.a array.a byte.a fmt.a str.a case.a http.a pe.a socket.a ndelay.a 
	$(CROSS_COMPILE)$(CC) $(LDFLAGS) -o $@ $^ $(LIBS) $(WINSOCK_LIB)
$(BUILDDIR)io$(EXEEXT): examples/io.c io.a socket.a iarray.a buffer.a byte.a fmt.a
	$(CROSS_COMPILE)$(CC) $(LDFLAGS) -o $@ $^ $(LIBS) $(WINSOCK_LIB)
$(BUILDDIR)io2$(EXEEXT): examples/io2.c io.a socket.a iarray.a array.a buffer.a str.a byte.a fmt.a safemult.a
	$(CROSS_COMPILE)$(CC) $(LDFLAGS) -o $@ $^ $(LIBS) $(WINSOCK_LIB)
$(BUILDDIR)io3$(EXEEXT): examples/io3.c io.a socket.a iarray.a buffer.a byte.a fmt.a str.a
	$(CROSS_COMPILE)$(CC) $(LDFLAGS) -o $@ $^ $(LIBS) $(WINSOCK_LIB)
$(BUILDDIR)io4$(EXEEXT): examples/io4.c io.a socket.a iarray.a buffer.a byte.a fmt.a taia.a str.a
	$(CROSS_COMPILE)$(CC) $(LDFLAGS) -o $@ $^ $(LIBS) $(WINSOCK_LIB)
$(BUILDDIR)io5$(EXEEXT): examples/io5.c io.a socket.a iarray.a array.a buffer.a socket.a ndelay.a str.a fmt.a byte.a uint16.a safemult.a
	$(CROSS_COMPILE)$(CC) $(LDFLAGS) -o $@ $^ $(LIBS) $(WINSOCK_LIB)
$(BUILDDIR)iob$(EXEEXT): examples/iob.c iob.a io.a socket.a iarray.a buffer.a byte.a fmt.a
	$(CROSS_COMPILE)$(CC) $(LDFLAGS) -o $@ $^ $(LIBS) $(WINSOCK_LIB)
$(BUILDDIR)iom$(EXEEXT): examples/iom.c io.a socket.a iarray.a buffer.a byte.a fmt.a
	$(CROSS_COMPILE)$(CC) $(LDFLAGS) -o $@ $^ $(LIBS) $(WINSOCK_LIB)
$(BUILDDIR)json$(EXEEXT): examples/json.c io.a iarray.a buffer.a scan.a byte.a fmt.a
	$(CROSS_COMPILE)$(CC) $(LDFLAGS) -o $@ $^ $(LIBS)
$(BUILDDIR)marshal$(EXEEXT): examples/marshal.c io.a iarray.a ndelay.a buffer.a byte.a fmt.a scan.a stralloc.a str.a map.a mmap.a uint16.a uint32.a uint64.a
	$(CROSS_COMPILE)$(CC) $(LDFLAGS) -o $@ $^ $(LIBS)
$(BUILDDIR)mult$(EXEEXT): examples/mult.c io.a iarray.a ndelay.a buffer.a byte.a fmt.a
	$(CROSS_COMPILE)$(CC) $(LDFLAGS) -o $@ $^ $(LIBS)
$(BUILDDIR)netstring$(EXEEXT): examples/netstring.c io.a iarray.a ndelay.a buffer.a fmt.a scan.a byte.a
	$(CROSS_COMPILE)$(CC) $(LDFLAGS) -o $@ $^ $(LIBS)
$(BUILDDIR)protobuf$(EXEEXT): examples/protobuf.c io.a iarray.a ndelay.a buffer.a fmt.a scan.a byte.a
	$(CROSS_COMPILE)$(CC) $(LDFLAGS) -o $@ $^ $(LIBS)
$(BUILDDIR)proxy$(EXEEXT): examples/proxy.c io.a iarray.a ndelay.a buffer.a byte.a scan.a stralloc.a str.a fmt.a dns.a pe.a socket.a
	$(CROSS_COMPILE)$(CC) $(LDFLAGS) -o $@ $^ $(LIBS) $(WINSOCK_LIB)
$(BUILDDIR)range$(EXEEXT): examples/range.c io.a iarray.a ndelay.a buffer.a byte.a fmt.a range.a uint16.a
	$(CROSS_COMPILE)$(CC) $(LDFLAGS) -o $@ $^ $(LIBS)
$(BUILDDIR)readhttp$(EXEEXT): examples/readhttp.c io.a iarray.a ndelay.a buffer.a byte.a stralloc.a fmt.a
	$(CROSS_COMPILE)$(CC) $(LDFLAGS) -o $@ $^ $(LIBS)
$(BUILDDIR)scan$(EXEEXT): examples/scan.c io.a iarray.a ndelay.a buffer.a scan.a byte.a fmt.a
	$(CROSS_COMPILE)$(CC) $(LDFLAGS) -o $@ $^ $(LIBS)
$(BUILDDIR)scan_long$(EXEEXT): examples/scan_long.c io.a iarray.a ndelay.a buffer.a fmt.a scan.a byte.a
	$(CROSS_COMPILE)$(CC) $(LDFLAGS) -o $@ $^ $(LIBS)
$(BUILDDIR)server$(EXEEXT): examples/server.c io.a iarray.a ndelay.a buffer.a byte.a scan.a str.a fmt.a ndelay.a pe.a socket.a
	$(CROSS_COMPILE)$(CC) $(LDFLAGS) -o $@ $^ $(LIBS) $(WINSOCK_LIB)
$(BUILDDIR)stralloc_buffer$(EXEEXT): examples/stralloc_buffer.c io.a iarray.a ndelay.a buffer.a stralloc.a byte.a fmt.a
	$(CROSS_COMPILE)$(CC) $(LDFLAGS) -o $@ $^ $(LIBS)
$(BUILDDIR)stralloc_chomp$(EXEEXT): examples/stralloc_chomp.c io.a iarray.a ndelay.a buffer.a stralloc.a byte.a fmt.a
	$(CROSS_COMPILE)$(CC) $(LDFLAGS) -o $@ $^ $(LIBS)
$(BUILDDIR)textcode$(EXEEXT): examples/textcode.c io.a iarray.a ndelay.a buffer.a array.a fmt.a scan.a byte.a textcode.a
	$(CROSS_COMPILE)$(CC) $(LDFLAGS) -o $@ $^ $(LIBS)
$(BUILDDIR)uint$(EXEEXT): examples/uint.c io.a iarray.a ndelay.a buffer.a byte.a fmt.a uint16.a uint32.a uint64.a
	$(CROSS_COMPILE)$(CC) $(LDFLAGS) -o $@ $^ $(LIBS)
$(BUILDDIR)unurl$(EXEEXT): examples/unurl.c io.a iarray.a ndelay.a buffer.a scan.a byte.a fmt.a
	$(CROSS_COMPILE)$(CC) $(LDFLAGS) -o $@ $^ $(LIBS)
$(BUILDDIR)urlencode$(EXEEXT): examples/urlencode.c io.a iarray.a ndelay.a buffer.a fmt.a byte.a
	$(CROSS_COMPILE)$(CC) $(LDFLAGS) -o $@ $^ $(LIBS)
$(BUILDDIR)utf8$(EXEEXT): examples/utf8.c io.a iarray.a ndelay.a buffer.a fmt.a scan.a byte.a
	$(CROSS_COMPILE)$(CC) $(LDFLAGS) -o $@ $^ $(LIBS)
$(BUILDDIR)uudecode$(EXEEXT): examples/uudecode.c io.a iarray.a ndelay.a buffer.a scan.a stralloc.a str.a byte.a fmt.a case.a open.a
	$(CROSS_COMPILE)$(CC) $(LDFLAGS) -o $@ $^ $(LIBS)
$(BUILDDIR)vd$(EXEEXT): examples/vd.c io.a iarray.a ndelay.a buffer.a byte.a fmt.a socket.a
	$(CROSS_COMPILE)$(CC) $(LDFLAGS) -o $@ $^ $(LIBS) $(WINSOCK_LIB)
