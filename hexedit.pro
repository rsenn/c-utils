TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

TARGET = hexedit

INCLUDEPATH += . $$PWD/lib

HEADERS = 

include(deployment.pri)
qtcAddDeployment()

SOURCES = hexedit.c lib/buffer/buffer_2.c lib/buffer/buffer_flush.c lib/buffer/buffer_put.c lib/buffer/buffer_putc.c lib/buffer/buffer_putflush.c lib/buffer/buffer_putnlflush.c lib/buffer/buffer_putspace.c lib/buffer/buffer_putxlong.c lib/buffer/buffer_stubborn.c lib/fmt/fmt_xlong.c lib/mmap/mmap_private.c lib/mmap/mmap_unmap.c lib/open/open_read.c
DEFINES += LSEEK=lseek64
