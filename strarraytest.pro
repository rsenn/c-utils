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

HEADERS = lib/array.h lib/buffer.h lib/byte.h lib/str.h lib/stralloc.h lib/strarray.h


SOURCES = lib/array/array_allocate.c lib/array/array_catb.c lib/array/array_fail.c lib/array/array_get.c lib/array/array_length.c lib/array/array_start.c lib/safemult/umult64.c lib/buffer/buffer_1.c lib/buffer/buffer_flush.c lib/buffer/buffer_put.c lib/buffer/buffer_putflush.c lib/buffer/buffer_putnlflush.c lib/buffer/buffer_putsa.c lib/buffer/buffer_stubborn.c lib/byte/byte_copy.c lib/byte/byte_diff.c lib/byte/byte_equal.c lib/byte/byte_zero.c lib/str/str_dup.c lib/str/str_len.c lib/stralloc/stralloc_append.c lib/stralloc/stralloc_catb.c lib/stralloc/stralloc_cats.c lib/stralloc/stralloc_endb.c lib/stralloc/stralloc_init.c lib/stralloc/stralloc_ready.c lib/stralloc/stralloc_readyplus.c lib/stralloc/stralloc_zero.c lib/strarray/strarray_from_argv.c lib/strarray/strarray_joins.c lib/strarray/strarray_push.c lib/strarray/strarray_removesuffixs.c strarraytest.c

DEFINES += HAVE_ERRNO_H=1
