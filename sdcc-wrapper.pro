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
    lib/buffer.h \
    lib/byte.h \
    lib/dir.h \
    lib/fmt.h \
    lib/str.h \
    lib/stralloc.h \
    lib/strlist.h \


SOURCES = \
  lib/buffer_1.c \
  lib/buffer_2.c \
  lib/buffer_flush.c \
  lib/buffer_put.c \
  lib/buffer_putflush.c \
  lib/buffer_putlong.c \
  lib/buffer_putnlflush.c \
  lib/buffer_puts.c \
  lib/buffer_putsa.c \
  lib/buffer_stubborn.c \
  lib/byte_copy.c \
  lib/byte_rchr.c \
  compiler-wrapper.c \
  lib/dir_close.c \
  lib/dir_open.c \
  lib/dir_read.c \
  lib/dir_type.c \
  lib/fmt_long.c \
  lib/fmt_ulong.c \
  lib/stralloc_append.c \
  lib/stralloc_cat.c \
  lib/stralloc_catb.c \
  lib/stralloc_cats.c \
  lib/stralloc_copy.c \
  lib/stralloc_copyb.c \
  lib/stralloc_copys.c \
  lib/stralloc_free.c \
  lib/stralloc_init.c \
  lib/stralloc_ready.c \
  lib/stralloc_readyplus.c \
  lib/strlist_at.c \
  lib/strlist_cat.c \
  lib/strlist_count.c \
  lib/strlist_index_of.c \
  lib/strlist_push.c \
  lib/strlist_pushm_internal.c \
  lib/strlist_push_sa.c \
  lib/strlist_push_unique.c \
  lib/strlist_to_argv.c \
  lib/strlist_unshift.c \
  lib/str_basename.c \
  lib/str_diff.c \
  lib/str_diffn.c \
  lib/str_dup.c \
  lib/str_len.c \
  lib/str_rchr.c \

include(deployment.pri)
qtcAddDeployment()

