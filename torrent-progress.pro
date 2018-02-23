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

INCLUDEPATH += . $$PWD/lib

HEADERS =

SOURCES = torrent-progress.c \
  lib/buffer.h \
  lib/buffer/buffer_1.c \
  lib/buffer/buffer_feed.c \
  lib/buffer/buffer_flush.c \
  lib/buffer/buffer_get.c \
  lib/buffer/buffer_put.c \
  lib/buffer/buffer_putflush.c \
  lib/buffer/buffer_putnlflush.c \
  lib/buffer/buffer_puts.c \
  lib/buffer/buffer_putulong.c \
  lib/buffer/buffer_stubborn.c \
  lib/buffer/buffer_stubborn2.c \
  lib/byte.h \
  lib/byte/byte_copy.c \
  lib/fmt.h \
  lib/fmt/fmt_ulong.c \
  lib/mmap.h \
  lib/mmap/mmap_map.c \
  lib/mmap/mmap_unmap.c \
  lib/open.h \
  lib/open/open_read.c \
  lib/str.h \
  lib/str/str_len.c

include(deployment.pri)
qtcAddDeployment()

