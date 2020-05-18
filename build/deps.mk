$(BUILDDIR)gzread.o: CPPFLAGS += -I3rdparty/zlib
$(BUILDDIR)gzread.o: 3rdparty/zlib/gzread.c 3rdparty/zlib/gzguts.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)inftrees.o: 3rdparty/zlib/inftrees.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)compress.o: 3rdparty/zlib/compress.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)deflate.o: 3rdparty/zlib/deflate.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)trees.o: 3rdparty/zlib/trees.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)gzwrite.o: 3rdparty/zlib/gzwrite.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)zutil.o: 3rdparty/zlib/zutil.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)inflate.o: 3rdparty/zlib/inflate.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)crc32.o: 3rdparty/zlib/crc32.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)gzclose.o: 3rdparty/zlib/gzclose.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)inffast.o: 3rdparty/zlib/inffast.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)gzlib.o: 3rdparty/zlib/gzlib.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)infback.o: 3rdparty/zlib/infback.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)uncompr.o: 3rdparty/zlib/uncompr.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)adler32.o: 3rdparty/zlib/adler32.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)feature_tests.o: 3rdparty/openssl/build/x86_64-linux-diet/CMakeFiles/feature_tests.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)LONG_INT.o: 3rdparty/openssl/build/x86_64-linux-diet/CMakeFiles/CheckTypeSize/LONG_INT.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)INT.o: 3rdparty/openssl/build/x86_64-linux-diet/CMakeFiles/CheckTypeSize/INT.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)LONG_LONG_INT.o: 3rdparty/openssl/build/x86_64-linux-diet/CMakeFiles/CheckTypeSize/LONG_LONG_INT.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)CMakeCCompilerId.o: 3rdparty/openssl/build/x86_64-linux-diet/CMakeFiles/3.1.3/CompilerIdC/CMakeCCompilerId.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)conf.o: 3rdparty/openssl/fuzz/conf.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)crl.o: 3rdparty/openssl/fuzz/crl.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bignum.o: 3rdparty/openssl/fuzz/bignum.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)asn1.o: 3rdparty/openssl/fuzz/asn1.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)asn1parse.o: 3rdparty/openssl/fuzz/asn1parse.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bndiv.o: 3rdparty/openssl/fuzz/bndiv.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)server.o: 3rdparty/openssl/fuzz/server.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ct.o: 3rdparty/openssl/fuzz/ct.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cms.o: 3rdparty/openssl/fuzz/cms.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)test-corpus.o: 3rdparty/openssl/fuzz/test-corpus.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)driver.o: 3rdparty/openssl/fuzz/driver.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)x509.o: 3rdparty/openssl/fuzz/x509.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)client.o: 3rdparty/openssl/fuzz/client.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)applink.o: 3rdparty/openssl/ms/applink.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)uplink.o: 3rdparty/openssl/ms/uplink.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)crl.o: 3rdparty/openssl/apps/crl.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)pkcs12.o: CPPFLAGS += -I3rdparty/openssl/apps
$(BUILDDIR)pkcs12.o: 3rdparty/openssl/apps/pkcs12.c 3rdparty/openssl/apps/apps.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ocsp.o: CPPFLAGS += -I3rdparty/openssl/include/internal
$(BUILDDIR)ocsp.o: 3rdparty/openssl/apps/ocsp.c 3rdparty/openssl/include/internal/sockets.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)vms_term_sock.o: 3rdparty/openssl/apps/vms_term_sock.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)dsa.o: 3rdparty/openssl/apps/dsa.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)opt.o: 3rdparty/openssl/apps/opt.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)genrsa.o: 3rdparty/openssl/apps/genrsa.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)pkey.o: 3rdparty/openssl/apps/pkey.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)crl2p7.o: 3rdparty/openssl/apps/crl2p7.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)req.o: 3rdparty/openssl/apps/req.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)rsa.o: 3rdparty/openssl/apps/rsa.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ec.o: 3rdparty/openssl/apps/ec.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)nseq.o: 3rdparty/openssl/apps/nseq.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)storeutl.o: 3rdparty/openssl/apps/storeutl.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)sess_id.o: 3rdparty/openssl/apps/sess_id.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)gendsa.o: 3rdparty/openssl/apps/gendsa.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)s_socket.o: 3rdparty/openssl/apps/s_socket.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)engine.o: 3rdparty/openssl/apps/engine.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)version.o: 3rdparty/openssl/apps/version.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cms.o: 3rdparty/openssl/apps/cms.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)speed.o: 3rdparty/openssl/apps/speed.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bf_prefix.o: 3rdparty/openssl/apps/bf_prefix.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)openssl.o: 3rdparty/openssl/apps/openssl.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)win32_init.o: 3rdparty/openssl/apps/win32_init.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ecparam.o: 3rdparty/openssl/apps/ecparam.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)vms_decc_init.o: 3rdparty/openssl/apps/vms_decc_init.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)apps.o: 3rdparty/openssl/apps/apps.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)app_rand.o: 3rdparty/openssl/apps/app_rand.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)prime.o: 3rdparty/openssl/apps/prime.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)pkcs7.o: 3rdparty/openssl/apps/pkcs7.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)dsaparam.o: 3rdparty/openssl/apps/dsaparam.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)dgst.o: 3rdparty/openssl/apps/dgst.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)errstr.o: 3rdparty/openssl/apps/errstr.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)s_server.o: 3rdparty/openssl/apps/s_server.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)rsautl.o: 3rdparty/openssl/apps/rsautl.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ts.o: 3rdparty/openssl/apps/ts.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)dhparam.o: 3rdparty/openssl/apps/dhparam.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)srp.o: 3rdparty/openssl/apps/srp.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)s_client.o: CPPFLAGS += -I3rdparty/openssl
$(BUILDDIR)s_client.o: 3rdparty/openssl/apps/s_client.c 3rdparty/openssl/e_os.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)rehash.o: 3rdparty/openssl/apps/rehash.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)enc.o: 3rdparty/openssl/apps/enc.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)asn1pars.o: 3rdparty/openssl/apps/asn1pars.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)passwd.o: 3rdparty/openssl/apps/passwd.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)pkcs8.o: 3rdparty/openssl/apps/pkcs8.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)pkeyutl.o: 3rdparty/openssl/apps/pkeyutl.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)rand.o: 3rdparty/openssl/apps/rand.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)spkac.o: 3rdparty/openssl/apps/spkac.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)smime.o: 3rdparty/openssl/apps/smime.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ciphers.o: 3rdparty/openssl/apps/ciphers.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)x509.o: 3rdparty/openssl/apps/x509.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)s_time.o: 3rdparty/openssl/apps/s_time.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)verify.o: 3rdparty/openssl/apps/verify.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ca.o: 3rdparty/openssl/apps/ca.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)s_cb.o: 3rdparty/openssl/apps/s_cb.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)pkeyparam.o: 3rdparty/openssl/apps/pkeyparam.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)genpkey.o: 3rdparty/openssl/apps/genpkey.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ecstresstest.o: 3rdparty/openssl/test/ecstresstest.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)sm4_internal_test.o: CPPFLAGS += -I3rdparty/openssl/include/crypto
$(BUILDDIR)sm4_internal_test.o: 3rdparty/openssl/test/sm4_internal_test.c 3rdparty/openssl/include/crypto/sm4.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)chacha_internal_test.o: 3rdparty/openssl/test/chacha_internal_test.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)exptest.o: 3rdparty/openssl/test/exptest.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)test_test.o: 3rdparty/openssl/test/test_test.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)pkey_meth_test.o: 3rdparty/openssl/test/pkey_meth_test.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bntest.o: 3rdparty/openssl/test/bntest.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)errtest.o: 3rdparty/openssl/test/errtest.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)srptest.o: CPPFLAGS += -I3rdparty/openssl/test
$(BUILDDIR)srptest.o: 3rdparty/openssl/test/srptest.c 3rdparty/openssl/test/testutil.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)uitest.o: 3rdparty/openssl/test/uitest.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ectest.o: 3rdparty/openssl/test/ectest.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)asn1_string_table_test.o: 3rdparty/openssl/test/asn1_string_table_test.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)asynctest.o: 3rdparty/openssl/test/asynctest.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ecdsatest.o: 3rdparty/openssl/test/ecdsatest.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)v3ext.o: 3rdparty/openssl/test/v3ext.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)aborttest.o: 3rdparty/openssl/test/aborttest.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)gosttest.o: 3rdparty/openssl/test/gosttest.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bio_memleak_test.o: 3rdparty/openssl/test/bio_memleak_test.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)dhtest.o: 3rdparty/openssl/test/dhtest.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bftest.o: 3rdparty/openssl/test/bftest.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)x509aux.o: 3rdparty/openssl/test/x509aux.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cipherlist_test.o: 3rdparty/openssl/test/cipherlist_test.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)afalgtest.o: 3rdparty/openssl/test/afalgtest.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)pkey_meth_kdf_test.o: 3rdparty/openssl/test/pkey_meth_kdf_test.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ocspapitest.o: 3rdparty/openssl/test/ocspapitest.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ideatest.o: 3rdparty/openssl/test/ideatest.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)threadstest.o: 3rdparty/openssl/test/threadstest.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)sslbuffertest.o: 3rdparty/openssl/test/sslbuffertest.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ssltest_old.o: 3rdparty/openssl/test/ssltest_old.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)d2i_test.o: 3rdparty/openssl/test/d2i_test.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)versions.o: 3rdparty/openssl/test/versions.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)exdatatest.o: 3rdparty/openssl/test/exdatatest.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)mdc2test.o: 3rdparty/openssl/test/mdc2test.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)sslapitest.o: 3rdparty/openssl/test/sslapitest.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ec_internal_test.o: 3rdparty/openssl/test/ec_internal_test.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)evp_extra_test.o: 3rdparty/openssl/test/evp_extra_test.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)shlibloadtest.o: 3rdparty/openssl/test/shlibloadtest.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)dsatest.o: 3rdparty/openssl/test/dsatest.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ssl_cert_table_internal_test.o: 3rdparty/openssl/test/ssl_cert_table_internal_test.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)fatalerrtest.o: 3rdparty/openssl/test/fatalerrtest.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)siphash_internal_test.o: 3rdparty/openssl/test/siphash_internal_test.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)dsa_no_digest_size_test.o: 3rdparty/openssl/test/dsa_no_digest_size_test.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bioprinttest.o: 3rdparty/openssl/test/bioprinttest.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)handshake_helper.o: 3rdparty/openssl/test/handshake_helper.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)stack_test.o: 3rdparty/openssl/test/stack_test.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)curve448_internal_test.o: 3rdparty/openssl/test/curve448_internal_test.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)x509_internal_test.o: 3rdparty/openssl/test/x509_internal_test.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)gmdifftest.o: 3rdparty/openssl/test/gmdifftest.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bio_enc_test.o: 3rdparty/openssl/test/bio_enc_test.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)crltest.o: 3rdparty/openssl/test/crltest.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)rc5test.o: 3rdparty/openssl/test/rc5test.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)x509_dup_cert_test.o: 3rdparty/openssl/test/x509_dup_cert_test.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)mdc2_internal_test.o: 3rdparty/openssl/test/mdc2_internal_test.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)recordlentest.o: 3rdparty/openssl/test/recordlentest.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)evp_test.o: 3rdparty/openssl/test/evp_test.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)asn1_encode_test.o: 3rdparty/openssl/test/asn1_encode_test.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)clienthellotest.o: 3rdparty/openssl/test/clienthellotest.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)danetest.o: 3rdparty/openssl/test/danetest.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)rsa_test.o: 3rdparty/openssl/test/rsa_test.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)pbelutest.o: 3rdparty/openssl/test/pbelutest.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)servername_test.o: 3rdparty/openssl/test/servername_test.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)sslcorrupttest.o: 3rdparty/openssl/test/sslcorrupttest.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)lhash_test.o: 3rdparty/openssl/test/lhash_test.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)asynciotest.o: 3rdparty/openssl/test/asynciotest.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)rsa_complex.o: 3rdparty/openssl/test/rsa_complex.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cipher_overhead_test.o: 3rdparty/openssl/test/cipher_overhead_test.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)constant_time_test.o: 3rdparty/openssl/test/constant_time_test.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)wpackettest.o: 3rdparty/openssl/test/wpackettest.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ssl_test.o: 3rdparty/openssl/test/ssl_test.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ssl_test_ctx.o: 3rdparty/openssl/test/ssl_test_ctx.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)rdrand_sanitytest.o: 3rdparty/openssl/test/rdrand_sanitytest.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)drbgtest.o: 3rdparty/openssl/test/drbgtest.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cipherbytes_test.o: 3rdparty/openssl/test/cipherbytes_test.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)tls13secretstest.o: 3rdparty/openssl/test/tls13secretstest.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)x509_time_test.o: 3rdparty/openssl/test/x509_time_test.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)sm2_internal_test.o: 3rdparty/openssl/test/sm2_internal_test.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)tls13encryptiontest.o: 3rdparty/openssl/test/tls13encryptiontest.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)memleaktest.o: 3rdparty/openssl/test/memleaktest.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)basic_output.o: 3rdparty/openssl/test/testutil/basic_output.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)testutil_init.o: 3rdparty/openssl/test/testutil/testutil_init.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)format_output.o: 3rdparty/openssl/test/testutil/format_output.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)random.o: 3rdparty/openssl/test/testutil/random.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)driver.o: 3rdparty/openssl/test/testutil/driver.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)stanza.o: 3rdparty/openssl/test/testutil/stanza.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cb.o: CPPFLAGS += -I3rdparty/openssl/test/testutil
$(BUILDDIR)cb.o: 3rdparty/openssl/test/testutil/cb.c 3rdparty/openssl/test/testutil/output.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)tap_bio.o: 3rdparty/openssl/test/testutil/tap_bio.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)main.o: 3rdparty/openssl/test/testutil/main.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)output_helpers.o: 3rdparty/openssl/test/testutil/output_helpers.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)tests.o: 3rdparty/openssl/test/testutil/tests.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)test_cleanup.o: 3rdparty/openssl/test/testutil/test_cleanup.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)rc2test.o: 3rdparty/openssl/test/rc2test.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ctype_internal_test.o: 3rdparty/openssl/test/ctype_internal_test.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)casttest.o: 3rdparty/openssl/test/casttest.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)packettest.o: 3rdparty/openssl/test/packettest.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)v3nametest.o: 3rdparty/openssl/test/v3nametest.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)sysdefaulttest.o: 3rdparty/openssl/test/sysdefaulttest.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ssl_ctx_test.o: 3rdparty/openssl/test/ssl_ctx_test.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)enginetest.o: 3rdparty/openssl/test/enginetest.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)drbg_cavs_test.o: 3rdparty/openssl/test/drbg_cavs_test.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)poly1305_internal_test.o: 3rdparty/openssl/test/poly1305_internal_test.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)conf_include_test.o: 3rdparty/openssl/test/conf_include_test.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)secmemtest.o: 3rdparty/openssl/test/secmemtest.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bad_dtls_test.o: 3rdparty/openssl/test/bad_dtls_test.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)modes_internal_test.o: 3rdparty/openssl/test/modes_internal_test.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)asn1_decode_test.o: 3rdparty/openssl/test/asn1_decode_test.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ssltestlib.o: 3rdparty/openssl/test/ssltestlib.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ssl_test_ctx_test.o: 3rdparty/openssl/test/ssl_test_ctx_test.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)destest.o: 3rdparty/openssl/test/destest.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)rsa_mp_test.o: 3rdparty/openssl/test/rsa_mp_test.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)x509_check_cert_pkey_test.o: 3rdparty/openssl/test/x509_check_cert_pkey_test.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)asn1_time_test.o: 3rdparty/openssl/test/asn1_time_test.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ciphername_test.o: 3rdparty/openssl/test/ciphername_test.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)igetest.o: 3rdparty/openssl/test/igetest.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)dtls_mtu_test.o: 3rdparty/openssl/test/dtls_mtu_test.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)md2test.o: 3rdparty/openssl/test/md2test.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bio_callback_test.o: 3rdparty/openssl/test/bio_callback_test.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ct_test.o: 3rdparty/openssl/test/ct_test.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)dtlstest.o: 3rdparty/openssl/test/dtlstest.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)rc4test.o: 3rdparty/openssl/test/rc4test.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)dtlsv1listentest.o: 3rdparty/openssl/test/dtlsv1listentest.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)sanitytest.o: 3rdparty/openssl/test/sanitytest.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)asn1_internal_test.o: 3rdparty/openssl/test/asn1_internal_test.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)tls13ccstest.o: 3rdparty/openssl/test/tls13ccstest.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)verify_extra_test.o: 3rdparty/openssl/test/verify_extra_test.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cmsapitest.o: 3rdparty/openssl/test/cmsapitest.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)time_offset_test.o: 3rdparty/openssl/test/time_offset_test.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)hmactest.o: 3rdparty/openssl/test/hmactest.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)drbg_cavs_data.o: 3rdparty/openssl/test/drbg_cavs_data.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)pemtest.o: 3rdparty/openssl/test/pemtest.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)s3_msg.o: CPPFLAGS += -I3rdparty/openssl/ssl
$(BUILDDIR)s3_msg.o: 3rdparty/openssl/ssl/s3_msg.c 3rdparty/openssl/ssl/ssl_local.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)t1_enc.o: 3rdparty/openssl/ssl/t1_enc.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ssl_stat.o: 3rdparty/openssl/ssl/ssl_stat.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)d1_srtp.o: 3rdparty/openssl/ssl/d1_srtp.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ssl_txt.o: 3rdparty/openssl/ssl/ssl_txt.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ssl_mcnf.o: 3rdparty/openssl/ssl/ssl_mcnf.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bio_ssl.o: 3rdparty/openssl/ssl/bio_ssl.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ssl_asn1.o: 3rdparty/openssl/ssl/ssl_asn1.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)t1_lib.o: 3rdparty/openssl/ssl/t1_lib.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)rec_layer_d1.o: 3rdparty/openssl/ssl/record/rec_layer_d1.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)dtls1_bitmap.o: 3rdparty/openssl/ssl/record/dtls1_bitmap.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ssl3_record_tls13.o: 3rdparty/openssl/ssl/record/ssl3_record_tls13.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)rec_layer_s3.o: 3rdparty/openssl/ssl/record/rec_layer_s3.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ssl3_buffer.o: 3rdparty/openssl/ssl/record/ssl3_buffer.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ssl3_record.o: 3rdparty/openssl/ssl/record/ssl3_record.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)statem_lib.o: 3rdparty/openssl/ssl/statem/statem_lib.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)statem.o: 3rdparty/openssl/ssl/statem/statem.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)statem_srvr.o: 3rdparty/openssl/ssl/statem/statem_srvr.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)statem_clnt.o: 3rdparty/openssl/ssl/statem/statem_clnt.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)extensions_srvr.o: 3rdparty/openssl/ssl/statem/extensions_srvr.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)statem_dtls.o: 3rdparty/openssl/ssl/statem/statem_dtls.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)extensions_cust.o: 3rdparty/openssl/ssl/statem/extensions_cust.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)extensions_clnt.o: 3rdparty/openssl/ssl/statem/extensions_clnt.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)extensions.o: 3rdparty/openssl/ssl/statem/extensions.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ssl_ciph.o: 3rdparty/openssl/ssl/ssl_ciph.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)s3_enc.o: 3rdparty/openssl/ssl/s3_enc.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)tls_srp.o: 3rdparty/openssl/ssl/tls_srp.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ssl_lib.o: 3rdparty/openssl/ssl/ssl_lib.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ssl_err.o: 3rdparty/openssl/ssl/ssl_err.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ssl_rsa.o: 3rdparty/openssl/ssl/ssl_rsa.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)packet.o: 3rdparty/openssl/ssl/packet.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)s3_cbc.o: 3rdparty/openssl/ssl/s3_cbc.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)pqueue.o: 3rdparty/openssl/ssl/pqueue.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ssl_utst.o: 3rdparty/openssl/ssl/ssl_utst.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ssl_conf.o: 3rdparty/openssl/ssl/ssl_conf.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)s3_lib.o: 3rdparty/openssl/ssl/s3_lib.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)methods.o: 3rdparty/openssl/ssl/methods.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)tls13_enc.o: 3rdparty/openssl/ssl/tls13_enc.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)d1_msg.o: 3rdparty/openssl/ssl/d1_msg.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ssl_sess.o: 3rdparty/openssl/ssl/ssl_sess.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ssl_cert.o: 3rdparty/openssl/ssl/ssl_cert.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)d1_lib.o: 3rdparty/openssl/ssl/d1_lib.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ssl_init.o: 3rdparty/openssl/ssl/ssl_init.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)t1_trce.o: 3rdparty/openssl/ssl/t1_trce.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)mem_clr.o: 3rdparty/openssl/crypto/mem_clr.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)rc5_ecb.o: 3rdparty/openssl/crypto/rc5/rc5_ecb.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)rc5_skey.o: 3rdparty/openssl/crypto/rc5/rc5_skey.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)rc5cfb64.o: 3rdparty/openssl/crypto/rc5/rc5cfb64.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)rc5ofb64.o: 3rdparty/openssl/crypto/rc5/rc5ofb64.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)rc5_enc.o: 3rdparty/openssl/crypto/rc5/rc5_enc.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)o_time.o: 3rdparty/openssl/crypto/o_time.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)sparcv9cap.o: 3rdparty/openssl/crypto/sparcv9cap.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)md2_one.o: 3rdparty/openssl/crypto/md2/md2_one.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)md2_dgst.o: 3rdparty/openssl/crypto/md2/md2_dgst.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)eng_openssl.o: 3rdparty/openssl/crypto/engine/eng_openssl.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)eng_cnf.o: CPPFLAGS += -I3rdparty/openssl/crypto/engine
$(BUILDDIR)eng_cnf.o: 3rdparty/openssl/crypto/engine/eng_cnf.c 3rdparty/openssl/crypto/engine/eng_local.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)eng_init.o: 3rdparty/openssl/crypto/engine/eng_init.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)eng_err.o: 3rdparty/openssl/crypto/engine/eng_err.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)tb_dh.o: 3rdparty/openssl/crypto/engine/tb_dh.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)eng_fat.o: 3rdparty/openssl/crypto/engine/eng_fat.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)eng_dyn.o: 3rdparty/openssl/crypto/engine/eng_dyn.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)tb_asnmth.o: 3rdparty/openssl/crypto/engine/tb_asnmth.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)tb_cipher.o: 3rdparty/openssl/crypto/engine/tb_cipher.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)tb_eckey.o: 3rdparty/openssl/crypto/engine/tb_eckey.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)eng_rdrand.o: 3rdparty/openssl/crypto/engine/eng_rdrand.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)tb_rsa.o: 3rdparty/openssl/crypto/engine/tb_rsa.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)tb_rand.o: 3rdparty/openssl/crypto/engine/tb_rand.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)tb_pkmeth.o: 3rdparty/openssl/crypto/engine/tb_pkmeth.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)eng_devcrypto.o: 3rdparty/openssl/crypto/engine/eng_devcrypto.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)tb_dsa.o: 3rdparty/openssl/crypto/engine/tb_dsa.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)eng_pkey.o: 3rdparty/openssl/crypto/engine/eng_pkey.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)eng_ctrl.o: 3rdparty/openssl/crypto/engine/eng_ctrl.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)eng_table.o: 3rdparty/openssl/crypto/engine/eng_table.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)eng_list.o: 3rdparty/openssl/crypto/engine/eng_list.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)eng_lib.o: 3rdparty/openssl/crypto/engine/eng_lib.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)tb_digest.o: 3rdparty/openssl/crypto/engine/tb_digest.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)eng_all.o: 3rdparty/openssl/crypto/engine/eng_all.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)sha256.o: 3rdparty/openssl/crypto/sha/sha256.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)sha1dgst.o: CPPFLAGS += -I3rdparty/openssl/crypto/sha
$(BUILDDIR)sha1dgst.o: 3rdparty/openssl/crypto/sha/sha1dgst.c 3rdparty/openssl/crypto/sha/sha_local.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)sha1_one.o: 3rdparty/openssl/crypto/sha/sha1_one.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)sha512.o: 3rdparty/openssl/crypto/sha/sha512.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)keccak1600.o: 3rdparty/openssl/crypto/sha/keccak1600.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)pem_pkey.o: 3rdparty/openssl/crypto/pem/pem_pkey.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)pem_err.o: 3rdparty/openssl/crypto/pem/pem_err.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)pem_lib.o: 3rdparty/openssl/crypto/pem/pem_lib.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)pem_oth.o: 3rdparty/openssl/crypto/pem/pem_oth.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)pem_x509.o: 3rdparty/openssl/crypto/pem/pem_x509.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)pvkfmt.o: 3rdparty/openssl/crypto/pem/pvkfmt.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)pem_info.o: 3rdparty/openssl/crypto/pem/pem_info.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)pem_sign.o: 3rdparty/openssl/crypto/pem/pem_sign.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)pem_all.o: 3rdparty/openssl/crypto/pem/pem_all.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)pem_pk8.o: 3rdparty/openssl/crypto/pem/pem_pk8.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)pem_xaux.o: 3rdparty/openssl/crypto/pem/pem_xaux.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cm_pmeth.o: 3rdparty/openssl/crypto/cmac/cm_pmeth.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cmac.o: 3rdparty/openssl/crypto/cmac/cmac.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cm_ameth.o: 3rdparty/openssl/crypto/cmac/cm_ameth.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cms_smime.o: 3rdparty/openssl/crypto/cms/cms_smime.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cms_dd.o: 3rdparty/openssl/crypto/cms/cms_dd.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cms_env.o: 3rdparty/openssl/crypto/cms/cms_env.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cms_cd.o: 3rdparty/openssl/crypto/cms/cms_cd.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cms_lib.o: 3rdparty/openssl/crypto/cms/cms_lib.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cms_asn1.o: 3rdparty/openssl/crypto/cms/cms_asn1.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cms_att.o: 3rdparty/openssl/crypto/cms/cms_att.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cms_err.o: 3rdparty/openssl/crypto/cms/cms_err.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cms_ess.o: 3rdparty/openssl/crypto/cms/cms_ess.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cms_kari.o: 3rdparty/openssl/crypto/cms/cms_kari.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cms_sd.o: 3rdparty/openssl/crypto/cms/cms_sd.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cms_enc.o: 3rdparty/openssl/crypto/cms/cms_enc.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cms_io.o: 3rdparty/openssl/crypto/cms/cms_io.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cms_pwri.o: 3rdparty/openssl/crypto/cms/cms_pwri.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)aria.o: 3rdparty/openssl/crypto/aria/aria.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)aes_core.o: 3rdparty/openssl/crypto/aes/aes_core.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)aes_wrap.o: 3rdparty/openssl/crypto/aes/aes_wrap.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)aes_ecb.o: 3rdparty/openssl/crypto/aes/aes_ecb.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)aes_cfb.o: 3rdparty/openssl/crypto/aes/aes_cfb.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)aes_ofb.o: 3rdparty/openssl/crypto/aes/aes_ofb.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)aes_ige.o: 3rdparty/openssl/crypto/aes/aes_ige.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)aes_cbc.o: 3rdparty/openssl/crypto/aes/aes_cbc.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)aes_misc.o: 3rdparty/openssl/crypto/aes/aes_misc.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)aes_x86core.o: 3rdparty/openssl/crypto/aes/aes_x86core.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)dllmain.o: 3rdparty/openssl/crypto/dllmain.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)uid.o: 3rdparty/openssl/crypto/uid.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cversion.o: 3rdparty/openssl/crypto/cversion.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)md5_one.o: 3rdparty/openssl/crypto/md5/md5_one.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)md5_dgst.o: 3rdparty/openssl/crypto/md5/md5_dgst.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)conf_api.o: 3rdparty/openssl/crypto/conf/conf_api.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)conf_def.o: 3rdparty/openssl/crypto/conf/conf_def.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)conf_sap.o: 3rdparty/openssl/crypto/conf/conf_sap.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)conf_mall.o: 3rdparty/openssl/crypto/conf/conf_mall.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)conf_mod.o: 3rdparty/openssl/crypto/conf/conf_mod.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)conf_lib.o: 3rdparty/openssl/crypto/conf/conf_lib.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)conf_ssl.o: 3rdparty/openssl/crypto/conf/conf_ssl.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)conf_err.o: 3rdparty/openssl/crypto/conf/conf_err.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buf_err.o: 3rdparty/openssl/crypto/buffer/buf_err.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer.o: 3rdparty/openssl/crypto/buffer/buffer.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)rc2ofb64.o: 3rdparty/openssl/crypto/rc2/rc2ofb64.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)rc2_ecb.o: 3rdparty/openssl/crypto/rc2/rc2_ecb.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)rc2_cbc.o: 3rdparty/openssl/crypto/rc2/rc2_cbc.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)rc2cfb64.o: 3rdparty/openssl/crypto/rc2/rc2cfb64.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)rc2_skey.o: 3rdparty/openssl/crypto/rc2/rc2_skey.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)sm2_pmeth.o: 3rdparty/openssl/crypto/sm2/sm2_pmeth.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)sm2_err.o: 3rdparty/openssl/crypto/sm2/sm2_err.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)sm2_crypt.o: 3rdparty/openssl/crypto/sm2/sm2_crypt.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)sm2_sign.o: 3rdparty/openssl/crypto/sm2/sm2_sign.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)mem.o: 3rdparty/openssl/crypto/mem.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)async.o: CPPFLAGS += -I3rdparty/openssl/crypto/async
$(BUILDDIR)async.o: 3rdparty/openssl/crypto/async/async.c 3rdparty/openssl/crypto/async/async_local.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)async_null.o: 3rdparty/openssl/crypto/async/arch/async_null.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)async_posix.o: 3rdparty/openssl/crypto/async/arch/async_posix.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)async_win.o: 3rdparty/openssl/crypto/async/arch/async_win.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)async_wait.o: 3rdparty/openssl/crypto/async/async_wait.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)async_err.o: 3rdparty/openssl/crypto/async/async_err.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)o_names.o: 3rdparty/openssl/crypto/objects/o_names.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)obj_xref.o: 3rdparty/openssl/crypto/objects/obj_xref.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)obj_err.o: 3rdparty/openssl/crypto/objects/obj_err.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)obj_dat.o: 3rdparty/openssl/crypto/objects/obj_dat.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)obj_lib.o: 3rdparty/openssl/crypto/objects/obj_lib.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)sm4.o: 3rdparty/openssl/crypto/sm4/sm4.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cbc128.o: 3rdparty/openssl/crypto/modes/cbc128.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cfb128.o: 3rdparty/openssl/crypto/modes/cfb128.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)wrap128.o: 3rdparty/openssl/crypto/modes/wrap128.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ocb128.o: 3rdparty/openssl/crypto/modes/ocb128.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ctr128.o: 3rdparty/openssl/crypto/modes/ctr128.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ofb128.o: 3rdparty/openssl/crypto/modes/ofb128.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ccm128.o: 3rdparty/openssl/crypto/modes/ccm128.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)xts128.o: 3rdparty/openssl/crypto/modes/xts128.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cts128.o: 3rdparty/openssl/crypto/modes/cts128.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)gcm128.o: CPPFLAGS += -I3rdparty/openssl/crypto
$(BUILDDIR)gcm128.o: 3rdparty/openssl/crypto/modes/gcm128.c 3rdparty/openssl/crypto/arm_arch.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)hm_ameth.o: 3rdparty/openssl/crypto/hmac/hm_ameth.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)hm_pmeth.o: 3rdparty/openssl/crypto/hmac/hm_pmeth.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)hmac.o: 3rdparty/openssl/crypto/hmac/hmac.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)init.o: 3rdparty/openssl/crypto/init.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ct_vfy.o: 3rdparty/openssl/crypto/ct/ct_vfy.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ct_err.o: 3rdparty/openssl/crypto/ct/ct_err.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ct_sct_ctx.o: 3rdparty/openssl/crypto/ct/ct_sct_ctx.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ct_sct.o: 3rdparty/openssl/crypto/ct/ct_sct.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ct_x509v3.o: CPPFLAGS += -I3rdparty/openssl/crypto/ct
$(BUILDDIR)ct_x509v3.o: 3rdparty/openssl/crypto/ct/ct_x509v3.c 3rdparty/openssl/crypto/ct/ct_local.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ct_prn.o: 3rdparty/openssl/crypto/ct/ct_prn.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ct_log.o: 3rdparty/openssl/crypto/ct/ct_log.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ct_b64.o: 3rdparty/openssl/crypto/ct/ct_b64.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ct_policy.o: 3rdparty/openssl/crypto/ct/ct_policy.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ct_oct.o: 3rdparty/openssl/crypto/ct/ct_oct.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)e_camellia.o: CPPFLAGS += -I3rdparty/openssl/include/openssl -I3rdparty/openssl/crypto/modes
$(BUILDDIR)e_camellia.o: 3rdparty/openssl/crypto/evp/e_camellia.c 3rdparty/openssl/include/openssl/evp.h 3rdparty/openssl/crypto/modes/modes_local.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)e_idea.o: 3rdparty/openssl/crypto/evp/e_idea.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)m_md4.o: 3rdparty/openssl/crypto/evp/m_md4.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)names.o: 3rdparty/openssl/crypto/evp/names.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)evp_err.o: 3rdparty/openssl/crypto/evp/evp_err.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)m_sha1.o: 3rdparty/openssl/crypto/evp/m_sha1.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bio_md.o: 3rdparty/openssl/crypto/evp/bio_md.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)m_ripemd.o: 3rdparty/openssl/crypto/evp/m_ripemd.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)evp_lib.o: 3rdparty/openssl/crypto/evp/evp_lib.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bio_enc.o: 3rdparty/openssl/crypto/evp/bio_enc.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)pmeth_gn.o: 3rdparty/openssl/crypto/evp/pmeth_gn.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)c_alld.o: 3rdparty/openssl/crypto/evp/c_alld.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)m_sha3.o: 3rdparty/openssl/crypto/evp/m_sha3.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)e_rc2.o: 3rdparty/openssl/crypto/evp/e_rc2.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bio_b64.o: 3rdparty/openssl/crypto/evp/bio_b64.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)p_enc.o: 3rdparty/openssl/crypto/evp/p_enc.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)e_null.o: 3rdparty/openssl/crypto/evp/e_null.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)e_aes_cbc_hmac_sha256.o: 3rdparty/openssl/crypto/evp/e_aes_cbc_hmac_sha256.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)e_rc4_hmac_md5.o: 3rdparty/openssl/crypto/evp/e_rc4_hmac_md5.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)p_open.o: 3rdparty/openssl/crypto/evp/p_open.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)p_lib.o: 3rdparty/openssl/crypto/evp/p_lib.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)e_bf.o: 3rdparty/openssl/crypto/evp/e_bf.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)e_sm4.o: 3rdparty/openssl/crypto/evp/e_sm4.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)pmeth_lib.o: 3rdparty/openssl/crypto/evp/pmeth_lib.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)encode.o: 3rdparty/openssl/crypto/evp/encode.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)p5_crpt2.o: CPPFLAGS += -I3rdparty/openssl/crypto/evp
$(BUILDDIR)p5_crpt2.o: 3rdparty/openssl/crypto/evp/p5_crpt2.c 3rdparty/openssl/crypto/evp/evp_local.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)e_seed.o: 3rdparty/openssl/crypto/evp/e_seed.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)e_aes_cbc_hmac_sha1.o: 3rdparty/openssl/crypto/evp/e_aes_cbc_hmac_sha1.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)evp_pkey.o: 3rdparty/openssl/crypto/evp/evp_pkey.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)e_des3.o: 3rdparty/openssl/crypto/evp/e_des3.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)e_rc5.o: 3rdparty/openssl/crypto/evp/e_rc5.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)m_md5.o: 3rdparty/openssl/crypto/evp/m_md5.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)p_dec.o: 3rdparty/openssl/crypto/evp/p_dec.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)m_md2.o: 3rdparty/openssl/crypto/evp/m_md2.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)e_aria.o: 3rdparty/openssl/crypto/evp/e_aria.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bio_ok.o: 3rdparty/openssl/crypto/evp/bio_ok.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)p5_crpt.o: 3rdparty/openssl/crypto/evp/p5_crpt.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)p_seal.o: 3rdparty/openssl/crypto/evp/p_seal.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)evp_pbe.o: 3rdparty/openssl/crypto/evp/evp_pbe.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)e_rc4.o: 3rdparty/openssl/crypto/evp/e_rc4.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)p_verify.o: 3rdparty/openssl/crypto/evp/p_verify.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)evp_enc.o: 3rdparty/openssl/crypto/evp/evp_enc.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cmeth_lib.o: 3rdparty/openssl/crypto/evp/cmeth_lib.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)digest.o: 3rdparty/openssl/crypto/evp/digest.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)pbe_scrypt.o: 3rdparty/openssl/crypto/evp/pbe_scrypt.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)e_chacha20_poly1305.o: 3rdparty/openssl/crypto/evp/e_chacha20_poly1305.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)e_des.o: 3rdparty/openssl/crypto/evp/e_des.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)m_mdc2.o: 3rdparty/openssl/crypto/evp/m_mdc2.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)evp_cnf.o: 3rdparty/openssl/crypto/evp/evp_cnf.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)e_old.o: 3rdparty/openssl/crypto/evp/e_old.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)c_allc.o: 3rdparty/openssl/crypto/evp/c_allc.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)m_md5_sha1.o: 3rdparty/openssl/crypto/evp/m_md5_sha1.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)evp_key.o: 3rdparty/openssl/crypto/evp/evp_key.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)m_wp.o: 3rdparty/openssl/crypto/evp/m_wp.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)pmeth_fn.o: 3rdparty/openssl/crypto/evp/pmeth_fn.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)m_sigver.o: 3rdparty/openssl/crypto/evp/m_sigver.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)e_cast.o: 3rdparty/openssl/crypto/evp/e_cast.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)e_aes.o: 3rdparty/openssl/crypto/evp/e_aes.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)p_sign.o: 3rdparty/openssl/crypto/evp/p_sign.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)e_xcbc_d.o: 3rdparty/openssl/crypto/evp/e_xcbc_d.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)m_null.o: 3rdparty/openssl/crypto/evp/m_null.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)seed_cbc.o: 3rdparty/openssl/crypto/seed/seed_cbc.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)seed_cfb.o: 3rdparty/openssl/crypto/seed/seed_cfb.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)seed_ofb.o: 3rdparty/openssl/crypto/seed/seed_ofb.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)seed_ecb.o: 3rdparty/openssl/crypto/seed/seed_ecb.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)seed.o: CPPFLAGS += -I3rdparty/openssl/crypto/seed
$(BUILDDIR)seed.o: 3rdparty/openssl/crypto/seed/seed.c 3rdparty/openssl/crypto/seed/seed_local.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ts_rsp_print.o: 3rdparty/openssl/crypto/ts/ts_rsp_print.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ts_err.o: 3rdparty/openssl/crypto/ts/ts_err.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ts_asn1.o: 3rdparty/openssl/crypto/ts/ts_asn1.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ts_req_print.o: 3rdparty/openssl/crypto/ts/ts_req_print.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ts_rsp_verify.o: 3rdparty/openssl/crypto/ts/ts_rsp_verify.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ts_req_utils.o: 3rdparty/openssl/crypto/ts/ts_req_utils.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ts_rsp_sign.o: 3rdparty/openssl/crypto/ts/ts_rsp_sign.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ts_verify_ctx.o: 3rdparty/openssl/crypto/ts/ts_verify_ctx.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ts_lib.o: 3rdparty/openssl/crypto/ts/ts_lib.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ts_rsp_utils.o: 3rdparty/openssl/crypto/ts/ts_rsp_utils.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ts_conf.o: 3rdparty/openssl/crypto/ts/ts_conf.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)dso_err.o: 3rdparty/openssl/crypto/dso/dso_err.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)dso_dl.o: CPPFLAGS += -I3rdparty/openssl/crypto/dso
$(BUILDDIR)dso_dl.o: 3rdparty/openssl/crypto/dso/dso_dl.c 3rdparty/openssl/crypto/dso/dso_local.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)dso_win32.o: 3rdparty/openssl/crypto/dso/dso_win32.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)dso_dlfcn.o: 3rdparty/openssl/crypto/dso/dso_dlfcn.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)dso_openssl.o: 3rdparty/openssl/crypto/dso/dso_openssl.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)dso_vms.o: 3rdparty/openssl/crypto/dso/dso_vms.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)dso_lib.o: 3rdparty/openssl/crypto/dso/dso_lib.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)rmd_dgst.o: 3rdparty/openssl/crypto/ripemd/rmd_dgst.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)rmd_one.o: 3rdparty/openssl/crypto/ripemd/rmd_one.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)threads_pthread.o: 3rdparty/openssl/crypto/threads_pthread.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bf_skey.o: 3rdparty/openssl/crypto/bf/bf_skey.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bf_cfb64.o: 3rdparty/openssl/crypto/bf/bf_cfb64.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bf_enc.o: 3rdparty/openssl/crypto/bf/bf_enc.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bf_ofb64.o: 3rdparty/openssl/crypto/bf/bf_ofb64.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bf_ecb.o: 3rdparty/openssl/crypto/bf/bf_ecb.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)i_ecb.o: 3rdparty/openssl/crypto/idea/i_ecb.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)i_ofb64.o: 3rdparty/openssl/crypto/idea/i_ofb64.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)i_cbc.o: 3rdparty/openssl/crypto/idea/i_cbc.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)i_skey.o: 3rdparty/openssl/crypto/idea/i_skey.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)i_cfb64.o: 3rdparty/openssl/crypto/idea/i_cfb64.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)txt_db.o: 3rdparty/openssl/crypto/txt_db/txt_db.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ameth_lib.o: 3rdparty/openssl/crypto/asn1/ameth_lib.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)a_utctm.o: 3rdparty/openssl/crypto/asn1/a_utctm.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)p5_pbe.o: 3rdparty/openssl/crypto/asn1/p5_pbe.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)asn1_gen.o: 3rdparty/openssl/crypto/asn1/asn1_gen.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)asn_mstbl.o: 3rdparty/openssl/crypto/asn1/asn_mstbl.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)asn1_par.o: 3rdparty/openssl/crypto/asn1/asn1_par.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bio_asn1.o: 3rdparty/openssl/crypto/asn1/bio_asn1.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)a_strnid.o: 3rdparty/openssl/crypto/asn1/a_strnid.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)i2d_pu.o: 3rdparty/openssl/crypto/asn1/i2d_pu.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)t_pkey.o: 3rdparty/openssl/crypto/asn1/t_pkey.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)d2i_pr.o: 3rdparty/openssl/crypto/asn1/d2i_pr.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)a_gentm.o: 3rdparty/openssl/crypto/asn1/a_gentm.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)a_verify.o: 3rdparty/openssl/crypto/asn1/a_verify.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)a_strex.o: 3rdparty/openssl/crypto/asn1/a_strex.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)tasn_prn.o: 3rdparty/openssl/crypto/asn1/tasn_prn.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)tasn_fre.o: 3rdparty/openssl/crypto/asn1/tasn_fre.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)f_string.o: 3rdparty/openssl/crypto/asn1/f_string.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)a_bitstr.o: 3rdparty/openssl/crypto/asn1/a_bitstr.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)x_sig.o: 3rdparty/openssl/crypto/asn1/x_sig.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)x_long.o: 3rdparty/openssl/crypto/asn1/x_long.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)a_print.o: 3rdparty/openssl/crypto/asn1/a_print.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)x_spki.o: 3rdparty/openssl/crypto/asn1/x_spki.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)asn1_err.o: 3rdparty/openssl/crypto/asn1/asn1_err.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)a_int.o: 3rdparty/openssl/crypto/asn1/a_int.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)asn_mime.o: 3rdparty/openssl/crypto/asn1/asn_mime.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)a_i2d_fp.o: 3rdparty/openssl/crypto/asn1/a_i2d_fp.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)p5_pbev2.o: 3rdparty/openssl/crypto/asn1/p5_pbev2.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bio_ndef.o: 3rdparty/openssl/crypto/asn1/bio_ndef.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)a_type.o: 3rdparty/openssl/crypto/asn1/a_type.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)n_pkey.o: 3rdparty/openssl/crypto/asn1/n_pkey.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)x_int64.o: 3rdparty/openssl/crypto/asn1/x_int64.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)a_octet.o: 3rdparty/openssl/crypto/asn1/a_octet.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)p5_scrypt.o: 3rdparty/openssl/crypto/asn1/p5_scrypt.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)nsseq.o: 3rdparty/openssl/crypto/asn1/nsseq.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)a_time.o: 3rdparty/openssl/crypto/asn1/a_time.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)asn1_lib.o: 3rdparty/openssl/crypto/asn1/asn1_lib.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)x_algor.o: 3rdparty/openssl/crypto/asn1/x_algor.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)i2d_pr.o: 3rdparty/openssl/crypto/asn1/i2d_pr.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)f_int.o: 3rdparty/openssl/crypto/asn1/f_int.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)x_bignum.o: 3rdparty/openssl/crypto/asn1/x_bignum.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)a_sign.o: 3rdparty/openssl/crypto/asn1/a_sign.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)asn_moid.o: 3rdparty/openssl/crypto/asn1/asn_moid.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)a_d2i_fp.o: 3rdparty/openssl/crypto/asn1/a_d2i_fp.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)tasn_new.o: 3rdparty/openssl/crypto/asn1/tasn_new.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)a_object.o: 3rdparty/openssl/crypto/asn1/a_object.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)d2i_pu.o: 3rdparty/openssl/crypto/asn1/d2i_pu.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)t_bitst.o: 3rdparty/openssl/crypto/asn1/t_bitst.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)evp_asn1.o: 3rdparty/openssl/crypto/asn1/evp_asn1.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)a_utf8.o: 3rdparty/openssl/crypto/asn1/a_utf8.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)tasn_scn.o: 3rdparty/openssl/crypto/asn1/tasn_scn.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)a_dup.o: 3rdparty/openssl/crypto/asn1/a_dup.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)p8_pkey.o: 3rdparty/openssl/crypto/asn1/p8_pkey.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)asn1_item_list.o: 3rdparty/openssl/crypto/asn1/asn1_item_list.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)tasn_utl.o: 3rdparty/openssl/crypto/asn1/tasn_utl.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)tasn_dec.o: 3rdparty/openssl/crypto/asn1/tasn_dec.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)t_spki.o: 3rdparty/openssl/crypto/asn1/t_spki.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)a_digest.o: 3rdparty/openssl/crypto/asn1/a_digest.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)a_mbstr.o: 3rdparty/openssl/crypto/asn1/a_mbstr.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)tasn_typ.o: 3rdparty/openssl/crypto/asn1/tasn_typ.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)asn_pack.o: 3rdparty/openssl/crypto/asn1/asn_pack.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)tasn_enc.o: 3rdparty/openssl/crypto/asn1/tasn_enc.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)x_info.o: 3rdparty/openssl/crypto/asn1/x_info.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)x_val.o: 3rdparty/openssl/crypto/asn1/x_val.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)x_pkey.o: 3rdparty/openssl/crypto/asn1/x_pkey.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)siphash_pmeth.o: 3rdparty/openssl/crypto/siphash/siphash_pmeth.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)siphash_ameth.o: 3rdparty/openssl/crypto/siphash/siphash_ameth.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)siphash.o: 3rdparty/openssl/crypto/siphash/siphash.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)store_register.o: 3rdparty/openssl/crypto/store/store_register.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)store_lib.o: 3rdparty/openssl/crypto/store/store_lib.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)store_err.o: 3rdparty/openssl/crypto/store/store_err.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)loader_file.o: 3rdparty/openssl/crypto/store/loader_file.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)store_strings.o: 3rdparty/openssl/crypto/store/store_strings.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)store_init.o: 3rdparty/openssl/crypto/store/store_init.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)dh_rfc7919.o: 3rdparty/openssl/crypto/dh/dh_rfc7919.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)dh_kdf.o: 3rdparty/openssl/crypto/dh/dh_kdf.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)dh_lib.o: 3rdparty/openssl/crypto/dh/dh_lib.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)dh_rfc5114.o: 3rdparty/openssl/crypto/dh/dh_rfc5114.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)dh_prn.o: 3rdparty/openssl/crypto/dh/dh_prn.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)dh_pmeth.o: 3rdparty/openssl/crypto/dh/dh_pmeth.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)dh_asn1.o: 3rdparty/openssl/crypto/dh/dh_asn1.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)dh_err.o: 3rdparty/openssl/crypto/dh/dh_err.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)dh_meth.o: CPPFLAGS += -I3rdparty/openssl/crypto/dh
$(BUILDDIR)dh_meth.o: 3rdparty/openssl/crypto/dh/dh_meth.c 3rdparty/openssl/crypto/dh/dh_local.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)dh_depr.o: 3rdparty/openssl/crypto/dh/dh_depr.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)dh_check.o: 3rdparty/openssl/crypto/dh/dh_check.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)dh_ameth.o: 3rdparty/openssl/crypto/dh/dh_ameth.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)dh_gen.o: 3rdparty/openssl/crypto/dh/dh_gen.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)dh_key.o: 3rdparty/openssl/crypto/dh/dh_key.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ebcdic.o: 3rdparty/openssl/crypto/ebcdic.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)mdc2dgst.o: 3rdparty/openssl/crypto/mdc2/mdc2dgst.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)mdc2_one.o: 3rdparty/openssl/crypto/mdc2/mdc2_one.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cmll_ofb.o: 3rdparty/openssl/crypto/camellia/cmll_ofb.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cmll_cbc.o: 3rdparty/openssl/crypto/camellia/cmll_cbc.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cmll_misc.o: 3rdparty/openssl/crypto/camellia/cmll_misc.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)camellia.o: 3rdparty/openssl/crypto/camellia/camellia.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cmll_cfb.o: 3rdparty/openssl/crypto/camellia/cmll_cfb.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cmll_ctr.o: 3rdparty/openssl/crypto/camellia/cmll_ctr.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cmll_ecb.o: 3rdparty/openssl/crypto/camellia/cmll_ecb.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)LPdir_wince.o: 3rdparty/openssl/crypto/LPdir_wince.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)o_init.o: 3rdparty/openssl/crypto/o_init.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)m_sm3.o: 3rdparty/openssl/crypto/sm3/m_sm3.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)sm3.o: 3rdparty/openssl/crypto/sm3/sm3.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)chacha_enc.o: 3rdparty/openssl/crypto/chacha/chacha_enc.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)c_ecb.o: 3rdparty/openssl/crypto/cast/c_ecb.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)c_ofb64.o: 3rdparty/openssl/crypto/cast/c_ofb64.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)c_enc.o: 3rdparty/openssl/crypto/cast/c_enc.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)c_skey.o: 3rdparty/openssl/crypto/cast/c_skey.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)c_cfb64.o: 3rdparty/openssl/crypto/cast/c_cfb64.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)LPdir_unix.o: 3rdparty/openssl/crypto/LPdir_unix.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)s390xcap.o: 3rdparty/openssl/crypto/s390xcap.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)LPdir_vms.o: 3rdparty/openssl/crypto/LPdir_vms.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)wp_block.o: CPPFLAGS += -I3rdparty/openssl/crypto/whrlpool
$(BUILDDIR)wp_block.o: 3rdparty/openssl/crypto/whrlpool/wp_block.c 3rdparty/openssl/crypto/whrlpool/wp_local.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)wp_dgst.o: 3rdparty/openssl/crypto/whrlpool/wp_dgst.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)mem_sec.o: 3rdparty/openssl/crypto/mem_sec.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)pcbc_enc.o: CPPFLAGS += -I3rdparty/openssl/crypto/des
$(BUILDDIR)pcbc_enc.o: 3rdparty/openssl/crypto/des/pcbc_enc.c 3rdparty/openssl/crypto/des/des_local.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ecb3_enc.o: 3rdparty/openssl/crypto/des/ecb3_enc.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)str2key.o: 3rdparty/openssl/crypto/des/str2key.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)xcbc_enc.o: 3rdparty/openssl/crypto/des/xcbc_enc.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)fcrypt.o: 3rdparty/openssl/crypto/des/fcrypt.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ncbc_enc.o: 3rdparty/openssl/crypto/des/ncbc_enc.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)des_enc.o: 3rdparty/openssl/crypto/des/des_enc.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ofb64ede.o: 3rdparty/openssl/crypto/des/ofb64ede.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)qud_cksm.o: 3rdparty/openssl/crypto/des/qud_cksm.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cfb_enc.o: 3rdparty/openssl/crypto/des/cfb_enc.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)rand_key.o: 3rdparty/openssl/crypto/des/rand_key.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cbc_cksm.o: 3rdparty/openssl/crypto/des/cbc_cksm.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)fcrypt_b.o: 3rdparty/openssl/crypto/des/fcrypt_b.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ecb_enc.o: 3rdparty/openssl/crypto/des/ecb_enc.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cfb64ede.o: 3rdparty/openssl/crypto/des/cfb64ede.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)set_key.o: 3rdparty/openssl/crypto/des/set_key.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cfb64enc.o: 3rdparty/openssl/crypto/des/cfb64enc.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ofb_enc.o: 3rdparty/openssl/crypto/des/ofb_enc.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ofb64enc.o: 3rdparty/openssl/crypto/des/ofb64enc.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cbc_enc.o: 3rdparty/openssl/crypto/des/cbc_enc.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ui_err.o: 3rdparty/openssl/crypto/ui/ui_err.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ui_openssl.o: CPPFLAGS += -I3rdparty/openssl/crypto/ui
$(BUILDDIR)ui_openssl.o: 3rdparty/openssl/crypto/ui/ui_openssl.c 3rdparty/openssl/crypto/ui/ui_local.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ui_util.o: 3rdparty/openssl/crypto/ui/ui_util.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ui_null.o: 3rdparty/openssl/crypto/ui/ui_null.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ui_lib.o: 3rdparty/openssl/crypto/ui/ui_lib.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)mem_dbg.o: 3rdparty/openssl/crypto/mem_dbg.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)md4_dgst.o: 3rdparty/openssl/crypto/md4/md4_dgst.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)md4_one.o: 3rdparty/openssl/crypto/md4/md4_one.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cryptlib.o: 3rdparty/openssl/crypto/cryptlib.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)lhash.o: 3rdparty/openssl/crypto/lhash/lhash.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)lh_stats.o: 3rdparty/openssl/crypto/lhash/lh_stats.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)o_fopen.o: 3rdparty/openssl/crypto/o_fopen.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ocsp_srv.o: 3rdparty/openssl/crypto/ocsp/ocsp_srv.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ocsp_prn.o: 3rdparty/openssl/crypto/ocsp/ocsp_prn.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ocsp_vfy.o: 3rdparty/openssl/crypto/ocsp/ocsp_vfy.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ocsp_lib.o: 3rdparty/openssl/crypto/ocsp/ocsp_lib.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ocsp_asn.o: 3rdparty/openssl/crypto/ocsp/ocsp_asn.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ocsp_ht.o: 3rdparty/openssl/crypto/ocsp/ocsp_ht.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)v3_ocsp.o: CPPFLAGS += -I3rdparty/openssl/crypto/ocsp
$(BUILDDIR)v3_ocsp.o: 3rdparty/openssl/crypto/ocsp/v3_ocsp.c 3rdparty/openssl/crypto/ocsp/ocsp_local.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ocsp_err.o: 3rdparty/openssl/crypto/ocsp/ocsp_err.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ocsp_ext.o: 3rdparty/openssl/crypto/ocsp/ocsp_ext.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ocsp_cl.o: 3rdparty/openssl/crypto/ocsp/ocsp_cl.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)srp_lib.o: 3rdparty/openssl/crypto/srp/srp_lib.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)srp_vfy.o: 3rdparty/openssl/crypto/srp/srp_vfy.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)comp_err.o: 3rdparty/openssl/crypto/comp/comp_err.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)comp_lib.o: 3rdparty/openssl/crypto/comp/comp_lib.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)c_zlib.o: 3rdparty/openssl/crypto/comp/c_zlib.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)pk7_lib.o: 3rdparty/openssl/crypto/pkcs7/pk7_lib.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)pk7_asn1.o: 3rdparty/openssl/crypto/pkcs7/pk7_asn1.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)pk7_doit.o: 3rdparty/openssl/crypto/pkcs7/pk7_doit.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bio_pk7.o: 3rdparty/openssl/crypto/pkcs7/bio_pk7.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)pkcs7err.o: 3rdparty/openssl/crypto/pkcs7/pkcs7err.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)pk7_mime.o: 3rdparty/openssl/crypto/pkcs7/pk7_mime.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)pk7_attr.o: 3rdparty/openssl/crypto/pkcs7/pk7_attr.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)pk7_smime.o: 3rdparty/openssl/crypto/pkcs7/pk7_smime.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)hkdf.o: 3rdparty/openssl/crypto/kdf/hkdf.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)kdf_err.o: 3rdparty/openssl/crypto/kdf/kdf_err.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)scrypt.o: 3rdparty/openssl/crypto/kdf/scrypt.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)tls1_prf.o: 3rdparty/openssl/crypto/kdf/tls1_prf.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)dsa_depr.o: 3rdparty/openssl/crypto/dsa/dsa_depr.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)dsa_key.o: 3rdparty/openssl/crypto/dsa/dsa_key.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)dsa_ossl.o: 3rdparty/openssl/crypto/dsa/dsa_ossl.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)dsa_lib.o: 3rdparty/openssl/crypto/dsa/dsa_lib.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)dsa_ameth.o: 3rdparty/openssl/crypto/dsa/dsa_ameth.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)dsa_prn.o: 3rdparty/openssl/crypto/dsa/dsa_prn.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)dsa_vrf.o: 3rdparty/openssl/crypto/dsa/dsa_vrf.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)dsa_pmeth.o: 3rdparty/openssl/crypto/dsa/dsa_pmeth.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)dsa_asn1.o: 3rdparty/openssl/crypto/dsa/dsa_asn1.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)dsa_err.o: 3rdparty/openssl/crypto/dsa/dsa_err.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)dsa_sign.o: 3rdparty/openssl/crypto/dsa/dsa_sign.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)dsa_gen.o: 3rdparty/openssl/crypto/dsa/dsa_gen.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)dsa_meth.o: CPPFLAGS += -I3rdparty/openssl/crypto/dsa
$(BUILDDIR)dsa_meth.o: 3rdparty/openssl/crypto/dsa/dsa_meth.c 3rdparty/openssl/crypto/dsa/dsa_local.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ctype.o: 3rdparty/openssl/crypto/ctype.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)o_fips.o: 3rdparty/openssl/crypto/o_fips.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ppccap.o: 3rdparty/openssl/crypto/ppccap.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)rc4_skey.o: 3rdparty/openssl/crypto/rc4/rc4_skey.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)rc4_enc.o: 3rdparty/openssl/crypto/rc4/rc4_enc.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)threads_none.o: 3rdparty/openssl/crypto/threads_none.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)o_str.o: 3rdparty/openssl/crypto/o_str.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)err_all.o: 3rdparty/openssl/crypto/err/err_all.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)err.o: 3rdparty/openssl/crypto/err/err.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)err_prn.o: 3rdparty/openssl/crypto/err/err_prn.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)armcap.o: 3rdparty/openssl/crypto/armcap.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bf_lbuf.o: 3rdparty/openssl/crypto/bio/bf_lbuf.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)b_sock2.o: 3rdparty/openssl/crypto/bio/b_sock2.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bss_null.o: 3rdparty/openssl/crypto/bio/bss_null.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bss_acpt.o: 3rdparty/openssl/crypto/bio/bss_acpt.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bss_mem.o: 3rdparty/openssl/crypto/bio/bss_mem.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bf_buff.o: 3rdparty/openssl/crypto/bio/bf_buff.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)b_sock.o: 3rdparty/openssl/crypto/bio/b_sock.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)b_print.o: 3rdparty/openssl/crypto/bio/b_print.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bss_dgram.o: 3rdparty/openssl/crypto/bio/bss_dgram.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bio_err.o: 3rdparty/openssl/crypto/bio/bio_err.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)b_addr.o: 3rdparty/openssl/crypto/bio/b_addr.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bss_sock.o: 3rdparty/openssl/crypto/bio/bss_sock.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bio_cb.o: 3rdparty/openssl/crypto/bio/bio_cb.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bss_bio.o: 3rdparty/openssl/crypto/bio/bss_bio.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bio_lib.o: 3rdparty/openssl/crypto/bio/bio_lib.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bf_null.o: 3rdparty/openssl/crypto/bio/bf_null.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bss_log.o: 3rdparty/openssl/crypto/bio/bss_log.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bss_conn.o: 3rdparty/openssl/crypto/bio/bss_conn.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bss_fd.o: 3rdparty/openssl/crypto/bio/bss_fd.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bf_nbio.o: 3rdparty/openssl/crypto/bio/bf_nbio.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)b_dump.o: 3rdparty/openssl/crypto/bio/b_dump.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bio_meth.o: CPPFLAGS += -I3rdparty/openssl/crypto/bio
$(BUILDDIR)bio_meth.o: 3rdparty/openssl/crypto/bio/bio_meth.c 3rdparty/openssl/crypto/bio/bio_local.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bss_file.o: 3rdparty/openssl/crypto/bio/bss_file.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)stack.o: 3rdparty/openssl/crypto/stack/stack.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cpt_err.o: 3rdparty/openssl/crypto/cpt_err.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)getenv.o: 3rdparty/openssl/crypto/getenv.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)m_blake2s.o: CPPFLAGS += -I3rdparty/openssl/crypto/blake2
$(BUILDDIR)m_blake2s.o: 3rdparty/openssl/crypto/blake2/m_blake2s.c 3rdparty/openssl/crypto/blake2/blake2_local.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)m_blake2b.o: 3rdparty/openssl/crypto/blake2/m_blake2b.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)blake2s.o: 3rdparty/openssl/crypto/blake2/blake2s.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)blake2b.o: 3rdparty/openssl/crypto/blake2/blake2b.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)threads_win.o: 3rdparty/openssl/crypto/threads_win.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)rsa_none.o: 3rdparty/openssl/crypto/rsa/rsa_none.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)rsa_x931g.o: 3rdparty/openssl/crypto/rsa/rsa_x931g.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)rsa_prn.o: 3rdparty/openssl/crypto/rsa/rsa_prn.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)rsa_meth.o: 3rdparty/openssl/crypto/rsa/rsa_meth.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)rsa_pmeth.o: 3rdparty/openssl/crypto/rsa/rsa_pmeth.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)rsa_oaep.o: 3rdparty/openssl/crypto/rsa/rsa_oaep.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)rsa_crpt.o: 3rdparty/openssl/crypto/rsa/rsa_crpt.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)rsa_mp.o: 3rdparty/openssl/crypto/rsa/rsa_mp.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)rsa_saos.o: 3rdparty/openssl/crypto/rsa/rsa_saos.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)rsa_ossl.o: 3rdparty/openssl/crypto/rsa/rsa_ossl.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)rsa_asn1.o: 3rdparty/openssl/crypto/rsa/rsa_asn1.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)rsa_sign.o: 3rdparty/openssl/crypto/rsa/rsa_sign.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)rsa_ssl.o: 3rdparty/openssl/crypto/rsa/rsa_ssl.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)rsa_gen.o: 3rdparty/openssl/crypto/rsa/rsa_gen.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)rsa_ameth.o: 3rdparty/openssl/crypto/rsa/rsa_ameth.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)rsa_err.o: 3rdparty/openssl/crypto/rsa/rsa_err.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)rsa_pk1.o: 3rdparty/openssl/crypto/rsa/rsa_pk1.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)rsa_lib.o: 3rdparty/openssl/crypto/rsa/rsa_lib.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)rsa_x931.o: 3rdparty/openssl/crypto/rsa/rsa_x931.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)rsa_pss.o: 3rdparty/openssl/crypto/rsa/rsa_pss.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)rsa_depr.o: 3rdparty/openssl/crypto/rsa/rsa_depr.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)rsa_chk.o: 3rdparty/openssl/crypto/rsa/rsa_chk.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)v3_conf.o: 3rdparty/openssl/crypto/x509v3/v3_conf.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)pcy_lib.o: 3rdparty/openssl/crypto/x509v3/pcy_lib.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)v3err.o: 3rdparty/openssl/crypto/x509v3/v3err.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)v3_akey.o: 3rdparty/openssl/crypto/x509v3/v3_akey.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)v3_pcia.o: 3rdparty/openssl/crypto/x509v3/v3_pcia.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)v3_sxnet.o: 3rdparty/openssl/crypto/x509v3/v3_sxnet.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)v3_ncons.o: 3rdparty/openssl/crypto/x509v3/v3_ncons.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)v3_alt.o: 3rdparty/openssl/crypto/x509v3/v3_alt.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)pcy_data.o: 3rdparty/openssl/crypto/x509v3/pcy_data.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)v3_pci.o: 3rdparty/openssl/crypto/x509v3/v3_pci.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)v3_utl.o: 3rdparty/openssl/crypto/x509v3/v3_utl.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)v3_enum.o: 3rdparty/openssl/crypto/x509v3/v3_enum.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)v3_bcons.o: 3rdparty/openssl/crypto/x509v3/v3_bcons.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)v3_akeya.o: 3rdparty/openssl/crypto/x509v3/v3_akeya.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)v3_genn.o: 3rdparty/openssl/crypto/x509v3/v3_genn.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)v3_skey.o: 3rdparty/openssl/crypto/x509v3/v3_skey.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)v3_extku.o: 3rdparty/openssl/crypto/x509v3/v3_extku.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)v3_pmaps.o: 3rdparty/openssl/crypto/x509v3/v3_pmaps.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)v3_tlsf.o: 3rdparty/openssl/crypto/x509v3/v3_tlsf.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)v3_prn.o: 3rdparty/openssl/crypto/x509v3/v3_prn.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)pcy_tree.o: 3rdparty/openssl/crypto/x509v3/pcy_tree.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)v3_addr.o: 3rdparty/openssl/crypto/x509v3/v3_addr.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)v3_bitst.o: 3rdparty/openssl/crypto/x509v3/v3_bitst.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)v3_int.o: 3rdparty/openssl/crypto/x509v3/v3_int.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)pcy_cache.o: 3rdparty/openssl/crypto/x509v3/pcy_cache.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)v3_info.o: 3rdparty/openssl/crypto/x509v3/v3_info.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)v3_admis.o: 3rdparty/openssl/crypto/x509v3/v3_admis.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)v3_ia5.o: 3rdparty/openssl/crypto/x509v3/v3_ia5.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)pcy_map.o: 3rdparty/openssl/crypto/x509v3/pcy_map.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)v3_purp.o: 3rdparty/openssl/crypto/x509v3/v3_purp.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)v3_cpols.o: 3rdparty/openssl/crypto/x509v3/v3_cpols.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)v3_pcons.o: 3rdparty/openssl/crypto/x509v3/v3_pcons.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)v3_asid.o: 3rdparty/openssl/crypto/x509v3/v3_asid.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)v3_lib.o: 3rdparty/openssl/crypto/x509v3/v3_lib.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)pcy_node.o: 3rdparty/openssl/crypto/x509v3/pcy_node.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)v3_pku.o: 3rdparty/openssl/crypto/x509v3/v3_pku.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)v3_crld.o: 3rdparty/openssl/crypto/x509v3/v3_crld.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ex_data.o: 3rdparty/openssl/crypto/ex_data.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)LPdir_win32.o: 3rdparty/openssl/crypto/LPdir_win32.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)o_dir.o: 3rdparty/openssl/crypto/o_dir.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)LPdir_win.o: 3rdparty/openssl/crypto/LPdir_win.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)LPdir_nyi.o: 3rdparty/openssl/crypto/LPdir_nyi.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ecp_smpl.o: 3rdparty/openssl/crypto/ec/ecp_smpl.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ecdh_kdf.o: 3rdparty/openssl/crypto/ec/ecdh_kdf.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ecx_meth.o: 3rdparty/openssl/crypto/ec/ecx_meth.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ec_mult.o: 3rdparty/openssl/crypto/ec/ec_mult.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ecp_oct.o: 3rdparty/openssl/crypto/ec/ecp_oct.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)curve25519.o: 3rdparty/openssl/crypto/ec/curve25519.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)curve448.o: 3rdparty/openssl/crypto/ec/curve448/curve448.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)f_generic.o: CPPFLAGS += -I3rdparty/openssl/crypto/ec/curve448
$(BUILDDIR)f_generic.o: 3rdparty/openssl/crypto/ec/curve448/f_generic.c 3rdparty/openssl/crypto/ec/curve448/field.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)f_impl.o: 3rdparty/openssl/crypto/ec/curve448/arch_32/f_impl.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)scalar.o: 3rdparty/openssl/crypto/ec/curve448/scalar.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)eddsa.o: 3rdparty/openssl/crypto/ec/curve448/eddsa.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)curve448_tables.o: 3rdparty/openssl/crypto/ec/curve448/curve448_tables.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ecdsa_ossl.o: 3rdparty/openssl/crypto/ec/ecdsa_ossl.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ecp_mont.o: 3rdparty/openssl/crypto/ec/ecp_mont.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ec_curve.o: 3rdparty/openssl/crypto/ec/ec_curve.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ec_key.o: 3rdparty/openssl/crypto/ec/ec_key.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ec_asn1.o: 3rdparty/openssl/crypto/ec/ec_asn1.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ec_lib.o: 3rdparty/openssl/crypto/ec/ec_lib.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ecdsa_vrf.o: 3rdparty/openssl/crypto/ec/ecdsa_vrf.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ecp_nistputil.o: CPPFLAGS += -I3rdparty/openssl/crypto/ec
$(BUILDDIR)ecp_nistputil.o: 3rdparty/openssl/crypto/ec/ecp_nistputil.c 3rdparty/openssl/crypto/ec/ec_local.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ec_oct.o: 3rdparty/openssl/crypto/ec/ec_oct.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ecdsa_sign.o: 3rdparty/openssl/crypto/ec/ecdsa_sign.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)eck_prn.o: 3rdparty/openssl/crypto/ec/eck_prn.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ecp_nistp256.o: 3rdparty/openssl/crypto/ec/ecp_nistp256.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ec_kmeth.o: 3rdparty/openssl/crypto/ec/ec_kmeth.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ec2_oct.o: 3rdparty/openssl/crypto/ec/ec2_oct.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ecp_nistz256_table.o: 3rdparty/openssl/crypto/ec/ecp_nistz256_table.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ecp_nistp224.o: 3rdparty/openssl/crypto/ec/ecp_nistp224.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ec_cvt.o: 3rdparty/openssl/crypto/ec/ec_cvt.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ec2_smpl.o: 3rdparty/openssl/crypto/ec/ec2_smpl.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ecdh_ossl.o: 3rdparty/openssl/crypto/ec/ecdh_ossl.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ec_check.o: 3rdparty/openssl/crypto/ec/ec_check.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ecp_nistp521.o: 3rdparty/openssl/crypto/ec/ecp_nistp521.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ec_err.o: 3rdparty/openssl/crypto/ec/ec_err.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ec_print.o: 3rdparty/openssl/crypto/ec/ec_print.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ecp_nistz256.o: 3rdparty/openssl/crypto/ec/ecp_nistz256.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ec_pmeth.o: 3rdparty/openssl/crypto/ec/ec_pmeth.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ec_ameth.o: 3rdparty/openssl/crypto/ec/ec_ameth.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ecp_nist.o: 3rdparty/openssl/crypto/ec/ecp_nist.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)x509_def.o: 3rdparty/openssl/crypto/x509/x509_def.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)x509_r2x.o: 3rdparty/openssl/crypto/x509/x509_r2x.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)t_req.o: 3rdparty/openssl/crypto/x509/t_req.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)x509spki.o: 3rdparty/openssl/crypto/x509/x509spki.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)t_crl.o: 3rdparty/openssl/crypto/x509/t_crl.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)x_attrib.o: 3rdparty/openssl/crypto/x509/x_attrib.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)x_all.o: 3rdparty/openssl/crypto/x509/x_all.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)x509_vfy.o: 3rdparty/openssl/crypto/x509/x509_vfy.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)x509_set.o: 3rdparty/openssl/crypto/x509/x509_set.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)x509type.o: 3rdparty/openssl/crypto/x509/x509type.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)x_pubkey.o: 3rdparty/openssl/crypto/x509/x_pubkey.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)x509_trs.o: 3rdparty/openssl/crypto/x509/x509_trs.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)x509rset.o: 3rdparty/openssl/crypto/x509/x509rset.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)x509name.o: 3rdparty/openssl/crypto/x509/x509name.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)x_req.o: 3rdparty/openssl/crypto/x509/x_req.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)x509_meth.o: 3rdparty/openssl/crypto/x509/x509_meth.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)t_x509.o: 3rdparty/openssl/crypto/x509/t_x509.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)x_x509a.o: 3rdparty/openssl/crypto/x509/x_x509a.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)x509_obj.o: 3rdparty/openssl/crypto/x509/x509_obj.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)x509_ext.o: 3rdparty/openssl/crypto/x509/x509_ext.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)x509_lu.o: 3rdparty/openssl/crypto/x509/x509_lu.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)x_x509.o: 3rdparty/openssl/crypto/x509/x_x509.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)x_crl.o: 3rdparty/openssl/crypto/x509/x_crl.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)x_name.o: 3rdparty/openssl/crypto/x509/x_name.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)x509_req.o: 3rdparty/openssl/crypto/x509/x509_req.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)x509_att.o: 3rdparty/openssl/crypto/x509/x509_att.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)x509_v3.o: 3rdparty/openssl/crypto/x509/x509_v3.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)x509cset.o: 3rdparty/openssl/crypto/x509/x509cset.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)x509_err.o: 3rdparty/openssl/crypto/x509/x509_err.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)by_file.o: 3rdparty/openssl/crypto/x509/by_file.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)x_exten.o: 3rdparty/openssl/crypto/x509/x_exten.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)x509_txt.o: 3rdparty/openssl/crypto/x509/x509_txt.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)x509_cmp.o: 3rdparty/openssl/crypto/x509/x509_cmp.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)x509_vpm.o: 3rdparty/openssl/crypto/x509/x509_vpm.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)x509_d2.o: 3rdparty/openssl/crypto/x509/x509_d2.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)by_dir.o: 3rdparty/openssl/crypto/x509/by_dir.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)p12_attr.o: 3rdparty/openssl/crypto/pkcs12/p12_attr.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)p12_mutl.o: 3rdparty/openssl/crypto/pkcs12/p12_mutl.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)p12_add.o: 3rdparty/openssl/crypto/pkcs12/p12_add.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)p12_kiss.o: 3rdparty/openssl/crypto/pkcs12/p12_kiss.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)p12_asn.o: 3rdparty/openssl/crypto/pkcs12/p12_asn.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)p12_crt.o: 3rdparty/openssl/crypto/pkcs12/p12_crt.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)pk12err.o: 3rdparty/openssl/crypto/pkcs12/pk12err.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)p12_sbag.o: 3rdparty/openssl/crypto/pkcs12/p12_sbag.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)p12_init.o: 3rdparty/openssl/crypto/pkcs12/p12_init.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)p12_key.o: 3rdparty/openssl/crypto/pkcs12/p12_key.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)p12_decr.o: 3rdparty/openssl/crypto/pkcs12/p12_decr.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)p12_p8d.o: 3rdparty/openssl/crypto/pkcs12/p12_p8d.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)p12_crpt.o: 3rdparty/openssl/crypto/pkcs12/p12_crpt.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)p12_npas.o: 3rdparty/openssl/crypto/pkcs12/p12_npas.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)p12_p8e.o: 3rdparty/openssl/crypto/pkcs12/p12_p8e.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)p12_utl.o: 3rdparty/openssl/crypto/pkcs12/p12_utl.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bn_lib.o: 3rdparty/openssl/crypto/bn/bn_lib.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bn_gf2m.o: 3rdparty/openssl/crypto/bn/bn_gf2m.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bn_mpi.o: 3rdparty/openssl/crypto/bn/bn_mpi.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bn_word.o: 3rdparty/openssl/crypto/bn/bn_word.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bn_blind.o: 3rdparty/openssl/crypto/bn/bn_blind.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bn_print.o: 3rdparty/openssl/crypto/bn/bn_print.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bn_rand.o: 3rdparty/openssl/crypto/bn/bn_rand.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bn_intern.o: 3rdparty/openssl/crypto/bn/bn_intern.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bn_add.o: 3rdparty/openssl/crypto/bn/bn_add.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bn_sqrt.o: 3rdparty/openssl/crypto/bn/bn_sqrt.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bn_exp2.o: 3rdparty/openssl/crypto/bn/bn_exp2.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bn_recp.o: 3rdparty/openssl/crypto/bn/bn_recp.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bn_div.o: 3rdparty/openssl/crypto/bn/bn_div.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)rsaz_exp.o: 3rdparty/openssl/crypto/bn/rsaz_exp.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bn_sqr.o: 3rdparty/openssl/crypto/bn/bn_sqr.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bn_mont.o: 3rdparty/openssl/crypto/bn/bn_mont.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bn_kron.o: 3rdparty/openssl/crypto/bn/bn_kron.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)x86_64-gcc.o: 3rdparty/openssl/crypto/bn/asm/x86_64-gcc.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bn_nist.o: CPPFLAGS += -I3rdparty/openssl/crypto/bn
$(BUILDDIR)bn_nist.o: 3rdparty/openssl/crypto/bn/bn_nist.c 3rdparty/openssl/crypto/bn/bn_local.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bn_x931p.o: 3rdparty/openssl/crypto/bn/bn_x931p.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bn_exp.o: 3rdparty/openssl/crypto/bn/bn_exp.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bn_asm.o: 3rdparty/openssl/crypto/bn/bn_asm.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bn_prime.o: 3rdparty/openssl/crypto/bn/bn_prime.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bn_ctx.o: 3rdparty/openssl/crypto/bn/bn_ctx.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bn_gcd.o: 3rdparty/openssl/crypto/bn/bn_gcd.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bn_err.o: 3rdparty/openssl/crypto/bn/bn_err.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bn_srp.o: 3rdparty/openssl/crypto/bn/bn_srp.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bn_depr.o: 3rdparty/openssl/crypto/bn/bn_depr.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bn_dh.o: 3rdparty/openssl/crypto/bn/bn_dh.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bn_shift.o: 3rdparty/openssl/crypto/bn/bn_shift.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bn_mul.o: 3rdparty/openssl/crypto/bn/bn_mul.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bn_mod.o: 3rdparty/openssl/crypto/bn/bn_mod.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bn_const.o: 3rdparty/openssl/crypto/bn/bn_const.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)poly1305_ieee754.o: 3rdparty/openssl/crypto/poly1305/poly1305_ieee754.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)poly1305_ameth.o: 3rdparty/openssl/crypto/poly1305/poly1305_ameth.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)poly1305_pmeth.o: 3rdparty/openssl/crypto/poly1305/poly1305_pmeth.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)poly1305.o: 3rdparty/openssl/crypto/poly1305/poly1305.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)poly1305_base2_44.o: 3rdparty/openssl/crypto/poly1305/poly1305_base2_44.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)randfile.o: 3rdparty/openssl/crypto/rand/randfile.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)rand_err.o: 3rdparty/openssl/crypto/rand/rand_err.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)rand_win.o: 3rdparty/openssl/crypto/rand/rand_win.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)drbg_lib.o: 3rdparty/openssl/crypto/rand/drbg_lib.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)rand_egd.o: 3rdparty/openssl/crypto/rand/rand_egd.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)drbg_ctr.o: 3rdparty/openssl/crypto/rand/drbg_ctr.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)rand_vms.o: CPPFLAGS += -I3rdparty/openssl/crypto/rand
$(BUILDDIR)rand_vms.o: 3rdparty/openssl/crypto/rand/rand_vms.c 3rdparty/openssl/crypto/rand/rand_local.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)rand_lib.o: 3rdparty/openssl/crypto/rand/rand_lib.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)rand_unix.o: 3rdparty/openssl/crypto/rand/rand_unix.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cms_ver.o: 3rdparty/openssl/demos/cms/cms_ver.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cms_dec.o: 3rdparty/openssl/demos/cms/cms_dec.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cms_uncomp.o: 3rdparty/openssl/demos/cms/cms_uncomp.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cms_sign2.o: 3rdparty/openssl/demos/cms/cms_sign2.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cms_sign.o: 3rdparty/openssl/demos/cms/cms_sign.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cms_denc.o: 3rdparty/openssl/demos/cms/cms_denc.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cms_comp.o: 3rdparty/openssl/demos/cms/cms_comp.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cms_ddec.o: 3rdparty/openssl/demos/cms/cms_ddec.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cms_enc.o: 3rdparty/openssl/demos/cms/cms_enc.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)aesccm.o: 3rdparty/openssl/demos/evp/aesccm.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)aesgcm.o: 3rdparty/openssl/demos/evp/aesgcm.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)smdec.o: 3rdparty/openssl/demos/smime/smdec.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)smsign.o: 3rdparty/openssl/demos/smime/smsign.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)smsign2.o: 3rdparty/openssl/demos/smime/smsign2.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)smver.o: 3rdparty/openssl/demos/smime/smver.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)smenc.o: 3rdparty/openssl/demos/smime/smenc.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)client-conf.o: 3rdparty/openssl/demos/bio/client-conf.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)saccept.o: 3rdparty/openssl/demos/bio/saccept.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)sconnect.o: 3rdparty/openssl/demos/bio/sconnect.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)server-conf.o: 3rdparty/openssl/demos/bio/server-conf.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)server-arg.o: 3rdparty/openssl/demos/bio/server-arg.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)server-cmod.o: 3rdparty/openssl/demos/bio/server-cmod.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)client-arg.o: 3rdparty/openssl/demos/bio/client-arg.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)pkwrite.o: 3rdparty/openssl/demos/pkcs12/pkwrite.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)pkread.o: 3rdparty/openssl/demos/pkcs12/pkread.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)e_afalg.o: CPPFLAGS += -I3rdparty/openssl/engines
$(BUILDDIR)e_afalg.o: 3rdparty/openssl/engines/e_afalg.c 3rdparty/openssl/engines/e_afalg.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)e_dasync.o: 3rdparty/openssl/engines/e_dasync.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)e_afalg_err.o: 3rdparty/openssl/engines/e_afalg_err.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)e_dasync_err.o: 3rdparty/openssl/engines/e_dasync_err.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)e_ossltest.o: 3rdparty/openssl/engines/e_ossltest.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)e_ossltest_err.o: 3rdparty/openssl/engines/e_ossltest_err.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)e_capi_err.o: 3rdparty/openssl/engines/e_capi_err.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)e_capi.o: 3rdparty/openssl/engines/e_capi.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)e_padlock.o: 3rdparty/openssl/engines/e_padlock.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)decompress.o: CPPFLAGS += -I3rdparty/bzip2
$(BUILDDIR)decompress.o: 3rdparty/bzip2/decompress.c 3rdparty/bzip2/bzlib_private.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)huffman.o: 3rdparty/bzip2/huffman.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)compress.o: 3rdparty/bzip2/compress.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)crctable.o: 3rdparty/bzip2/crctable.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bzlib.o: 3rdparty/bzip2/bzlib.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)randtable.o: 3rdparty/bzip2/randtable.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)blocksort.o: 3rdparty/bzip2/blocksort.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)xz_pipe_decomp.o: 3rdparty/xz/doc/examples_old/xz_pipe_decomp.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)xz_pipe_comp.o: 3rdparty/xz/doc/examples_old/xz_pipe_comp.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)02_decompress.o: 3rdparty/xz/doc/examples/02_decompress.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)01_compress_easy.o: 3rdparty/xz/doc/examples/01_compress_easy.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)04_compress_easy_mt.o: 3rdparty/xz/doc/examples/04_compress_easy_mt.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)03_compress_custom.o: 3rdparty/xz/doc/examples/03_compress_custom.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)getopt1.o: CPPFLAGS += -Ilib
$(BUILDDIR)getopt1.o: 3rdparty/xz/lib/getopt1.c ./lib/getopt.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)getopt.o: 3rdparty/xz/lib/getopt.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)scanlzma.o: 3rdparty/xz/extra/scanlzma/scanlzma.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)test_bcj_exact_size.o: CPPFLAGS += -I3rdparty/xz/tests
$(BUILDDIR)test_bcj_exact_size.o: 3rdparty/xz/tests/test_bcj_exact_size.c 3rdparty/xz/tests/tests.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bcj_test.o: 3rdparty/xz/tests/bcj_test.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)create_compress_files.o: CPPFLAGS += -I3rdparty/xz/src/common
$(BUILDDIR)create_compress_files.o: 3rdparty/xz/tests/create_compress_files.c 3rdparty/xz/src/common/sysdefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)test_check.o: 3rdparty/xz/tests/test_check.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)test_index.o: 3rdparty/xz/tests/test_index.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)test_stream_flags.o: 3rdparty/xz/tests/test_stream_flags.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)test_filter_flags.o: 3rdparty/xz/tests/test_filter_flags.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)test_block_header.o: 3rdparty/xz/tests/test_block_header.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)hex2bin.o: 3rdparty/xz/debug/hex2bin.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)known_sizes.o: 3rdparty/xz/debug/known_sizes.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)sync_flush.o: 3rdparty/xz/debug/sync_flush.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)crc32.o: 3rdparty/xz/debug/crc32.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)memusage.o: 3rdparty/xz/debug/memusage.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)full_flush.o: 3rdparty/xz/debug/full_flush.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)repeat.o: 3rdparty/xz/debug/repeat.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)lzmainfo.o: 3rdparty/xz/src/lzmainfo/lzmainfo.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)xzdec.o: 3rdparty/xz/src/xzdec/xzdec.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)mytime.o: CPPFLAGS += -I3rdparty/xz/src/xz
$(BUILDDIR)mytime.o: 3rdparty/xz/src/xz/mytime.c 3rdparty/xz/src/xz/private.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)options.o: 3rdparty/xz/src/xz/options.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)coder.o: 3rdparty/xz/src/xz/coder.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)suffix.o: 3rdparty/xz/src/xz/suffix.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)hardware.o: 3rdparty/xz/src/xz/hardware.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)signals.o: 3rdparty/xz/src/xz/signals.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)args.o: 3rdparty/xz/src/xz/args.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)file_io.o: 3rdparty/xz/src/xz/file_io.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)message.o: 3rdparty/xz/src/xz/message.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)list.o: 3rdparty/xz/src/xz/list.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)main.o: 3rdparty/xz/src/xz/main.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)util.o: 3rdparty/xz/src/xz/util.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)simple_decoder.o: CPPFLAGS += -I3rdparty/xz/src/liblzma/simple
$(BUILDDIR)simple_decoder.o: 3rdparty/xz/src/liblzma/simple/simple_decoder.c 3rdparty/xz/src/liblzma/simple/simple_decoder.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)powerpc.o: 3rdparty/xz/src/liblzma/simple/powerpc.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)x86.o: 3rdparty/xz/src/liblzma/simple/x86.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)simple_encoder.o: 3rdparty/xz/src/liblzma/simple/simple_encoder.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)armthumb.o: 3rdparty/xz/src/liblzma/simple/armthumb.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)simple_coder.o: 3rdparty/xz/src/liblzma/simple/simple_coder.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)arm.o: 3rdparty/xz/src/liblzma/simple/arm.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ia64.o: 3rdparty/xz/src/liblzma/simple/ia64.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)sparc.o: 3rdparty/xz/src/liblzma/simple/sparc.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)lzma_encoder.o: CPPFLAGS += -I3rdparty/xz/src/liblzma/lzma
$(BUILDDIR)lzma_encoder.o: 3rdparty/xz/src/liblzma/lzma/lzma_encoder.c 3rdparty/xz/src/liblzma/lzma/lzma2_encoder.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)fastpos_table.o: CPPFLAGS += -I3rdparty/xz/src/liblzma/common
$(BUILDDIR)fastpos_table.o: 3rdparty/xz/src/liblzma/lzma/fastpos_table.c 3rdparty/xz/src/liblzma/common/common.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)lzma_encoder_presets.o: 3rdparty/xz/src/liblzma/lzma/lzma_encoder_presets.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)lzma_encoder_optimum_normal.o: 3rdparty/xz/src/liblzma/lzma/lzma_encoder_optimum_normal.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)lzma_encoder_optimum_fast.o: 3rdparty/xz/src/liblzma/lzma/lzma_encoder_optimum_fast.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)fastpos_tablegen.o: 3rdparty/xz/src/liblzma/lzma/fastpos_tablegen.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)lzma2_decoder.o: 3rdparty/xz/src/liblzma/lzma/lzma2_decoder.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)lzma_decoder.o: CPPFLAGS += -I3rdparty/xz/src/liblzma/lz
$(BUILDDIR)lzma_decoder.o: 3rdparty/xz/src/liblzma/lzma/lzma_decoder.c 3rdparty/xz/src/liblzma/lz/lz_decoder.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)lzma2_encoder.o: 3rdparty/xz/src/liblzma/lzma/lzma2_encoder.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)crc64_table.o: 3rdparty/xz/src/liblzma/check/crc64_table.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)crc64_fast.o: CPPFLAGS += -I3rdparty/xz/src/liblzma/check
$(BUILDDIR)crc64_fast.o: 3rdparty/xz/src/liblzma/check/crc64_fast.c 3rdparty/xz/src/liblzma/check/check.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)crc32_table.o: 3rdparty/xz/src/liblzma/check/crc32_table.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)sha256.o: 3rdparty/xz/src/liblzma/check/sha256.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)check.o: 3rdparty/xz/src/liblzma/check/check.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)crc32_small.o: 3rdparty/xz/src/liblzma/check/crc32_small.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)crc32_tablegen.o: 3rdparty/xz/src/liblzma/check/crc32_tablegen.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)crc32_fast.o: 3rdparty/xz/src/liblzma/check/crc32_fast.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)crc64_tablegen.o: 3rdparty/xz/src/liblzma/check/crc64_tablegen.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)crc64_small.o: 3rdparty/xz/src/liblzma/check/crc64_small.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)delta_decoder.o: CPPFLAGS += -I3rdparty/xz/src/liblzma/delta
$(BUILDDIR)delta_decoder.o: 3rdparty/xz/src/liblzma/delta/delta_decoder.c 3rdparty/xz/src/liblzma/delta/delta_decoder.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)delta_encoder.o: 3rdparty/xz/src/liblzma/delta/delta_encoder.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)delta_common.o: 3rdparty/xz/src/liblzma/delta/delta_common.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)lz_decoder.o: 3rdparty/xz/src/liblzma/lz/lz_decoder.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)lz_encoder_mf.o: 3rdparty/xz/src/liblzma/lz/lz_encoder_mf.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)lz_encoder.o: 3rdparty/xz/src/liblzma/lz/lz_encoder.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)price_table.o: CPPFLAGS += -I3rdparty/xz/src/liblzma/rangecoder
$(BUILDDIR)price_table.o: 3rdparty/xz/src/liblzma/rangecoder/price_table.c 3rdparty/xz/src/liblzma/rangecoder/range_encoder.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)price_tablegen.o: 3rdparty/xz/src/liblzma/rangecoder/price_tablegen.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)filter_buffer_decoder.o: 3rdparty/xz/src/liblzma/common/filter_buffer_decoder.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)block_buffer_decoder.o: 3rdparty/xz/src/liblzma/common/block_buffer_decoder.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)filter_decoder.o: 3rdparty/xz/src/liblzma/common/filter_decoder.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)alone_encoder.o: 3rdparty/xz/src/liblzma/common/alone_encoder.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)stream_flags_common.o: 3rdparty/xz/src/liblzma/common/stream_flags_common.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)easy_encoder.o: 3rdparty/xz/src/liblzma/common/easy_encoder.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)index_decoder.o: 3rdparty/xz/src/liblzma/common/index_decoder.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)common.o: 3rdparty/xz/src/liblzma/common/common.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)stream_decoder.o: 3rdparty/xz/src/liblzma/common/stream_decoder.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)easy_decoder_memusage.o: 3rdparty/xz/src/liblzma/common/easy_decoder_memusage.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)stream_encoder.o: 3rdparty/xz/src/liblzma/common/stream_encoder.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)vli_encoder.o: 3rdparty/xz/src/liblzma/common/vli_encoder.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)stream_buffer_encoder.o: 3rdparty/xz/src/liblzma/common/stream_buffer_encoder.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)outqueue.o: 3rdparty/xz/src/liblzma/common/outqueue.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)block_util.o: 3rdparty/xz/src/liblzma/common/block_util.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)filter_flags_decoder.o: 3rdparty/xz/src/liblzma/common/filter_flags_decoder.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)block_encoder.o: 3rdparty/xz/src/liblzma/common/block_encoder.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)filter_common.o: 3rdparty/xz/src/liblzma/common/filter_common.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)hardware_cputhreads.o: 3rdparty/xz/src/liblzma/common/hardware_cputhreads.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)vli_decoder.o: 3rdparty/xz/src/liblzma/common/vli_decoder.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)filter_encoder.o: 3rdparty/xz/src/liblzma/common/filter_encoder.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)block_header_decoder.o: 3rdparty/xz/src/liblzma/common/block_header_decoder.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)index_hash.o: 3rdparty/xz/src/liblzma/common/index_hash.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)filter_buffer_encoder.o: 3rdparty/xz/src/liblzma/common/filter_buffer_encoder.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)alone_decoder.o: 3rdparty/xz/src/liblzma/common/alone_decoder.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)block_buffer_encoder.o: 3rdparty/xz/src/liblzma/common/block_buffer_encoder.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)stream_buffer_decoder.o: 3rdparty/xz/src/liblzma/common/stream_buffer_decoder.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)stream_flags_decoder.o: 3rdparty/xz/src/liblzma/common/stream_flags_decoder.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)easy_buffer_encoder.o: 3rdparty/xz/src/liblzma/common/easy_buffer_encoder.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)block_decoder.o: 3rdparty/xz/src/liblzma/common/block_decoder.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)vli_size.o: 3rdparty/xz/src/liblzma/common/vli_size.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)block_header_encoder.o: 3rdparty/xz/src/liblzma/common/block_header_encoder.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)easy_preset.o: 3rdparty/xz/src/liblzma/common/easy_preset.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)index_encoder.o: 3rdparty/xz/src/liblzma/common/index_encoder.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)stream_encoder_mt.o: 3rdparty/xz/src/liblzma/common/stream_encoder_mt.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)easy_encoder_memusage.o: 3rdparty/xz/src/liblzma/common/easy_encoder_memusage.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)hardware_physmem.o: 3rdparty/xz/src/liblzma/common/hardware_physmem.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)filter_flags_encoder.o: 3rdparty/xz/src/liblzma/common/filter_flags_encoder.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)auto_decoder.o: 3rdparty/xz/src/liblzma/common/auto_decoder.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)index.o: 3rdparty/xz/src/liblzma/common/index.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)stream_flags_encoder.o: 3rdparty/xz/src/liblzma/common/stream_flags_encoder.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)tuklib_mbstr_fw.o: 3rdparty/xz/src/common/tuklib_mbstr_fw.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)tuklib_exit.o: 3rdparty/xz/src/common/tuklib_exit.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)tuklib_open_stdxxx.o: 3rdparty/xz/src/common/tuklib_open_stdxxx.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)tuklib_progname.o: 3rdparty/xz/src/common/tuklib_progname.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)tuklib_physmem.o: 3rdparty/xz/src/common/tuklib_physmem.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)tuklib_mbstr_width.o: 3rdparty/xz/src/common/tuklib_mbstr_width.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)tuklib_cpucores.o: 3rdparty/xz/src/common/tuklib_cpucores.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)gzread.o: CPPFLAGS += -I3rdparty/zlib
$(BUILDDIR)gzread.o: 3rdparty/zlib/gzread.c 3rdparty/zlib/gzguts.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)inftrees.o: 3rdparty/zlib/inftrees.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)compress.o: 3rdparty/zlib/compress.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)deflate.o: 3rdparty/zlib/deflate.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)trees.o: 3rdparty/zlib/trees.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)gzwrite.o: 3rdparty/zlib/gzwrite.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)zutil.o: 3rdparty/zlib/zutil.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)inflate.o: 3rdparty/zlib/inflate.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)crc32.o: 3rdparty/zlib/crc32.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)gzclose.o: 3rdparty/zlib/gzclose.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)inffast.o: 3rdparty/zlib/inffast.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)gzlib.o: 3rdparty/zlib/gzlib.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)infback.o: 3rdparty/zlib/infback.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)uncompr.o: 3rdparty/zlib/uncompr.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)adler32.o: 3rdparty/zlib/adler32.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)feature_tests.o: 3rdparty/openssl/build/x86_64-linux-diet/CMakeFiles/feature_tests.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)LONG_INT.o: 3rdparty/openssl/build/x86_64-linux-diet/CMakeFiles/CheckTypeSize/LONG_INT.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)INT.o: 3rdparty/openssl/build/x86_64-linux-diet/CMakeFiles/CheckTypeSize/INT.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)LONG_LONG_INT.o: 3rdparty/openssl/build/x86_64-linux-diet/CMakeFiles/CheckTypeSize/LONG_LONG_INT.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)CMakeCCompilerId.o: 3rdparty/openssl/build/x86_64-linux-diet/CMakeFiles/3.1.3/CompilerIdC/CMakeCCompilerId.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)conf.o: 3rdparty/openssl/fuzz/conf.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)crl.o: 3rdparty/openssl/fuzz/crl.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bignum.o: 3rdparty/openssl/fuzz/bignum.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)asn1.o: 3rdparty/openssl/fuzz/asn1.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)asn1parse.o: 3rdparty/openssl/fuzz/asn1parse.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bndiv.o: 3rdparty/openssl/fuzz/bndiv.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)server.o: 3rdparty/openssl/fuzz/server.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ct.o: 3rdparty/openssl/fuzz/ct.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cms.o: 3rdparty/openssl/fuzz/cms.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)test-corpus.o: 3rdparty/openssl/fuzz/test-corpus.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)driver.o: 3rdparty/openssl/fuzz/driver.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)x509.o: 3rdparty/openssl/fuzz/x509.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)client.o: 3rdparty/openssl/fuzz/client.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)applink.o: 3rdparty/openssl/ms/applink.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)uplink.o: 3rdparty/openssl/ms/uplink.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)crl.o: 3rdparty/openssl/apps/crl.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)pkcs12.o: CPPFLAGS += -I3rdparty/openssl/apps
$(BUILDDIR)pkcs12.o: 3rdparty/openssl/apps/pkcs12.c 3rdparty/openssl/apps/apps.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ocsp.o: CPPFLAGS += -I3rdparty/openssl/include/internal
$(BUILDDIR)ocsp.o: 3rdparty/openssl/apps/ocsp.c 3rdparty/openssl/include/internal/sockets.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)vms_term_sock.o: 3rdparty/openssl/apps/vms_term_sock.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)dsa.o: 3rdparty/openssl/apps/dsa.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)opt.o: 3rdparty/openssl/apps/opt.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)genrsa.o: 3rdparty/openssl/apps/genrsa.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)pkey.o: 3rdparty/openssl/apps/pkey.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)crl2p7.o: 3rdparty/openssl/apps/crl2p7.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)req.o: 3rdparty/openssl/apps/req.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)rsa.o: 3rdparty/openssl/apps/rsa.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ec.o: 3rdparty/openssl/apps/ec.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)nseq.o: 3rdparty/openssl/apps/nseq.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)gzread.o: CPPFLAGS += -I3rdparty/zlib
$(BUILDDIR)gzread.o: 3rdparty/zlib/gzread.c 3rdparty/zlib/gzguts.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)inftrees.o: 3rdparty/zlib/inftrees.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)compress.o: 3rdparty/zlib/compress.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)deflate.o: 3rdparty/zlib/deflate.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)trees.o: 3rdparty/zlib/trees.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)gzwrite.o: 3rdparty/zlib/gzwrite.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)zutil.o: 3rdparty/zlib/zutil.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)inflate.o: 3rdparty/zlib/inflate.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)gzread.o: CPPFLAGS += -I3rdparty/zlib
$(BUILDDIR)gzread.o: 3rdparty/zlib/gzread.c 3rdparty/zlib/gzguts.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)inftrees.o: 3rdparty/zlib/inftrees.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)compress.o: 3rdparty/zlib/compress.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)deflate.o: 3rdparty/zlib/deflate.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)trees.o: 3rdparty/zlib/trees.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)gzwrite.o: 3rdparty/zlib/gzwrite.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)zutil.o: 3rdparty/zlib/zutil.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)inflate.o: 3rdparty/zlib/inflate.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)crc32.o: 3rdparty/zlib/crc32.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)gzclose.o: 3rdparty/zlib/gzclose.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)inffast.o: 3rdparty/zlib/inffast.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)gzlib.o: 3rdparty/zlib/gzlib.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)infback.o: 3rdparty/zlib/infback.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)uncompr.o: 3rdparty/zlib/uncompr.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)adler32.o: 3rdparty/zlib/adler32.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)feature_tests.o: 3rdparty/openssl/build/x86_64-linux-diet/CMakeFiles/feature_tests.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)LONG_INT.o: 3rdparty/openssl/build/x86_64-linux-diet/CMakeFiles/CheckTypeSize/LONG_INT.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)INT.o: 3rdparty/openssl/build/x86_64-linux-diet/CMakeFiles/CheckTypeSize/INT.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)LONG_LONG_INT.o: 3rdparty/openssl/build/x86_64-linux-diet/CMakeFiles/CheckTypeSize/LONG_LONG_INT.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)CMakeCCompilerId.o: 3rdparty/openssl/build/x86_64-linux-diet/CMakeFiles/3.1.3/CompilerIdC/CMakeCCompilerId.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)conf.o: 3rdparty/openssl/fuzz/conf.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)crl.o: 3rdparty/openssl/fuzz/crl.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bignum.o: 3rdparty/openssl/fuzz/bignum.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)asn1.o: 3rdparty/openssl/fuzz/asn1.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)asn1parse.o: 3rdparty/openssl/fuzz/asn1parse.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bndiv.o: 3rdparty/openssl/fuzz/bndiv.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)server.o: 3rdparty/openssl/fuzz/server.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ct.o: 3rdparty/openssl/fuzz/ct.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cms.o: 3rdparty/openssl/fuzz/cms.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)test-corpus.o: 3rdparty/openssl/fuzz/test-corpus.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)driver.o: 3rdparty/openssl/fuzz/driver.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)x509.o: 3rdparty/openssl/fuzz/x509.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)client.o: 3rdparty/openssl/fuzz/client.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)applink.o: 3rdparty/openssl/ms/applink.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)uplink.o: 3rdparty/openssl/ms/uplink.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)crl.o: 3rdparty/openssl/apps/crl.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)pkcs12.o: CPPFLAGS += -I3rdparty/openssl/apps
$(BUILDDIR)pkcs12.o: 3rdparty/openssl/apps/pkcs12.c 3rdparty/openssl/apps/apps.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ocsp.o: CPPFLAGS += -I3rdparty/openssl/include/internal
$(BUILDDIR)ocsp.o: 3rdparty/openssl/apps/ocsp.c 3rdparty/openssl/include/internal/sockets.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)vms_term_sock.o: 3rdparty/openssl/apps/vms_term_sock.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)dsa.o: 3rdparty/openssl/apps/dsa.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)opt.o: 3rdparty/openssl/apps/opt.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)genrsa.o: 3rdparty/openssl/apps/genrsa.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)pkey.o: 3rdparty/openssl/apps/pkey.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)crl2p7.o: 3rdparty/openssl/apps/crl2p7.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)req.o: 3rdparty/openssl/apps/req.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)rsa.o: 3rdparty/openssl/apps/rsa.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ec.o: 3rdparty/openssl/apps/ec.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)nseq.o: 3rdparty/openssl/apps/nseq.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)storeutl.o: 3rdparty/openssl/apps/storeutl.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)sess_id.o: 3rdparty/openssl/apps/sess_id.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)gendsa.o: 3rdparty/openssl/apps/gendsa.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)s_socket.o: 3rdparty/openssl/apps/s_socket.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)engine.o: 3rdparty/openssl/apps/engine.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)version.o: 3rdparty/openssl/apps/version.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cms.o: 3rdparty/openssl/apps/cms.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)speed.o: 3rdparty/openssl/apps/speed.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bf_prefix.o: 3rdparty/openssl/apps/bf_prefix.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)openssl.o: 3rdparty/openssl/apps/openssl.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)win32_init.o: 3rdparty/openssl/apps/win32_init.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ecparam.o: 3rdparty/openssl/apps/ecparam.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)vms_decc_init.o: 3rdparty/openssl/apps/vms_decc_init.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)apps.o: 3rdparty/openssl/apps/apps.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)app_rand.o: 3rdparty/openssl/apps/app_rand.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)prime.o: 3rdparty/openssl/apps/prime.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)pkcs7.o: 3rdparty/openssl/apps/pkcs7.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)dsaparam.o: 3rdparty/openssl/apps/dsaparam.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)dgst.o: 3rdparty/openssl/apps/dgst.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)errstr.o: 3rdparty/openssl/apps/errstr.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)s_server.o: 3rdparty/openssl/apps/s_server.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)gzread.o: CPPFLAGS += -I3rdparty/zlib
$(BUILDDIR)gzread.o: 3rdparty/zlib/gzread.c 3rdparty/zlib/gzguts.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)inftrees.o: 3rdparty/zlib/inftrees.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)compress.o: 3rdparty/zlib/compress.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)deflate.o: 3rdparty/zlib/deflate.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)trees.o: 3rdparty/zlib/trees.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)gzwrite.o: 3rdparty/zlib/gzwrite.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)zutil.o: 3rdparty/zlib/zutil.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)inflate.o: 3rdparty/zlib/inflate.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)crc32.o: 3rdparty/zlib/crc32.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)gzclose.o: 3rdparty/zlib/gzclose.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)inffast.o: 3rdparty/zlib/inffast.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)gzlib.o: 3rdparty/zlib/gzlib.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)infback.o: 3rdparty/zlib/infback.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)uncompr.o: 3rdparty/zlib/uncompr.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)adler32.o: 3rdparty/zlib/adler32.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)feature_tests.o: 3rdparty/openssl/build/x86_64-linux-diet/CMakeFiles/feature_tests.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)LONG_INT.o: 3rdparty/openssl/build/x86_64-linux-diet/CMakeFiles/CheckTypeSize/LONG_INT.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)INT.o: 3rdparty/openssl/build/x86_64-linux-diet/CMakeFiles/CheckTypeSize/INT.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)LONG_LONG_INT.o: 3rdparty/openssl/build/x86_64-linux-diet/CMakeFiles/CheckTypeSize/LONG_LONG_INT.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)CMakeCCompilerId.o: 3rdparty/openssl/build/x86_64-linux-diet/CMakeFiles/3.1.3/CompilerIdC/CMakeCCompilerId.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)conf.o: 3rdparty/openssl/fuzz/conf.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)crl.o: 3rdparty/openssl/fuzz/crl.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bignum.o: 3rdparty/openssl/fuzz/bignum.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)asn1.o: 3rdparty/openssl/fuzz/asn1.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)asn1parse.o: 3rdparty/openssl/fuzz/asn1parse.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bndiv.o: 3rdparty/openssl/fuzz/bndiv.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)server.o: 3rdparty/openssl/fuzz/server.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ct.o: 3rdparty/openssl/fuzz/ct.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cms.o: 3rdparty/openssl/fuzz/cms.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)test-corpus.o: 3rdparty/openssl/fuzz/test-corpus.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)driver.o: 3rdparty/openssl/fuzz/driver.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)x509.o: 3rdparty/openssl/fuzz/x509.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)client.o: 3rdparty/openssl/fuzz/client.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)applink.o: 3rdparty/openssl/ms/applink.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)uplink.o: 3rdparty/openssl/ms/uplink.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)crl.o: 3rdparty/openssl/apps/crl.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)pkcs12.o: CPPFLAGS += -I3rdparty/openssl/apps
$(BUILDDIR)pkcs12.o: 3rdparty/openssl/apps/pkcs12.c 3rdparty/openssl/apps/apps.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ocsp.o: CPPFLAGS += -I3rdparty/openssl/include/internal
$(BUILDDIR)ocsp.o: 3rdparty/openssl/apps/ocsp.c 3rdparty/openssl/include/internal/sockets.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)vms_term_sock.o: 3rdparty/openssl/apps/vms_term_sock.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)dsa.o: 3rdparty/openssl/apps/dsa.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)opt.o: 3rdparty/openssl/apps/opt.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)genrsa.o: 3rdparty/openssl/apps/genrsa.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)pkey.o: 3rdparty/openssl/apps/pkey.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)crl2p7.o: 3rdparty/openssl/apps/crl2p7.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)req.o: 3rdparty/openssl/apps/req.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)rsa.o: 3rdparty/openssl/apps/rsa.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ec.o: 3rdparty/openssl/apps/ec.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)nseq.o: 3rdparty/openssl/apps/nseq.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)storeutl.o: 3rdparty/openssl/apps/storeutl.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)sess_id.o: 3rdparty/openssl/apps/sess_id.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)gendsa.o: 3rdparty/openssl/apps/gendsa.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)s_socket.o: 3rdparty/openssl/apps/s_socket.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)engine.o: 3rdparty/openssl/apps/engine.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)version.o: 3rdparty/openssl/apps/version.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cms.o: 3rdparty/openssl/apps/cms.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)speed.o: 3rdparty/openssl/apps/speed.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bf_prefix.o: 3rdparty/openssl/apps/bf_prefix.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)openssl.o: 3rdparty/openssl/apps/openssl.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)win32_init.o: 3rdparty/openssl/apps/win32_init.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ecparam.o: 3rdparty/openssl/apps/ecparam.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)vms_decc_init.o: 3rdparty/openssl/apps/vms_decc_init.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)apps.o: 3rdparty/openssl/apps/apps.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)app_rand.o: 3rdparty/openssl/apps/app_rand.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)prime.o: 3rdparty/openssl/apps/prime.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)pkcs7.o: 3rdparty/openssl/apps/pkcs7.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)dsaparam.o: 3rdparty/openssl/apps/dsaparam.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)dgst.o: 3rdparty/openssl/apps/dgst.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)errstr.o: 3rdparty/openssl/apps/errstr.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)s_server.o: 3rdparty/openssl/apps/s_server.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)rsautl.o: 3rdparty/openssl/apps/rsautl.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ts.o: 3rdparty/openssl/apps/ts.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)dhparam.o: 3rdparty/openssl/apps/dhparam.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)srp.o: 3rdparty/openssl/apps/srp.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)s_client.o: CPPFLAGS += -I3rdparty/openssl
$(BUILDDIR)s_client.o: 3rdparty/openssl/apps/s_client.c 3rdparty/openssl/e_os.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)rehash.o: 3rdparty/openssl/apps/rehash.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)enc.o: 3rdparty/openssl/apps/enc.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)asn1pars.o: 3rdparty/openssl/apps/asn1pars.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)passwd.o: 3rdparty/openssl/apps/passwd.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)pkcs8.o: 3rdparty/openssl/apps/pkcs8.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)pkeyutl.o: 3rdparty/openssl/apps/pkeyutl.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)rand.o: 3rdparty/openssl/apps/rand.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)spkac.o: 3rdparty/openssl/apps/spkac.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)smime.o: 3rdparty/openssl/apps/smime.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ciphers.o: 3rdparty/openssl/apps/ciphers.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)x509.o: 3rdparty/openssl/apps/x509.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)s_time.o: 3rdparty/openssl/apps/s_time.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)verify.o: 3rdparty/openssl/apps/verify.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ca.o: 3rdparty/openssl/apps/ca.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)s_cb.o: 3rdparty/openssl/apps/s_cb.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)pkeyparam.o: 3rdparty/openssl/apps/pkeyparam.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)genpkey.o: 3rdparty/openssl/apps/genpkey.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ecstresstest.o: 3rdparty/openssl/test/ecstresstest.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)sm4_internal_test.o: CPPFLAGS += -I3rdparty/openssl/include/crypto
$(BUILDDIR)sm4_internal_test.o: 3rdparty/openssl/test/sm4_internal_test.c 3rdparty/openssl/include/crypto/sm4.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)chacha_internal_test.o: 3rdparty/openssl/test/chacha_internal_test.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)exptest.o: 3rdparty/openssl/test/exptest.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)test_test.o: 3rdparty/openssl/test/test_test.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)pkey_meth_test.o: 3rdparty/openssl/test/pkey_meth_test.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bntest.o: 3rdparty/openssl/test/bntest.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)errtest.o: 3rdparty/openssl/test/errtest.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)srptest.o: CPPFLAGS += -I3rdparty/openssl/test
$(BUILDDIR)srptest.o: 3rdparty/openssl/test/srptest.c 3rdparty/openssl/test/testutil.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)uitest.o: 3rdparty/openssl/test/uitest.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ectest.o: 3rdparty/openssl/test/ectest.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)asn1_string_table_test.o: 3rdparty/openssl/test/asn1_string_table_test.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)asynctest.o: 3rdparty/openssl/test/asynctest.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ecdsatest.o: 3rdparty/openssl/test/ecdsatest.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)v3ext.o: 3rdparty/openssl/test/v3ext.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)aborttest.o: 3rdparty/openssl/test/aborttest.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)gosttest.o: 3rdparty/openssl/test/gosttest.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bio_memleak_test.o: 3rdparty/openssl/test/bio_memleak_test.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)dhtest.o: 3rdparty/openssl/test/dhtest.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bftest.o: 3rdparty/openssl/test/bftest.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)x509aux.o: 3rdparty/openssl/test/x509aux.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cipherlist_test.o: 3rdparty/openssl/test/cipherlist_test.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)gzread.o: CPPFLAGS += -I3rdparty/zlib
$(BUILDDIR)gzread.o: 3rdparty/zlib/gzread.c 3rdparty/zlib/gzguts.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)inftrees.o: 3rdparty/zlib/inftrees.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)compress.o: 3rdparty/zlib/compress.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)deflate.o: 3rdparty/zlib/deflate.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)trees.o: 3rdparty/zlib/trees.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)gzwrite.o: 3rdparty/zlib/gzwrite.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)zutil.o: 3rdparty/zlib/zutil.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)inflate.o: 3rdparty/zlib/inflate.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)crc32.o: 3rdparty/zlib/crc32.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)gzclose.o: 3rdparty/zlib/gzclose.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)inffast.o: 3rdparty/zlib/inffast.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)gzlib.o: 3rdparty/zlib/gzlib.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)infback.o: 3rdparty/zlib/infback.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)uncompr.o: 3rdparty/zlib/uncompr.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)adler32.o: 3rdparty/zlib/adler32.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)feature_tests.o: 3rdparty/openssl/build/x86_64-linux-diet/CMakeFiles/feature_tests.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)LONG_INT.o: 3rdparty/openssl/build/x86_64-linux-diet/CMakeFiles/CheckTypeSize/LONG_INT.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)INT.o: 3rdparty/openssl/build/x86_64-linux-diet/CMakeFiles/CheckTypeSize/INT.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)LONG_LONG_INT.o: 3rdparty/openssl/build/x86_64-linux-diet/CMakeFiles/CheckTypeSize/LONG_LONG_INT.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)CMakeCCompilerId.o: 3rdparty/openssl/build/x86_64-linux-diet/CMakeFiles/3.1.3/CompilerIdC/CMakeCCompilerId.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)conf.o: 3rdparty/openssl/fuzz/conf.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)crl.o: 3rdparty/openssl/fuzz/crl.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bignum.o: 3rdparty/openssl/fuzz/bignum.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)asn1.o: 3rdparty/openssl/fuzz/asn1.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)asn1parse.o: 3rdparty/openssl/fuzz/asn1parse.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bndiv.o: 3rdparty/openssl/fuzz/bndiv.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)server.o: 3rdparty/openssl/fuzz/server.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ct.o: 3rdparty/openssl/fuzz/ct.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cms.o: 3rdparty/openssl/fuzz/cms.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)test-corpus.o: 3rdparty/openssl/fuzz/test-corpus.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)driver.o: 3rdparty/openssl/fuzz/driver.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)x509.o: 3rdparty/openssl/fuzz/x509.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)client.o: 3rdparty/openssl/fuzz/client.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)applink.o: 3rdparty/openssl/ms/applink.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)uplink.o: 3rdparty/openssl/ms/uplink.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)crl.o: 3rdparty/openssl/apps/crl.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)pkcs12.o: CPPFLAGS += -I3rdparty/openssl/apps
$(BUILDDIR)pkcs12.o: 3rdparty/openssl/apps/pkcs12.c 3rdparty/openssl/apps/apps.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ocsp.o: CPPFLAGS += -I3rdparty/openssl/include/internal
$(BUILDDIR)ocsp.o: 3rdparty/openssl/apps/ocsp.c 3rdparty/openssl/include/internal/sockets.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)vms_term_sock.o: 3rdparty/openssl/apps/vms_term_sock.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)dsa.o: 3rdparty/openssl/apps/dsa.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)opt.o: 3rdparty/openssl/apps/opt.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)genrsa.o: 3rdparty/openssl/apps/genrsa.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)pkey.o: 3rdparty/openssl/apps/pkey.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)crl2p7.o: 3rdparty/openssl/apps/crl2p7.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)req.o: 3rdparty/openssl/apps/req.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)rsa.o: 3rdparty/openssl/apps/rsa.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ec.o: 3rdparty/openssl/apps/ec.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)nseq.o: 3rdparty/openssl/apps/nseq.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)storeutl.o: 3rdparty/openssl/apps/storeutl.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)sess_id.o: 3rdparty/openssl/apps/sess_id.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)gendsa.o: 3rdparty/openssl/apps/gendsa.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)s_socket.o: 3rdparty/openssl/apps/s_socket.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)engine.o: 3rdparty/openssl/apps/engine.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)version.o: 3rdparty/openssl/apps/version.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cms.o: 3rdparty/openssl/apps/cms.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)speed.o: 3rdparty/openssl/apps/speed.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bf_prefix.o: 3rdparty/openssl/apps/bf_prefix.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)openssl.o: 3rdparty/openssl/apps/openssl.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)win32_init.o: 3rdparty/openssl/apps/win32_init.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)gzread.o: CPPFLAGS += -I3rdparty/zlib
$(BUILDDIR)gzread.o: 3rdparty/zlib/gzread.c 3rdparty/zlib/gzguts.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)inftrees.o: 3rdparty/zlib/inftrees.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)compress.o: 3rdparty/zlib/compress.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)deflate.o: 3rdparty/zlib/deflate.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)trees.o: 3rdparty/zlib/trees.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)gzwrite.o: 3rdparty/zlib/gzwrite.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)zutil.o: 3rdparty/zlib/zutil.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)inflate.o: 3rdparty/zlib/inflate.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)crc32.o: 3rdparty/zlib/crc32.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)gzclose.o: 3rdparty/zlib/gzclose.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)inffast.o: 3rdparty/zlib/inffast.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)gzlib.o: 3rdparty/zlib/gzlib.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)infback.o: 3rdparty/zlib/infback.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)uncompr.o: 3rdparty/zlib/uncompr.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)adler32.o: 3rdparty/zlib/adler32.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)feature_tests.o: 3rdparty/openssl/build/x86_64-linux-diet/CMakeFiles/feature_tests.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)LONG_INT.o: 3rdparty/openssl/build/x86_64-linux-diet/CMakeFiles/CheckTypeSize/LONG_INT.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)INT.o: 3rdparty/openssl/build/x86_64-linux-diet/CMakeFiles/CheckTypeSize/INT.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)LONG_LONG_INT.o: 3rdparty/openssl/build/x86_64-linux-diet/CMakeFiles/CheckTypeSize/LONG_LONG_INT.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)CMakeCCompilerId.o: 3rdparty/openssl/build/x86_64-linux-diet/CMakeFiles/3.1.3/CompilerIdC/CMakeCCompilerId.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)conf.o: 3rdparty/openssl/fuzz/conf.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)crl.o: 3rdparty/openssl/fuzz/crl.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bignum.o: 3rdparty/openssl/fuzz/bignum.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)asn1.o: 3rdparty/openssl/fuzz/asn1.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)asn1parse.o: 3rdparty/openssl/fuzz/asn1parse.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bndiv.o: 3rdparty/openssl/fuzz/bndiv.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)server.o: 3rdparty/openssl/fuzz/server.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ct.o: 3rdparty/openssl/fuzz/ct.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cms.o: 3rdparty/openssl/fuzz/cms.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)test-corpus.o: 3rdparty/openssl/fuzz/test-corpus.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)driver.o: 3rdparty/openssl/fuzz/driver.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)x509.o: 3rdparty/openssl/fuzz/x509.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)client.o: 3rdparty/openssl/fuzz/client.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)applink.o: 3rdparty/openssl/ms/applink.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)uplink.o: 3rdparty/openssl/ms/uplink.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)crl.o: 3rdparty/openssl/apps/crl.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)gzread.o: CPPFLAGS += -I3rdparty/zlib
$(BUILDDIR)gzread.o: 3rdparty/zlib/gzread.c 3rdparty/zlib/gzguts.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)inftrees.o: 3rdparty/zlib/inftrees.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)compress.o: 3rdparty/zlib/compress.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)deflate.o: 3rdparty/zlib/deflate.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)trees.o: 3rdparty/zlib/trees.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)gzwrite.o: 3rdparty/zlib/gzwrite.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)zutil.o: 3rdparty/zlib/zutil.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)inflate.o: 3rdparty/zlib/inflate.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)crc32.o: 3rdparty/zlib/crc32.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)gzclose.o: 3rdparty/zlib/gzclose.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)inffast.o: 3rdparty/zlib/inffast.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)gzlib.o: 3rdparty/zlib/gzlib.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)infback.o: 3rdparty/zlib/infback.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)uncompr.o: 3rdparty/zlib/uncompr.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)adler32.o: 3rdparty/zlib/adler32.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)feature_tests.o: 3rdparty/openssl/build/x86_64-linux-diet/CMakeFiles/feature_tests.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)LONG_INT.o: 3rdparty/openssl/build/x86_64-linux-diet/CMakeFiles/CheckTypeSize/LONG_INT.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)INT.o: 3rdparty/openssl/build/x86_64-linux-diet/CMakeFiles/CheckTypeSize/INT.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)LONG_LONG_INT.o: 3rdparty/openssl/build/x86_64-linux-diet/CMakeFiles/CheckTypeSize/LONG_LONG_INT.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)CMakeCCompilerId.o: 3rdparty/openssl/build/x86_64-linux-diet/CMakeFiles/3.1.3/CompilerIdC/CMakeCCompilerId.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)conf.o: 3rdparty/openssl/fuzz/conf.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)crl.o: 3rdparty/openssl/fuzz/crl.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bignum.o: 3rdparty/openssl/fuzz/bignum.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)asn1.o: 3rdparty/openssl/fuzz/asn1.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)asn1parse.o: 3rdparty/openssl/fuzz/asn1parse.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bndiv.o: 3rdparty/openssl/fuzz/bndiv.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)server.o: 3rdparty/openssl/fuzz/server.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ct.o: 3rdparty/openssl/fuzz/ct.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cms.o: 3rdparty/openssl/fuzz/cms.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)test-corpus.o: 3rdparty/openssl/fuzz/test-corpus.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)driver.o: 3rdparty/openssl/fuzz/driver.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)x509.o: 3rdparty/openssl/fuzz/x509.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)client.o: 3rdparty/openssl/fuzz/client.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)applink.o: 3rdparty/openssl/ms/applink.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)uplink.o: 3rdparty/openssl/ms/uplink.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)crl.o: 3rdparty/openssl/apps/crl.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)pkcs12.o: CPPFLAGS += -I3rdparty/openssl/apps
$(BUILDDIR)pkcs12.o: 3rdparty/openssl/apps/pkcs12.c 3rdparty/openssl/apps/apps.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ocsp.o: CPPFLAGS += -I3rdparty/openssl/include/internal
$(BUILDDIR)ocsp.o: 3rdparty/openssl/apps/ocsp.c 3rdparty/openssl/include/internal/sockets.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)vms_term_sock.o: 3rdparty/openssl/apps/vms_term_sock.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)dsa.o: 3rdparty/openssl/apps/dsa.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)opt.o: 3rdparty/openssl/apps/opt.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)genrsa.o: 3rdparty/openssl/apps/genrsa.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)pkey.o: 3rdparty/openssl/apps/pkey.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)crl2p7.o: 3rdparty/openssl/apps/crl2p7.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)req.o: 3rdparty/openssl/apps/req.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)rsa.o: 3rdparty/openssl/apps/rsa.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ec.o: 3rdparty/openssl/apps/ec.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)nseq.o: 3rdparty/openssl/apps/nseq.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)storeutl.o: 3rdparty/openssl/apps/storeutl.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)sess_id.o: 3rdparty/openssl/apps/sess_id.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)gendsa.o: 3rdparty/openssl/apps/gendsa.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)s_socket.o: 3rdparty/openssl/apps/s_socket.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)engine.o: 3rdparty/openssl/apps/engine.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)version.o: 3rdparty/openssl/apps/version.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cms.o: 3rdparty/openssl/apps/cms.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)speed.o: 3rdparty/openssl/apps/speed.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bf_prefix.o: 3rdparty/openssl/apps/bf_prefix.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)openssl.o: 3rdparty/openssl/apps/openssl.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)win32_init.o: 3rdparty/openssl/apps/win32_init.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ecparam.o: 3rdparty/openssl/apps/ecparam.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)vms_decc_init.o: 3rdparty/openssl/apps/vms_decc_init.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)apps.o: 3rdparty/openssl/apps/apps.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)app_rand.o: 3rdparty/openssl/apps/app_rand.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)prime.o: 3rdparty/openssl/apps/prime.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)pkcs7.o: 3rdparty/openssl/apps/pkcs7.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)dsaparam.o: 3rdparty/openssl/apps/dsaparam.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)dgst.o: 3rdparty/openssl/apps/dgst.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)errstr.o: 3rdparty/openssl/apps/errstr.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)s_server.o: 3rdparty/openssl/apps/s_server.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)rsautl.o: 3rdparty/openssl/apps/rsautl.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ts.o: 3rdparty/openssl/apps/ts.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)dhparam.o: 3rdparty/openssl/apps/dhparam.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)srp.o: 3rdparty/openssl/apps/srp.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)s_client.o: CPPFLAGS += -I3rdparty/openssl
$(BUILDDIR)s_client.o: 3rdparty/openssl/apps/s_client.c 3rdparty/openssl/e_os.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)rehash.o: 3rdparty/openssl/apps/rehash.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)enc.o: 3rdparty/openssl/apps/enc.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)asn1pars.o: 3rdparty/openssl/apps/asn1pars.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)passwd.o: 3rdparty/openssl/apps/passwd.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)pkcs8.o: 3rdparty/openssl/apps/pkcs8.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)pkeyutl.o: 3rdparty/openssl/apps/pkeyutl.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)rand.o: 3rdparty/openssl/apps/rand.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)spkac.o: 3rdparty/openssl/apps/spkac.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)smime.o: 3rdparty/openssl/apps/smime.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ciphers.o: 3rdparty/openssl/apps/ciphers.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)x509.o: 3rdparty/openssl/apps/x509.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)s_time.o: 3rdparty/openssl/apps/s_time.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)verify.o: 3rdparty/openssl/apps/verify.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ca.o: 3rdparty/openssl/apps/ca.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)s_cb.o: 3rdparty/openssl/apps/s_cb.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)pkeyparam.o: 3rdparty/openssl/apps/pkeyparam.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)genpkey.o: 3rdparty/openssl/apps/genpkey.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ecstresstest.o: 3rdparty/openssl/test/ecstresstest.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)sm4_internal_test.o: CPPFLAGS += -I3rdparty/openssl/include/crypto
$(BUILDDIR)sm4_internal_test.o: 3rdparty/openssl/test/sm4_internal_test.c 3rdparty/openssl/include/crypto/sm4.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)chacha_internal_test.o: 3rdparty/openssl/test/chacha_internal_test.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)exptest.o: 3rdparty/openssl/test/exptest.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)test_test.o: 3rdparty/openssl/test/test_test.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)pkey_meth_test.o: 3rdparty/openssl/test/pkey_meth_test.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bntest.o: 3rdparty/openssl/test/bntest.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)errtest.o: 3rdparty/openssl/test/errtest.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)srptest.o: CPPFLAGS += -I3rdparty/openssl/test
$(BUILDDIR)srptest.o: 3rdparty/openssl/test/srptest.c 3rdparty/openssl/test/testutil.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)uitest.o: 3rdparty/openssl/test/uitest.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ectest.o: 3rdparty/openssl/test/ectest.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)asn1_string_table_test.o: 3rdparty/openssl/test/asn1_string_table_test.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)asynctest.o: 3rdparty/openssl/test/asynctest.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ecdsatest.o: 3rdparty/openssl/test/ecdsatest.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)v3ext.o: 3rdparty/openssl/test/v3ext.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)aborttest.o: 3rdparty/openssl/test/aborttest.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)gosttest.o: 3rdparty/openssl/test/gosttest.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bio_memleak_test.o: 3rdparty/openssl/test/bio_memleak_test.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)dhtest.o: 3rdparty/openssl/test/dhtest.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bftest.o: 3rdparty/openssl/test/bftest.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)x509aux.o: 3rdparty/openssl/test/x509aux.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cipherlist_test.o: 3rdparty/openssl/test/cipherlist_test.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)afalgtest.o: 3rdparty/openssl/test/afalgtest.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)pkey_meth_kdf_test.o: 3rdparty/openssl/test/pkey_meth_kdf_test.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ocspapitest.o: 3rdparty/openssl/test/ocspapitest.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ideatest.o: 3rdparty/openssl/test/ideatest.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)threadstest.o: 3rdparty/openssl/test/threadstest.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)sslbuffertest.o: 3rdparty/openssl/test/sslbuffertest.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ssltest_old.o: 3rdparty/openssl/test/ssltest_old.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)d2i_test.o: 3rdparty/openssl/test/d2i_test.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)versions.o: 3rdparty/openssl/test/versions.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)exdatatest.o: 3rdparty/openssl/test/exdatatest.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)mdc2test.o: 3rdparty/openssl/test/mdc2test.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)sslapitest.o: 3rdparty/openssl/test/sslapitest.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ec_internal_test.o: 3rdparty/openssl/test/ec_internal_test.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)evp_extra_test.o: 3rdparty/openssl/test/evp_extra_test.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)shlibloadtest.o: 3rdparty/openssl/test/shlibloadtest.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)dsatest.o: 3rdparty/openssl/test/dsatest.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ssl_cert_table_internal_test.o: 3rdparty/openssl/test/ssl_cert_table_internal_test.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)fatalerrtest.o: 3rdparty/openssl/test/fatalerrtest.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)siphash_internal_test.o: 3rdparty/openssl/test/siphash_internal_test.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)dsa_no_digest_size_test.o: 3rdparty/openssl/test/dsa_no_digest_size_test.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bioprinttest.o: 3rdparty/openssl/test/bioprinttest.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)handshake_helper.o: 3rdparty/openssl/test/handshake_helper.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)stack_test.o: 3rdparty/openssl/test/stack_test.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)curve448_internal_test.o: 3rdparty/openssl/test/curve448_internal_test.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)x509_internal_test.o: 3rdparty/openssl/test/x509_internal_test.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)gmdifftest.o: 3rdparty/openssl/test/gmdifftest.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bio_enc_test.o: 3rdparty/openssl/test/bio_enc_test.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)crltest.o: 3rdparty/openssl/test/crltest.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)rc5test.o: 3rdparty/openssl/test/rc5test.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)x509_dup_cert_test.o: 3rdparty/openssl/test/x509_dup_cert_test.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)mdc2_internal_test.o: 3rdparty/openssl/test/mdc2_internal_test.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)recordlentest.o: 3rdparty/openssl/test/recordlentest.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)evp_test.o: 3rdparty/openssl/test/evp_test.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)asn1_encode_test.o: 3rdparty/openssl/test/asn1_encode_test.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)clienthellotest.o: 3rdparty/openssl/test/clienthellotest.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)danetest.o: 3rdparty/openssl/test/danetest.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)rsa_test.o: 3rdparty/openssl/test/rsa_test.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)pbelutest.o: 3rdparty/openssl/test/pbelutest.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)servername_test.o: 3rdparty/openssl/test/servername_test.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)sslcorrupttest.o: 3rdparty/openssl/test/sslcorrupttest.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)lhash_test.o: 3rdparty/openssl/test/lhash_test.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)asynciotest.o: 3rdparty/openssl/test/asynciotest.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)rsa_complex.o: 3rdparty/openssl/test/rsa_complex.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cipher_overhead_test.o: 3rdparty/openssl/test/cipher_overhead_test.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)constant_time_test.o: 3rdparty/openssl/test/constant_time_test.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)wpackettest.o: 3rdparty/openssl/test/wpackettest.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ssl_test.o: 3rdparty/openssl/test/ssl_test.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ssl_test_ctx.o: 3rdparty/openssl/test/ssl_test_ctx.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)rdrand_sanitytest.o: 3rdparty/openssl/test/rdrand_sanitytest.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)drbgtest.o: 3rdparty/openssl/test/drbgtest.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cipherbytes_test.o: 3rdparty/openssl/test/cipherbytes_test.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)tls13secretstest.o: 3rdparty/openssl/test/tls13secretstest.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)x509_time_test.o: 3rdparty/openssl/test/x509_time_test.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)sm2_internal_test.o: 3rdparty/openssl/test/sm2_internal_test.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)tls13encryptiontest.o: 3rdparty/openssl/test/tls13encryptiontest.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)memleaktest.o: 3rdparty/openssl/test/memleaktest.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)basic_output.o: 3rdparty/openssl/test/testutil/basic_output.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)testutil_init.o: 3rdparty/openssl/test/testutil/testutil_init.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)format_output.o: 3rdparty/openssl/test/testutil/format_output.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)random.o: 3rdparty/openssl/test/testutil/random.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)driver.o: 3rdparty/openssl/test/testutil/driver.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)stanza.o: 3rdparty/openssl/test/testutil/stanza.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cb.o: CPPFLAGS += -I3rdparty/openssl/test/testutil
$(BUILDDIR)cb.o: 3rdparty/openssl/test/testutil/cb.c 3rdparty/openssl/test/testutil/output.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)tap_bio.o: 3rdparty/openssl/test/testutil/tap_bio.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)main.o: 3rdparty/openssl/test/testutil/main.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)output_helpers.o: 3rdparty/openssl/test/testutil/output_helpers.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)tests.o: 3rdparty/openssl/test/testutil/tests.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)test_cleanup.o: 3rdparty/openssl/test/testutil/test_cleanup.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)rc2test.o: 3rdparty/openssl/test/rc2test.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ctype_internal_test.o: 3rdparty/openssl/test/ctype_internal_test.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)casttest.o: 3rdparty/openssl/test/casttest.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)packettest.o: 3rdparty/openssl/test/packettest.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)v3nametest.o: 3rdparty/openssl/test/v3nametest.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)sysdefaulttest.o: 3rdparty/openssl/test/sysdefaulttest.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ssl_ctx_test.o: 3rdparty/openssl/test/ssl_ctx_test.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)enginetest.o: 3rdparty/openssl/test/enginetest.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)drbg_cavs_test.o: 3rdparty/openssl/test/drbg_cavs_test.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)poly1305_internal_test.o: 3rdparty/openssl/test/poly1305_internal_test.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)conf_include_test.o: 3rdparty/openssl/test/conf_include_test.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)secmemtest.o: 3rdparty/openssl/test/secmemtest.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bad_dtls_test.o: 3rdparty/openssl/test/bad_dtls_test.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)modes_internal_test.o: 3rdparty/openssl/test/modes_internal_test.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)asn1_decode_test.o: 3rdparty/openssl/test/asn1_decode_test.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ssltestlib.o: 3rdparty/openssl/test/ssltestlib.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ssl_test_ctx_test.o: 3rdparty/openssl/test/ssl_test_ctx_test.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)destest.o: 3rdparty/openssl/test/destest.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)rsa_mp_test.o: 3rdparty/openssl/test/rsa_mp_test.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)x509_check_cert_pkey_test.o: 3rdparty/openssl/test/x509_check_cert_pkey_test.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)asn1_time_test.o: 3rdparty/openssl/test/asn1_time_test.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ciphername_test.o: 3rdparty/openssl/test/ciphername_test.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)igetest.o: 3rdparty/openssl/test/igetest.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)dtls_mtu_test.o: 3rdparty/openssl/test/dtls_mtu_test.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)md2test.o: 3rdparty/openssl/test/md2test.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bio_callback_test.o: 3rdparty/openssl/test/bio_callback_test.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ct_test.o: 3rdparty/openssl/test/ct_test.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)dtlstest.o: 3rdparty/openssl/test/dtlstest.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)rc4test.o: 3rdparty/openssl/test/rc4test.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)dtlsv1listentest.o: 3rdparty/openssl/test/dtlsv1listentest.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)sanitytest.o: 3rdparty/openssl/test/sanitytest.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)asn1_internal_test.o: 3rdparty/openssl/test/asn1_internal_test.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)tls13ccstest.o: 3rdparty/openssl/test/tls13ccstest.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)verify_extra_test.o: 3rdparty/openssl/test/verify_extra_test.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cmsapitest.o: 3rdparty/openssl/test/cmsapitest.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)time_offset_test.o: 3rdparty/openssl/test/time_offset_test.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)hmactest.o: 3rdparty/openssl/test/hmactest.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)drbg_cavs_data.o: 3rdparty/openssl/test/drbg_cavs_data.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)pemtest.o: 3rdparty/openssl/test/pemtest.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)s3_msg.o: CPPFLAGS += -I3rdparty/openssl/ssl
$(BUILDDIR)s3_msg.o: 3rdparty/openssl/ssl/s3_msg.c 3rdparty/openssl/ssl/ssl_local.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)t1_enc.o: 3rdparty/openssl/ssl/t1_enc.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ssl_stat.o: 3rdparty/openssl/ssl/ssl_stat.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)d1_srtp.o: 3rdparty/openssl/ssl/d1_srtp.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ssl_txt.o: 3rdparty/openssl/ssl/ssl_txt.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ssl_mcnf.o: 3rdparty/openssl/ssl/ssl_mcnf.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bio_ssl.o: 3rdparty/openssl/ssl/bio_ssl.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ssl_asn1.o: 3rdparty/openssl/ssl/ssl_asn1.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)t1_lib.o: 3rdparty/openssl/ssl/t1_lib.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)rec_layer_d1.o: 3rdparty/openssl/ssl/record/rec_layer_d1.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)dtls1_bitmap.o: 3rdparty/openssl/ssl/record/dtls1_bitmap.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ssl3_record_tls13.o: 3rdparty/openssl/ssl/record/ssl3_record_tls13.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)rec_layer_s3.o: 3rdparty/openssl/ssl/record/rec_layer_s3.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ssl3_buffer.o: 3rdparty/openssl/ssl/record/ssl3_buffer.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ssl3_record.o: 3rdparty/openssl/ssl/record/ssl3_record.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)statem_lib.o: 3rdparty/openssl/ssl/statem/statem_lib.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)statem.o: 3rdparty/openssl/ssl/statem/statem.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)statem_srvr.o: 3rdparty/openssl/ssl/statem/statem_srvr.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)statem_clnt.o: 3rdparty/openssl/ssl/statem/statem_clnt.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)extensions_srvr.o: 3rdparty/openssl/ssl/statem/extensions_srvr.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)statem_dtls.o: 3rdparty/openssl/ssl/statem/statem_dtls.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)extensions_cust.o: 3rdparty/openssl/ssl/statem/extensions_cust.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)extensions_clnt.o: 3rdparty/openssl/ssl/statem/extensions_clnt.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)extensions.o: 3rdparty/openssl/ssl/statem/extensions.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ssl_ciph.o: 3rdparty/openssl/ssl/ssl_ciph.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)s3_enc.o: 3rdparty/openssl/ssl/s3_enc.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)tls_srp.o: 3rdparty/openssl/ssl/tls_srp.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ssl_lib.o: 3rdparty/openssl/ssl/ssl_lib.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ssl_err.o: 3rdparty/openssl/ssl/ssl_err.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ssl_rsa.o: 3rdparty/openssl/ssl/ssl_rsa.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)packet.o: 3rdparty/openssl/ssl/packet.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)s3_cbc.o: 3rdparty/openssl/ssl/s3_cbc.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)pqueue.o: 3rdparty/openssl/ssl/pqueue.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ssl_utst.o: 3rdparty/openssl/ssl/ssl_utst.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ssl_conf.o: 3rdparty/openssl/ssl/ssl_conf.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)s3_lib.o: 3rdparty/openssl/ssl/s3_lib.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)methods.o: 3rdparty/openssl/ssl/methods.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)tls13_enc.o: 3rdparty/openssl/ssl/tls13_enc.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)d1_msg.o: 3rdparty/openssl/ssl/d1_msg.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ssl_sess.o: 3rdparty/openssl/ssl/ssl_sess.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ssl_cert.o: 3rdparty/openssl/ssl/ssl_cert.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)d1_lib.o: 3rdparty/openssl/ssl/d1_lib.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ssl_init.o: 3rdparty/openssl/ssl/ssl_init.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)t1_trce.o: 3rdparty/openssl/ssl/t1_trce.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)mem_clr.o: 3rdparty/openssl/crypto/mem_clr.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)rc5_ecb.o: 3rdparty/openssl/crypto/rc5/rc5_ecb.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)rc5_skey.o: 3rdparty/openssl/crypto/rc5/rc5_skey.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)rc5cfb64.o: 3rdparty/openssl/crypto/rc5/rc5cfb64.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)rc5ofb64.o: 3rdparty/openssl/crypto/rc5/rc5ofb64.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)rc5_enc.o: 3rdparty/openssl/crypto/rc5/rc5_enc.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)o_time.o: 3rdparty/openssl/crypto/o_time.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)sparcv9cap.o: 3rdparty/openssl/crypto/sparcv9cap.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)md2_one.o: 3rdparty/openssl/crypto/md2/md2_one.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)md2_dgst.o: 3rdparty/openssl/crypto/md2/md2_dgst.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)eng_openssl.o: 3rdparty/openssl/crypto/engine/eng_openssl.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)eng_cnf.o: CPPFLAGS += -I3rdparty/openssl/crypto/engine
$(BUILDDIR)eng_cnf.o: 3rdparty/openssl/crypto/engine/eng_cnf.c 3rdparty/openssl/crypto/engine/eng_local.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)eng_init.o: 3rdparty/openssl/crypto/engine/eng_init.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)eng_err.o: 3rdparty/openssl/crypto/engine/eng_err.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)tb_dh.o: 3rdparty/openssl/crypto/engine/tb_dh.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)eng_fat.o: 3rdparty/openssl/crypto/engine/eng_fat.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)eng_dyn.o: 3rdparty/openssl/crypto/engine/eng_dyn.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)tb_asnmth.o: 3rdparty/openssl/crypto/engine/tb_asnmth.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)tb_cipher.o: 3rdparty/openssl/crypto/engine/tb_cipher.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)tb_eckey.o: 3rdparty/openssl/crypto/engine/tb_eckey.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)eng_rdrand.o: 3rdparty/openssl/crypto/engine/eng_rdrand.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)tb_rsa.o: 3rdparty/openssl/crypto/engine/tb_rsa.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)tb_rand.o: 3rdparty/openssl/crypto/engine/tb_rand.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)tb_pkmeth.o: 3rdparty/openssl/crypto/engine/tb_pkmeth.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)eng_devcrypto.o: 3rdparty/openssl/crypto/engine/eng_devcrypto.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)tb_dsa.o: 3rdparty/openssl/crypto/engine/tb_dsa.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)eng_pkey.o: 3rdparty/openssl/crypto/engine/eng_pkey.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)eng_ctrl.o: 3rdparty/openssl/crypto/engine/eng_ctrl.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)eng_table.o: 3rdparty/openssl/crypto/engine/eng_table.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)eng_list.o: 3rdparty/openssl/crypto/engine/eng_list.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)gzread.o: CPPFLAGS += -I3rdparty/zlib
$(BUILDDIR)gzread.o: 3rdparty/zlib/gzread.c 3rdparty/zlib/gzguts.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)inftrees.o: 3rdparty/zlib/inftrees.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)compress.o: 3rdparty/zlib/compress.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)deflate.o: 3rdparty/zlib/deflate.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)trees.o: 3rdparty/zlib/trees.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)gzwrite.o: 3rdparty/zlib/gzwrite.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)zutil.o: 3rdparty/zlib/zutil.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)inflate.o: 3rdparty/zlib/inflate.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)crc32.o: 3rdparty/zlib/crc32.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)gzclose.o: 3rdparty/zlib/gzclose.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)inffast.o: 3rdparty/zlib/inffast.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)gzlib.o: 3rdparty/zlib/gzlib.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)infback.o: 3rdparty/zlib/infback.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)uncompr.o: 3rdparty/zlib/uncompr.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)adler32.o: 3rdparty/zlib/adler32.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)feature_tests.o: 3rdparty/openssl/build/x86_64-linux-diet/CMakeFiles/feature_tests.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)LONG_INT.o: 3rdparty/openssl/build/x86_64-linux-diet/CMakeFiles/CheckTypeSize/LONG_INT.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)INT.o: 3rdparty/openssl/build/x86_64-linux-diet/CMakeFiles/CheckTypeSize/INT.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)LONG_LONG_INT.o: 3rdparty/openssl/build/x86_64-linux-diet/CMakeFiles/CheckTypeSize/LONG_LONG_INT.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)CMakeCCompilerId.o: 3rdparty/openssl/build/x86_64-linux-diet/CMakeFiles/3.1.3/CompilerIdC/CMakeCCompilerId.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)conf.o: 3rdparty/openssl/fuzz/conf.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)crl.o: 3rdparty/openssl/fuzz/crl.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bignum.o: 3rdparty/openssl/fuzz/bignum.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)asn1.o: 3rdparty/openssl/fuzz/asn1.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)asn1parse.o: 3rdparty/openssl/fuzz/asn1parse.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bndiv.o: 3rdparty/openssl/fuzz/bndiv.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)server.o: 3rdparty/openssl/fuzz/server.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ct.o: 3rdparty/openssl/fuzz/ct.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cms.o: 3rdparty/openssl/fuzz/cms.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)test-corpus.o: 3rdparty/openssl/fuzz/test-corpus.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)driver.o: 3rdparty/openssl/fuzz/driver.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)x509.o: 3rdparty/openssl/fuzz/x509.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)client.o: 3rdparty/openssl/fuzz/client.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)applink.o: 3rdparty/openssl/ms/applink.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)uplink.o: 3rdparty/openssl/ms/uplink.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)crl.o: 3rdparty/openssl/apps/crl.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)pkcs12.o: CPPFLAGS += -I3rdparty/openssl/apps
$(BUILDDIR)pkcs12.o: 3rdparty/openssl/apps/pkcs12.c 3rdparty/openssl/apps/apps.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ocsp.o: CPPFLAGS += -I3rdparty/openssl/include/internal
$(BUILDDIR)ocsp.o: 3rdparty/openssl/apps/ocsp.c 3rdparty/openssl/include/internal/sockets.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)vms_term_sock.o: 3rdparty/openssl/apps/vms_term_sock.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)dsa.o: 3rdparty/openssl/apps/dsa.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)opt.o: 3rdparty/openssl/apps/opt.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)genrsa.o: 3rdparty/openssl/apps/genrsa.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)pkey.o: 3rdparty/openssl/apps/pkey.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)crl2p7.o: 3rdparty/openssl/apps/crl2p7.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)req.o: 3rdparty/openssl/apps/req.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)rsa.o: 3rdparty/openssl/apps/rsa.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ec.o: 3rdparty/openssl/apps/ec.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)nseq.o: 3rdparty/openssl/apps/nseq.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)storeutl.o: 3rdparty/openssl/apps/storeutl.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)sess_id.o: 3rdparty/openssl/apps/sess_id.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)gendsa.o: 3rdparty/openssl/apps/gendsa.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)s_socket.o: 3rdparty/openssl/apps/s_socket.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)engine.o: 3rdparty/openssl/apps/engine.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)version.o: 3rdparty/openssl/apps/version.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cms.o: 3rdparty/openssl/apps/cms.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)speed.o: 3rdparty/openssl/apps/speed.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bf_prefix.o: 3rdparty/openssl/apps/bf_prefix.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)openssl.o: 3rdparty/openssl/apps/openssl.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)win32_init.o: 3rdparty/openssl/apps/win32_init.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ecparam.o: 3rdparty/openssl/apps/ecparam.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)vms_decc_init.o: 3rdparty/openssl/apps/vms_decc_init.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)apps.o: 3rdparty/openssl/apps/apps.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)app_rand.o: 3rdparty/openssl/apps/app_rand.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)prime.o: 3rdparty/openssl/apps/prime.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)pkcs7.o: 3rdparty/openssl/apps/pkcs7.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)dsaparam.o: 3rdparty/openssl/apps/dsaparam.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)dgst.o: 3rdparty/openssl/apps/dgst.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)errstr.o: 3rdparty/openssl/apps/errstr.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)s_server.o: 3rdparty/openssl/apps/s_server.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)rsautl.o: 3rdparty/openssl/apps/rsautl.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ts.o: 3rdparty/openssl/apps/ts.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)dhparam.o: 3rdparty/openssl/apps/dhparam.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)srp.o: 3rdparty/openssl/apps/srp.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)s_client.o: CPPFLAGS += -I3rdparty/openssl
$(BUILDDIR)s_client.o: 3rdparty/openssl/apps/s_client.c 3rdparty/openssl/e_os.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)rehash.o: 3rdparty/openssl/apps/rehash.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)enc.o: 3rdparty/openssl/apps/enc.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)asn1pars.o: 3rdparty/openssl/apps/asn1pars.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)passwd.o: 3rdparty/openssl/apps/passwd.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)pkcs8.o: 3rdparty/openssl/apps/pkcs8.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)pkeyutl.o: 3rdparty/openssl/apps/pkeyutl.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)rand.o: 3rdparty/openssl/apps/rand.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)spkac.o: 3rdparty/openssl/apps/spkac.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)smime.o: 3rdparty/openssl/apps/smime.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ciphers.o: 3rdparty/openssl/apps/ciphers.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)x509.o: 3rdparty/openssl/apps/x509.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)s_time.o: 3rdparty/openssl/apps/s_time.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)verify.o: 3rdparty/openssl/apps/verify.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ca.o: 3rdparty/openssl/apps/ca.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)s_cb.o: 3rdparty/openssl/apps/s_cb.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)pkeyparam.o: 3rdparty/openssl/apps/pkeyparam.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)genpkey.o: 3rdparty/openssl/apps/genpkey.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ecstresstest.o: 3rdparty/openssl/test/ecstresstest.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)sm4_internal_test.o: CPPFLAGS += -I3rdparty/openssl/include/crypto
$(BUILDDIR)sm4_internal_test.o: 3rdparty/openssl/test/sm4_internal_test.c 3rdparty/openssl/include/crypto/sm4.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)chacha_internal_test.o: 3rdparty/openssl/test/chacha_internal_test.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)exptest.o: 3rdparty/openssl/test/exptest.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)test_test.o: 3rdparty/openssl/test/test_test.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)pkey_meth_test.o: 3rdparty/openssl/test/pkey_meth_test.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bntest.o: 3rdparty/openssl/test/bntest.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)errtest.o: 3rdparty/openssl/test/errtest.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)srptest.o: CPPFLAGS += -I3rdparty/openssl/test
$(BUILDDIR)srptest.o: 3rdparty/openssl/test/srptest.c 3rdparty/openssl/test/testutil.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)uitest.o: 3rdparty/openssl/test/uitest.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ectest.o: 3rdparty/openssl/test/ectest.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)asn1_string_table_test.o: 3rdparty/openssl/test/asn1_string_table_test.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)asynctest.o: 3rdparty/openssl/test/asynctest.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ecdsatest.o: 3rdparty/openssl/test/ecdsatest.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)v3ext.o: 3rdparty/openssl/test/v3ext.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)aborttest.o: 3rdparty/openssl/test/aborttest.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)gosttest.o: 3rdparty/openssl/test/gosttest.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bio_memleak_test.o: 3rdparty/openssl/test/bio_memleak_test.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)dhtest.o: 3rdparty/openssl/test/dhtest.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bftest.o: 3rdparty/openssl/test/bftest.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)x509aux.o: 3rdparty/openssl/test/x509aux.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cipherlist_test.o: 3rdparty/openssl/test/cipherlist_test.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)afalgtest.o: 3rdparty/openssl/test/afalgtest.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)pkey_meth_kdf_test.o: 3rdparty/openssl/test/pkey_meth_kdf_test.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ocspapitest.o: 3rdparty/openssl/test/ocspapitest.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ideatest.o: 3rdparty/openssl/test/ideatest.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)threadstest.o: 3rdparty/openssl/test/threadstest.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)sslbuffertest.o: 3rdparty/openssl/test/sslbuffertest.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ssltest_old.o: 3rdparty/openssl/test/ssltest_old.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)d2i_test.o: 3rdparty/openssl/test/d2i_test.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)versions.o: 3rdparty/openssl/test/versions.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)exdatatest.o: 3rdparty/openssl/test/exdatatest.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)mdc2test.o: 3rdparty/openssl/test/mdc2test.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)sslapitest.o: 3rdparty/openssl/test/sslapitest.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ec_internal_test.o: 3rdparty/openssl/test/ec_internal_test.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)evp_extra_test.o: 3rdparty/openssl/test/evp_extra_test.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)shlibloadtest.o: 3rdparty/openssl/test/shlibloadtest.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)dsatest.o: 3rdparty/openssl/test/dsatest.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ssl_cert_table_internal_test.o: 3rdparty/openssl/test/ssl_cert_table_internal_test.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)fatalerrtest.o: 3rdparty/openssl/test/fatalerrtest.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)siphash_internal_test.o: 3rdparty/openssl/test/siphash_internal_test.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)dsa_no_digest_size_test.o: 3rdparty/openssl/test/dsa_no_digest_size_test.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bioprinttest.o: 3rdparty/openssl/test/bioprinttest.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)handshake_helper.o: 3rdparty/openssl/test/handshake_helper.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)stack_test.o: 3rdparty/openssl/test/stack_test.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)curve448_internal_test.o: 3rdparty/openssl/test/curve448_internal_test.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)x509_internal_test.o: 3rdparty/openssl/test/x509_internal_test.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)gmdifftest.o: 3rdparty/openssl/test/gmdifftest.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bio_enc_test.o: 3rdparty/openssl/test/bio_enc_test.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)crltest.o: 3rdparty/openssl/test/crltest.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)rc5test.o: 3rdparty/openssl/test/rc5test.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)x509_dup_cert_test.o: 3rdparty/openssl/test/x509_dup_cert_test.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)mdc2_internal_test.o: 3rdparty/openssl/test/mdc2_internal_test.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)recordlentest.o: 3rdparty/openssl/test/recordlentest.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)evp_test.o: 3rdparty/openssl/test/evp_test.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)asn1_encode_test.o: 3rdparty/openssl/test/asn1_encode_test.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)clienthellotest.o: 3rdparty/openssl/test/clienthellotest.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)danetest.o: 3rdparty/openssl/test/danetest.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)rsa_test.o: 3rdparty/openssl/test/rsa_test.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)pbelutest.o: 3rdparty/openssl/test/pbelutest.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)servername_test.o: 3rdparty/openssl/test/servername_test.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)sslcorrupttest.o: 3rdparty/openssl/test/sslcorrupttest.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)lhash_test.o: 3rdparty/openssl/test/lhash_test.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)asynciotest.o: 3rdparty/openssl/test/asynciotest.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)rsa_complex.o: 3rdparty/openssl/test/rsa_complex.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cipher_overhead_test.o: 3rdparty/openssl/test/cipher_overhead_test.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)constant_time_test.o: 3rdparty/openssl/test/constant_time_test.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)wpackettest.o: 3rdparty/openssl/test/wpackettest.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ssl_test.o: 3rdparty/openssl/test/ssl_test.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ssl_test_ctx.o: 3rdparty/openssl/test/ssl_test_ctx.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)rdrand_sanitytest.o: 3rdparty/openssl/test/rdrand_sanitytest.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)drbgtest.o: 3rdparty/openssl/test/drbgtest.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cipherbytes_test.o: 3rdparty/openssl/test/cipherbytes_test.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)tls13secretstest.o: 3rdparty/openssl/test/tls13secretstest.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)x509_time_test.o: 3rdparty/openssl/test/x509_time_test.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)sm2_internal_test.o: 3rdparty/openssl/test/sm2_internal_test.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)tls13encryptiontest.o: 3rdparty/openssl/test/tls13encryptiontest.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)memleaktest.o: 3rdparty/openssl/test/memleaktest.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)basic_output.o: 3rdparty/openssl/test/testutil/basic_output.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)testutil_init.o: 3rdparty/openssl/test/testutil/testutil_init.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)format_output.o: 3rdparty/openssl/test/testutil/format_output.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)random.o: 3rdparty/openssl/test/testutil/random.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)driver.o: 3rdparty/openssl/test/testutil/driver.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)stanza.o: 3rdparty/openssl/test/testutil/stanza.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cb.o: CPPFLAGS += -I3rdparty/openssl/test/testutil
$(BUILDDIR)cb.o: 3rdparty/openssl/test/testutil/cb.c 3rdparty/openssl/test/testutil/output.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)tap_bio.o: 3rdparty/openssl/test/testutil/tap_bio.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)main.o: 3rdparty/openssl/test/testutil/main.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)output_helpers.o: 3rdparty/openssl/test/testutil/output_helpers.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)tests.o: 3rdparty/openssl/test/testutil/tests.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)test_cleanup.o: 3rdparty/openssl/test/testutil/test_cleanup.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)rc2test.o: 3rdparty/openssl/test/rc2test.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ctype_internal_test.o: 3rdparty/openssl/test/ctype_internal_test.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)casttest.o: 3rdparty/openssl/test/casttest.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)packettest.o: 3rdparty/openssl/test/packettest.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)v3nametest.o: 3rdparty/openssl/test/v3nametest.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)sysdefaulttest.o: 3rdparty/openssl/test/sysdefaulttest.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ssl_ctx_test.o: 3rdparty/openssl/test/ssl_ctx_test.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)enginetest.o: 3rdparty/openssl/test/enginetest.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)drbg_cavs_test.o: 3rdparty/openssl/test/drbg_cavs_test.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)poly1305_internal_test.o: 3rdparty/openssl/test/poly1305_internal_test.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)conf_include_test.o: 3rdparty/openssl/test/conf_include_test.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)secmemtest.o: 3rdparty/openssl/test/secmemtest.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bad_dtls_test.o: 3rdparty/openssl/test/bad_dtls_test.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)modes_internal_test.o: 3rdparty/openssl/test/modes_internal_test.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)asn1_decode_test.o: 3rdparty/openssl/test/asn1_decode_test.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ssltestlib.o: 3rdparty/openssl/test/ssltestlib.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ssl_test_ctx_test.o: 3rdparty/openssl/test/ssl_test_ctx_test.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)destest.o: 3rdparty/openssl/test/destest.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)rsa_mp_test.o: 3rdparty/openssl/test/rsa_mp_test.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)x509_check_cert_pkey_test.o: 3rdparty/openssl/test/x509_check_cert_pkey_test.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)asn1_time_test.o: 3rdparty/openssl/test/asn1_time_test.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ciphername_test.o: 3rdparty/openssl/test/ciphername_test.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)igetest.o: 3rdparty/openssl/test/igetest.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)dtls_mtu_test.o: 3rdparty/openssl/test/dtls_mtu_test.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)md2test.o: 3rdparty/openssl/test/md2test.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bio_callback_test.o: 3rdparty/openssl/test/bio_callback_test.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ct_test.o: 3rdparty/openssl/test/ct_test.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)dtlstest.o: 3rdparty/openssl/test/dtlstest.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)rc4test.o: 3rdparty/openssl/test/rc4test.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)dtlsv1listentest.o: 3rdparty/openssl/test/dtlsv1listentest.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)sanitytest.o: 3rdparty/openssl/test/sanitytest.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)asn1_internal_test.o: 3rdparty/openssl/test/asn1_internal_test.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)tls13ccstest.o: 3rdparty/openssl/test/tls13ccstest.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)verify_extra_test.o: 3rdparty/openssl/test/verify_extra_test.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cmsapitest.o: 3rdparty/openssl/test/cmsapitest.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)time_offset_test.o: 3rdparty/openssl/test/time_offset_test.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)hmactest.o: 3rdparty/openssl/test/hmactest.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)drbg_cavs_data.o: 3rdparty/openssl/test/drbg_cavs_data.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)pemtest.o: 3rdparty/openssl/test/pemtest.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)s3_msg.o: CPPFLAGS += -I3rdparty/openssl/ssl
$(BUILDDIR)s3_msg.o: 3rdparty/openssl/ssl/s3_msg.c 3rdparty/openssl/ssl/ssl_local.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)t1_enc.o: 3rdparty/openssl/ssl/t1_enc.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ssl_stat.o: 3rdparty/openssl/ssl/ssl_stat.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)d1_srtp.o: 3rdparty/openssl/ssl/d1_srtp.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ssl_txt.o: 3rdparty/openssl/ssl/ssl_txt.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ssl_mcnf.o: 3rdparty/openssl/ssl/ssl_mcnf.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bio_ssl.o: 3rdparty/openssl/ssl/bio_ssl.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ssl_asn1.o: 3rdparty/openssl/ssl/ssl_asn1.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)t1_lib.o: 3rdparty/openssl/ssl/t1_lib.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)rec_layer_d1.o: 3rdparty/openssl/ssl/record/rec_layer_d1.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)dtls1_bitmap.o: 3rdparty/openssl/ssl/record/dtls1_bitmap.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ssl3_record_tls13.o: 3rdparty/openssl/ssl/record/ssl3_record_tls13.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)rec_layer_s3.o: 3rdparty/openssl/ssl/record/rec_layer_s3.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ssl3_buffer.o: 3rdparty/openssl/ssl/record/ssl3_buffer.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ssl3_record.o: 3rdparty/openssl/ssl/record/ssl3_record.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)statem_lib.o: 3rdparty/openssl/ssl/statem/statem_lib.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)statem.o: 3rdparty/openssl/ssl/statem/statem.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)statem_srvr.o: 3rdparty/openssl/ssl/statem/statem_srvr.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)statem_clnt.o: 3rdparty/openssl/ssl/statem/statem_clnt.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)extensions_srvr.o: 3rdparty/openssl/ssl/statem/extensions_srvr.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)statem_dtls.o: 3rdparty/openssl/ssl/statem/statem_dtls.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)extensions_cust.o: 3rdparty/openssl/ssl/statem/extensions_cust.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)extensions_clnt.o: 3rdparty/openssl/ssl/statem/extensions_clnt.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)extensions.o: 3rdparty/openssl/ssl/statem/extensions.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ssl_ciph.o: 3rdparty/openssl/ssl/ssl_ciph.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)s3_enc.o: 3rdparty/openssl/ssl/s3_enc.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)tls_srp.o: 3rdparty/openssl/ssl/tls_srp.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ssl_lib.o: 3rdparty/openssl/ssl/ssl_lib.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ssl_err.o: 3rdparty/openssl/ssl/ssl_err.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ssl_rsa.o: 3rdparty/openssl/ssl/ssl_rsa.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)packet.o: 3rdparty/openssl/ssl/packet.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)s3_cbc.o: 3rdparty/openssl/ssl/s3_cbc.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)pqueue.o: 3rdparty/openssl/ssl/pqueue.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ssl_utst.o: 3rdparty/openssl/ssl/ssl_utst.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ssl_conf.o: 3rdparty/openssl/ssl/ssl_conf.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)s3_lib.o: 3rdparty/openssl/ssl/s3_lib.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)methods.o: 3rdparty/openssl/ssl/methods.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)tls13_enc.o: 3rdparty/openssl/ssl/tls13_enc.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)d1_msg.o: 3rdparty/openssl/ssl/d1_msg.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ssl_sess.o: 3rdparty/openssl/ssl/ssl_sess.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ssl_cert.o: 3rdparty/openssl/ssl/ssl_cert.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)d1_lib.o: 3rdparty/openssl/ssl/d1_lib.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ssl_init.o: 3rdparty/openssl/ssl/ssl_init.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)t1_trce.o: 3rdparty/openssl/ssl/t1_trce.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)mem_clr.o: 3rdparty/openssl/crypto/mem_clr.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)rc5_ecb.o: 3rdparty/openssl/crypto/rc5/rc5_ecb.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)rc5_skey.o: 3rdparty/openssl/crypto/rc5/rc5_skey.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)rc5cfb64.o: 3rdparty/openssl/crypto/rc5/rc5cfb64.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)rc5ofb64.o: 3rdparty/openssl/crypto/rc5/rc5ofb64.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)rc5_enc.o: 3rdparty/openssl/crypto/rc5/rc5_enc.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)o_time.o: 3rdparty/openssl/crypto/o_time.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)sparcv9cap.o: 3rdparty/openssl/crypto/sparcv9cap.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)md2_one.o: 3rdparty/openssl/crypto/md2/md2_one.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)md2_dgst.o: 3rdparty/openssl/crypto/md2/md2_dgst.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)eng_openssl.o: 3rdparty/openssl/crypto/engine/eng_openssl.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)eng_cnf.o: CPPFLAGS += -I3rdparty/openssl/crypto/engine
$(BUILDDIR)eng_cnf.o: 3rdparty/openssl/crypto/engine/eng_cnf.c 3rdparty/openssl/crypto/engine/eng_local.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)eng_init.o: 3rdparty/openssl/crypto/engine/eng_init.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)eng_err.o: 3rdparty/openssl/crypto/engine/eng_err.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)tb_dh.o: 3rdparty/openssl/crypto/engine/tb_dh.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)eng_fat.o: 3rdparty/openssl/crypto/engine/eng_fat.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)eng_dyn.o: 3rdparty/openssl/crypto/engine/eng_dyn.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)tb_asnmth.o: 3rdparty/openssl/crypto/engine/tb_asnmth.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)tb_cipher.o: 3rdparty/openssl/crypto/engine/tb_cipher.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)tb_eckey.o: 3rdparty/openssl/crypto/engine/tb_eckey.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)eng_rdrand.o: 3rdparty/openssl/crypto/engine/eng_rdrand.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)tb_rsa.o: 3rdparty/openssl/crypto/engine/tb_rsa.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)tb_rand.o: 3rdparty/openssl/crypto/engine/tb_rand.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)tb_pkmeth.o: 3rdparty/openssl/crypto/engine/tb_pkmeth.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)eng_devcrypto.o: 3rdparty/openssl/crypto/engine/eng_devcrypto.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)tb_dsa.o: 3rdparty/openssl/crypto/engine/tb_dsa.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)eng_pkey.o: 3rdparty/openssl/crypto/engine/eng_pkey.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)eng_ctrl.o: 3rdparty/openssl/crypto/engine/eng_ctrl.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)eng_table.o: 3rdparty/openssl/crypto/engine/eng_table.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)eng_list.o: 3rdparty/openssl/crypto/engine/eng_list.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)eng_lib.o: 3rdparty/openssl/crypto/engine/eng_lib.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)tb_digest.o: 3rdparty/openssl/crypto/engine/tb_digest.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)eng_all.o: 3rdparty/openssl/crypto/engine/eng_all.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)sha256.o: 3rdparty/openssl/crypto/sha/sha256.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)sha1dgst.o: CPPFLAGS += -I3rdparty/openssl/crypto/sha
$(BUILDDIR)sha1dgst.o: 3rdparty/openssl/crypto/sha/sha1dgst.c 3rdparty/openssl/crypto/sha/sha_local.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)sha1_one.o: 3rdparty/openssl/crypto/sha/sha1_one.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)sha512.o: 3rdparty/openssl/crypto/sha/sha512.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)keccak1600.o: 3rdparty/openssl/crypto/sha/keccak1600.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)pem_pkey.o: 3rdparty/openssl/crypto/pem/pem_pkey.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)pem_err.o: 3rdparty/openssl/crypto/pem/pem_err.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)pem_lib.o: 3rdparty/openssl/crypto/pem/pem_lib.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)pem_oth.o: 3rdparty/openssl/crypto/pem/pem_oth.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)pem_x509.o: 3rdparty/openssl/crypto/pem/pem_x509.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)pvkfmt.o: 3rdparty/openssl/crypto/pem/pvkfmt.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)pem_info.o: 3rdparty/openssl/crypto/pem/pem_info.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)pem_sign.o: 3rdparty/openssl/crypto/pem/pem_sign.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)pem_all.o: 3rdparty/openssl/crypto/pem/pem_all.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)pem_pk8.o: 3rdparty/openssl/crypto/pem/pem_pk8.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)pem_xaux.o: 3rdparty/openssl/crypto/pem/pem_xaux.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cm_pmeth.o: 3rdparty/openssl/crypto/cmac/cm_pmeth.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cmac.o: 3rdparty/openssl/crypto/cmac/cmac.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cm_ameth.o: 3rdparty/openssl/crypto/cmac/cm_ameth.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cms_smime.o: 3rdparty/openssl/crypto/cms/cms_smime.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cms_dd.o: 3rdparty/openssl/crypto/cms/cms_dd.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cms_env.o: 3rdparty/openssl/crypto/cms/cms_env.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cms_cd.o: 3rdparty/openssl/crypto/cms/cms_cd.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cms_lib.o: 3rdparty/openssl/crypto/cms/cms_lib.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cms_asn1.o: 3rdparty/openssl/crypto/cms/cms_asn1.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cms_att.o: 3rdparty/openssl/crypto/cms/cms_att.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cms_err.o: 3rdparty/openssl/crypto/cms/cms_err.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cms_ess.o: 3rdparty/openssl/crypto/cms/cms_ess.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cms_kari.o: 3rdparty/openssl/crypto/cms/cms_kari.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cms_sd.o: 3rdparty/openssl/crypto/cms/cms_sd.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cms_enc.o: 3rdparty/openssl/crypto/cms/cms_enc.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cms_io.o: 3rdparty/openssl/crypto/cms/cms_io.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cms_pwri.o: 3rdparty/openssl/crypto/cms/cms_pwri.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)aria.o: 3rdparty/openssl/crypto/aria/aria.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)aes_core.o: 3rdparty/openssl/crypto/aes/aes_core.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)aes_wrap.o: 3rdparty/openssl/crypto/aes/aes_wrap.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)aes_ecb.o: 3rdparty/openssl/crypto/aes/aes_ecb.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)aes_cfb.o: 3rdparty/openssl/crypto/aes/aes_cfb.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)aes_ofb.o: 3rdparty/openssl/crypto/aes/aes_ofb.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)aes_ige.o: 3rdparty/openssl/crypto/aes/aes_ige.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)aes_cbc.o: 3rdparty/openssl/crypto/aes/aes_cbc.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)aes_misc.o: 3rdparty/openssl/crypto/aes/aes_misc.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)aes_x86core.o: 3rdparty/openssl/crypto/aes/aes_x86core.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)dllmain.o: 3rdparty/openssl/crypto/dllmain.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)uid.o: 3rdparty/openssl/crypto/uid.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cversion.o: 3rdparty/openssl/crypto/cversion.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)md5_one.o: 3rdparty/openssl/crypto/md5/md5_one.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)md5_dgst.o: 3rdparty/openssl/crypto/md5/md5_dgst.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)conf_api.o: 3rdparty/openssl/crypto/conf/conf_api.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)conf_def.o: 3rdparty/openssl/crypto/conf/conf_def.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)conf_sap.o: 3rdparty/openssl/crypto/conf/conf_sap.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)conf_mall.o: 3rdparty/openssl/crypto/conf/conf_mall.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)conf_mod.o: 3rdparty/openssl/crypto/conf/conf_mod.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)conf_lib.o: 3rdparty/openssl/crypto/conf/conf_lib.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)conf_ssl.o: 3rdparty/openssl/crypto/conf/conf_ssl.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)conf_err.o: 3rdparty/openssl/crypto/conf/conf_err.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buf_err.o: 3rdparty/openssl/crypto/buffer/buf_err.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)buffer.o: 3rdparty/openssl/crypto/buffer/buffer.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)rc2ofb64.o: 3rdparty/openssl/crypto/rc2/rc2ofb64.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)rc2_ecb.o: 3rdparty/openssl/crypto/rc2/rc2_ecb.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)rc2_cbc.o: 3rdparty/openssl/crypto/rc2/rc2_cbc.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)rc2cfb64.o: 3rdparty/openssl/crypto/rc2/rc2cfb64.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)rc2_skey.o: 3rdparty/openssl/crypto/rc2/rc2_skey.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)sm2_pmeth.o: 3rdparty/openssl/crypto/sm2/sm2_pmeth.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)sm2_err.o: 3rdparty/openssl/crypto/sm2/sm2_err.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)sm2_crypt.o: 3rdparty/openssl/crypto/sm2/sm2_crypt.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)sm2_sign.o: 3rdparty/openssl/crypto/sm2/sm2_sign.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)mem.o: 3rdparty/openssl/crypto/mem.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)async.o: CPPFLAGS += -I3rdparty/openssl/crypto/async
$(BUILDDIR)async.o: 3rdparty/openssl/crypto/async/async.c 3rdparty/openssl/crypto/async/async_local.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)async_null.o: 3rdparty/openssl/crypto/async/arch/async_null.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)async_posix.o: 3rdparty/openssl/crypto/async/arch/async_posix.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)async_win.o: 3rdparty/openssl/crypto/async/arch/async_win.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)async_wait.o: 3rdparty/openssl/crypto/async/async_wait.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)async_err.o: 3rdparty/openssl/crypto/async/async_err.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)o_names.o: 3rdparty/openssl/crypto/objects/o_names.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)obj_xref.o: 3rdparty/openssl/crypto/objects/obj_xref.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)obj_err.o: 3rdparty/openssl/crypto/objects/obj_err.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)obj_dat.o: 3rdparty/openssl/crypto/objects/obj_dat.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)obj_lib.o: 3rdparty/openssl/crypto/objects/obj_lib.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)sm4.o: 3rdparty/openssl/crypto/sm4/sm4.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cbc128.o: 3rdparty/openssl/crypto/modes/cbc128.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cfb128.o: 3rdparty/openssl/crypto/modes/cfb128.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)wrap128.o: 3rdparty/openssl/crypto/modes/wrap128.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ocb128.o: 3rdparty/openssl/crypto/modes/ocb128.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ctr128.o: 3rdparty/openssl/crypto/modes/ctr128.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ofb128.o: 3rdparty/openssl/crypto/modes/ofb128.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ccm128.o: 3rdparty/openssl/crypto/modes/ccm128.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)xts128.o: 3rdparty/openssl/crypto/modes/xts128.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cts128.o: 3rdparty/openssl/crypto/modes/cts128.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)gcm128.o: CPPFLAGS += -I3rdparty/openssl/crypto
$(BUILDDIR)gcm128.o: 3rdparty/openssl/crypto/modes/gcm128.c 3rdparty/openssl/crypto/arm_arch.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)hm_ameth.o: 3rdparty/openssl/crypto/hmac/hm_ameth.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)hm_pmeth.o: 3rdparty/openssl/crypto/hmac/hm_pmeth.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)hmac.o: 3rdparty/openssl/crypto/hmac/hmac.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)init.o: 3rdparty/openssl/crypto/init.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ct_vfy.o: 3rdparty/openssl/crypto/ct/ct_vfy.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ct_err.o: 3rdparty/openssl/crypto/ct/ct_err.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ct_sct_ctx.o: 3rdparty/openssl/crypto/ct/ct_sct_ctx.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ct_sct.o: 3rdparty/openssl/crypto/ct/ct_sct.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ct_x509v3.o: CPPFLAGS += -I3rdparty/openssl/crypto/ct
$(BUILDDIR)ct_x509v3.o: 3rdparty/openssl/crypto/ct/ct_x509v3.c 3rdparty/openssl/crypto/ct/ct_local.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ct_prn.o: 3rdparty/openssl/crypto/ct/ct_prn.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ct_log.o: 3rdparty/openssl/crypto/ct/ct_log.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ct_b64.o: 3rdparty/openssl/crypto/ct/ct_b64.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ct_policy.o: 3rdparty/openssl/crypto/ct/ct_policy.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ct_oct.o: 3rdparty/openssl/crypto/ct/ct_oct.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)e_camellia.o: CPPFLAGS += -I3rdparty/openssl/include/openssl -I3rdparty/openssl/crypto/modes
$(BUILDDIR)e_camellia.o: 3rdparty/openssl/crypto/evp/e_camellia.c 3rdparty/openssl/include/openssl/evp.h 3rdparty/openssl/crypto/modes/modes_local.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)e_idea.o: 3rdparty/openssl/crypto/evp/e_idea.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)m_md4.o: 3rdparty/openssl/crypto/evp/m_md4.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)names.o: 3rdparty/openssl/crypto/evp/names.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)evp_err.o: 3rdparty/openssl/crypto/evp/evp_err.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)m_sha1.o: 3rdparty/openssl/crypto/evp/m_sha1.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bio_md.o: 3rdparty/openssl/crypto/evp/bio_md.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)m_ripemd.o: 3rdparty/openssl/crypto/evp/m_ripemd.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)evp_lib.o: 3rdparty/openssl/crypto/evp/evp_lib.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bio_enc.o: 3rdparty/openssl/crypto/evp/bio_enc.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)pmeth_gn.o: 3rdparty/openssl/crypto/evp/pmeth_gn.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)c_alld.o: 3rdparty/openssl/crypto/evp/c_alld.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)m_sha3.o: 3rdparty/openssl/crypto/evp/m_sha3.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)e_rc2.o: 3rdparty/openssl/crypto/evp/e_rc2.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bio_b64.o: 3rdparty/openssl/crypto/evp/bio_b64.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)p_enc.o: 3rdparty/openssl/crypto/evp/p_enc.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)e_null.o: 3rdparty/openssl/crypto/evp/e_null.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)e_aes_cbc_hmac_sha256.o: 3rdparty/openssl/crypto/evp/e_aes_cbc_hmac_sha256.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)e_rc4_hmac_md5.o: 3rdparty/openssl/crypto/evp/e_rc4_hmac_md5.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)p_open.o: 3rdparty/openssl/crypto/evp/p_open.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)p_lib.o: 3rdparty/openssl/crypto/evp/p_lib.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)e_bf.o: 3rdparty/openssl/crypto/evp/e_bf.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)e_sm4.o: 3rdparty/openssl/crypto/evp/e_sm4.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)pmeth_lib.o: 3rdparty/openssl/crypto/evp/pmeth_lib.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)encode.o: 3rdparty/openssl/crypto/evp/encode.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)p5_crpt2.o: CPPFLAGS += -I3rdparty/openssl/crypto/evp
$(BUILDDIR)p5_crpt2.o: 3rdparty/openssl/crypto/evp/p5_crpt2.c 3rdparty/openssl/crypto/evp/evp_local.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)e_seed.o: 3rdparty/openssl/crypto/evp/e_seed.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)e_aes_cbc_hmac_sha1.o: 3rdparty/openssl/crypto/evp/e_aes_cbc_hmac_sha1.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)evp_pkey.o: 3rdparty/openssl/crypto/evp/evp_pkey.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)e_des3.o: 3rdparty/openssl/crypto/evp/e_des3.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)e_rc5.o: 3rdparty/openssl/crypto/evp/e_rc5.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)m_md5.o: 3rdparty/openssl/crypto/evp/m_md5.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)p_dec.o: 3rdparty/openssl/crypto/evp/p_dec.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)m_md2.o: 3rdparty/openssl/crypto/evp/m_md2.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)e_aria.o: 3rdparty/openssl/crypto/evp/e_aria.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bio_ok.o: 3rdparty/openssl/crypto/evp/bio_ok.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)p5_crpt.o: 3rdparty/openssl/crypto/evp/p5_crpt.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)p_seal.o: 3rdparty/openssl/crypto/evp/p_seal.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)evp_pbe.o: 3rdparty/openssl/crypto/evp/evp_pbe.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)e_rc4.o: 3rdparty/openssl/crypto/evp/e_rc4.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)p_verify.o: 3rdparty/openssl/crypto/evp/p_verify.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)evp_enc.o: 3rdparty/openssl/crypto/evp/evp_enc.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cmeth_lib.o: 3rdparty/openssl/crypto/evp/cmeth_lib.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)digest.o: 3rdparty/openssl/crypto/evp/digest.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)pbe_scrypt.o: 3rdparty/openssl/crypto/evp/pbe_scrypt.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)e_chacha20_poly1305.o: 3rdparty/openssl/crypto/evp/e_chacha20_poly1305.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)e_des.o: 3rdparty/openssl/crypto/evp/e_des.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)m_mdc2.o: 3rdparty/openssl/crypto/evp/m_mdc2.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)evp_cnf.o: 3rdparty/openssl/crypto/evp/evp_cnf.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)e_old.o: 3rdparty/openssl/crypto/evp/e_old.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)c_allc.o: 3rdparty/openssl/crypto/evp/c_allc.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)m_md5_sha1.o: 3rdparty/openssl/crypto/evp/m_md5_sha1.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)evp_key.o: 3rdparty/openssl/crypto/evp/evp_key.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)m_wp.o: 3rdparty/openssl/crypto/evp/m_wp.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)pmeth_fn.o: 3rdparty/openssl/crypto/evp/pmeth_fn.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)m_sigver.o: 3rdparty/openssl/crypto/evp/m_sigver.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)e_cast.o: 3rdparty/openssl/crypto/evp/e_cast.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)e_aes.o: 3rdparty/openssl/crypto/evp/e_aes.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)p_sign.o: 3rdparty/openssl/crypto/evp/p_sign.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)e_xcbc_d.o: 3rdparty/openssl/crypto/evp/e_xcbc_d.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)m_null.o: 3rdparty/openssl/crypto/evp/m_null.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)seed_cbc.o: 3rdparty/openssl/crypto/seed/seed_cbc.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)seed_cfb.o: 3rdparty/openssl/crypto/seed/seed_cfb.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)seed_ofb.o: 3rdparty/openssl/crypto/seed/seed_ofb.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)seed_ecb.o: 3rdparty/openssl/crypto/seed/seed_ecb.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)seed.o: CPPFLAGS += -I3rdparty/openssl/crypto/seed
$(BUILDDIR)seed.o: 3rdparty/openssl/crypto/seed/seed.c 3rdparty/openssl/crypto/seed/seed_local.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ts_rsp_print.o: 3rdparty/openssl/crypto/ts/ts_rsp_print.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ts_err.o: 3rdparty/openssl/crypto/ts/ts_err.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ts_asn1.o: 3rdparty/openssl/crypto/ts/ts_asn1.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ts_req_print.o: 3rdparty/openssl/crypto/ts/ts_req_print.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ts_rsp_verify.o: 3rdparty/openssl/crypto/ts/ts_rsp_verify.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ts_req_utils.o: 3rdparty/openssl/crypto/ts/ts_req_utils.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ts_rsp_sign.o: 3rdparty/openssl/crypto/ts/ts_rsp_sign.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ts_verify_ctx.o: 3rdparty/openssl/crypto/ts/ts_verify_ctx.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ts_lib.o: 3rdparty/openssl/crypto/ts/ts_lib.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ts_rsp_utils.o: 3rdparty/openssl/crypto/ts/ts_rsp_utils.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ts_conf.o: 3rdparty/openssl/crypto/ts/ts_conf.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)dso_err.o: 3rdparty/openssl/crypto/dso/dso_err.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)dso_dl.o: CPPFLAGS += -I3rdparty/openssl/crypto/dso
$(BUILDDIR)dso_dl.o: 3rdparty/openssl/crypto/dso/dso_dl.c 3rdparty/openssl/crypto/dso/dso_local.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)dso_win32.o: 3rdparty/openssl/crypto/dso/dso_win32.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)dso_dlfcn.o: 3rdparty/openssl/crypto/dso/dso_dlfcn.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)dso_openssl.o: 3rdparty/openssl/crypto/dso/dso_openssl.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)dso_vms.o: 3rdparty/openssl/crypto/dso/dso_vms.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)dso_lib.o: 3rdparty/openssl/crypto/dso/dso_lib.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)rmd_dgst.o: 3rdparty/openssl/crypto/ripemd/rmd_dgst.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)rmd_one.o: 3rdparty/openssl/crypto/ripemd/rmd_one.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)threads_pthread.o: 3rdparty/openssl/crypto/threads_pthread.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bf_skey.o: 3rdparty/openssl/crypto/bf/bf_skey.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bf_cfb64.o: 3rdparty/openssl/crypto/bf/bf_cfb64.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bf_enc.o: 3rdparty/openssl/crypto/bf/bf_enc.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bf_ofb64.o: 3rdparty/openssl/crypto/bf/bf_ofb64.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bf_ecb.o: 3rdparty/openssl/crypto/bf/bf_ecb.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)i_ecb.o: 3rdparty/openssl/crypto/idea/i_ecb.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)i_ofb64.o: 3rdparty/openssl/crypto/idea/i_ofb64.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)i_cbc.o: 3rdparty/openssl/crypto/idea/i_cbc.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)i_skey.o: 3rdparty/openssl/crypto/idea/i_skey.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)i_cfb64.o: 3rdparty/openssl/crypto/idea/i_cfb64.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)txt_db.o: 3rdparty/openssl/crypto/txt_db/txt_db.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ameth_lib.o: 3rdparty/openssl/crypto/asn1/ameth_lib.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)a_utctm.o: 3rdparty/openssl/crypto/asn1/a_utctm.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)p5_pbe.o: 3rdparty/openssl/crypto/asn1/p5_pbe.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)asn1_gen.o: 3rdparty/openssl/crypto/asn1/asn1_gen.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)asn_mstbl.o: 3rdparty/openssl/crypto/asn1/asn_mstbl.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)asn1_par.o: 3rdparty/openssl/crypto/asn1/asn1_par.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bio_asn1.o: 3rdparty/openssl/crypto/asn1/bio_asn1.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)a_strnid.o: 3rdparty/openssl/crypto/asn1/a_strnid.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)i2d_pu.o: 3rdparty/openssl/crypto/asn1/i2d_pu.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)t_pkey.o: 3rdparty/openssl/crypto/asn1/t_pkey.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)d2i_pr.o: 3rdparty/openssl/crypto/asn1/d2i_pr.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)a_gentm.o: 3rdparty/openssl/crypto/asn1/a_gentm.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)a_verify.o: 3rdparty/openssl/crypto/asn1/a_verify.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)a_strex.o: 3rdparty/openssl/crypto/asn1/a_strex.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)tasn_prn.o: 3rdparty/openssl/crypto/asn1/tasn_prn.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)tasn_fre.o: 3rdparty/openssl/crypto/asn1/tasn_fre.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)f_string.o: 3rdparty/openssl/crypto/asn1/f_string.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)a_bitstr.o: 3rdparty/openssl/crypto/asn1/a_bitstr.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)x_sig.o: 3rdparty/openssl/crypto/asn1/x_sig.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)x_long.o: 3rdparty/openssl/crypto/asn1/x_long.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)a_print.o: 3rdparty/openssl/crypto/asn1/a_print.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)x_spki.o: 3rdparty/openssl/crypto/asn1/x_spki.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)asn1_err.o: 3rdparty/openssl/crypto/asn1/asn1_err.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)a_int.o: 3rdparty/openssl/crypto/asn1/a_int.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)asn_mime.o: 3rdparty/openssl/crypto/asn1/asn_mime.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)a_i2d_fp.o: 3rdparty/openssl/crypto/asn1/a_i2d_fp.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)p5_pbev2.o: 3rdparty/openssl/crypto/asn1/p5_pbev2.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bio_ndef.o: 3rdparty/openssl/crypto/asn1/bio_ndef.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)a_type.o: 3rdparty/openssl/crypto/asn1/a_type.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)n_pkey.o: 3rdparty/openssl/crypto/asn1/n_pkey.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)x_int64.o: 3rdparty/openssl/crypto/asn1/x_int64.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)a_octet.o: 3rdparty/openssl/crypto/asn1/a_octet.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)p5_scrypt.o: 3rdparty/openssl/crypto/asn1/p5_scrypt.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)nsseq.o: 3rdparty/openssl/crypto/asn1/nsseq.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)a_time.o: 3rdparty/openssl/crypto/asn1/a_time.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)asn1_lib.o: 3rdparty/openssl/crypto/asn1/asn1_lib.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)x_algor.o: 3rdparty/openssl/crypto/asn1/x_algor.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)i2d_pr.o: 3rdparty/openssl/crypto/asn1/i2d_pr.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)f_int.o: 3rdparty/openssl/crypto/asn1/f_int.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)x_bignum.o: 3rdparty/openssl/crypto/asn1/x_bignum.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)a_sign.o: 3rdparty/openssl/crypto/asn1/a_sign.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)asn_moid.o: 3rdparty/openssl/crypto/asn1/asn_moid.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)a_d2i_fp.o: 3rdparty/openssl/crypto/asn1/a_d2i_fp.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)tasn_new.o: 3rdparty/openssl/crypto/asn1/tasn_new.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)a_object.o: 3rdparty/openssl/crypto/asn1/a_object.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)d2i_pu.o: 3rdparty/openssl/crypto/asn1/d2i_pu.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)t_bitst.o: 3rdparty/openssl/crypto/asn1/t_bitst.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)evp_asn1.o: 3rdparty/openssl/crypto/asn1/evp_asn1.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)a_utf8.o: 3rdparty/openssl/crypto/asn1/a_utf8.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)tasn_scn.o: 3rdparty/openssl/crypto/asn1/tasn_scn.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)a_dup.o: 3rdparty/openssl/crypto/asn1/a_dup.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)p8_pkey.o: 3rdparty/openssl/crypto/asn1/p8_pkey.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)asn1_item_list.o: 3rdparty/openssl/crypto/asn1/asn1_item_list.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)tasn_utl.o: 3rdparty/openssl/crypto/asn1/tasn_utl.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)tasn_dec.o: 3rdparty/openssl/crypto/asn1/tasn_dec.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)t_spki.o: 3rdparty/openssl/crypto/asn1/t_spki.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)a_digest.o: 3rdparty/openssl/crypto/asn1/a_digest.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)a_mbstr.o: 3rdparty/openssl/crypto/asn1/a_mbstr.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)tasn_typ.o: 3rdparty/openssl/crypto/asn1/tasn_typ.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)asn_pack.o: 3rdparty/openssl/crypto/asn1/asn_pack.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)tasn_enc.o: 3rdparty/openssl/crypto/asn1/tasn_enc.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)x_info.o: 3rdparty/openssl/crypto/asn1/x_info.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)x_val.o: 3rdparty/openssl/crypto/asn1/x_val.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)x_pkey.o: 3rdparty/openssl/crypto/asn1/x_pkey.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)siphash_pmeth.o: 3rdparty/openssl/crypto/siphash/siphash_pmeth.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)siphash_ameth.o: 3rdparty/openssl/crypto/siphash/siphash_ameth.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)siphash.o: 3rdparty/openssl/crypto/siphash/siphash.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)store_register.o: 3rdparty/openssl/crypto/store/store_register.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)store_lib.o: 3rdparty/openssl/crypto/store/store_lib.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)store_err.o: 3rdparty/openssl/crypto/store/store_err.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)loader_file.o: 3rdparty/openssl/crypto/store/loader_file.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)store_strings.o: 3rdparty/openssl/crypto/store/store_strings.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)store_init.o: 3rdparty/openssl/crypto/store/store_init.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)dh_rfc7919.o: 3rdparty/openssl/crypto/dh/dh_rfc7919.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)dh_kdf.o: 3rdparty/openssl/crypto/dh/dh_kdf.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)dh_lib.o: 3rdparty/openssl/crypto/dh/dh_lib.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)dh_rfc5114.o: 3rdparty/openssl/crypto/dh/dh_rfc5114.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)dh_prn.o: 3rdparty/openssl/crypto/dh/dh_prn.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)dh_pmeth.o: 3rdparty/openssl/crypto/dh/dh_pmeth.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)dh_asn1.o: 3rdparty/openssl/crypto/dh/dh_asn1.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)dh_err.o: 3rdparty/openssl/crypto/dh/dh_err.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)dh_meth.o: CPPFLAGS += -I3rdparty/openssl/crypto/dh
$(BUILDDIR)dh_meth.o: 3rdparty/openssl/crypto/dh/dh_meth.c 3rdparty/openssl/crypto/dh/dh_local.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)dh_depr.o: 3rdparty/openssl/crypto/dh/dh_depr.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)dh_check.o: 3rdparty/openssl/crypto/dh/dh_check.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)dh_ameth.o: 3rdparty/openssl/crypto/dh/dh_ameth.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)dh_gen.o: 3rdparty/openssl/crypto/dh/dh_gen.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)dh_key.o: 3rdparty/openssl/crypto/dh/dh_key.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ebcdic.o: 3rdparty/openssl/crypto/ebcdic.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)mdc2dgst.o: 3rdparty/openssl/crypto/mdc2/mdc2dgst.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)mdc2_one.o: 3rdparty/openssl/crypto/mdc2/mdc2_one.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cmll_ofb.o: 3rdparty/openssl/crypto/camellia/cmll_ofb.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cmll_cbc.o: 3rdparty/openssl/crypto/camellia/cmll_cbc.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cmll_misc.o: 3rdparty/openssl/crypto/camellia/cmll_misc.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)camellia.o: 3rdparty/openssl/crypto/camellia/camellia.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cmll_cfb.o: 3rdparty/openssl/crypto/camellia/cmll_cfb.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cmll_ctr.o: 3rdparty/openssl/crypto/camellia/cmll_ctr.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cmll_ecb.o: 3rdparty/openssl/crypto/camellia/cmll_ecb.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)LPdir_wince.o: 3rdparty/openssl/crypto/LPdir_wince.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)o_init.o: 3rdparty/openssl/crypto/o_init.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)m_sm3.o: 3rdparty/openssl/crypto/sm3/m_sm3.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)sm3.o: 3rdparty/openssl/crypto/sm3/sm3.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)chacha_enc.o: 3rdparty/openssl/crypto/chacha/chacha_enc.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)c_ecb.o: 3rdparty/openssl/crypto/cast/c_ecb.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)c_ofb64.o: 3rdparty/openssl/crypto/cast/c_ofb64.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)c_enc.o: 3rdparty/openssl/crypto/cast/c_enc.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)c_skey.o: 3rdparty/openssl/crypto/cast/c_skey.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)c_cfb64.o: 3rdparty/openssl/crypto/cast/c_cfb64.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)LPdir_unix.o: 3rdparty/openssl/crypto/LPdir_unix.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)s390xcap.o: 3rdparty/openssl/crypto/s390xcap.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)LPdir_vms.o: 3rdparty/openssl/crypto/LPdir_vms.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)wp_block.o: CPPFLAGS += -I3rdparty/openssl/crypto/whrlpool
$(BUILDDIR)wp_block.o: 3rdparty/openssl/crypto/whrlpool/wp_block.c 3rdparty/openssl/crypto/whrlpool/wp_local.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)wp_dgst.o: 3rdparty/openssl/crypto/whrlpool/wp_dgst.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)mem_sec.o: 3rdparty/openssl/crypto/mem_sec.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)pcbc_enc.o: CPPFLAGS += -I3rdparty/openssl/crypto/des
$(BUILDDIR)pcbc_enc.o: 3rdparty/openssl/crypto/des/pcbc_enc.c 3rdparty/openssl/crypto/des/des_local.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ecb3_enc.o: 3rdparty/openssl/crypto/des/ecb3_enc.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)str2key.o: 3rdparty/openssl/crypto/des/str2key.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)xcbc_enc.o: 3rdparty/openssl/crypto/des/xcbc_enc.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)fcrypt.o: 3rdparty/openssl/crypto/des/fcrypt.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ncbc_enc.o: 3rdparty/openssl/crypto/des/ncbc_enc.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)des_enc.o: 3rdparty/openssl/crypto/des/des_enc.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ofb64ede.o: 3rdparty/openssl/crypto/des/ofb64ede.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)qud_cksm.o: 3rdparty/openssl/crypto/des/qud_cksm.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cfb_enc.o: 3rdparty/openssl/crypto/des/cfb_enc.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)rand_key.o: 3rdparty/openssl/crypto/des/rand_key.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cbc_cksm.o: 3rdparty/openssl/crypto/des/cbc_cksm.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)fcrypt_b.o: 3rdparty/openssl/crypto/des/fcrypt_b.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ecb_enc.o: 3rdparty/openssl/crypto/des/ecb_enc.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cfb64ede.o: 3rdparty/openssl/crypto/des/cfb64ede.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)set_key.o: 3rdparty/openssl/crypto/des/set_key.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cfb64enc.o: 3rdparty/openssl/crypto/des/cfb64enc.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ofb_enc.o: 3rdparty/openssl/crypto/des/ofb_enc.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ofb64enc.o: 3rdparty/openssl/crypto/des/ofb64enc.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cbc_enc.o: 3rdparty/openssl/crypto/des/cbc_enc.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ui_err.o: 3rdparty/openssl/crypto/ui/ui_err.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ui_openssl.o: CPPFLAGS += -I3rdparty/openssl/crypto/ui
$(BUILDDIR)ui_openssl.o: 3rdparty/openssl/crypto/ui/ui_openssl.c 3rdparty/openssl/crypto/ui/ui_local.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ui_util.o: 3rdparty/openssl/crypto/ui/ui_util.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ui_null.o: 3rdparty/openssl/crypto/ui/ui_null.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ui_lib.o: 3rdparty/openssl/crypto/ui/ui_lib.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)mem_dbg.o: 3rdparty/openssl/crypto/mem_dbg.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)md4_dgst.o: 3rdparty/openssl/crypto/md4/md4_dgst.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)md4_one.o: 3rdparty/openssl/crypto/md4/md4_one.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cryptlib.o: 3rdparty/openssl/crypto/cryptlib.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)lhash.o: 3rdparty/openssl/crypto/lhash/lhash.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)lh_stats.o: 3rdparty/openssl/crypto/lhash/lh_stats.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)o_fopen.o: 3rdparty/openssl/crypto/o_fopen.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ocsp_srv.o: 3rdparty/openssl/crypto/ocsp/ocsp_srv.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ocsp_prn.o: 3rdparty/openssl/crypto/ocsp/ocsp_prn.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ocsp_vfy.o: 3rdparty/openssl/crypto/ocsp/ocsp_vfy.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ocsp_lib.o: 3rdparty/openssl/crypto/ocsp/ocsp_lib.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ocsp_asn.o: 3rdparty/openssl/crypto/ocsp/ocsp_asn.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ocsp_ht.o: 3rdparty/openssl/crypto/ocsp/ocsp_ht.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)v3_ocsp.o: CPPFLAGS += -I3rdparty/openssl/crypto/ocsp
$(BUILDDIR)v3_ocsp.o: 3rdparty/openssl/crypto/ocsp/v3_ocsp.c 3rdparty/openssl/crypto/ocsp/ocsp_local.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ocsp_err.o: 3rdparty/openssl/crypto/ocsp/ocsp_err.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ocsp_ext.o: 3rdparty/openssl/crypto/ocsp/ocsp_ext.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ocsp_cl.o: 3rdparty/openssl/crypto/ocsp/ocsp_cl.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)srp_lib.o: 3rdparty/openssl/crypto/srp/srp_lib.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)srp_vfy.o: 3rdparty/openssl/crypto/srp/srp_vfy.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)comp_err.o: 3rdparty/openssl/crypto/comp/comp_err.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)comp_lib.o: 3rdparty/openssl/crypto/comp/comp_lib.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)c_zlib.o: 3rdparty/openssl/crypto/comp/c_zlib.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)pk7_lib.o: 3rdparty/openssl/crypto/pkcs7/pk7_lib.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)pk7_asn1.o: 3rdparty/openssl/crypto/pkcs7/pk7_asn1.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)pk7_doit.o: 3rdparty/openssl/crypto/pkcs7/pk7_doit.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bio_pk7.o: 3rdparty/openssl/crypto/pkcs7/bio_pk7.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)pkcs7err.o: 3rdparty/openssl/crypto/pkcs7/pkcs7err.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)pk7_mime.o: 3rdparty/openssl/crypto/pkcs7/pk7_mime.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)pk7_attr.o: 3rdparty/openssl/crypto/pkcs7/pk7_attr.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)pk7_smime.o: 3rdparty/openssl/crypto/pkcs7/pk7_smime.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)hkdf.o: 3rdparty/openssl/crypto/kdf/hkdf.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)kdf_err.o: 3rdparty/openssl/crypto/kdf/kdf_err.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)scrypt.o: 3rdparty/openssl/crypto/kdf/scrypt.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)tls1_prf.o: 3rdparty/openssl/crypto/kdf/tls1_prf.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)dsa_depr.o: 3rdparty/openssl/crypto/dsa/dsa_depr.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)dsa_key.o: 3rdparty/openssl/crypto/dsa/dsa_key.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)dsa_ossl.o: 3rdparty/openssl/crypto/dsa/dsa_ossl.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)dsa_lib.o: 3rdparty/openssl/crypto/dsa/dsa_lib.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)dsa_ameth.o: 3rdparty/openssl/crypto/dsa/dsa_ameth.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)dsa_prn.o: 3rdparty/openssl/crypto/dsa/dsa_prn.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)dsa_vrf.o: 3rdparty/openssl/crypto/dsa/dsa_vrf.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)dsa_pmeth.o: 3rdparty/openssl/crypto/dsa/dsa_pmeth.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)dsa_asn1.o: 3rdparty/openssl/crypto/dsa/dsa_asn1.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)dsa_err.o: 3rdparty/openssl/crypto/dsa/dsa_err.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)dsa_sign.o: 3rdparty/openssl/crypto/dsa/dsa_sign.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)dsa_gen.o: 3rdparty/openssl/crypto/dsa/dsa_gen.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)dsa_meth.o: CPPFLAGS += -I3rdparty/openssl/crypto/dsa
$(BUILDDIR)dsa_meth.o: 3rdparty/openssl/crypto/dsa/dsa_meth.c 3rdparty/openssl/crypto/dsa/dsa_local.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ctype.o: 3rdparty/openssl/crypto/ctype.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)o_fips.o: 3rdparty/openssl/crypto/o_fips.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ppccap.o: 3rdparty/openssl/crypto/ppccap.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)rc4_skey.o: 3rdparty/openssl/crypto/rc4/rc4_skey.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)rc4_enc.o: 3rdparty/openssl/crypto/rc4/rc4_enc.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)threads_none.o: 3rdparty/openssl/crypto/threads_none.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)o_str.o: 3rdparty/openssl/crypto/o_str.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)err_all.o: 3rdparty/openssl/crypto/err/err_all.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)err.o: 3rdparty/openssl/crypto/err/err.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)err_prn.o: 3rdparty/openssl/crypto/err/err_prn.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)armcap.o: 3rdparty/openssl/crypto/armcap.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bf_lbuf.o: 3rdparty/openssl/crypto/bio/bf_lbuf.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)b_sock2.o: 3rdparty/openssl/crypto/bio/b_sock2.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bss_null.o: 3rdparty/openssl/crypto/bio/bss_null.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bss_acpt.o: 3rdparty/openssl/crypto/bio/bss_acpt.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bss_mem.o: 3rdparty/openssl/crypto/bio/bss_mem.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bf_buff.o: 3rdparty/openssl/crypto/bio/bf_buff.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)b_sock.o: 3rdparty/openssl/crypto/bio/b_sock.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)b_print.o: 3rdparty/openssl/crypto/bio/b_print.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bss_dgram.o: 3rdparty/openssl/crypto/bio/bss_dgram.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bio_err.o: 3rdparty/openssl/crypto/bio/bio_err.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)b_addr.o: 3rdparty/openssl/crypto/bio/b_addr.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bss_sock.o: 3rdparty/openssl/crypto/bio/bss_sock.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bio_cb.o: 3rdparty/openssl/crypto/bio/bio_cb.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bss_bio.o: 3rdparty/openssl/crypto/bio/bss_bio.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bio_lib.o: 3rdparty/openssl/crypto/bio/bio_lib.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bf_null.o: 3rdparty/openssl/crypto/bio/bf_null.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bss_log.o: 3rdparty/openssl/crypto/bio/bss_log.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bss_conn.o: 3rdparty/openssl/crypto/bio/bss_conn.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bss_fd.o: 3rdparty/openssl/crypto/bio/bss_fd.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bf_nbio.o: 3rdparty/openssl/crypto/bio/bf_nbio.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)b_dump.o: 3rdparty/openssl/crypto/bio/b_dump.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bio_meth.o: CPPFLAGS += -I3rdparty/openssl/crypto/bio
$(BUILDDIR)bio_meth.o: 3rdparty/openssl/crypto/bio/bio_meth.c 3rdparty/openssl/crypto/bio/bio_local.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bss_file.o: 3rdparty/openssl/crypto/bio/bss_file.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)stack.o: 3rdparty/openssl/crypto/stack/stack.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cpt_err.o: 3rdparty/openssl/crypto/cpt_err.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)getenv.o: 3rdparty/openssl/crypto/getenv.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)m_blake2s.o: CPPFLAGS += -I3rdparty/openssl/crypto/blake2
$(BUILDDIR)m_blake2s.o: 3rdparty/openssl/crypto/blake2/m_blake2s.c 3rdparty/openssl/crypto/blake2/blake2_local.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)m_blake2b.o: 3rdparty/openssl/crypto/blake2/m_blake2b.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)blake2s.o: 3rdparty/openssl/crypto/blake2/blake2s.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)blake2b.o: 3rdparty/openssl/crypto/blake2/blake2b.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)threads_win.o: 3rdparty/openssl/crypto/threads_win.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)rsa_none.o: 3rdparty/openssl/crypto/rsa/rsa_none.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)rsa_x931g.o: 3rdparty/openssl/crypto/rsa/rsa_x931g.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)rsa_prn.o: 3rdparty/openssl/crypto/rsa/rsa_prn.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)rsa_meth.o: 3rdparty/openssl/crypto/rsa/rsa_meth.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)rsa_pmeth.o: 3rdparty/openssl/crypto/rsa/rsa_pmeth.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)rsa_oaep.o: 3rdparty/openssl/crypto/rsa/rsa_oaep.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)rsa_crpt.o: 3rdparty/openssl/crypto/rsa/rsa_crpt.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)rsa_mp.o: 3rdparty/openssl/crypto/rsa/rsa_mp.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)rsa_saos.o: 3rdparty/openssl/crypto/rsa/rsa_saos.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)rsa_ossl.o: 3rdparty/openssl/crypto/rsa/rsa_ossl.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)rsa_asn1.o: 3rdparty/openssl/crypto/rsa/rsa_asn1.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)rsa_sign.o: 3rdparty/openssl/crypto/rsa/rsa_sign.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)rsa_ssl.o: 3rdparty/openssl/crypto/rsa/rsa_ssl.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)rsa_gen.o: 3rdparty/openssl/crypto/rsa/rsa_gen.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)rsa_ameth.o: 3rdparty/openssl/crypto/rsa/rsa_ameth.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)rsa_err.o: 3rdparty/openssl/crypto/rsa/rsa_err.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)rsa_pk1.o: 3rdparty/openssl/crypto/rsa/rsa_pk1.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)rsa_lib.o: 3rdparty/openssl/crypto/rsa/rsa_lib.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)rsa_x931.o: 3rdparty/openssl/crypto/rsa/rsa_x931.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)rsa_pss.o: 3rdparty/openssl/crypto/rsa/rsa_pss.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)rsa_depr.o: 3rdparty/openssl/crypto/rsa/rsa_depr.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)rsa_chk.o: 3rdparty/openssl/crypto/rsa/rsa_chk.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)v3_conf.o: 3rdparty/openssl/crypto/x509v3/v3_conf.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)pcy_lib.o: 3rdparty/openssl/crypto/x509v3/pcy_lib.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)v3err.o: 3rdparty/openssl/crypto/x509v3/v3err.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)v3_akey.o: 3rdparty/openssl/crypto/x509v3/v3_akey.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)v3_pcia.o: 3rdparty/openssl/crypto/x509v3/v3_pcia.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)v3_sxnet.o: 3rdparty/openssl/crypto/x509v3/v3_sxnet.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)v3_ncons.o: 3rdparty/openssl/crypto/x509v3/v3_ncons.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)v3_alt.o: 3rdparty/openssl/crypto/x509v3/v3_alt.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)pcy_data.o: 3rdparty/openssl/crypto/x509v3/pcy_data.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)v3_pci.o: 3rdparty/openssl/crypto/x509v3/v3_pci.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)v3_utl.o: 3rdparty/openssl/crypto/x509v3/v3_utl.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)v3_enum.o: 3rdparty/openssl/crypto/x509v3/v3_enum.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)v3_bcons.o: 3rdparty/openssl/crypto/x509v3/v3_bcons.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)v3_akeya.o: 3rdparty/openssl/crypto/x509v3/v3_akeya.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)v3_genn.o: 3rdparty/openssl/crypto/x509v3/v3_genn.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)v3_skey.o: 3rdparty/openssl/crypto/x509v3/v3_skey.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)v3_extku.o: 3rdparty/openssl/crypto/x509v3/v3_extku.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)v3_pmaps.o: 3rdparty/openssl/crypto/x509v3/v3_pmaps.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)v3_tlsf.o: 3rdparty/openssl/crypto/x509v3/v3_tlsf.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)v3_prn.o: 3rdparty/openssl/crypto/x509v3/v3_prn.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)pcy_tree.o: 3rdparty/openssl/crypto/x509v3/pcy_tree.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)v3_addr.o: 3rdparty/openssl/crypto/x509v3/v3_addr.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)v3_bitst.o: 3rdparty/openssl/crypto/x509v3/v3_bitst.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)v3_int.o: 3rdparty/openssl/crypto/x509v3/v3_int.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)pcy_cache.o: 3rdparty/openssl/crypto/x509v3/pcy_cache.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)v3_info.o: 3rdparty/openssl/crypto/x509v3/v3_info.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)v3_admis.o: 3rdparty/openssl/crypto/x509v3/v3_admis.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)v3_ia5.o: 3rdparty/openssl/crypto/x509v3/v3_ia5.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)pcy_map.o: 3rdparty/openssl/crypto/x509v3/pcy_map.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)v3_purp.o: 3rdparty/openssl/crypto/x509v3/v3_purp.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)v3_cpols.o: 3rdparty/openssl/crypto/x509v3/v3_cpols.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)v3_pcons.o: 3rdparty/openssl/crypto/x509v3/v3_pcons.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)v3_asid.o: 3rdparty/openssl/crypto/x509v3/v3_asid.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)v3_lib.o: 3rdparty/openssl/crypto/x509v3/v3_lib.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)pcy_node.o: 3rdparty/openssl/crypto/x509v3/pcy_node.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)v3_pku.o: 3rdparty/openssl/crypto/x509v3/v3_pku.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)v3_crld.o: 3rdparty/openssl/crypto/x509v3/v3_crld.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ex_data.o: 3rdparty/openssl/crypto/ex_data.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)LPdir_win32.o: 3rdparty/openssl/crypto/LPdir_win32.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)o_dir.o: 3rdparty/openssl/crypto/o_dir.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)LPdir_win.o: 3rdparty/openssl/crypto/LPdir_win.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)LPdir_nyi.o: 3rdparty/openssl/crypto/LPdir_nyi.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ecp_smpl.o: 3rdparty/openssl/crypto/ec/ecp_smpl.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ecdh_kdf.o: 3rdparty/openssl/crypto/ec/ecdh_kdf.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ecx_meth.o: 3rdparty/openssl/crypto/ec/ecx_meth.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ec_mult.o: 3rdparty/openssl/crypto/ec/ec_mult.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ecp_oct.o: 3rdparty/openssl/crypto/ec/ecp_oct.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)curve25519.o: 3rdparty/openssl/crypto/ec/curve25519.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)curve448.o: 3rdparty/openssl/crypto/ec/curve448/curve448.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)f_generic.o: CPPFLAGS += -I3rdparty/openssl/crypto/ec/curve448
$(BUILDDIR)f_generic.o: 3rdparty/openssl/crypto/ec/curve448/f_generic.c 3rdparty/openssl/crypto/ec/curve448/field.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)f_impl.o: 3rdparty/openssl/crypto/ec/curve448/arch_32/f_impl.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)scalar.o: 3rdparty/openssl/crypto/ec/curve448/scalar.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)eddsa.o: 3rdparty/openssl/crypto/ec/curve448/eddsa.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)curve448_tables.o: 3rdparty/openssl/crypto/ec/curve448/curve448_tables.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ecdsa_ossl.o: 3rdparty/openssl/crypto/ec/ecdsa_ossl.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ecp_mont.o: 3rdparty/openssl/crypto/ec/ecp_mont.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ec_curve.o: 3rdparty/openssl/crypto/ec/ec_curve.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ec_key.o: 3rdparty/openssl/crypto/ec/ec_key.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ec_asn1.o: 3rdparty/openssl/crypto/ec/ec_asn1.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ec_lib.o: 3rdparty/openssl/crypto/ec/ec_lib.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ecdsa_vrf.o: 3rdparty/openssl/crypto/ec/ecdsa_vrf.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ecp_nistputil.o: CPPFLAGS += -I3rdparty/openssl/crypto/ec
$(BUILDDIR)ecp_nistputil.o: 3rdparty/openssl/crypto/ec/ecp_nistputil.c 3rdparty/openssl/crypto/ec/ec_local.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ec_oct.o: 3rdparty/openssl/crypto/ec/ec_oct.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ecdsa_sign.o: 3rdparty/openssl/crypto/ec/ecdsa_sign.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)eck_prn.o: 3rdparty/openssl/crypto/ec/eck_prn.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ecp_nistp256.o: 3rdparty/openssl/crypto/ec/ecp_nistp256.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ec_kmeth.o: 3rdparty/openssl/crypto/ec/ec_kmeth.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ec2_oct.o: 3rdparty/openssl/crypto/ec/ec2_oct.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ecp_nistz256_table.o: 3rdparty/openssl/crypto/ec/ecp_nistz256_table.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ecp_nistp224.o: 3rdparty/openssl/crypto/ec/ecp_nistp224.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ec_cvt.o: 3rdparty/openssl/crypto/ec/ec_cvt.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ec2_smpl.o: 3rdparty/openssl/crypto/ec/ec2_smpl.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ecdh_ossl.o: 3rdparty/openssl/crypto/ec/ecdh_ossl.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ec_check.o: 3rdparty/openssl/crypto/ec/ec_check.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ecp_nistp521.o: 3rdparty/openssl/crypto/ec/ecp_nistp521.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ec_err.o: 3rdparty/openssl/crypto/ec/ec_err.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ec_print.o: 3rdparty/openssl/crypto/ec/ec_print.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ecp_nistz256.o: 3rdparty/openssl/crypto/ec/ecp_nistz256.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ec_pmeth.o: 3rdparty/openssl/crypto/ec/ec_pmeth.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ec_ameth.o: 3rdparty/openssl/crypto/ec/ec_ameth.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ecp_nist.o: 3rdparty/openssl/crypto/ec/ecp_nist.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)x509_def.o: 3rdparty/openssl/crypto/x509/x509_def.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)x509_r2x.o: 3rdparty/openssl/crypto/x509/x509_r2x.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)t_req.o: 3rdparty/openssl/crypto/x509/t_req.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)x509spki.o: 3rdparty/openssl/crypto/x509/x509spki.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)t_crl.o: 3rdparty/openssl/crypto/x509/t_crl.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)x_attrib.o: 3rdparty/openssl/crypto/x509/x_attrib.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)x_all.o: 3rdparty/openssl/crypto/x509/x_all.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)x509_vfy.o: 3rdparty/openssl/crypto/x509/x509_vfy.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)x509_set.o: 3rdparty/openssl/crypto/x509/x509_set.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)x509type.o: 3rdparty/openssl/crypto/x509/x509type.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)x_pubkey.o: 3rdparty/openssl/crypto/x509/x_pubkey.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)x509_trs.o: 3rdparty/openssl/crypto/x509/x509_trs.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)x509rset.o: 3rdparty/openssl/crypto/x509/x509rset.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)x509name.o: 3rdparty/openssl/crypto/x509/x509name.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)x_req.o: 3rdparty/openssl/crypto/x509/x_req.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)x509_meth.o: 3rdparty/openssl/crypto/x509/x509_meth.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)t_x509.o: 3rdparty/openssl/crypto/x509/t_x509.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)x_x509a.o: 3rdparty/openssl/crypto/x509/x_x509a.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)x509_obj.o: 3rdparty/openssl/crypto/x509/x509_obj.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)x509_ext.o: 3rdparty/openssl/crypto/x509/x509_ext.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)x509_lu.o: 3rdparty/openssl/crypto/x509/x509_lu.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)x_x509.o: 3rdparty/openssl/crypto/x509/x_x509.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)x_crl.o: 3rdparty/openssl/crypto/x509/x_crl.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)x_name.o: 3rdparty/openssl/crypto/x509/x_name.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)x509_req.o: 3rdparty/openssl/crypto/x509/x509_req.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)x509_att.o: 3rdparty/openssl/crypto/x509/x509_att.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)x509_v3.o: 3rdparty/openssl/crypto/x509/x509_v3.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)x509cset.o: 3rdparty/openssl/crypto/x509/x509cset.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)x509_err.o: 3rdparty/openssl/crypto/x509/x509_err.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)by_file.o: 3rdparty/openssl/crypto/x509/by_file.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)x_exten.o: 3rdparty/openssl/crypto/x509/x_exten.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)x509_txt.o: 3rdparty/openssl/crypto/x509/x509_txt.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)x509_cmp.o: 3rdparty/openssl/crypto/x509/x509_cmp.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)x509_vpm.o: 3rdparty/openssl/crypto/x509/x509_vpm.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)x509_d2.o: 3rdparty/openssl/crypto/x509/x509_d2.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)by_dir.o: 3rdparty/openssl/crypto/x509/by_dir.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)p12_attr.o: 3rdparty/openssl/crypto/pkcs12/p12_attr.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)p12_mutl.o: 3rdparty/openssl/crypto/pkcs12/p12_mutl.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)p12_add.o: 3rdparty/openssl/crypto/pkcs12/p12_add.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)p12_kiss.o: 3rdparty/openssl/crypto/pkcs12/p12_kiss.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)p12_asn.o: 3rdparty/openssl/crypto/pkcs12/p12_asn.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)p12_crt.o: 3rdparty/openssl/crypto/pkcs12/p12_crt.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)pk12err.o: 3rdparty/openssl/crypto/pkcs12/pk12err.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)p12_sbag.o: 3rdparty/openssl/crypto/pkcs12/p12_sbag.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)p12_init.o: 3rdparty/openssl/crypto/pkcs12/p12_init.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)p12_key.o: 3rdparty/openssl/crypto/pkcs12/p12_key.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)p12_decr.o: 3rdparty/openssl/crypto/pkcs12/p12_decr.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)p12_p8d.o: 3rdparty/openssl/crypto/pkcs12/p12_p8d.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)p12_crpt.o: 3rdparty/openssl/crypto/pkcs12/p12_crpt.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)p12_npas.o: 3rdparty/openssl/crypto/pkcs12/p12_npas.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)p12_p8e.o: 3rdparty/openssl/crypto/pkcs12/p12_p8e.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)p12_utl.o: 3rdparty/openssl/crypto/pkcs12/p12_utl.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bn_lib.o: 3rdparty/openssl/crypto/bn/bn_lib.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bn_gf2m.o: 3rdparty/openssl/crypto/bn/bn_gf2m.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bn_mpi.o: 3rdparty/openssl/crypto/bn/bn_mpi.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bn_word.o: 3rdparty/openssl/crypto/bn/bn_word.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bn_blind.o: 3rdparty/openssl/crypto/bn/bn_blind.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bn_print.o: 3rdparty/openssl/crypto/bn/bn_print.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bn_rand.o: 3rdparty/openssl/crypto/bn/bn_rand.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bn_intern.o: 3rdparty/openssl/crypto/bn/bn_intern.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bn_add.o: 3rdparty/openssl/crypto/bn/bn_add.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bn_sqrt.o: 3rdparty/openssl/crypto/bn/bn_sqrt.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bn_exp2.o: 3rdparty/openssl/crypto/bn/bn_exp2.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bn_recp.o: 3rdparty/openssl/crypto/bn/bn_recp.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bn_div.o: 3rdparty/openssl/crypto/bn/bn_div.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)rsaz_exp.o: 3rdparty/openssl/crypto/bn/rsaz_exp.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bn_sqr.o: 3rdparty/openssl/crypto/bn/bn_sqr.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bn_mont.o: 3rdparty/openssl/crypto/bn/bn_mont.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bn_kron.o: 3rdparty/openssl/crypto/bn/bn_kron.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)x86_64-gcc.o: 3rdparty/openssl/crypto/bn/asm/x86_64-gcc.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bn_nist.o: CPPFLAGS += -I3rdparty/openssl/crypto/bn
$(BUILDDIR)bn_nist.o: 3rdparty/openssl/crypto/bn/bn_nist.c 3rdparty/openssl/crypto/bn/bn_local.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bn_x931p.o: 3rdparty/openssl/crypto/bn/bn_x931p.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bn_exp.o: 3rdparty/openssl/crypto/bn/bn_exp.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bn_asm.o: 3rdparty/openssl/crypto/bn/bn_asm.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bn_prime.o: 3rdparty/openssl/crypto/bn/bn_prime.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bn_ctx.o: 3rdparty/openssl/crypto/bn/bn_ctx.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bn_gcd.o: 3rdparty/openssl/crypto/bn/bn_gcd.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bn_err.o: 3rdparty/openssl/crypto/bn/bn_err.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bn_srp.o: 3rdparty/openssl/crypto/bn/bn_srp.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bn_depr.o: 3rdparty/openssl/crypto/bn/bn_depr.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bn_dh.o: 3rdparty/openssl/crypto/bn/bn_dh.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bn_shift.o: 3rdparty/openssl/crypto/bn/bn_shift.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bn_mul.o: 3rdparty/openssl/crypto/bn/bn_mul.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bn_mod.o: 3rdparty/openssl/crypto/bn/bn_mod.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bn_const.o: 3rdparty/openssl/crypto/bn/bn_const.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)poly1305_ieee754.o: 3rdparty/openssl/crypto/poly1305/poly1305_ieee754.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)poly1305_ameth.o: 3rdparty/openssl/crypto/poly1305/poly1305_ameth.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)poly1305_pmeth.o: 3rdparty/openssl/crypto/poly1305/poly1305_pmeth.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)poly1305.o: 3rdparty/openssl/crypto/poly1305/poly1305.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)poly1305_base2_44.o: 3rdparty/openssl/crypto/poly1305/poly1305_base2_44.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)randfile.o: 3rdparty/openssl/crypto/rand/randfile.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)rand_err.o: 3rdparty/openssl/crypto/rand/rand_err.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)rand_win.o: 3rdparty/openssl/crypto/rand/rand_win.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)drbg_lib.o: 3rdparty/openssl/crypto/rand/drbg_lib.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)rand_egd.o: 3rdparty/openssl/crypto/rand/rand_egd.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)drbg_ctr.o: 3rdparty/openssl/crypto/rand/drbg_ctr.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)rand_vms.o: CPPFLAGS += -I3rdparty/openssl/crypto/rand
$(BUILDDIR)rand_vms.o: 3rdparty/openssl/crypto/rand/rand_vms.c 3rdparty/openssl/crypto/rand/rand_local.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)rand_lib.o: 3rdparty/openssl/crypto/rand/rand_lib.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)rand_unix.o: 3rdparty/openssl/crypto/rand/rand_unix.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cms_ver.o: 3rdparty/openssl/demos/cms/cms_ver.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cms_dec.o: 3rdparty/openssl/demos/cms/cms_dec.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cms_uncomp.o: 3rdparty/openssl/demos/cms/cms_uncomp.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cms_sign2.o: 3rdparty/openssl/demos/cms/cms_sign2.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cms_sign.o: 3rdparty/openssl/demos/cms/cms_sign.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cms_denc.o: 3rdparty/openssl/demos/cms/cms_denc.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cms_comp.o: 3rdparty/openssl/demos/cms/cms_comp.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cms_ddec.o: 3rdparty/openssl/demos/cms/cms_ddec.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)cms_enc.o: 3rdparty/openssl/demos/cms/cms_enc.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)aesccm.o: 3rdparty/openssl/demos/evp/aesccm.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)aesgcm.o: 3rdparty/openssl/demos/evp/aesgcm.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)smdec.o: 3rdparty/openssl/demos/smime/smdec.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)smsign.o: 3rdparty/openssl/demos/smime/smsign.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)smsign2.o: 3rdparty/openssl/demos/smime/smsign2.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)smver.o: 3rdparty/openssl/demos/smime/smver.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)smenc.o: 3rdparty/openssl/demos/smime/smenc.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)client-conf.o: 3rdparty/openssl/demos/bio/client-conf.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)saccept.o: 3rdparty/openssl/demos/bio/saccept.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)sconnect.o: 3rdparty/openssl/demos/bio/sconnect.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)server-conf.o: 3rdparty/openssl/demos/bio/server-conf.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)server-arg.o: 3rdparty/openssl/demos/bio/server-arg.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)server-cmod.o: 3rdparty/openssl/demos/bio/server-cmod.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)client-arg.o: 3rdparty/openssl/demos/bio/client-arg.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)pkwrite.o: 3rdparty/openssl/demos/pkcs12/pkwrite.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)pkread.o: 3rdparty/openssl/demos/pkcs12/pkread.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)e_afalg.o: CPPFLAGS += -I3rdparty/openssl/engines
$(BUILDDIR)e_afalg.o: 3rdparty/openssl/engines/e_afalg.c 3rdparty/openssl/engines/e_afalg.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)e_dasync.o: 3rdparty/openssl/engines/e_dasync.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)e_afalg_err.o: 3rdparty/openssl/engines/e_afalg_err.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)e_dasync_err.o: 3rdparty/openssl/engines/e_dasync_err.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)e_ossltest.o: 3rdparty/openssl/engines/e_ossltest.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)e_ossltest_err.o: 3rdparty/openssl/engines/e_ossltest_err.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)e_capi_err.o: 3rdparty/openssl/engines/e_capi_err.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)e_capi.o: 3rdparty/openssl/engines/e_capi.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)e_padlock.o: 3rdparty/openssl/engines/e_padlock.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)decompress.o: CPPFLAGS += -I3rdparty/bzip2
$(BUILDDIR)decompress.o: 3rdparty/bzip2/decompress.c 3rdparty/bzip2/bzlib_private.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)huffman.o: 3rdparty/bzip2/huffman.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)compress.o: 3rdparty/bzip2/compress.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)crctable.o: 3rdparty/bzip2/crctable.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bzlib.o: 3rdparty/bzip2/bzlib.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)randtable.o: 3rdparty/bzip2/randtable.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)blocksort.o: 3rdparty/bzip2/blocksort.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)xz_pipe_decomp.o: 3rdparty/xz/doc/examples_old/xz_pipe_decomp.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)xz_pipe_comp.o: 3rdparty/xz/doc/examples_old/xz_pipe_comp.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)02_decompress.o: 3rdparty/xz/doc/examples/02_decompress.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)01_compress_easy.o: 3rdparty/xz/doc/examples/01_compress_easy.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)04_compress_easy_mt.o: 3rdparty/xz/doc/examples/04_compress_easy_mt.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)03_compress_custom.o: 3rdparty/xz/doc/examples/03_compress_custom.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)getopt1.o: CPPFLAGS += -Ilib
$(BUILDDIR)getopt1.o: 3rdparty/xz/lib/getopt1.c ./lib/getopt.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)getopt.o: 3rdparty/xz/lib/getopt.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)scanlzma.o: 3rdparty/xz/extra/scanlzma/scanlzma.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)test_bcj_exact_size.o: CPPFLAGS += -I3rdparty/xz/tests
$(BUILDDIR)test_bcj_exact_size.o: 3rdparty/xz/tests/test_bcj_exact_size.c 3rdparty/xz/tests/tests.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)bcj_test.o: 3rdparty/xz/tests/bcj_test.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)create_compress_files.o: CPPFLAGS += -I3rdparty/xz/src/common
$(BUILDDIR)create_compress_files.o: 3rdparty/xz/tests/create_compress_files.c 3rdparty/xz/src/common/sysdefs.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)test_check.o: 3rdparty/xz/tests/test_check.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)test_index.o: 3rdparty/xz/tests/test_index.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)test_stream_flags.o: 3rdparty/xz/tests/test_stream_flags.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)test_filter_flags.o: 3rdparty/xz/tests/test_filter_flags.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)test_block_header.o: 3rdparty/xz/tests/test_block_header.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)hex2bin.o: 3rdparty/xz/debug/hex2bin.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)known_sizes.o: 3rdparty/xz/debug/known_sizes.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)sync_flush.o: 3rdparty/xz/debug/sync_flush.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)crc32.o: 3rdparty/xz/debug/crc32.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)memusage.o: 3rdparty/xz/debug/memusage.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)full_flush.o: 3rdparty/xz/debug/full_flush.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)repeat.o: 3rdparty/xz/debug/repeat.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)lzmainfo.o: 3rdparty/xz/src/lzmainfo/lzmainfo.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)xzdec.o: 3rdparty/xz/src/xzdec/xzdec.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)mytime.o: CPPFLAGS += -I3rdparty/xz/src/xz
$(BUILDDIR)mytime.o: 3rdparty/xz/src/xz/mytime.c 3rdparty/xz/src/xz/private.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)options.o: 3rdparty/xz/src/xz/options.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)coder.o: 3rdparty/xz/src/xz/coder.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)suffix.o: 3rdparty/xz/src/xz/suffix.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)hardware.o: 3rdparty/xz/src/xz/hardware.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)signals.o: 3rdparty/xz/src/xz/signals.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)args.o: 3rdparty/xz/src/xz/args.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)file_io.o: 3rdparty/xz/src/xz/file_io.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)message.o: 3rdparty/xz/src/xz/message.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)list.o: 3rdparty/xz/src/xz/list.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)main.o: 3rdparty/xz/src/xz/main.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)util.o: 3rdparty/xz/src/xz/util.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)simple_decoder.o: CPPFLAGS += -I3rdparty/xz/src/liblzma/simple
$(BUILDDIR)simple_decoder.o: 3rdparty/xz/src/liblzma/simple/simple_decoder.c 3rdparty/xz/src/liblzma/simple/simple_decoder.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)powerpc.o: 3rdparty/xz/src/liblzma/simple/powerpc.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)x86.o: 3rdparty/xz/src/liblzma/simple/x86.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)simple_encoder.o: 3rdparty/xz/src/liblzma/simple/simple_encoder.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)armthumb.o: 3rdparty/xz/src/liblzma/simple/armthumb.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)simple_coder.o: 3rdparty/xz/src/liblzma/simple/simple_coder.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)arm.o: 3rdparty/xz/src/liblzma/simple/arm.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)ia64.o: 3rdparty/xz/src/liblzma/simple/ia64.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)sparc.o: 3rdparty/xz/src/liblzma/simple/sparc.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)lzma_encoder.o: CPPFLAGS += -I3rdparty/xz/src/liblzma/lzma
$(BUILDDIR)lzma_encoder.o: 3rdparty/xz/src/liblzma/lzma/lzma_encoder.c 3rdparty/xz/src/liblzma/lzma/lzma2_encoder.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)fastpos_table.o: CPPFLAGS += -I3rdparty/xz/src/liblzma/common
$(BUILDDIR)fastpos_table.o: 3rdparty/xz/src/liblzma/lzma/fastpos_table.c 3rdparty/xz/src/liblzma/common/common.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)lzma_encoder_presets.o: 3rdparty/xz/src/liblzma/lzma/lzma_encoder_presets.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)lzma_encoder_optimum_normal.o: 3rdparty/xz/src/liblzma/lzma/lzma_encoder_optimum_normal.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)lzma_encoder_optimum_fast.o: 3rdparty/xz/src/liblzma/lzma/lzma_encoder_optimum_fast.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)fastpos_tablegen.o: 3rdparty/xz/src/liblzma/lzma/fastpos_tablegen.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)lzma2_decoder.o: 3rdparty/xz/src/liblzma/lzma/lzma2_decoder.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)lzma_decoder.o: CPPFLAGS += -I3rdparty/xz/src/liblzma/lz
$(BUILDDIR)lzma_decoder.o: 3rdparty/xz/src/liblzma/lzma/lzma_decoder.c 3rdparty/xz/src/liblzma/lz/lz_decoder.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)lzma2_encoder.o: 3rdparty/xz/src/liblzma/lzma/lzma2_encoder.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)crc64_table.o: 3rdparty/xz/src/liblzma/check/crc64_table.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)crc64_fast.o: CPPFLAGS += -I3rdparty/xz/src/liblzma/check
$(BUILDDIR)crc64_fast.o: 3rdparty/xz/src/liblzma/check/crc64_fast.c 3rdparty/xz/src/liblzma/check/check.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)crc32_table.o: 3rdparty/xz/src/liblzma/check/crc32_table.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)sha256.o: 3rdparty/xz/src/liblzma/check/sha256.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)check.o: 3rdparty/xz/src/liblzma/check/check.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)crc32_small.o: 3rdparty/xz/src/liblzma/check/crc32_small.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)crc32_tablegen.o: 3rdparty/xz/src/liblzma/check/crc32_tablegen.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)crc32_fast.o: 3rdparty/xz/src/liblzma/check/crc32_fast.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)crc64_tablegen.o: 3rdparty/xz/src/liblzma/check/crc64_tablegen.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)crc64_small.o: 3rdparty/xz/src/liblzma/check/crc64_small.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)delta_decoder.o: CPPFLAGS += -I3rdparty/xz/src/liblzma/delta
$(BUILDDIR)delta_decoder.o: 3rdparty/xz/src/liblzma/delta/delta_decoder.c 3rdparty/xz/src/liblzma/delta/delta_decoder.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)delta_encoder.o: 3rdparty/xz/src/liblzma/delta/delta_encoder.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)delta_common.o: 3rdparty/xz/src/liblzma/delta/delta_common.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)lz_decoder.o: 3rdparty/xz/src/liblzma/lz/lz_decoder.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)lz_encoder_mf.o: 3rdparty/xz/src/liblzma/lz/lz_encoder_mf.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)lz_encoder.o: 3rdparty/xz/src/liblzma/lz/lz_encoder.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)price_table.o: CPPFLAGS += -I3rdparty/xz/src/liblzma/rangecoder
$(BUILDDIR)price_table.o: 3rdparty/xz/src/liblzma/rangecoder/price_table.c 3rdparty/xz/src/liblzma/rangecoder/range_encoder.h
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)price_tablegen.o: 3rdparty/xz/src/liblzma/rangecoder/price_tablegen.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)filter_buffer_decoder.o: 3rdparty/xz/src/liblzma/common/filter_buffer_decoder.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)block_buffer_decoder.o: 3rdparty/xz/src/liblzma/common/block_buffer_decoder.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)filter_decoder.o: 3rdparty/xz/src/liblzma/common/filter_decoder.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)alone_encoder.o: 3rdparty/xz/src/liblzma/common/alone_encoder.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)stream_flags_common.o: 3rdparty/xz/src/liblzma/common/stream_flags_common.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)easy_encoder.o: 3rdparty/xz/src/liblzma/common/easy_encoder.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)index_decoder.o: 3rdparty/xz/src/liblzma/common/index_decoder.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)common.o: 3rdparty/xz/src/liblzma/common/common.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)stream_decoder.o: 3rdparty/xz/src/liblzma/common/stream_decoder.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)easy_decoder_memusage.o: 3rdparty/xz/src/liblzma/common/easy_decoder_memusage.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)stream_encoder.o: 3rdparty/xz/src/liblzma/common/stream_encoder.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)vli_encoder.o: 3rdparty/xz/src/liblzma/common/vli_encoder.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)stream_buffer_encoder.o: 3rdparty/xz/src/liblzma/common/stream_buffer_encoder.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)outqueue.o: 3rdparty/xz/src/liblzma/common/outqueue.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)block_util.o: 3rdparty/xz/src/liblzma/common/block_util.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)filter_flags_decoder.o: 3rdparty/xz/src/liblzma/common/filter_flags_decoder.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)block_encoder.o: 3rdparty/xz/src/liblzma/common/block_encoder.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)filter_common.o: 3rdparty/xz/src/liblzma/common/filter_common.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)hardware_cputhreads.o: 3rdparty/xz/src/liblzma/common/hardware_cputhreads.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)vli_decoder.o: 3rdparty/xz/src/liblzma/common/vli_decoder.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)filter_encoder.o: 3rdparty/xz/src/liblzma/common/filter_encoder.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)block_header_decoder.o: 3rdparty/xz/src/liblzma/common/block_header_decoder.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)index_hash.o: 3rdparty/xz/src/liblzma/common/index_hash.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)filter_buffer_encoder.o: 3rdparty/xz/src/liblzma/common/filter_buffer_encoder.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)alone_decoder.o: 3rdparty/xz/src/liblzma/common/alone_decoder.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)block_buffer_encoder.o: 3rdparty/xz/src/liblzma/common/block_buffer_encoder.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)stream_buffer_decoder.o: 3rdparty/xz/src/liblzma/common/stream_buffer_decoder.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)stream_flags_decoder.o: 3rdparty/xz/src/liblzma/common/stream_flags_decoder.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)easy_buffer_encoder.o: 3rdparty/xz/src/liblzma/common/easy_buffer_encoder.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)block_decoder.o: 3rdparty/xz/src/liblzma/common/block_decoder.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)vli_size.o: 3rdparty/xz/src/liblzma/common/vli_size.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)block_header_encoder.o: 3rdparty/xz/src/liblzma/common/block_header_encoder.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)easy_preset.o: 3rdparty/xz/src/liblzma/common/easy_preset.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)index_encoder.o: 3rdparty/xz/src/liblzma/common/index_encoder.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)stream_encoder_mt.o: 3rdparty/xz/src/liblzma/common/stream_encoder_mt.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)easy_encoder_memusage.o: 3rdparty/xz/src/liblzma/common/easy_encoder_memusage.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)hardware_physmem.o: 3rdparty/xz/src/liblzma/common/hardware_physmem.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)filter_flags_encoder.o: 3rdparty/xz/src/liblzma/common/filter_flags_encoder.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)auto_decoder.o: 3rdparty/xz/src/liblzma/common/auto_decoder.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)index.o: 3rdparty/xz/src/liblzma/common/index.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)stream_flags_encoder.o: 3rdparty/xz/src/liblzma/common/stream_flags_encoder.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)tuklib_mbstr_fw.o: 3rdparty/xz/src/common/tuklib_mbstr_fw.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)tuklib_exit.o: 3rdparty/xz/src/common/tuklib_exit.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)tuklib_open_stdxxx.o: 3rdparty/xz/src/common/tuklib_open_stdxxx.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)tuklib_progname.o: 3rdparty/xz/src/common/tuklib_progname.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)tuklib_physmem.o: 3rdparty/xz/src/common/tuklib_physmem.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)tuklib_mbstr_width.o: 3rdparty/xz/src/common/tuklib_mbstr_width.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
$(BUILDDIR)tuklib_cpucores.o: 3rdparty/xz/src/common/tuklib_cpucores.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<
