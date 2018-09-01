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


SOURCES = lib/buffer/buffer_2.c lib/buffer/buffer_flush.c lib/buffer/buffer_put.c lib/buffer/buffer_puts.c lib/buffer/buffer_putsa.c lib/buffer/buffer_stubborn.c lib/byte/byte_copy.c lib/byte/byte_diff.c lib/byte/byte_equal.c lib/byte/byte_zero.c lib/path/path_basename.c lib/socket/init.c lib/str/str_len.c lib/str/str_rchr.c lib/stralloc/stralloc_copy.c lib/stralloc/stralloc_copyb.c lib/stralloc/stralloc_copys.c lib/stralloc/stralloc_findb.c lib/stralloc/stralloc_finds.c lib/stralloc/stralloc_init.c lib/stralloc/stralloc_nul.c lib/stralloc/stralloc_ready.c sln.c
HEADERS = lib/buffer.h lib/byte.h lib/path.h lib/str.h lib/stralloc.h
