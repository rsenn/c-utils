TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += link_pkgconfig

TARGET = ziptest

INCLUDEPATH += . $$PWD/lib

PKGCONFIG += zlib
DEFINES += HAVE_ZLIB=1

PKGCONFIG += bzlib
DEFINES += HAVE_LIBBZ2=1

PKGCONFIG += liblzma
DEFINES += HAVE_LIBLZMA=1

HEADERS = 
SOURCES = 
