TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

TARGET = hexedit

INCLUDEPATH += . $$PWD/lib

HEADERS = lib/buffer.h lib/fmt.h lib/mmap.h lib/open.h lib/scan.h lib/str.h


SOURCES = hexedit.c lib/buffer/buffer_2.c lib/buffer/buffer_flush.c lib/buffer/buffer_put.c lib/buffer/buffer_putc.c lib/buffer/buffer_putflush.c lib/buffer/buffer_putm_internal.c lib/buffer/buffer_putnlflush.c lib/buffer/buffer_puts.c lib/buffer/buffer_putspace.c lib/buffer/buffer_putxlong.c lib/buffer/buffer_stubborn.c lib/fmt/fmt_xlong.c lib/mmap/mmap_private.c lib/mmap/mmap_unmap.c lib/open/open_read.c lib/scan/scan_fromhex.c lib/scan/scan_xlonglong.c lib/socket/init.c lib/str/str_len.c
DEFINES += io_seek=lseek64
