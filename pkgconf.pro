TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += link_pkgconfig

TARGET = pkgconf

INCLUDEPATH += . $$PWD/lib

PKGCONFIG += liblzma
DEFINES += HAVE_LIBLZMA=1

PKGCONFIG += zlib
DEFINES += HAVE_ZLIB=1


SOURCES = lib/buffer/buffer_1.c lib/buffer/buffer_2.c lib/buffer/buffer_dummyread.c lib/buffer/buffer_feed.c lib/buffer/buffer_flush.c lib/buffer/buffer_get_new_token_sa.c lib/buffer/buffer_get_token_sa.c lib/buffer/buffer_getc.c lib/buffer/buffer_getline_sa.c lib/buffer/buffer_mmapprivate.c lib/buffer/buffer_mmapread.c lib/buffer/buffer_munmap.c lib/buffer/buffer_peekc.c lib/buffer/buffer_prefetch.c lib/buffer/buffer_put.c lib/buffer/buffer_putflush.c lib/buffer/buffer_putm_internal.c lib/buffer/buffer_putnlflush.c lib/buffer/buffer_puts.c lib/buffer/buffer_putsa.c lib/buffer/buffer_skipc.c lib/buffer/buffer_stubborn.c lib/buffer/buffer_stubborn2.c lib/byte/byte_chr.c lib/byte/byte_copy.c lib/byte/byte_diff.c lib/byte/byte_equal.c lib/byte/byte_zero.c lib/cbmap/alloc.c lib/cbmap/cbmap_get.c lib/cbmap/cbmap_insert.c lib/cbmap/cbmap_internal_node.c lib/cbmap/cbmap_new.c lib/dir/dir_open.c lib/dir/dir_read.c lib/mmap/mmap_private.c lib/mmap/mmap_read.c lib/open/open_read.c lib/path/path_basename.c lib/path/path_readlink.c lib/socket/init.c lib/str/str_dup.c lib/str/str_len.c lib/str/str_ndup.c lib/str/str_rchr.c lib/stralloc/stralloc_append.c lib/stralloc/stralloc_catb.c lib/stralloc/stralloc_catm_internal.c lib/stralloc/stralloc_cats.c lib/stralloc/stralloc_chomp.c lib/stralloc/stralloc_copy.c lib/stralloc/stralloc_copyb.c lib/stralloc/stralloc_copys.c lib/stralloc/stralloc_endb.c lib/stralloc/stralloc_findb.c lib/stralloc/stralloc_finds.c lib/stralloc/stralloc_init.c lib/stralloc/stralloc_nul.c lib/stralloc/stralloc_ready.c lib/stralloc/stralloc_readyplus.c lib/stralloc/stralloc_trim.c lib/stralloc/stralloc_trunc.c lib/stralloc/stralloc_zero.c lib/strlist/strlist_at_sa.c lib/strlist/strlist_count.c lib/strlist/strlist_froms.c lib/strlist/strlist_push_sa.c pkgconf.c \
    lib/cbmap/cbmap_visit_all.c \
    lib/cbmap/cbmap_visit.c
HEADERS = lib/buffer.h lib/byte.h lib/cbmap.h lib/dir.h lib/mmap.h lib/open.h lib/path.h lib/str.h lib/stralloc.h lib/strlist.h
