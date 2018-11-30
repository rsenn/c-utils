TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

*msvc* {
  DEFINES += INLINE=__inline
  DEFINES += _CRT_NONSTDC_NO_DEPRECATE=1
  DEFINES += _CRT_SECURE_NO_WARNINGS=1

  QMAKE_CFLAGS_WARN_ON = -W3 -Wno-unused-parameter
}

!*msvc* {
  DEFINES += INLINE=inline

gcc: QMAKE_CFLAGS_WARN_ON += -Wno-sign-compare -Wno-unused-variable -Wno-unused-parameter -Wno-unused-but-set-variable
}

INCLUDEPATH += .

HEADERS = lib/buffer.h lib/byte.h lib/fmt.h lib/mmap.h lib/open.h lib/str.h

SOURCES = lib/buffer/buffer_1.c lib/buffer/buffer_2.c lib/buffer/buffer_feed.c lib/buffer/buffer_flush.c lib/buffer/buffer_get.c lib/buffer/buffer_put.c lib/buffer/buffer_putflush.c lib/buffer/buffer_putnlflush.c lib/buffer/buffer_puts.c lib/buffer/buffer_putuint64.c lib/buffer/buffer_putulong.c lib/buffer/buffer_putxint64.c lib/buffer/buffer_putxlong.c lib/buffer/buffer_stubborn.c lib/buffer/buffer_stubborn2.c lib/byte/byte_copy.c lib/fmt/fmt_uint64.c lib/fmt/fmt_ulong.c lib/fmt/fmt_xint64.c lib/fmt/fmt_xlong.c lib/mmap/mmap_map.c lib/mmap/mmap_unmap.c lib/open/open_read.c lib/str/str_len.c torrent-progress.c



DEFINES += HAVE_ERRNO_H=1
