TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

TARGET = bu

INCLUDEPATH += . $$PWD/lib

HEADERS = 
SOURCES = bu.c

include(deployment.pri)
qtcAddDeployment()

