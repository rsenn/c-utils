TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

TARGET = elfwrsec.c

INCLUDEPATH += . $$PWD/lib

HEADERS = 
SOURCES = elfwrsec.c.c

include(deployment.pri)
qtcAddDeployment()

