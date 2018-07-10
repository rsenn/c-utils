TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

TARGET = hexedit

INCLUDEPATH += . $$PWD/lib

HEADERS = hexedit.c
SOURCES = lib/buffer.h
lib/mmap.h

include(deployment.pri)
qtcAddDeployment()

