TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

TARGET = xmltest4

INCLUDEPATH += . $$PWD/lib

DEFINES += LSEEK=lseek64 _LARGEFILE64_SOURCE=1

HEADERS =  lib/xml.h

include(deployment.pri)
qtcAddDeployment()

SOURCES = xmltest4.c lib/xml/xml_read.c lib/stralloc/stralloc_readyplus.c lib/stralloc/stralloc_ready.c lib/stralloc/stralloc_init.c lib/stralloc/stralloc_append.c lib/str/str_len.c lib/open/open_read.c lib/mmap/mmap_private.c lib/byte/byte_copy.c lib/byte/byte_chr.c lib/buffer/buffer_stubborn2.c lib/buffer/buffer_stubborn.c lib/buffer/buffer_skip_until.c lib/buffer/buffer_putsa.c lib/buffer/buffer_putnlflush.c lib/buffer/buffer_putflush.c lib/buffer/buffer_put.c lib/buffer/buffer_munmap.c lib/buffer/buffer_mmapprivate.c lib/buffer/buffer_getc.c lib/buffer/buffer_get_token_sa.c lib/buffer/buffer_flush.c lib/buffer/buffer_feed.c lib/buffer/buffer_1.c

