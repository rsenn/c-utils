TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

TARGET = ntldd

INCLUDEPATH += . $$PWD/lib

HEADERS = ntldd.c
SOURCES = lib/str.h
lib/uint64.h
libntldd.h

include(deployment.pri)
qtcAddDeployment()

