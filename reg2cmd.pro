TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

msvc: DEFINES += INLINE=__inline
!msvc: DEFINES += INLINE=inline
INCLUDEPATH += .

SOURCES += reg2cmd.c stralloc_init.c stralloc_catb.c stralloc_zero.c buffer_flush.c buffer_put.c buffer_puts.c buffer_getline.c buffer_putulong.c buffer_putulonglong.c scan_xlong.c scan_xlonglong.c scan_fromhex.c buffer_putc.c buffer_0.c buffer_1.c buffer_2.c

include(deployment.pri)
qtcAddDeployment()

