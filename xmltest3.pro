TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

CONFIG += link_pkgconfig
PKGCONFIG += libxml-2.0

TARGET = xmltest3

INCLUDEPATH += . $$PWD/lib

SOURCES = lib/buffer/buffer_2.c lib/buffer/buffer_flush.c lib/buffer/buffer_put.c lib/buffer/buffer_putflush.c lib/buffer/buffer_putm_internal_flush.c lib/buffer/buffer_puts.c lib/buffer/buffer_stubborn.c lib/str/str_len.c xmltest3.c

include(deployment.pri)
qtcAddDeployment()

