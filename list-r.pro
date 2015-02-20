TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

msvc: DEFINES += INLINE=__inline
!msvc: DEFINES += INLINE=inline
INCLUDEPATH += .

SOURCES += list-r.c buffer_1.c buffer_flush.c buffer_put.c buffer_putsa.c buffer_stubborn.c byte_copy.c dir_close.c dir_open.c dir_read.c dir_type.c fmt_ulong.c str_len.c stralloc_catb.c stralloc_cats.c stralloc_copyb.c stralloc_copys.c stralloc_init.c stralloc_nul.c stralloc_ready.c stralloc_readyplus.c

include(deployment.pri)
qtcAddDeployment()

