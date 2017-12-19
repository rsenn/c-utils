TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += link_pkgconfig
PKGCONFIG += libxml-2.0


TARGET = opensearch-dump

INCLUDEPATH += . $$PWD/lib

SOURCES = opensearch-dump.c
HEADERS = lib/buffer.h lib/stralloc.h 


include(deployment.pri)
qtcAddDeployment()

