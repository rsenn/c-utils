TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

TARGET = compiler-wrapper

unix: DEFINES += USE_READDIR=1

DEFINES += USE_WIDECHAR=1

INCLUDEPATH += . $$PWD/lib


HEADERS = lib/strlist.h lib/str.h lib/byte.h lib/fmt.h lib/buffer.h lib/dir_internal.h

SOURCES = compiler-wrapper.c \
lib/buffer_1.c lib/buffer_2.c lib/buffer_flush.c lib/buffer_put.c lib/buffer_putflush.c lib/buffer_putlong.c lib/buffer_putnlflush.c lib/buffer_puts.c lib/buffer_putsa.c lib/buffer_stubborn.c lib/byte_copy.c lib/byte_rchr.c lib/dir_close.c lib/dir_open.c lib/dir_read.c lib/dir_type.c lib/fmt_long.c lib/fmt_ulong.c lib/stralloc_append.c lib/stralloc_cat.c lib/stralloc_catb.c lib/stralloc_cats.c lib/stralloc_copy.c lib/stralloc_copyb.c lib/stralloc_copys.c lib/stralloc_free.c lib/stralloc_init.c lib/stralloc_ready.c lib/stralloc_readyplus.c lib/strlist_at.c lib/strlist_cat.c lib/strlist_count.c lib/strlist_index_of.c lib/strlist_push.c lib/strlist_pushm_internal.c lib/strlist_push_sa.c lib/strlist_push_unique.c lib/strlist_to_argv.c lib/strlist_unshift.c lib/str_basename.c lib/str_diff.c lib/str_diffn.c lib/str_dup.c lib/str_len.c lib/str_rchr.c


include(deployment.pri)
qtcAddDeployment()

