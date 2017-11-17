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

HEADERS = \
    lib/http.h \
    lib/io.h \
    lib/io_internal.h \
    lib/ndelay.h \
    lib/socket.h \

SOURCES = httptest.c lib/buffer_1.c lib/buffer_2.c lib/buffer_feed.c lib/buffer_flush.c lib/buffer_get.c lib/buffer_getc.c lib/buffer_getline.c lib/buffer_get_token.c lib/buffer_init.c lib/buffer_put.c lib/buffer_putflush.c lib/buffer_putlong.c lib/buffer_putnlflush.c lib/buffer_putsa.c lib/buffer_puts.c lib/buffer_putulong.c lib/buffer_stubborn2.c lib/buffer_stubborn.c lib/byte_chr.c lib/byte_copy.c lib/byte_zero.c lib/fmt_long.c lib/fmt_ulong.c lib/http_get.c lib/http_init.c lib/http_readable.c lib/http_sendreq.c lib/iarray_allocate.c lib/iarray_get.c lib/iarray_init.c lib/io_fd.c lib/io_nonblock.c lib/iopause.c lib/ndelay_on.c lib/scan_ulong.c lib/scan_ulongn.c lib/socket_connect4.c lib/socket_tcp4b.c lib/socket_tcp4.c lib/stralloc_append.c lib/stralloc_catb.c lib/stralloc_copyb.c lib/stralloc_copys.c lib/stralloc_init.c lib/stralloc_ready.c lib/stralloc_readyplus.c lib/str_len.c lib/taia_approx.c lib/taia_frac.c lib/taia_less.c lib/taia_sub.c lib/taia_uint.c lib/uint16_pack_big.c


include(deployment.pri)
qtcAddDeployment()

DISTFILES += \
    mediathek-list.cbp

