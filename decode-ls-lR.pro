TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

*msvc* {
  DEFINES += INLINE=__inline _CRT_NONSTDC_NO_DEPRECATE=1 _CRT_SECURE_NO_WARNINGS=1
  QMAKE_CFLAGS_WARN_ON = -W3
}
!*msvc*: DEFINES += INLINE=inline
INCLUDEPATH += .

SOURCES += decode-ls-lR.c buffer.h buffer_0.c buffer_1.c buffer_2.c buffer_feed.c buffer_flush.c buffer_get_token.c buffer_getc.c buffer_getline.c buffer_put.c buffer_puts.c buffer_stubborn.c buffer_stubborn2.c byte.h byte_chr.c byte_copy.c str.h str_diffn.c str_len.c stralloc.h stralloc_catb.c stralloc_copyb.c stralloc_copys.c stralloc_ready.c stralloc_readyplus.c

include(deployment.pri)
qtcAddDeployment()

