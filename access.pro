TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

TARGET = access

INCLUDEPATH += . $$PWD/lib

HEADERS = 

include(deployment.pri)
qtcAddDeployment()


SOURCES = access.c
