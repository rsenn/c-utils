TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

TARGET = ntldd

INCLUDEPATH += . $$PWD/lib

HEADERS = lib/byte.h lib/mmap.h lib/open.h lib/str.h


SOURCES = lib/byte/byte_copy.c lib/byte/byte_zero.c lib/mmap/mmap_read.c lib/mmap/mmap_unmap.c lib/open/open_read.c lib/socket/init.c lib/str/str_basename.c lib/str/str_case_diff.c lib/str/str_case_diffn.c lib/str/str_copy.c lib/str/str_diff.c lib/str/str_dup.c lib/str/str_len.c lib/str/str_rchr.c libntldd.c ntldd.c
DEFINES += io_seek=lseek64
