TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

CONFIG += link_pkgconfig
#PKGCONFIG += libxml-2.0


TARGET = xmltest

INCLUDEPATH += . $$PWD/lib

SOURCES = xmltest.c
HEADERS = 

include(deployment.pri)
qtcAddDeployment()

