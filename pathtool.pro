TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

CONFIG += link_pkgconfig
#PKGCONFIG += libxml-2.0

TARGET = pathtool

INCLUDEPATH += . $$PWD/lib

HEADERS = lib/buffer.h lib/byte.h lib/str.h lib/stralloc.h lib/strlist.h

SOURCES = lib/buffer/buffer_1.c lib/buffer/buffer_flush.c lib/buffer/buffer_put.c lib/buffer/buffer_putflush.c lib/buffer/buffer_putnlflush.c lib/buffer/buffer_putsa.c lib/buffer/buffer_stubborn.c lib/byte/byte_copy.c lib/byte/byte_zero.c lib/str/str_dup.c lib/str/str_len.c lib/str/str_tok.c lib/stralloc/stralloc_append.c lib/stralloc/stralloc_catb.c lib/stralloc/stralloc_catc.c lib/stralloc/stralloc_copyb.c lib/stralloc/stralloc_copys.c lib/stralloc/stralloc_decamelize.c lib/stralloc/stralloc_init.c lib/stralloc/stralloc_nul.c lib/stralloc/stralloc_ready.c lib/stralloc/stralloc_readyplus.c lib/stralloc/stralloc_zero.c lib/strlist/strlist_join.c lib/strlist/strlist_push.c lib/strlist/strlist_push_tokens.c pathtool.c






