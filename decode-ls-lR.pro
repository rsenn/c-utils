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

HEADERS = lib/buffer.h lib/byte.h lib/str.h lib/stralloc.h

SOURCES = decode-ls-lR.c lib/buffer/buffer_0.c lib/buffer/buffer_1.c lib/buffer/buffer_2.c lib/buffer/buffer_close.c lib/buffer/buffer_feed.c lib/buffer/buffer_flush.c lib/buffer/buffer_get_token.c lib/buffer/buffer_getc.c lib/buffer/buffer_getline.c lib/buffer/buffer_put.c lib/buffer/buffer_puts.c lib/buffer/buffer_stubborn.c lib/buffer/buffer_stubborn2.c lib/byte/byte_chr.c lib/byte/byte_copy.c lib/byte/byte_zero.c lib/socket/init.c lib/str/str_diffn.c lib/str/str_len.c lib/stralloc/stralloc_catb.c lib/stralloc/stralloc_copyb.c lib/stralloc/stralloc_copys.c lib/stralloc/stralloc_ready.c lib/stralloc/stralloc_readyplus.c
HEADERS = lib/buffer.h lib/byte.h lib/str.h lib/stralloc.h

