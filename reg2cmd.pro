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

!*msvc* {
  DEFINES += INLINE=inline

  #QMAKE_CFLAGS_WARN_ON += -Wno-sign-compare -Wno-unused-parameter
}

INCLUDEPATH += .

SOURCES = reg2cmd.c \
  lib/buffer.h \
  lib/buffer_0.c \
  lib/buffer_1.c \
  lib/buffer_2.c \
  lib/buffer_feed.c \
  lib/buffer_flush.c \
  lib/buffer_get_token.c \
  lib/buffer_getc.c \
  lib/buffer_getline.c \
  lib/buffer_put.c \
  lib/buffer_putc.c \
  lib/buffer_puts.c \
  lib/buffer_putuint64.c \
  lib/buffer_stubborn.c \
  lib/buffer_stubborn2.c \
  lib/byte.h \
  lib/byte_chr.c \
  lib/byte_copy.c \
  lib/fmt.h \
  lib/fmt_uint64.c \
  lib/scan.h \
  lib/scan_fromhex.c \
  lib/scan_xlong.c \
  lib/scan_xlonglong.c \
  lib/str.h \
  lib/str_len.c \
  lib/stralloc.h \
  lib/stralloc_catb.c \
  lib/stralloc_init.c \
  lib/stralloc_ready.c \
  lib/stralloc_readyplus.c \
  lib/stralloc_zero.c

include(deployment.pri)
qtcAddDeployment()

