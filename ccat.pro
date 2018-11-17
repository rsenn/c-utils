TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += link_pkgconfig

TARGET = ziptest

INCLUDEPATH += .

PKGCONFIG += zlib
DEFINES += HAVE_ZLIB=1

PKGCONFIG += bzlib
DEFINES += HAVE_LIBBZ2=1

PKGCONFIG += liblzma
DEFINES += HAVE_LIBLZMA=1

HEADERS = lib/buffer.h lib/byte.h lib/mmap.h lib/open.h lib/str.h
SOURCES = ccat.c lib/buffer/buffer_0.c lib/buffer/buffer_1.c lib/buffer/buffer_2.c lib/buffer/buffer_bz2.c lib/buffer/buffer_close.c lib/buffer/buffer_deflate.c lib/buffer/buffer_dummyread.c lib/buffer/buffer_dummyreadmmap.c lib/buffer/buffer_feed.c lib/buffer/buffer_flush.c lib/buffer/buffer_free.c lib/buffer/buffer_get.c lib/buffer/buffer_inflate.c lib/buffer/buffer_init.c lib/buffer/buffer_lzma.c lib/buffer/buffer_mmapprivate.c lib/buffer/buffer_munmap.c lib/buffer/buffer_prefetch.c lib/buffer/buffer_put.c lib/buffer/buffer_putc.c lib/buffer/buffer_putflush.c lib/buffer/buffer_putm_internal.c lib/buffer/buffer_putnlflush.c lib/buffer/buffer_puts.c lib/buffer/buffer_stubborn.c lib/buffer/buffer_stubborn2.c lib/buffer/buffer_truncfile.c lib/byte/byte_copy.c lib/byte/byte_zero.c lib/mmap/mmap_private.c lib/open/open_read.c lib/open/open_trunc.c lib/str/str_basename.c lib/str/str_case_diff.c lib/str/str_case_equal.c lib/str/str_diff.c lib/str/str_len.c lib/str/str_rchr.c lib/unix/getopt.c \
    lib/fmt/fmt_int64.c

