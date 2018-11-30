TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

CONFIG += link_pkgconfig
#PKGCONFIG += libxml-2.0

TARGET = plsconv

INCLUDEPATH += .

DEFINES += io_seek=lseek64

HEADERS = lib/buffer.h lib/byte.h lib/fmt.h lib/hmap.h lib/open.h lib/playlist.h lib/scan.h lib/str.h lib/stralloc.h lib/xml.h
SOURCES = lib/buffer/buffer_1.c lib/buffer/buffer_2.c lib/buffer/buffer_close.c lib/buffer/buffer_dummyread.c lib/buffer/buffer_dummyreadmmap.c lib/buffer/buffer_feed.c lib/buffer/buffer_flush.c lib/buffer/buffer_free.c lib/buffer/buffer_getc.c lib/buffer/buffer_init.c lib/buffer/buffer_munmap.c lib/buffer/buffer_peek.c lib/buffer/buffer_peekc.c lib/buffer/buffer_prefetch.c lib/buffer/buffer_put.c lib/buffer/buffer_putc.c lib/buffer/buffer_putflush.c lib/buffer/buffer_putm_internal.c lib/buffer/buffer_putnlflush.c lib/buffer/buffer_puts.c lib/buffer/buffer_putsa.c lib/buffer/buffer_putulong.c lib/buffer/buffer_read_fd.c lib/buffer/buffer_skip_pred.c lib/buffer/buffer_skip_until.c lib/buffer/buffer_skipc.c lib/buffer/buffer_skipspace.c lib/buffer/buffer_stubborn.c lib/buffer/buffer_stubborn2.c lib/byte/byte_chr.c lib/byte/byte_copy.c lib/byte/byte_diff.c lib/byte/byte_fill.c lib/byte/byte_zero.c lib/fmt/fmt_ulong.c lib/hmap/hmap_add.c lib/hmap/hmap_cmp.c lib/hmap/hmap_destroy.c lib/hmap/hmap_free_data.c lib/hmap/hmap_hash.c lib/hmap/hmap_init.c lib/hmap/hmap_search.c lib/hmap/hmap_set.c lib/hmap/hmap_truncate.c lib/open/open_read.c lib/open/open_trunc.c lib/playlist/playlist_init.c lib/playlist/playlist_m3u.c lib/playlist/playlist_pls.c lib/playlist/playlist_read.c lib/playlist/playlist_write_entry.c lib/playlist/playlist_write_finish.c lib/playlist/playlist_write_start.c lib/playlist/playlist_xspf.c lib/scan/scan_uint.c lib/scan/scan_ulong.c lib/scan/scan_ulongn.c lib/str/str_basename.c lib/str/str_chr.c lib/str/str_diff.c lib/str/str_diffn.c lib/str/str_equal.c lib/str/str_len.c lib/str/str_rchr.c lib/stralloc/buffer_get_token_sa.c lib/stralloc/buffer_getline_sa.c lib/stralloc/buffer_gettok_sa.c lib/stralloc/stralloc_append.c lib/stralloc/stralloc_copy.c lib/stralloc/stralloc_copyb.c lib/stralloc/stralloc_copys.c lib/stralloc/stralloc_init.c lib/stralloc/stralloc_nul.c lib/stralloc/stralloc_ready.c lib/stralloc/stralloc_readyplus.c lib/stralloc/stralloc_zero.c lib/unix/getopt.c lib/xml/xml_newnode.c lib/xml/xml_read_callback.c lib/xml/xml_reader_init.c plsconv.c



gcc: QMAKE_CFLAGS_WARN_ON += -Wno-sign-compare -Wno-unused-variable -Wno-unused-parameter -Wno-unused-but-set-variable
