TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

TARGET = xmltest4

INCLUDEPATH += . $$PWD/lib

DEFINES += LSEEK=lseek64 _LARGEFILE64_SOURCE=1

contains(CONFIG,debug) {
  DEFINES += DEBUG=1
} else {
  DEFINES += NDEBUG=1
}

include(deployment.pri)
qtcAddDeployment()

HEADERS = lib/buffer.h lib/byte.h lib/fmt.h lib/hmap.h lib/hmap_internal.h lib/mmap.h lib/open.h lib/str.h lib/stralloc.h lib/strarray.h lib/strlist.h lib/xml.h lib/iterator.h

SOURCES = xmltest4.c lib/xml/xml_textnode.c lib/xml/xml_read_tree.c lib/xml/xml_read_callback.c lib/xml/xml_path.c lib/xml/xml_newnode.c lib/xml/xml_free.c lib/stralloc/stralloc_zero.c lib/stralloc/stralloc_readyplus.c lib/stralloc/stralloc_ready.c lib/stralloc/stralloc_nul.c lib/stralloc/stralloc_init.c lib/stralloc/stralloc_cats.c lib/stralloc/stralloc_catc.c lib/stralloc/stralloc_catb.c lib/stralloc/stralloc_append.c lib/str/str_ndup.c lib/str/str_len.c lib/open/open_read.c lib/mmap/mmap_private.c lib/hmap/hmap_truncate.c lib/hmap/hmap_set_stralloc.c lib/hmap/hmap_search.c lib/hmap/hmap_init.c lib/hmap/hmap_free_data.c lib/hmap/hmap_destroy.c lib/hmap/hmap_add.c lib/fmt/fmt_ulong.c lib/byte/byte_zero.c lib/byte/byte_copy.c lib/byte/byte_chr.c lib/buffer/buffer_stubborn2.c lib/buffer/buffer_stubborn.c lib/buffer/buffer_skip_until.c lib/buffer/buffer_skipc.c lib/buffer/buffer_putulong.c lib/buffer/buffer_putsa.c lib/buffer/buffer_puts.c lib/buffer/buffer_putnlflush.c lib/buffer/buffer_putm_internal.c lib/buffer/buffer_putflush.c lib/buffer/buffer_putc.c lib/buffer/buffer_put.c lib/buffer/buffer_prefetch.c lib/buffer/buffer_peekc.c lib/buffer/buffer_peek.c lib/buffer/buffer_munmap.c lib/buffer/buffer_mmapprivate.c lib/buffer/buffer_get_token_sa.c lib/buffer/buffer_gettok_sa.c lib/buffer/buffer_getc.c lib/buffer/buffer_flush.c lib/buffer/buffer_feed.c lib/buffer/buffer_dummyread.c lib/buffer/buffer_2.c lib/buffer/buffer_1.c
