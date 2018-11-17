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

  #QMAKE_CFLAGS_WARN_ON += -Wno-sign-compare -Wno-unused-parameter
}

INCLUDEPATH += .

HEADERS = lib/buffer.h lib/byte.h lib/errmsg.h lib/fmt.h lib/mmap.h lib/open.h lib/str.h


SOURCES = bsdiffcat.c lib/buffer/buffer_0.c lib/buffer/buffer_1.c lib/buffer/buffer_2.c lib/buffer/buffer_bz2.c lib/buffer/buffer_close.c lib/buffer/buffer_dummyread.c lib/buffer/buffer_dummyreadmmap.c lib/buffer/buffer_feed.c lib/buffer/buffer_flush.c lib/buffer/buffer_free.c lib/buffer/buffer_get.c lib/buffer/buffer_init.c lib/buffer/buffer_mmapread.c lib/buffer/buffer_mmapread_fd.c lib/buffer/buffer_munmap.c lib/buffer/buffer_prefetch.c lib/buffer/buffer_put.c lib/buffer/buffer_putc.c lib/buffer/buffer_putflush.c lib/buffer/buffer_putint64.c lib/buffer/buffer_putnc.c lib/buffer/buffer_putnlflush.c lib/buffer/buffer_putnspace.c lib/buffer/buffer_puts.c lib/buffer/buffer_putxint640.c lib/buffer/buffer_putxlong0.c lib/buffer/buffer_stubborn.c lib/buffer/buffer_stubborn2.c lib/buffer/buffer_truncfile.c lib/byte/byte_copy.c lib/byte/byte_count.c lib/byte/byte_diff.c lib/byte/byte_fill.c lib/byte/byte_zero.c lib/errmsg/errmsg_iam.c lib/errmsg/errmsg_infosys.c lib/errmsg/errmsg_puts.c lib/errmsg/errmsg_write.c lib/fmt/fmt_int64.c lib/fmt/fmt_uint64.c lib/fmt/fmt_xint64.c lib/fmt/fmt_xlong.c lib/mmap/mmap_read.c lib/mmap/mmap_read_fd.c lib/open/open_read.c lib/open/open_trunc.c lib/str/str_basename.c lib/str/str_len.c lib/str/str_rchr.c

DEFINES += HAVE_ERRNO_H=1
