TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += link_pkgconfig
PKGCONFIG += libxml-2.0


TARGET = opensearch-dump

INCLUDEPATH += . $$PWD/lib

SOURCES = opensearch-dump.c \
lib/buffer/buffer_1.c lib/buffer/buffer_2.c lib/buffer/buffer_flush.c lib/buffer/buffer_put.c lib/buffer/buffer_putflush.c lib/buffer/buffer_putnlflush.c lib/buffer/buffer_puts.c lib/buffer/buffer_putsa.c lib/buffer/buffer_stubborn.c lib/byte/byte_copy.c lib/stralloc/stralloc_cat.c lib/stralloc/stralloc_catb.c lib/stralloc/stralloc_cats.c lib/stralloc/stralloc_copy.c lib/stralloc/stralloc_copyb.c lib/stralloc/stralloc_copys.c lib/stralloc/stralloc_init.c lib/stralloc/stralloc_ready.c lib/stralloc/stralloc_readyplus.c lib/stralloc/stralloc_zero.c lib/str/str_len.c

HEADERS = lib/buffer.h lib/stralloc.h 


include(deployment.pri)
qtcAddDeployment()

