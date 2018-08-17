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

  QMAKE_CFLAGS_WARN_ON += -Wno-unused-parameter
}

INCLUDEPATH += . $$PWD/lib

HEADERS = lib/buffer.h lib/byte.h lib/fmt.h
SOURCES = count-depth.c lib/buffer/buffer_0.c lib/buffer/buffer_1.c lib/buffer/buffer_feed.c lib/buffer/buffer_flush.c lib/buffer/buffer_getc.c lib/buffer/buffer_getline.c lib/buffer/buffer_get_token.c lib/buffer/buffer_put.c lib/buffer/buffer_putulong.c lib/buffer/buffer_stubborn2.c lib/buffer/buffer_stubborn.c lib/byte/byte_chr.c lib/fmt/fmt_ulong.c



