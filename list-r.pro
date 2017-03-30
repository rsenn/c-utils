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

mingw | mingw32 | mingw64 | msvc {
  DEFINES += USE_READDIR=0

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

HEADERS =   lib/array.h \
  lib/buffer.h \
  lib/byte.h \
  lib/dir.h \
  lib/fmt.h \
lib/mmap.h \
  lib/str.h \
  lib/stralloc.h \
  lib/fnmatch.h \
lib/safemult.h


SOURCES = list-r.c \
  lib/array_allocate.c \
  lib/array_catb.c \
  lib/array_fail.c \
  lib/array_length.c \
  lib/array_start.c \
  lib/buffer_1.c \
  lib/buffer_2.c \
  lib/buffer_flush.c \
  lib/buffer_put.c \
  lib/buffer_putc.c \
  lib/buffer_puts.c \
  lib/buffer_putsa.c \
  lib/buffer_stubborn.c \
  lib/byte_copy.c \
  lib/byte_zero.c \
  lib/dir_close.c \
  lib/dir_name.c \
  lib/dir_open.c \
  lib/dir_read.c \
  lib/dir_type.c \
  lib/fmt_uint64.c \
  lib/fnmatch.c \
  lib/stralloc_catb.c \
  lib/stralloc_cats.c \
  lib/stralloc_copyb.c \
  lib/stralloc_copys.c \
  lib/stralloc_init.c \
  lib/stralloc_nul.c \
  lib/stralloc_ready.c \
  lib/stralloc_readyplus.c \
  lib/str_diffn.c \
  lib/str_len.c \
  lib/umult64.c \
  lib/utf8.c

include(deployment.pri)
qtcAddDeployment()

