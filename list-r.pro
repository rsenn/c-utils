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

!*msvc* {
  DEFINES += INLINE=inline

  #QMAKE_CFLAGS_WARN_ON += -Wno-sign-compare -Wno-unused-parameter
}

INCLUDEPATH += .

SOURCES = list-r.c \
  buffer.h \
  buffer_1.c \
  buffer_flush.c \
  buffer_put.c \
  buffer_putsa.c \
  buffer_stubborn.c \
  byte.h \
  byte_copy.c \
  dir.h \
  dir_close.c \
  dir_open.c \
  dir_read.c \
  dir_type.c \
  fmt.h \
  fmt_uint64.c \
  str.h \
  str_len.c \
  stralloc.h \
  stralloc_catb.c \
  stralloc_cats.c \
  stralloc_copyb.c \
  stralloc_copys.c \
  stralloc_init.c \
  stralloc_nul.c \
  stralloc_ready.c \
  stralloc_readyplus.c

include(deployment.pri)
qtcAddDeployment()

