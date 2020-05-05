$(BUILDDIR)gzread.o: CPPFLAGS += -I3rdparty/zlib
$(BUILDDIR)gzread.o: 3rdparty/zlib/gzread.c 3rdparty/zlib/gzguts.h
  $(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<

$(BUILDDIR)e_os.h.o: CPPFLAGS += -I3rdparty/openssl/include/internal
$(BUILDDIR)e_os.h.o: 3rdparty/openssl/e_os.h 3rdparty/openssl/include/internal/nelem.h
  $(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<

$(BUILDDIR)cryptlib.h.o: CPPFLAGS += -I3rdparty/openssl/include/crypto
$(BUILDDIR)cryptlib.h.o: 3rdparty/openssl/include/crypto/cryptlib.h 3rdparty/openssl/include/crypto/cryptlib.h
  $(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<

$(BUILDDIR)cryptlib.h.o: CPPFLAGS += -I3rdparty/openssl/ms
$(BUILDDIR)cryptlib.h.o: 3rdparty/openssl/include/internal/cryptlib.h 3rdparty/openssl/ms/uplink.h
  $(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<

$(BUILDDIR)pkcs12.o: CPPFLAGS += -I3rdparty/openssl/apps
$(BUILDDIR)pkcs12.o: 3rdparty/openssl/apps/pkcs12.c 3rdparty/openssl/apps/apps.h
  $(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<

$(BUILDDIR)apps.h.o: CPPFLAGS += -I3rdparty/openssl
$(BUILDDIR)apps.h.o: 3rdparty/openssl/apps/apps.h 3rdparty/openssl/e_os.h
  $(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<

$(BUILDDIR)srptest.o: CPPFLAGS += -I3rdparty/openssl/test
$(BUILDDIR)srptest.o: 3rdparty/openssl/test/srptest.c 3rdparty/openssl/test/testutil.h
  $(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<

$(BUILDDIR)cb.o: CPPFLAGS += -I3rdparty/openssl/test/testutil
$(BUILDDIR)cb.o: 3rdparty/openssl/test/testutil/cb.c 3rdparty/openssl/test/testutil/output.h
  $(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<

$(BUILDDIR)s3_msg.o: CPPFLAGS += -I3rdparty/openssl/ssl
$(BUILDDIR)s3_msg.o: 3rdparty/openssl/ssl/s3_msg.c 3rdparty/openssl/ssl/ssl_local.h
  $(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<

$(BUILDDIR)ssl_local.h.o: CPPFLAGS += -I3rdparty/openssl/ssl/record -I3rdparty/openssl/ssl/statem
$(BUILDDIR)ssl_local.h.o: 3rdparty/openssl/ssl/ssl_local.h 3rdparty/openssl/ssl/record/record.h 3rdparty/openssl/ssl/statem/statem.h
  $(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<

$(BUILDDIR)eng_cnf.o: CPPFLAGS += -I3rdparty/openssl/crypto/engine
$(BUILDDIR)eng_cnf.o: 3rdparty/openssl/crypto/engine/eng_cnf.c 3rdparty/openssl/crypto/engine/eng_local.h
  $(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<

$(BUILDDIR)eng_local.h.o: CPPFLAGS += -I3rdparty/openssl/include/openssl
$(BUILDDIR)eng_local.h.o: 3rdparty/openssl/crypto/engine/eng_local.h 3rdparty/openssl/include/openssl/engine.h
  $(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<

$(BUILDDIR)sha1dgst.o: CPPFLAGS += -I3rdparty/openssl/crypto/sha
$(BUILDDIR)sha1dgst.o: 3rdparty/openssl/crypto/sha/sha1dgst.c 3rdparty/openssl/crypto/sha/sha_local.h
  $(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<

$(BUILDDIR)async.o: CPPFLAGS += -I3rdparty/openssl/crypto/async
$(BUILDDIR)async.o: 3rdparty/openssl/crypto/async/async.c 3rdparty/openssl/crypto/async/async_local.h
  $(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<

$(BUILDDIR)gcm128.o: CPPFLAGS += -I3rdparty/openssl/crypto
$(BUILDDIR)gcm128.o: 3rdparty/openssl/crypto/modes/gcm128.c 3rdparty/openssl/crypto/arm_arch.h
  $(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<

$(BUILDDIR)ct_x509v3.o: CPPFLAGS += -I3rdparty/openssl/crypto/ct
$(BUILDDIR)ct_x509v3.o: 3rdparty/openssl/crypto/ct/ct_x509v3.c 3rdparty/openssl/crypto/ct/ct_local.h
  $(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<

$(BUILDDIR)e_camellia.o: CPPFLAGS += -I3rdparty/openssl/crypto/modes
$(BUILDDIR)e_camellia.o: 3rdparty/openssl/crypto/evp/e_camellia.c 3rdparty/openssl/crypto/modes/modes_local.h
  $(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<

$(BUILDDIR)p5_crpt2.o: CPPFLAGS += -I3rdparty/openssl/crypto/evp
$(BUILDDIR)p5_crpt2.o: 3rdparty/openssl/crypto/evp/p5_crpt2.c 3rdparty/openssl/crypto/evp/evp_local.h
  $(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<

$(BUILDDIR)seed.o: CPPFLAGS += -I3rdparty/openssl/crypto/seed
$(BUILDDIR)seed.o: 3rdparty/openssl/crypto/seed/seed.c 3rdparty/openssl/crypto/seed/seed_local.h
  $(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<

$(BUILDDIR)dso_dl.o: CPPFLAGS += -I3rdparty/openssl/crypto/dso
$(BUILDDIR)dso_dl.o: 3rdparty/openssl/crypto/dso/dso_dl.c 3rdparty/openssl/crypto/dso/dso_local.h
  $(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<

$(BUILDDIR)dh_meth.o: CPPFLAGS += -I3rdparty/openssl/crypto/dh
$(BUILDDIR)dh_meth.o: 3rdparty/openssl/crypto/dh/dh_meth.c 3rdparty/openssl/crypto/dh/dh_local.h
  $(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<

$(BUILDDIR)wp_block.o: CPPFLAGS += -I3rdparty/openssl/crypto/whrlpool
$(BUILDDIR)wp_block.o: 3rdparty/openssl/crypto/whrlpool/wp_block.c 3rdparty/openssl/crypto/whrlpool/wp_local.h
  $(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<

$(BUILDDIR)pcbc_enc.o: CPPFLAGS += -I3rdparty/openssl/crypto/des
$(BUILDDIR)pcbc_enc.o: 3rdparty/openssl/crypto/des/pcbc_enc.c 3rdparty/openssl/crypto/des/des_local.h
  $(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<

$(BUILDDIR)ui_openssl.o: CPPFLAGS += -I3rdparty/openssl/crypto/ui
$(BUILDDIR)ui_openssl.o: 3rdparty/openssl/crypto/ui/ui_openssl.c 3rdparty/openssl/crypto/ui/ui_local.h
  $(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<

$(BUILDDIR)v3_ocsp.o: CPPFLAGS += -I3rdparty/openssl/crypto/ocsp
$(BUILDDIR)v3_ocsp.o: 3rdparty/openssl/crypto/ocsp/v3_ocsp.c 3rdparty/openssl/crypto/ocsp/ocsp_local.h
  $(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<

$(BUILDDIR)dsa_meth.o: CPPFLAGS += -I3rdparty/openssl/crypto/dsa
$(BUILDDIR)dsa_meth.o: 3rdparty/openssl/crypto/dsa/dsa_meth.c 3rdparty/openssl/crypto/dsa/dsa_local.h
  $(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<

$(BUILDDIR)bio_meth.o: CPPFLAGS += -I3rdparty/openssl/crypto/bio
$(BUILDDIR)bio_meth.o: 3rdparty/openssl/crypto/bio/bio_meth.c 3rdparty/openssl/crypto/bio/bio_local.h
  $(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<

$(BUILDDIR)m_blake2s.o: CPPFLAGS += -I3rdparty/openssl/crypto/blake2
$(BUILDDIR)m_blake2s.o: 3rdparty/openssl/crypto/blake2/m_blake2s.c 3rdparty/openssl/crypto/blake2/blake2_local.h
  $(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<

$(BUILDDIR)curve448_local.h.o: CPPFLAGS += -I3rdparty/openssl/crypto/ec/curve448
$(BUILDDIR)curve448_local.h.o: 3rdparty/openssl/crypto/ec/curve448/curve448_local.h 3rdparty/openssl/crypto/ec/curve448/curve448utils.h
  $(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<

$(BUILDDIR)field.h.o: CPPFLAGS += -I3rdparty/openssl/crypto/ec/curve448/arch_32
$(BUILDDIR)field.h.o: 3rdparty/openssl/crypto/ec/curve448/field.h 3rdparty/openssl/crypto/ec/curve448/arch_32/f_impl.h
  $(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<

$(BUILDDIR)ecp_nistputil.o: CPPFLAGS += -I3rdparty/openssl/crypto/ec
$(BUILDDIR)ecp_nistputil.o: 3rdparty/openssl/crypto/ec/ecp_nistputil.c 3rdparty/openssl/crypto/ec/ec_local.h
  $(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<

$(BUILDDIR)bn_nist.o: CPPFLAGS += -I3rdparty/openssl/crypto/bn
$(BUILDDIR)bn_nist.o: 3rdparty/openssl/crypto/bn/bn_nist.c 3rdparty/openssl/crypto/bn/bn_local.h
  $(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<

$(BUILDDIR)rand_vms.o: CPPFLAGS += -I3rdparty/openssl/crypto/rand
$(BUILDDIR)rand_vms.o: 3rdparty/openssl/crypto/rand/rand_vms.c 3rdparty/openssl/crypto/rand/rand_local.h
  $(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<

$(BUILDDIR)e_afalg.o: CPPFLAGS += -I3rdparty/openssl/engines
$(BUILDDIR)e_afalg.o: 3rdparty/openssl/engines/e_afalg.c 3rdparty/openssl/engines/e_afalg.h
  $(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<

$(BUILDDIR)decompress.o: CPPFLAGS += -I3rdparty/bzip2
$(BUILDDIR)decompress.o: 3rdparty/bzip2/decompress.c 3rdparty/bzip2/bzlib_private.h
  $(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<

$(BUILDDIR)getopt1.o: CPPFLAGS += -Ilib
$(BUILDDIR)getopt1.o: 3rdparty/xz/lib/getopt1.c ./lib/getopt.h
  $(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<

$(BUILDDIR)test_bcj_exact_size.o: CPPFLAGS += -I3rdparty/xz/tests
$(BUILDDIR)test_bcj_exact_size.o: 3rdparty/xz/tests/test_bcj_exact_size.c 3rdparty/xz/tests/tests.h
  $(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<

$(BUILDDIR)create_compress_files.o: CPPFLAGS += -I3rdparty/xz/src/common
$(BUILDDIR)create_compress_files.o: 3rdparty/xz/tests/create_compress_files.c 3rdparty/xz/src/common/sysdefs.h
  $(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<

$(BUILDDIR)mytime.o: CPPFLAGS += -I3rdparty/xz/src/xz
$(BUILDDIR)mytime.o: 3rdparty/xz/src/xz/mytime.c 3rdparty/xz/src/xz/private.h
  $(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<

$(BUILDDIR)simple_decoder.o: CPPFLAGS += -I3rdparty/xz/src/liblzma/simple
$(BUILDDIR)simple_decoder.o: 3rdparty/xz/src/liblzma/simple/simple_decoder.c 3rdparty/xz/src/liblzma/simple/simple_decoder.h
  $(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<

$(BUILDDIR)simple_coder.h.o: CPPFLAGS += -I3rdparty/xz/src/liblzma/common
$(BUILDDIR)simple_coder.h.o: 3rdparty/xz/src/liblzma/simple/simple_coder.h 3rdparty/xz/src/liblzma/common/common.h
  $(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<

$(BUILDDIR)lzma_encoder.o: CPPFLAGS += -I3rdparty/xz/src/liblzma/lzma
$(BUILDDIR)lzma_encoder.o: 3rdparty/xz/src/liblzma/lzma/lzma_encoder.c 3rdparty/xz/src/liblzma/lzma/lzma2_encoder.h
  $(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<

$(BUILDDIR)lzma_encoder_private.h.o: CPPFLAGS += -I3rdparty/xz/src/liblzma/lz
$(BUILDDIR)lzma_encoder_private.h.o: 3rdparty/xz/src/liblzma/lzma/lzma_encoder_private.h 3rdparty/xz/src/liblzma/lz/lz_encoder.h
  $(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<

$(BUILDDIR)crc64_fast.o: CPPFLAGS += -I3rdparty/xz/src/liblzma/check
$(BUILDDIR)crc64_fast.o: 3rdparty/xz/src/liblzma/check/crc64_fast.c 3rdparty/xz/src/liblzma/check/check.h
  $(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<

$(BUILDDIR)delta_encoder.h.o: CPPFLAGS += -I3rdparty/xz/src/liblzma/delta
$(BUILDDIR)delta_encoder.h.o: 3rdparty/xz/src/liblzma/delta/delta_encoder.h 3rdparty/xz/src/liblzma/delta/delta_common.h
  $(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<

$(BUILDDIR)lz_encoder_hash.h.o: CPPFLAGS += -I3rdparty/xz/src/liblzma/api/lzma
$(BUILDDIR)lz_encoder_hash.h.o: 3rdparty/xz/src/liblzma/lz/lz_encoder_hash.h 3rdparty/xz/src/liblzma/api/lzma/check.h
  $(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<

$(BUILDDIR)range_decoder.h.o: CPPFLAGS += -I3rdparty/xz/src/liblzma/rangecoder
$(BUILDDIR)range_decoder.h.o: 3rdparty/xz/src/liblzma/rangecoder/range_decoder.h 3rdparty/xz/src/liblzma/rangecoder/range_common.h
  $(CROSS_COMPILE)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFS) -c -o $@ $<

