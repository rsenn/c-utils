TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

TARGET = ntldd

INCLUDEPATH += . $$PWD/lib

HEADERS = 

include(deployment.pri)
qtcAddDeployment()

SOURCES = lib/mmap/mmap_read.c lib/mmap/mmap_unmap.c lib/open/open_read.c lib/str/str_case_diff.c lib/str/str_case_diffn.c libntldd.c ntldd.c
DEFINES += LSEEK=lseek64
