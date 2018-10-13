TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

CONFIG += link_pkgconfig
#PKGCONFIG += libxml-2.0

TARGET = plsconv

INCLUDEPATH += . $$PWD/lib

DEFINES += io_seek=lseek64

HEADERS = 
SOURCES = 



