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

gcc:   QMAKE_CFLAGS_WARN_ON = -Wno-unused

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

HEADERS = lib/buffer.h lib/byte.h lib/fmt.h lib/http.h lib/iarray.h lib/io.h lib/ndelay.h lib/scan.h lib/socket.h lib/str.h lib/stralloc.h lib/taia.h lib/uint16.h
SOURCES = httptest.c lib/buffer/buffer_1.c lib/buffer/buffer_2.c lib/buffer/buffer_feed.c lib/buffer/buffer_flush.c lib/buffer/buffer_get.c lib/buffer/buffer_get_token.c lib/buffer/buffer_getc.c lib/buffer/buffer_getline.c lib/buffer/buffer_init.c lib/buffer/buffer_put.c lib/buffer/buffer_putflush.c lib/buffer/buffer_putlong.c lib/buffer/buffer_putnlflush.c lib/buffer/buffer_puts.c lib/buffer/buffer_putsa.c lib/buffer/buffer_putulong.c lib/buffer/buffer_stubborn.c lib/buffer/buffer_stubborn2.c lib/byte/byte_chr.c lib/byte/byte_copy.c lib/byte/byte_diff.c lib/byte/byte_equal.c lib/byte/byte_zero.c lib/fmt/fmt_long.c lib/fmt/fmt_ulong.c lib/http/http_get.c lib/http/http_init.c lib/http/http_readable.c lib/http/http_sendreq.c lib/iarray/iarray_allocate.c lib/iarray/iarray_get.c lib/iarray/iarray_init.c lib/io/io_fd.c lib/io/io_nonblock.c lib/iopause.c lib/ndelay/ndelay_on.c lib/scan/scan_fromhex.c lib/scan/scan_xlong.c lib/socket/socket_connect4.c lib/socket/socket_tcp4.c lib/socket/socket_tcp4b.c lib/str/str_chr.c lib/str/str_diffn.c lib/str/str_find.c lib/str/str_findb.c lib/str/str_len.c lib/stralloc/stralloc_append.c lib/stralloc/stralloc_catb.c lib/stralloc/stralloc_copyb.c lib/stralloc/stralloc_copys.c lib/stralloc/stralloc_init.c lib/stralloc/stralloc_ready.c lib/stralloc/stralloc_readyplus.c lib/stralloc/stralloc_zero.c lib/taia/taia_approx.c lib/taia/taia_frac.c lib/taia/taia_less.c lib/taia/taia_sub.c lib/taia/taia_uint.c lib/uint16/uint16_pack_big.c



DISTFILES += \


DEFINES += HAVE_ERRNO_H=1
