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

INCLUDEPATH += . $$PWD/lib

HEADERS =   lib/array.h lib/buffer.h lib/byte.h lib/dir.h lib/fmt.h lib/mmap.h lib/str.h lib/stralloc.h lib/fnmatch.h lib/safemult.h


SOURCES = list-r.c \
  lib/array/array_allocate.c \
  lib/array/array_catb.c \
  lib/array/array_fail.c \
  lib/array/array_length.c \
  lib/array/array_start.c \
  lib/buffer/buffer_1.c \
  lib/buffer/buffer_2.c \
  lib/buffer/buffer_flush.c \
  lib/buffer/buffer_put.c \
  lib/buffer/buffer_putc.c \
  lib/buffer/buffer_puts.c \
  lib/buffer/buffer_putsa.c \
  lib/buffer/buffer_stubborn.c \
  lib/byte/byte_copy.c \
  lib/byte/byte_zero.c \
  lib/dir/dir_close.c \
  lib/dir/dir_name.c \
  lib/dir/dir_open.c \
  lib/dir/dir_read.c \
  lib/dir/dir_type.c \
  lib/fmt/fmt_uint64.c \
  lib/fnmatch.c \
  lib/stralloc/stralloc_catb.c \
  lib/stralloc/stralloc_cats.c \
  lib/stralloc/stralloc_copyb.c \
  lib/stralloc/stralloc_copys.c \
  lib/stralloc/stralloc_init.c \
  lib/stralloc/stralloc_nul.c \
  lib/stralloc/stralloc_ready.c \
  lib/stralloc/stralloc_readyplus.c \
  lib/str/str_diffn.c \
  lib/str/str_len.c \
  lib/umult64.c \
  lib/utf8.c

include(deployment.pri)
qtcAddDeployment()

