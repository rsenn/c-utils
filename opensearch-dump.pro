TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += link_pkgconfig
PKGCONFIG += libxml-2.0


TARGET = opensearch-dump

INCLUDEPATH += . $$PWD/lib

SOURCES = opensearch-dump.c \
lib/buffer_1.c lib/buffer_2.c lib/buffer_flush.c lib/buffer_put.c lib/buffer_putflush.c lib/buffer_putnlflush.c lib/buffer_puts.c lib/buffer_putsa.c lib/buffer_stubborn.c lib/byte_copy.c lib/stralloc_cat.c lib/stralloc_catb.c lib/stralloc_cats.c lib/stralloc_copy.c lib/stralloc_copyb.c lib/stralloc_copys.c lib/stralloc_init.c lib/stralloc_ready.c lib/stralloc_readyplus.c lib/stralloc_zero.c lib/str_len.c

HEADERS = lib/buffer.h lib/stralloc.h 


include(deployment.pri)
qtcAddDeployment()

