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
	mediathek-parser.c \
	lib/array_allocate.c \
	lib/array_length.c \
	lib/array_start.c \
	lib/array_trunc.c \
	lib/buffer_1.c \
	lib/buffer_2.c \
	lib/buffer_close.c \
	lib/buffer_feed.c \
	lib/buffer_flush.c \
	lib/buffer_getc.c \
	lib/buffer_getline_sa.c \
	lib/buffer_get_token_sa.c \
	lib/buffer_init.c \
	lib/buffer_mmapread.c \
	lib/buffer_munmap.c \
	lib/buffer_put.c \
	lib/buffer_putflush.c \
	lib/buffer_putlong.c \
	lib/buffer_putnlflush.c \
	lib/buffer_puts.c \
	lib/buffer_putulong.c \
	lib/buffer_stubborn2.c \
	lib/buffer_stubborn.c \
	lib/byte_chr.c \
	lib/byte_copy.c \
	lib/byte_rchr.c \
	lib/byte_zero.c \
	lib/fmt_long.c \
	lib/fmt_ulong.c \
	lib/isleap.c \
	lib/mmap_read.c \
	lib/open_read.c \
	lib/stralloc_append.c \
	lib/stralloc_catb.c \
	lib/stralloc_catc.c \
	lib/stralloc_cats.c \
	lib/stralloc_copyb.c \
	lib/stralloc_copys.c \
	lib/stralloc_free.c \
	lib/stralloc_init.c \
	lib/stralloc_nul.c \
	lib/stralloc_ready.c \
	lib/stralloc_readyplus.c \
	lib/stralloc_zero.c \
	lib/str_chr.c \
	lib/str_diff.c \
        lib/str_len.c \
        lib/str_dup.c \
        lib/strptime.c \
	lib/time_table_spd.c \
	lib/umult64.c

include(deployment.pri)
qtcAddDeployment()

