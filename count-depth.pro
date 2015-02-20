TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

msvc: DEFINES += INLINE=__inline
!msvc: DEFINES += INLINE=inline
INCLUDEPATH += .

SOURCES += count-depth.c buffer_stubborn2.c buffer_feed.c buffer_getc.c byte_chr.c buffer_get_token.c buffer_stubborn.c byte_copy.c fmt_ulong.c buffer_0.c buffer_1.c buffer_flush.c buffer_getline.c buffer_put.c buffer_putulong.c


include(deployment.pri)
qtcAddDeployment()

