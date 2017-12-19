TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

TARGET = compiler-wrapper

INCLUDEPATH += . $$PWD/lib

SOURCES = compiler-wrapper.c
HEADERS = lib/strlist.h lib/str.h lib/byte.h lib/fmt.h lib/buffer.h lib/dir_internal.h 

include(deployment.pri)
qtcAddDeployment()

