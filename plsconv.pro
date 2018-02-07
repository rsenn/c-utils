TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

CONFIG += link_pkgconfig
PKGCONFIG += libxml-2.0

TARGET = plsconv

INCLUDEPATH += . $$PWD/lib

HEADERS = lib/playlist.h \
    lib/buffer.h
SOURCES = \
  lib/buffer_2.c \
  lib/buffer_close.c \
  lib/buffer_dummyreadmmap.c \
  lib/buffer_feed.c \
  lib/buffer_flush.c \
  lib/buffer_get.c \
  lib/buffer_getc.c \
  lib/buffer_getline_sa.c \
  lib/buffer_get_token_sa.c \
  lib/buffer_init.c \
  lib/buffer_mmapread_fd.c \
  lib/buffer_munmap.c \
  lib/buffer_put.c \
  lib/buffer_putflush.c \
  lib/buffer_putm_internal.c \
  lib/buffer_putsa.c \
  lib/buffer_puts.c \
  lib/buffer_putulong.c \
  lib/buffer_stubborn2.c \
  lib/buffer_stubborn.c \
  lib/byte_chr.c \
  lib/byte_copy.c \
  lib/byte_fill.c \
  lib/byte_zero.c \
  lib/fmt_ulong.c \
  lib/mmap_read_fd.c \
  lib/open_read.c \
  lib/open_trunc.c \
  lib/playlist/playlist_init.c \
  lib/playlist/playlist_m3u.c \
  lib/playlist/playlist_read.c \
  lib/playlist/playlist_write_entry.c \
  lib/playlist/playlist_write_finish.c \
  lib/playlist/playlist_write_start.c \
  lib/playlist/playlist_xspf.c \
  lib/scan_ulong.c \
  lib/scan_ulongn.c \
  lib/stralloc_append.c \
  lib/stralloc_catb.c \
  lib/stralloc_cats.c \
  lib/stralloc_copyb.c \
  lib/stralloc_copy.c \
  lib/stralloc_copys.c \
  lib/stralloc_init.c \
  lib/stralloc_ready.c \
  lib/stralloc_readyplus.c \
  lib/stralloc_zero.c \
  lib/str_diff.c \
  lib/str_diffn.c \
  lib/str_len.c \
  lib/str_rchr.c \
  plsconv.c \
    lib/buffer_read_fd.c \
    lib/buffer_free.c


include(deployment.pri)
qtcAddDeployment()

