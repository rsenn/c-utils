TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += link_pkgconfig

TARGET = ziptest

INCLUDEPATH += .

PKGCONFIG += zlib
DEFINES += HAVE_LIBZ=1

PKGCONFIG += bzlib
DEFINES += HAVE_LIBBZ2=1

PKGCONFIG += liblzma
DEFINES += HAVE_LIBLZMA=1

HEADERS = lib/buffer.h lib/byte.h lib/mmap.h lib/open.h lib/str.h
SOURCES = lib/buffer/buffer_0.c lib/buffer/buffer_1.c lib/buffer/buffer_2.c lib/buffer/buffer_bzip.c lib/buffer/buffer_close.c lib/buffer/buffer_dummyreadmmap.c lib/buffer/buffer_feed.c lib/buffer/buffer_flush.c lib/buffer/buffer_free.c lib/buffer/buffer_get.c lib/buffer/buffer_gzip.c lib/buffer/buffer_mmapprivate.c lib/buffer/buffer_munmap.c lib/buffer/buffer_put.c lib/buffer/buffer_putflush.c lib/buffer/buffer_putm_internal.c lib/buffer/buffer_putnlflush.c lib/buffer/buffer_puts.c lib/buffer/buffer_stubborn.c lib/buffer/buffer_stubborn2.c lib/buffer/buffer_truncfile.c lib/byte/byte_copy.c lib/mmap/mmap_private.c lib/open/open_read.c lib/open/open_trunc.c lib/str/str_basename.c lib/str/str_case_diff.c lib/str/str_case_equal.c lib/str/str_diff.c lib/str/str_len.c lib/str/str_rchr.c lib/unix/getopt.c ziptest.c \
    lib/buffer/buffer_putc.c
gcc: QMAKE_CFLAGS_WARN_ON += -Wno-sign-compare -Wno-unused-variable -Wno-unused-parameter -Wno-unused-but-set-variable
