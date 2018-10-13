TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

CONFIG += link_pkgconfig
#PKGCONFIG += libxml-2.0

TARGET = pathtool

INCLUDEPATH += .

HEADERS = lib/buffer.h lib/byte.h lib/errmsg.h lib/fmt.h lib/path.h lib/str.h lib/stralloc.h lib/strlist.h

SOURCES = lib/buffer/buffer_1.c lib/buffer/buffer_2.c lib/buffer/buffer_flush.c lib/buffer/buffer_put.c lib/buffer/buffer_putflush.c lib/buffer/buffer_putlong.c lib/buffer/buffer_putm_internal.c lib/buffer/buffer_putnlflush.c lib/buffer/buffer_puts.c lib/buffer/buffer_putsa.c lib/buffer/buffer_stubborn.c lib/byte/byte_chr.c lib/byte/byte_copy.c lib/byte/byte_diff.c lib/byte/byte_zero.c lib/cbmap/alloc.c lib/errmsg/errmsg_iam.c lib/errmsg/errmsg_puts.c lib/errmsg/errmsg_warnsys.c lib/errmsg/errmsg_write.c lib/fmt/fmt_long.c lib/fmt/fmt_ulong.c lib/path/path_absolute.c lib/path/path_canonicalize.c lib/path/path_getcwd.c lib/path/path_len_s.c lib/path/path_realpath.c lib/path/path_right.c lib/str/str_basename.c lib/str/str_copyn.c lib/str/str_dup.c lib/str/str_len.c lib/str/str_rchr.c lib/str/str_tok.c lib/stralloc/stralloc_catb.c lib/stralloc/stralloc_catc.c lib/stralloc/stralloc_copy.c lib/stralloc/stralloc_copyb.c lib/stralloc/stralloc_copys.c lib/stralloc/stralloc_init.c lib/stralloc/stralloc_nul.c lib/stralloc/stralloc_ready.c lib/stralloc/stralloc_readyplus.c lib/stralloc/stralloc_trunc.c lib/stralloc/stralloc_zero.c lib/strlist/strlist_at_n.c lib/strlist/strlist_count.c lib/strlist/strlist_join.c lib/strlist/strlist_push.c lib/strlist/strlist_push_tokens.c lib/strlist/strlist_pushb.c pathtool.c






