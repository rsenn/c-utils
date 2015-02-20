TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

msvc: DEFINES += INLINE=__inline
!msvc: DEFINES += INLINE=inline
INCLUDEPATH += .

SOURCES += torrent-progress.c buffer_1.c buffer_feed.c buffer_flush.c buffer_get.c buffer_put.c buffer_putflush.c buffer_putnlflush.c buffer_puts.c buffer_putulong.c buffer_stubborn.c buffer_stubborn2.c byte_copy.c fmt_ulong.c mmap_map.c mmap_unmap.c open_read.c str_len.c

include(deployment.pri)
qtcAddDeployment()

