TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

*msvc* {
  DEFINES += INLINE=__inline
  DEFINES += _CRT_NONSTDC_NO_DEPRECATE=1
  DEFINES += _CRT_SECURE_NO_WARNINGS=1

  QMAKE_CFLAGS_WARN_ON = -W3
}

#mingw | mingw32 | mingw64 | msvc {
win32 | winn64 {
  DEFINES += USE_READDIR=0

} else {
   DEFINES += USE_READDIR=1
}

#win32|win64|msvc|mingw32:CONFIG(release, debug|release):  LIBS += -ladvapi32 -lws2_32
msvc: LIBS += advapi32.lib ws2_32.lib
mingw32|mingw64: LIBS += -ladvapi32 -lws2_32

mingw | mingw32 | mingw64 {
  QMAKE_LFLAGS += -static-libgcc -static-libstdc++
}

!*msvc* {
  DEFINES += INLINE=inline

  #!*mingw*: DEFINES += USE_READDIR=1

gcc: QMAKE_CFLAGS_WARN_ON += -Wno-sign-compare -Wno-unused-variable -Wno-unused-parameter -Wno-unused-but-set-variable
}

INCLUDEPATH += .

HEADERS = ../../lib/buffer.h ../../lib/byte.h ../../lib/errmsg.h ../../lib/fmt.h ../../lib/mmap.h ../../lib/omf.h ../../lib/open.h ../../lib/str.h ../../lib/stralloc.h


SOURCES = ../../lib/buffer/buffer_1.c ../../lib/buffer/buffer_2.c ../../lib/buffer/buffer_flush.c ../../lib/buffer/buffer_put.c ../../lib/buffer/buffer_putc.c ../../lib/buffer/buffer_putflush.c ../../lib/buffer/buffer_putlong.c ../../lib/buffer/buffer_putm_internal.c ../../lib/buffer/buffer_putnc.c ../../lib/buffer/buffer_putnlflush.c ../../lib/buffer/buffer_putnspace.c ../../lib/buffer/buffer_putptr.c ../../lib/buffer/buffer_puts.c ../../lib/buffer/buffer_putsa.c ../../lib/buffer/buffer_putspace.c ../../lib/buffer/buffer_putulong.c ../../lib/buffer/buffer_putxlong0.c ../../lib/buffer/buffer_stubborn.c ../../lib/byte/byte_copy.c ../../lib/byte/byte_fill.c ../../lib/byte/byte_zero.c ../../lib/errmsg/errmsg_iam.c ../../lib/errmsg/errmsg_puts.c ../../lib/errmsg/errmsg_warnsys.c ../../lib/errmsg/errmsg_write.c ../../lib/fmt/fmt_long.c ../../lib/fmt/fmt_ulong.c ../../lib/fmt/fmt_xlonglong.c ../../lib/fmt/fmt_xlong.c ../../lib/mmap/mmap_read.c ../../lib/mmap/mmap_unmap.c ../../lib/omf/omf_begin.c ../../lib/omf/omf_close.c ../../lib/omf/omf_data.c ../../lib/omf/omf_end.c ../../lib/omf/omf_name.c ../../lib/omf/omf_next.c ../../lib/omf/omf_open.c ../../lib/omf/omf_record_begin.c ../../lib/omf/omf_record_end.c ../../lib/omf/omf_record_next.c ../../lib/open/open_read.c ../../lib/str/str_basename.c ../../lib/str/str_len.c ../../lib/str/str_rchr.c ../../lib/stralloc/stralloc_catc.c ../../lib/stralloc/stralloc_free.c ../../lib/stralloc/stralloc_init.c ../../lib/stralloc/stralloc_ready.c ../../lib/stralloc/stralloc_zero.c ../../lib/unix/getopt.c ../../omflist.c ../../lib/str/str_diff.c ../../lib/str/str_rchrs.c

DEFINES += HAVE_ERRNO_H=1
