TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

TARGET = compiler-wrapper

unix: DEFINES += USE_READDIR=1

DEFINES += USE_WIDECHAR=1

INCLUDEPATH += . $$PWD/lib


HEADERS = 

SOURCES = 




DEFINES += HAVE_ERRNO_H=1
