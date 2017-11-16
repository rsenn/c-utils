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

  QMAKE_CFLAGS_WARN_ON += -Wno-sign-compare
}

INCLUDEPATH += .

SOURCES = torrent-progress.c \
  lib/buffer.h \
  lib/buffer_1.c \
  lib/buffer_feed.c \
  lib/buffer_flush.c \
  lib/buffer_get.c \
  lib/buffer_put.c \
  lib/buffer_putflush.c \
  lib/buffer_putnlflush.c \
  lib/buffer_puts.c \
  lib/buffer_putulong.c \
  lib/buffer_stubborn.c \
  lib/buffer_stubborn2.c \
  lib/byte.h \
  lib/byte_copy.c \
  lib/fmt.h \
  lib/fmt_ulong.c \
  lib/mmap.h \
  lib/mmap_map.c \
  lib/mmap_unmap.c \
  lib/open.h \
  lib/open_read.c \
  lib/str.h \
  lib/str_len.c

include(deployment.pri)
qtcAddDeployment()

