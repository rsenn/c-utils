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

SOURCES += reg2cmd.c buffer.h buffer_0.c buffer_1.c buffer_2.c buffer_feed.c buffer_flush.c buffer_get_token.c buffer_getc.c buffer_getline.c buffer_put.c buffer_putc.c buffer_puts.c buffer_putuint64.c buffer_stubborn.c buffer_stubborn2.c byte.h byte_chr.c byte_copy.c fmt.h fmt_uint64.c scan.h scan_fromhex.c scan_xlong.c scan_xlonglong.c str.h str_len.c stralloc.h stralloc_catb.c stralloc_init.c stralloc_ready.c stralloc_readyplus.c stralloc_zero.c

include(deployment.pri)
qtcAddDeployment()

