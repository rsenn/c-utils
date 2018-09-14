TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

TARGET = elfwrsec

INCLUDEPATH += . $$PWD/lib

HEADERS = lib/buffer.h lib/byte.h lib/fmt.h lib/mmap.h lib/open.h lib/str.h
SOURCES = elfwrsec.c lib/buffer/buffer_1.c lib/buffer/buffer_flush.c lib/buffer/buffer_put.c lib/buffer/buffer_putflush.c lib/buffer/buffer_putlong.c lib/buffer/buffer_putnc.c lib/buffer/buffer_putnlflush.c lib/buffer/buffer_putnspace.c lib/buffer/buffer_puts.c lib/buffer/buffer_putspad.c lib/buffer/buffer_putulong.c lib/buffer/buffer_putxlong0.c lib/buffer/buffer_stubborn.c lib/byte/byte_copy.c lib/byte/byte_fill.c lib/fmt/fmt_long.c lib/fmt/fmt_ulong.c lib/fmt/fmt_xlong.c lib/mmap/mmap_shared.c lib/mmap/mmap_unmap.c lib/open/open_rw.c lib/socket/init.c lib/str/str_diff.c lib/str/str_len.c
