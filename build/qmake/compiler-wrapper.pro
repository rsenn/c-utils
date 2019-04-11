TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

TARGET = compiler-wrapper

unix: DEFINES += USE_READDIR=1

DEFINES += USE_WIDECHAR=1

INCLUDEPATH += .


HEADERS = ../../lib/buffer.h ../../lib/byte.h ../../lib/dir.h ../../lib/fmt.h ../../lib/str.h ../../lib/stralloc.h ../../lib/strlist.h

SOURCES = ../../compiler-wrapper.c ../../lib/buffer/buffer_1.c ../../lib/buffer/buffer_2.c ../../lib/buffer/buffer_flush.c ../../lib/buffer/buffer_put.c ../../lib/buffer/buffer_putflush.c ../../lib/buffer/buffer_putlong.c ../../lib/buffer/buffer_putnlflush.c ../../lib/buffer/buffer_puts.c ../../lib/buffer/buffer_putsa.c ../../lib/buffer/buffer_stubborn.c ../../lib/byte/byte_chr.c ../../lib/byte/byte_copy.c ../../lib/byte/byte_copyr.c ../../lib/byte/byte_diff.c ../../lib/byte/byte_equal.c ../../lib/byte/byte_rchr.c ../../lib/byte/byte_zero.c ../../lib/dir/dir_close.c ../../lib/dir/dir_open.c ../../lib/dir/dir_read.c ../../lib/dir/dir_type.c ../../lib/fmt/fmt_long.c ../../lib/fmt/fmt_ulong.c ../../lib/str/str_basename.c ../../lib/str/str_case_diffn.c ../../lib/str/str_diff.c ../../lib/str/str_diffn.c ../../lib/str/str_dup.c ../../lib/str/str_len.c ../../lib/str/str_rchr.c ../../lib/stralloc/stralloc_append.c ../../lib/stralloc/stralloc_cat.c ../../lib/stralloc/stralloc_catb.c ../../lib/stralloc/stralloc_catc.c ../../lib/stralloc/stralloc_cats.c ../../lib/stralloc/stralloc_copy.c ../../lib/stralloc/stralloc_copyb.c ../../lib/stralloc/stralloc_copys.c ../../lib/stralloc/stralloc_init.c ../../lib/stralloc/stralloc_insertb.c ../../lib/stralloc/stralloc_ready.c ../../lib/stralloc/stralloc_readyplus.c ../../lib/strlist/strlist_at.c ../../lib/strlist/strlist_cat.c ../../lib/strlist/strlist_count.c ../../lib/strlist/strlist_index_of.c ../../lib/strlist/strlist_push.c ../../lib/strlist/strlist_push_sa.c ../../lib/strlist/strlist_push_unique.c ../../lib/strlist/strlist_pushm_internal.c ../../lib/strlist/strlist_to_argv.c ../../lib/strlist/strlist_unshift.c




DEFINES += HAVE_ERRNO_H=1
gcc: QMAKE_CFLAGS_WARN_ON += -Wno-sign-compare -Wno-unused-variable -Wno-unused-parameter -Wno-unused-but-set-variable
