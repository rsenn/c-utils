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

HEADERS = lib/buffer.h lib/byte.h lib/fmt.h lib/scan.h lib/str.h lib/stralloc.h lib/strlist.h


SOURCES = lib/buffer/buffer_1.c lib/buffer/buffer_2.c lib/buffer/buffer_feed.c lib/buffer/buffer_flush.c lib/buffer/buffer_get.c lib/buffer/buffer_get_token.c lib/buffer/buffer_getc.c lib/buffer/buffer_put.c lib/buffer/buffer_putflush.c lib/buffer/buffer_putm_internal.c lib/buffer/buffer_putnlflush.c lib/buffer/buffer_putnspace.c lib/buffer/buffer_puts.c lib/buffer/buffer_putulong.c lib/buffer/buffer_putulong0.c lib/buffer/buffer_stubborn.c lib/buffer/buffer_stubborn2.c lib/byte/byte_chr.c lib/byte/byte_copy.c lib/byte/byte_fill.c lib/byte/byte_zero.c lib/fmt/fmt_ulong.c lib/fmt/fmt_ulonglong.c lib/isleap.c lib/scan/scan_uint.c lib/scan/scan_ushort.c lib/str/str_dup.c lib/str/str_istr.c lib/str/str_len.c lib/str/str_ptime.c lib/str/str_rchr.c lib/str/str_tok.c lib/stralloc/stralloc_append.c lib/stralloc/stralloc_catb.c lib/stralloc/stralloc_cats.c lib/stralloc/stralloc_copyb.c lib/stralloc/stralloc_copys.c lib/stralloc/stralloc_free.c lib/stralloc/stralloc_init.c lib/stralloc/stralloc_ready.c lib/stralloc/stralloc_readyplus.c lib/stralloc/stralloc_zero.c lib/strlist/strlist_at.c lib/strlist/strlist_count.c lib/strlist/strlist_dump.c lib/strlist/strlist_join.c lib/strlist/strlist_push.c lib/strlist/strlist_push_tokens.c lib/strlist/strlist_pushb.c lib/time_table_spd.c mediathek-list.c


