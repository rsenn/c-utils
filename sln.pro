TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += link_slnig

TARGET = sln
QMAKE_CFLAGS_WARN_ON += -Wno-sign-compare -Wno-unused-parameter

INCLUDEPATH += . $$PWD/lib

PKGCONFIG += liblzma
DEFINES += HAVE_LIBLZMA=1

PKGCONFIG += zlib
DEFINES += HAVE_ZLIB=1


SOURCES = lib/buffer/buffer_2.c lib/buffer/buffer_close.c lib/buffer/buffer_feed.c lib/buffer/buffer_flush.c lib/buffer/buffer_get_new_token_sa.c lib/buffer/buffer_get_token_sa.c lib/buffer/buffer_getc.c lib/buffer/buffer_mmapprivate.c lib/buffer/buffer_mmapread.c lib/buffer/buffer_munmap.c lib/buffer/buffer_put.c lib/buffer/buffer_putflush.c lib/buffer/buffer_putnlflush.c lib/buffer/buffer_puts.c lib/buffer/buffer_putsa.c lib/buffer/buffer_stubborn.c lib/buffer/buffer_stubborn2.c lib/byte/byte_chr.c lib/byte/byte_copy.c lib/byte/byte_copyr.c lib/byte/byte_diff.c lib/byte/byte_equal.c lib/byte/byte_rchr.c lib/byte/byte_zero.c lib/errmsg/errmsg_iam.c lib/errmsg/errmsg_puts.c lib/errmsg/errmsg_warnsys.c lib/errmsg/errmsg_write.c lib/mmap/mmap_private.c lib/mmap/mmap_read.c lib/open/open_read.c lib/path/path_basename.c lib/socket/init.c lib/str/str_diff.c lib/str/str_diffn.c lib/str/str_len.c lib/str/str_rchr.c lib/stralloc/stralloc_append.c lib/stralloc/stralloc_catb.c lib/stralloc/stralloc_chomp.c lib/stralloc/stralloc_copy.c lib/stralloc/stralloc_copyb.c lib/stralloc/stralloc_copys.c lib/stralloc/stralloc_endb.c lib/stralloc/stralloc_findb.c lib/stralloc/stralloc_finds.c lib/stralloc/stralloc_init.c lib/stralloc/stralloc_insertb.c lib/stralloc/stralloc_nul.c lib/stralloc/stralloc_rchr.c lib/stralloc/stralloc_ready.c lib/stralloc/stralloc_readyplus.c lib/stralloc/stralloc_zero.c sln.c
HEADERS = lib/buffer.h lib/byte.h lib/errmsg.h lib/mmap.h lib/open.h lib/path.h lib/str.h lib/stralloc.h
