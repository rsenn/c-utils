TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

CONFIG += link_pkgconfig
PKGCONFIG += libxml-2.0

TARGET = plsconv

INCLUDEPATH += . $$PWD/lib

HEADERS = lib/playlist.h
SOURCES = plsconv.c \
  lib/buffer_2.c \
  lib/buffer_close.c \
  lib/buffer_mmapread.c \
  lib/buffer_put.c \
  lib/buffer_putm_internal.c \
  lib/buffer_putnlflush.c \
  lib/buffer_putulong.c \
  lib/playlist/playlist_init.c \
  lib/playlist/playlist_m3u.c \
  lib/playlist/playlist_read.c \
  lib/buffer_flush.c \
  lib/buffer_getline_sa.c \
  lib/buffer_munmap.c \
  lib/buffer_putflush.c \
  lib/buffer_puts.c \
  lib/buffer_stubborn.c \
  lib/byte_zero.c \
  lib/fmt_ulong.c \
  lib/mmap_read.c \
  lib/scan_ulong.c \
  lib/stralloc_append.c \
  lib/stralloc_copy.c \
  lib/stralloc_copys.c \
  lib/stralloc_init.c \
  lib/str_diffn.c \
  lib/buffer_get_token_sa.c \
  lib/open_read.c \
  lib/scan_ulongn.c \
  lib/stralloc_copyb.c \
  lib/stralloc_readyplus.c \
  lib/str_len.c \
  lib/buffer_feed.c \
  lib/buffer_getc.c \
  lib/buffer_stubborn2.c \
  lib/byte_chr.c \
  lib/byte_copy.c \
  lib/stralloc_ready.c

include(deployment.pri)
qtcAddDeployment()

