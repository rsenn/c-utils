TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

TARGET = jsontest

INCLUDEPATH += . $$PWD/lib

HEADERS = lib/buffer.h lib/byte.h lib/charbuf.h lib/fmt.h lib/hmap.h lib/json.h lib/open.h lib/scan.h lib/str.h lib/stralloc.h


SOURCES = jsontest.c lib/buffer/buffer_1.c lib/buffer/buffer_2.c lib/buffer/buffer_flush.c lib/buffer/buffer_put.c lib/buffer/buffer_putc.c lib/buffer/buffer_putdouble.c lib/buffer/buffer_putflush.c lib/buffer/buffer_putlonglong.c lib/buffer/buffer_putnlflush.c lib/buffer/buffer_putnspace.c lib/buffer/buffer_puts.c lib/buffer/buffer_putsa.c lib/buffer/buffer_putulong.c lib/buffer/buffer_stubborn.c lib/byte/byte_copy.c lib/byte/byte_fill.c lib/byte/byte_zero.c lib/charbuf/charbuf_close.c lib/charbuf/charbuf_init.c lib/charbuf/charbuf_nextc.c lib/charbuf/charbuf_peek.c lib/charbuf/charbuf_peekc.c lib/charbuf/charbuf_skip.c lib/charbuf/charbuf_skip_pred.c lib/fmt/fmt_double.c lib/fmt/fmt_escapecharc.c lib/fmt/fmt_longlong.c lib/fmt/fmt_ulong.c lib/fmt/fmt_ulonglong.c lib/hmap/hmap_add.c lib/hmap/hmap_init.c lib/hmap/hmap_search.c lib/json/json_free.c lib/json/json_newnode.c lib/json/json_parse.c lib/json/json_print.c lib/json/json_read_callback.c lib/json/json_read_tree.c lib/json/json_reader_init.c lib/open/open_read.c lib/scan/scan_double.c lib/scan/scan_longlong.c lib/scan/scan_ulonglong.c lib/socket/init.c lib/str/str_chr.c lib/str/str_len.c lib/stralloc/stralloc_append.c lib/stralloc/stralloc_fmt_pred.c lib/stralloc/stralloc_init.c lib/stralloc/stralloc_nul.c lib/stralloc/stralloc_ready.c lib/stralloc/stralloc_readyplus.c lib/stralloc/stralloc_zero.c
