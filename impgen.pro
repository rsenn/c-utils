TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

*msvc* {
  DEFINES += INLINE=__inline
  DEFINES += _CRT_NONSTDC_NO_DEPRECATE=1
  DEFINES += _CRT_SECURE_NO_WARNINGS=1

  QMAKE_CFLAGS_WARN_ON = -W3
}

#mingw | mingw32 | mingw64 | msvc {
win32 | winn64 {
  DEFINES += USE_READDIR=0

} else {
   DEFINES += USE_READDIR=1
}

#win32|win64|msvc|mingw32:CONFIG(release, debug|release):  LIBS += -ladvapi32 -lws2_32
msvc: LIBS += advapi32.lib ws2_32.lib
mingw32|mingw64: LIBS += -ladvapi32 -lws2_32

mingw | mingw32 | mingw64 {
  QMAKE_LFLAGS += -static-libgcc -static-libstdc++
}

!*msvc* {
  DEFINES += INLINE=inline

  #!*mingw*: DEFINES += USE_READDIR=1

  #QMAKE_CFLAGS_WARN_ON += -Wno-sign-compare -Wno-unused-parameter
}

INCLUDEPATH += . $$PWD/lib

HEADERS = \
  lib/pe.h lib/byte.h lib/buffer.h lib/uint64.h lib/mmap.h lib/open.h lib/uint16.h lib/uint32.h

SOURCES = impgen.c lib/buffer/buffer_0.c lib/buffer/buffer_0small.c lib/buffer/buffer_1.c lib/buffer/buffer_1small.c lib/buffer/buffer_2.c lib/buffer/buffer_close.c lib/buffer/buffer_default.c lib/buffer/buffer_dummyread.c lib/buffer/buffer_dummyreadmmap.c lib/buffer/buffer_dump.c lib/buffer/buffer_feed.c lib/buffer/buffer_flush.c lib/buffer/buffer_free.c lib/buffer/buffer_fromarray.c lib/buffer/buffer_frombuf.c lib/buffer/buffer_fromsa.c lib/buffer/buffer_fromstr.c lib/buffer/buffer_get.c lib/buffer/buffer_getc.c lib/buffer/buffer_getline.c lib/buffer/buffer_getline_sa.c lib/buffer/buffer_getn.c lib/buffer/buffer_getnewline_sa.c lib/buffer/buffer_get_new_token_sa.c lib/buffer/buffer_get_new_token_sa_pred.c lib/buffer/buffer_get_token.c lib/buffer/buffer_get_token_pred.c lib/buffer/buffer_get_token_sa.c lib/buffer/buffer_get_token_sa_pred.c lib/buffer/buffer_get_until.c lib/buffer/buffer_init.c lib/buffer/buffer_init_free.c lib/buffer/buffer_mmapprivate.c lib/buffer/buffer_mmapread.c lib/buffer/buffer_mmapread_fd.c lib/buffer/buffer_munmap.c lib/buffer/buffer_peek.c lib/buffer/buffer_prefetch.c lib/buffer/buffer_put.c lib/buffer/buffer_put8long.c lib/buffer/buffer_putalign.c lib/buffer/buffer_putc.c lib/buffer/buffer_puterror.c lib/buffer/buffer_puterror2.c lib/buffer/buffer_putflush.c lib/buffer/buffer_putlong.c lib/buffer/buffer_putlonglong.c lib/buffer/buffer_putm_internal.c lib/buffer/buffer_putm_internal_flush.c lib/buffer/buffer_putnlflush.c lib/buffer/buffer_putnspace.c lib/buffer/buffer_puts.c lib/buffer/buffer_putsa.c lib/buffer/buffer_putsaflush.c lib/buffer/buffer_putsalign.c lib/buffer/buffer_putsflush.c lib/buffer/buffer_putspace.c lib/buffer/buffer_putuint64.c lib/buffer/buffer_putulong.c lib/buffer/buffer_putulonglong.c lib/buffer/buffer_putxlong.c lib/buffer/buffer_read_fd.c lib/buffer/buffer_seek.c lib/buffer/buffer_skip_until.c lib/buffer/buffer_stubborn.c lib/buffer/buffer_stubborn2.c lib/buffer/buffer_tosa.c lib/buffer/buffer_truncfile.c lib/byte/byte_chr.c lib/byte/byte_copy.c lib/byte/byte_copyr.c lib/byte/byte_diff.c lib/byte/byte_fill.c lib/byte/byte_rchr.c lib/byte/byte_zero.c lib/mmap/mmap_map.c lib/mmap/mmap_private.c lib/mmap/mmap_read.c lib/mmap/mmap_read_fd.c lib/mmap/mmap_unmap.c lib/open/open_append.c lib/open/open_read.c lib/open/open_rw.c lib/open/open_trunc.c lib/pe/pe_offsets.c lib/uint16/uint16_pack.c lib/uint16/uint16_pack_big.c lib/uint16/uint16_unpack.c lib/uint16/uint16_unpack_big.c lib/uint32/uint32_pack.c lib/uint32/uint32_pack_big.c lib/uint32/uint32_unpack.c lib/uint32/uint32_unpack_big.c lib/uint64/uint64_pack.c lib/uint64/uint64_pack_big.c lib/uint64/uint64_unpack.c lib/uint64/uint64_unpack_big.c

include(deployment.pri)
qtcAddDeployment()

DEFINES += io_seek=lseek64
