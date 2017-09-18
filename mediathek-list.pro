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
	lib/fmt.h \
	lib/scan.h \
	lib/stralloc.h \
	lib/str.h \
	lib/strlist.h \
	lib/uint32.h \
	lib/uint64.h


SOURCES = \
	lib/buffer_1.c \
	lib/buffer_feed.c \
	lib/buffer_flush.c \
	lib/buffer_get.c \
	lib/buffer_getc.c \
	lib/buffer_get_token.c \
	lib/buffer_put.c \
	lib/buffer_putflush.c \
	lib/buffer_putm_internal.c \
	lib/buffer_putnlflush.c \
	lib/buffer_puts.c \
	lib/buffer_putulong.c \
	lib/buffer_stubborn2.c \
	lib/buffer_stubborn.c \
	lib/byte_chr.c \
	lib/byte_copy.c \
	lib/fmt_ulong.c \
	lib/fmt_ulonglong.c \
	lib/isleap.c \
	lib/scan_uint.c \
	lib/scan_ushort.c \
	lib/stralloc_append.c \
	lib/stralloc_catb.c \
	lib/stralloc_cats.c \
	lib/stralloc_copyb.c \
	lib/stralloc_copys.c \
	lib/stralloc_init.c \
	lib/stralloc_ready.c \
	lib/stralloc_readyplus.c \
	lib/stralloc_zero.c \
	lib/str_len.c \
	lib/strlist_at.c \
	lib/strlist_count.c \
	lib/strlist_dump.c \
	lib/strlist_pushb.c \
	lib/strlist_push.c \
	lib/strptime.c \
	lib/time_table_spd.c \
	mediathek-list.c 

include(deployment.pri)
qtcAddDeployment()

