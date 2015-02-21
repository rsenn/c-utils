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

SOURCES = count-depth.c \
  buffer.h \
  buffer_0.c \
  buffer_1.c \
  buffer_feed.c \
  buffer_flush.c \
  buffer_get_token.c \
  buffer_getc.c \
  buffer_getline.c \
  buffer_put.c \
  buffer_putulong.c \
  buffer_stubborn.c \
  buffer_stubborn2.c \
  byte.h \
  byte_chr.c \
  byte_copy.c \
  fmt.h \
  fmt_ulong.c


include(deployment.pri)
qtcAddDeployment()

