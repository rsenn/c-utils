TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

TARGET = ntldd

INCLUDEPATH += . $$PWD/lib

HEADERS = 

include(deployment.pri)
qtcAddDeployment()

SOURCES = libntldd.c lib/mmap/mmap_map.c lib/mmap/mmap_private.c lib/mmap/mmap_read.c lib/mmap/mmap_read_fd.c lib/mmap/mmap_unmap.c lib/open/open_append.c lib/open/open_read.c lib/open/open_rw.c lib/open/open_trunc.c lib/str/str_basename.c lib/str/str_case_diff.c lib/str/str_case_diffn.c lib/str/str_case_equal.c lib/str/str_case_start.c lib/str/str_cat.c lib/str/str_chr.c lib/str/str_copy.c lib/str/str_copyb.c lib/str/str_decamelize.c lib/str/str_diff.c lib/str/str_diffn.c lib/str/str_dup.c lib/str/str_equal.c lib/str/str_find.c lib/str/str_findb.c lib/str/str_istr.c lib/str/str_len.c lib/str/str_lower.c lib/str/str_ptime.c lib/str/str_rchr.c lib/str/str_start.c lib/str/str_tok.c ntldd.c
DEFINES += io_seek=lseek64
