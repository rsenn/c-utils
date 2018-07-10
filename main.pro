TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

TARGET = main

INCLUDEPATH += . $$PWD/lib

HEADERS = main.c
SOURCES = 

include(deployment.pri)
qtcAddDeployment()

