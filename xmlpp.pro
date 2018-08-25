TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

CONFIG += link_pkgconfig
#PKGCONFIG += libxml-2.0


TARGET = xmlpp

INCLUDEPATH += . $$PWD/lib

SOURCES = lib/buffer/buffer_1.c lib/buffer/buffer_close.c lib/buffer/buffer_dummyread.c lib/buffer/buffer_feed.c lib/buffer/buffer_flush.c lib/buffer/buffer_free.c lib/buffer/buffer_gettok_sa.c lib/buffer/buffer_mmapprivate.c lib/buffer/buffer_mmapread_fd.c lib/buffer/buffer_munmap.c lib/buffer/buffer_peek.c lib/buffer/buffer_peekc.c lib/buffer/buffer_prefetch.c lib/buffer/buffer_put.c lib/buffer/buffer_putc.c lib/buffer/buffer_putdouble.c lib/buffer/buffer_putflush.c lib/buffer/buffer_putlong.c lib/buffer/buffer_putlonglong.c lib/buffer/buffer_putm_internal.c lib/buffer/buffer_putnlflush.c lib/buffer/buffer_putnspace.c lib/buffer/buffer_putptr.c lib/buffer/buffer_puts.c lib/buffer/buffer_putsa.c lib/buffer/buffer_putspace.c lib/buffer/buffer_putulong.c lib/buffer/buffer_putulonglong.c lib/buffer/buffer_read_fd.c lib/buffer/buffer_skip_pred.c lib/buffer/buffer_skip_until.c lib/buffer/buffer_skipc.c lib/buffer/buffer_skipspace.c lib/buffer/buffer_stubborn.c lib/buffer/buffer_stubborn2.c lib/byte/byte_chr.c lib/byte/byte_copy.c lib/byte/byte_fill.c lib/byte/byte_zero.c lib/fmt/fmt_double.c lib/fmt/fmt_escapecharc.c lib/fmt/fmt_long.c lib/fmt/fmt_longlong.c lib/fmt/fmt_ulong.c lib/fmt/fmt_ulonglong.c lib/fmt/fmt_xlonglong.c lib/hmap/hmap_add.c lib/hmap/hmap_destroy.c lib/hmap/hmap_free_data.c lib/hmap/hmap_init.c lib/hmap/hmap_search.c lib/hmap/hmap_set.c lib/hmap/hmap_size.c lib/hmap/hmap_truncate.c lib/mmap/mmap_private.c lib/mmap/mmap_read_fd.c lib/open/open_read.c lib/str/str_chr.c lib/str/str_len.c lib/stralloc/stralloc_append.c lib/stralloc/stralloc_copy.c lib/stralloc/stralloc_copyb.c lib/stralloc/stralloc_diff.c lib/stralloc/stralloc_fmt_pred.c lib/stralloc/stralloc_init.c lib/stralloc/stralloc_nul.c lib/stralloc/stralloc_ready.c lib/stralloc/stralloc_readyplus.c lib/stralloc/stralloc_zero.c lib/xml/xml_newnode.c lib/xml/xml_print_attributes.c lib/xml/xml_read_callback.c lib/xml/xml_reader_init.c xmlpp.c
HEADERS = lib/buffer.h lib/byte.h lib/fmt.h lib/hmap.h lib/mmap.h lib/open.h lib/str.h lib/stralloc.h lib/xml.h


