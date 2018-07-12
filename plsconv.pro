TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

CONFIG += link_pkgconfig
PKGCONFIG += libxml-2.0

TARGET = plsconv

INCLUDEPATH += . $$PWD/lib

DEFINES += LSEEK=lseek64

HEADERS = lib/playlist.h lib/buffer.h
SOURCES = \
  lib/buffer/buffer_2.c \
  lib/buffer/buffer_close.c \
  lib/buffer/buffer_dummyreadmmap.c \
  lib/buffer/buffer_feed.c \
  lib/buffer/buffer_flush.c \
  lib/buffer/buffer_get.c \
  lib/buffer/buffer_getc.c \
  lib/buffer/buffer_getline_sa.c \
  lib/buffer/buffer_get_token_sa.c \
  lib/buffer/buffer_init.c \
  lib/buffer/buffer_mmapread_fd.c \
  lib/buffer/buffer_munmap.c \
  lib/buffer/buffer_put.c \
  lib/buffer/buffer_putflush.c \
  lib/buffer/buffer_putm_internal.c \
  lib/buffer/buffer_putsa.c \
  lib/buffer/buffer_puts.c \
  lib/buffer/buffer_putulong.c \
  lib/buffer/buffer_stubborn2.c \
  lib/buffer/buffer_stubborn.c \
  lib/byte/byte_chr.c \
  lib/byte/byte_copy.c \
  lib/byte/byte_fill.c \
  lib/byte/byte_zero.c \
  lib/fmt/fmt_ulong.c \
  lib/mmap/mmap_read_fd.c \
  lib/open/open_read.c \
  lib/open/open_trunc.c \
  lib/playlist/playlist_init.c \
  lib/playlist/playlist_m3u.c \
  lib/playlist/playlist_read.c \
  lib/playlist/playlist_write_entry.c \
  lib/playlist/playlist_write_finish.c \
  lib/playlist/playlist_write_start.c \
  lib/playlist/playlist_xspf.c \
  lib/scan/scan_ulong.c \
  lib/scan/scan_ulongn.c \
  lib/stralloc/stralloc_append.c \
  lib/stralloc/stralloc_catb.c \
  lib/stralloc/stralloc_cats.c \
  lib/stralloc/stralloc_copyb.c \
  lib/stralloc/stralloc_copy.c \
  lib/stralloc/stralloc_copys.c \
  lib/stralloc/stralloc_init.c \
  lib/stralloc/stralloc_ready.c \
  lib/stralloc/stralloc_readyplus.c \
  lib/stralloc/stralloc_zero.c \
  lib/str/str_diff.c \
  lib/str/str_diffn.c \
  lib/str/str_len.c \
  lib/str/str_rchr.c \
  plsconv.c \
    lib/buffer/buffer_read_fd.c \
    lib/buffer/buffer_free.c \
    lib/playlist/playlist_pls.c \
    lib/buffer/buffer_1.c \
    lib/str/str_chr.c


include(deployment.pri)
qtcAddDeployment()

