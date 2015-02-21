TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

*msvc* {
  DEFINES += INLINE=__inline
  QMAKE_CFLAGS_WARN_ON = -W3
}
!*msvc*: DEFINES += INLINE=inline
INCLUDEPATH += .

SOURCES += torrent-progress.c buffer.h buffer_1.c buffer_feed.c buffer_flush.c buffer_get.c buffer_put.c buffer_putflush.c buffer_putnlflush.c buffer_puts.c buffer_putulong.c buffer_stubborn.c buffer_stubborn2.c byte.h byte_copy.c fmt.h fmt_ulong.c mmap.h mmap_map.c mmap_unmap.c open.h open_read.c str.h str_len.c

include(deployment.pri)
qtcAddDeployment()

