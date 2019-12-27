TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += link_pkgconfig

TARGET = hexedit

INCLUDEPATH += .

HEADERS = ../../lib/array.h ../../lib/buffer.h ../../lib/byte.h ../../lib/errmsg.h ../../lib/fmt.h ../../lib/iarray.h ../../lib/io.h ../../lib/mmap.h ../../lib/open.h ../../lib/path.h ../../lib/scan.h ../../lib/str.h ../../lib/stralloc.h


SOURCES = ../../hexedit.c ../../lib/array/array_allocate.c ../../lib/array/array_get.c ../../lib/array/array_length.c ../../lib/buffer/buffer_2.c ../../lib/buffer/buffer_close.c ../../lib/buffer/buffer_dummyreadmmap.c ../../lib/buffer/buffer_flush.c ../../lib/buffer/buffer_mmapread_fd.c ../../lib/buffer/buffer_mmapshared_fd.c ../../lib/buffer/buffer_munmap.c ../../lib/buffer/buffer_put.c ../../lib/buffer/buffer_putc.c ../../lib/buffer/buffer_putflush.c ../../lib/buffer/buffer_putm_internal.c ../../lib/buffer/buffer_putnlflush.c ../../lib/buffer/buffer_puts.c ../../lib/buffer/buffer_putspace.c ../../lib/buffer/buffer_putxlong.c ../../lib/buffer/buffer_stubborn.c ../../lib/byte/byte_copy.c ../../lib/byte/byte_zero.c ../../lib/errmsg/errmsg_iam.c ../../lib/errmsg/errmsg_puts.c ../../lib/errmsg/errmsg_warnsys.c ../../lib/errmsg/errmsg_write.c ../../lib/fmt/fmt_ulong.c ../../lib/fmt/fmt_ulong0.c ../../lib/fmt/fmt_xlong.c ../../lib/iarray/iarray_allocate.c ../../lib/iarray/iarray_get.c ../../lib/iarray/iarray_init.c ../../lib/io/io_fd.c ../../lib/io/io_sendfile.c ../../lib/mmap/mmap_read.c ../../lib/mmap/mmap_read_fd.c ../../lib/mmap/mmap_shared_fd.c ../../lib/mmap/mmap_unmap.c ../../lib/open/open_read.c ../../lib/open/open_rw.c ../../lib/path/path_readlink.c ../../lib/scan/scan_fromhex.c ../../lib/scan/scan_xlonglong.c ../../lib/str/str_basename.c ../../lib/str/str_len.c ../../lib/str/str_rchr.c ../../lib/stralloc/stralloc_catb.c ../../lib/stralloc/stralloc_cats.c ../../lib/stralloc/stralloc_catulong.c ../../lib/stralloc/stralloc_catulong0.c ../../lib/stralloc/stralloc_copy.c ../../lib/stralloc/stralloc_copyb.c ../../lib/stralloc/stralloc_init.c ../../lib/stralloc/stralloc_nul.c ../../lib/stralloc/stralloc_ready.c ../../lib/stralloc/stralloc_readyplus.c ../../lib/stralloc/stralloc_trunc.c ../../lib/stralloc/stralloc_zero.c ../../lib/io/io_mmapwritefile.c ../../lib/io/io_eagain.c

DEFINES += io_seek=lseek

gcc {
  LIBS += -lz
} else {
  LIBS += zlib.lib
}

#PKGCONFIG += zlib
DEFINES += HAVE_ZLIB=1



gcc: QMAKE_CFLAGS_WARN_ON += -Wno-sign-compare -Wno-unused-variable -Wno-unused-parameter -Wno-unused-but-set-variable
