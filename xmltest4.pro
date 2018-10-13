TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

TARGET = xmltest4

INCLUDEPATH += . $$PWD/lib

DEFINES += io_seek=lseek64 _LARGEFILE64_SOURCE=1

contains(CONFIG,debug) {
  DEFINES += DEBUG=1
} else {
  DEFINES += NDEBUG=1
}


SOURCES = 
HEADERS = 
