TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

CONFIG += link_pkgconfig
PKGCONFIG += libxml-2.0

TARGET = pathtool

INCLUDEPATH += . $$PWD/lib

HEADERS = \
  lib/buffer.h \
  lib/byte.h \
  lib/stralloc.h \
  lib/str.h \
  lib/strlist.h \
  lib/uint64.h

SOURCES = \
  pathtool.c \
  lib/str/str_tok.c \
  lib/str/str_len.c \
  lib/str/str_dup.c \
  lib/stralloc/stralloc_init.c \
  lib/stralloc/stralloc_ready.c \
  lib/stralloc/stralloc_nul.c \
  lib/stralloc/stralloc_catb.c \
  lib/stralloc/stralloc_zero.c \
  lib/stralloc/stralloc_readyplus.c \
  lib/byte/byte_copy.c \
  lib/buffer/buffer_putnlflush.c \
  lib/buffer/buffer_putflush.c \
  lib/buffer/buffer_1.c \
  lib/buffer/buffer_stubborn.c \
  lib/buffer/buffer_flush.c \
  lib/buffer/buffer_put.c \
  lib/buffer/buffer_putsa.c \
  lib/strlist/strlist_at.c \
  lib/strlist/strlist_cat.c \
  lib/strlist/strlist_count.c \
  lib/strlist/strlist_dump.c \
  lib/strlist/strlist_index_of.c \
  lib/strlist/strlist_join.c \
  lib/strlist/strlist_pushb.c \
  lib/strlist/strlist_push.c \
  lib/strlist/strlist_pushm_internal.c \
  lib/strlist/strlist_push_sa.c \
  lib/strlist/strlist_pushsa.c \
  lib/strlist/strlist_push_tokens.c \
  lib/strlist/strlist_push_unique.c \
  lib/strlist/strlist_range.c \
  lib/strlist/strlist_shift.c \
  lib/strlist/strlist_sort.c \
  lib/strlist/strlist_to_argv.c \
  lib/strlist/strlist_unshift.c \
  lib/buffer/buffer_puts.c \
  lib/buffer/buffer_putulong.c \
  lib/fmt/fmt_ulong.c \
  lib/stralloc/stralloc_cat.c \
  lib/stralloc/stralloc_cats.c \
  lib/stralloc/stralloc_free.c


include(deployment.pri)
qtcAddDeployment()





