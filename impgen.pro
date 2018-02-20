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

HEADERS = \
  lib/pe.h \
  lib/byte.h \
  lib/buffer.h \
  lib/uint64.h \
  lib/mmap.h \
  lib/open.h \
  lib/uint16.h \
  lib/uint32.h

SOURCES = \
  impgen.c \
  lib/buffer_1.c \
  lib/buffer_flush.c \
  lib/buffer_put.c \
  lib/buffer_putflush.c \
  lib/buffer_putm_internal.c \
  lib/buffer_putnlflush.c \
  lib/buffer_puts.c \
  lib/buffer_putulong.c \
  lib/buffer_stubborn.c \
  lib/byte_copy.c \
  lib/fmt_ulong.c \
  lib/mmap_read.c \
  lib/mmap_unmap.c \
  lib/open_read.c \
  lib/str_len.c

include(deployment.pri)
qtcAddDeployment()

