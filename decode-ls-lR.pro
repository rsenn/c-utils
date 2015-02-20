TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

msvc: DEFINES += INLINE=__inline
!msvc: DEFINES += INLINE=inline
INCLUDEPATH += .

SOURCES += decode-ls-lR.c buffer_0.c buffer_1.c buffer_2.c buffer_feed.c buffer_flush.c buffer_get_token.c buffer_getc.c buffer_getline.c buffer_put.c buffer_puts.c buffer_stubborn.c buffer_stubborn2.c byte_chr.c byte_copy.c str_diffn.c str_len.c stralloc_catb.c stralloc_copyb.c stralloc_copys.c stralloc_ready.c stralloc_readyplus.c

include(deployment.pri)
qtcAddDeployment()

