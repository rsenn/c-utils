TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += link_pkgconfig

TARGET = buffertest

INCLUDEPATH += .

PKGCONFIG += liblzma
DEFINES += HAVE_LIBLZMA=1

PKGCONFIG += zlib
DEFINES += HAVE_LIBZ=1

PKGCONFIG += bzip2
DEFINES += HAVE_LIBBZ2=1

SOURCES = buffertest.c lib/buffer/buffer_1.c lib/buffer/buffer_2.c lib/buffer/buffer_bz2.c lib/buffer/buffer_close.c lib/buffer/buffer_deflate.c lib/buffer/buffer_dummyread.c lib/buffer/buffer_dummyreadmmap.c lib/buffer/buffer_feed.c lib/buffer/buffer_flush.c lib/buffer/buffer_free.c lib/buffer/buffer_get.c lib/buffer/buffer_get_token.c lib/buffer/buffer_getc.c lib/buffer/buffer_getline.c lib/buffer/buffer_inflate.c lib/buffer/buffer_init.c lib/buffer/buffer_mmapprivate.c lib/buffer/buffer_munmap.c lib/buffer/buffer_prefetch.c lib/buffer/buffer_put.c lib/buffer/buffer_putflush.c lib/buffer/buffer_putm_internal.c lib/buffer/buffer_putnlflush.c lib/buffer/buffer_puts.c lib/buffer/buffer_putsa.c lib/buffer/buffer_stubborn.c lib/buffer/buffer_stubborn2.c lib/buffer/buffer_truncfile.c lib/byte/byte_ccopy.c lib/byte/byte_chr.c lib/byte/byte_copy.c lib/byte/byte_zero.c lib/mmap/mmap_private.c lib/open/open_read.c lib/open/open_trunc.c lib/scan/scan_fromhex.c lib/scan/scan_nonwhitenskip.c lib/scan/scan_whitenskip.c lib/scan/scan_xlonglong.c lib/str/str_len.c lib/stralloc/mmap_filename.c lib/stralloc/stralloc_copyb.c lib/stralloc/stralloc_init.c lib/stralloc/stralloc_ready.c lib/buffer/buffer_lzma.c
HEADERS = lib/buffer.h lib/byte.h lib/mmap.h lib/open.h lib/scan.h lib/str.h lib/stralloc.h
gcc: QMAKE_CFLAGS_WARN_ON += -Wno-sign-compare -Wno-unused-variable -Wno-unused-parameter -Wno-unused-but-set-variable
