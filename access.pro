TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

TARGET = access

INCLUDEPATH += . $$PWD/lib

HEADERS = access.c
SOURCES = 

include(deployment.pri)
qtcAddDeployment()

