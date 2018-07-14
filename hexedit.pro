TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

TARGET = hexedit

INCLUDEPATH += . $$PWD/lib

HEADERS = 

include(deployment.pri)
qtcAddDeployment()

SOURCES = hexedit.c
DEFINES += LSEEK=lseek64
