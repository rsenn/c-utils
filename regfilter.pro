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
    lib/buffer.h \
    lib/byte.h \
    lib/fmt.h \
    lib/scan.h \
    lib/str.h \
    lib/stralloc.h \


SOURCES = \
  lib/buffer_0.c \
  lib/buffer_1.c \
  lib/buffer_2.c \
  lib/buffer_feed.c \
  lib/buffer_flush.c \
  lib/buffer_getc.c \
  lib/buffer_getline.c \
  lib/buffer_get_token.c \
  lib/buffer_put.c \
  lib/buffer_putc.c \
  lib/buffer_puts.c \
  lib/buffer_putsa.c \
  lib/buffer_putuint64.c \
  lib/buffer_stubborn.c \
  lib/buffer_stubborn2.c \
  lib/byte_chr.c \
  lib/byte_copy.c \
  lib/fmt_uint64.c \
  regfilter.c \
  lib/scan_fromhex.c \
  lib/scan_xlong.c \
  lib/scan_xlonglong.c \
  lib/stralloc_catb.c \
  lib/stralloc_copyb.c \
  lib/stralloc_copys.c \
  lib/stralloc_init.c \
  lib/stralloc_ready.c \
  lib/stralloc_readyplus.c \
  lib/stralloc_zero.c \
  lib/str_diffn.c \
  lib/str_len.c \

include(deployment.pri)
qtcAddDeployment()

