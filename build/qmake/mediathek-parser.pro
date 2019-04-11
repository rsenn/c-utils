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

HEADERS = ../../lib/array.h ../../lib/buffer.h ../../lib/byte.h ../../lib/fmt.h ../../lib/mmap.h ../../lib/open.h ../../lib/scan.h ../../lib/str.h ../../lib/stralloc.h ../../lib/strlist.h ../../lib/ucs.h


SOURCES = ../../lib/array/array_length.c ../../lib/array/array_start.c ../../lib/buffer/buffer_1.c ../../lib/buffer/buffer_2.c ../../lib/buffer/buffer_close.c ../../lib/buffer/buffer_dummyreadmmap.c ../../lib/buffer/buffer_feed.c ../../lib/buffer/buffer_flush.c ../../lib/buffer/buffer_getc.c ../../lib/buffer/buffer_init.c ../../lib/buffer/buffer_mmapread.c ../../lib/buffer/buffer_munmap.c ../../lib/buffer/buffer_put.c ../../lib/buffer/buffer_putc.c ../../lib/buffer/buffer_putflush.c ../../lib/buffer/buffer_putlong.c ../../lib/buffer/buffer_putm_internal.c ../../lib/buffer/buffer_putnlflush.c ../../lib/buffer/buffer_puts.c ../../lib/buffer/buffer_putulong.c ../../lib/buffer/buffer_stubborn.c ../../lib/buffer/buffer_stubborn2.c ../../lib/byte/byte_chr.c ../../lib/byte/byte_copy.c ../../lib/byte/byte_diff.c ../../lib/byte/byte_equal.c ../../lib/byte/byte_findb.c ../../lib/byte/byte_finds.c ../../lib/byte/byte_rchr.c ../../lib/byte/byte_zero.c ../../lib/fmt/fmt_long.c ../../lib/fmt/fmt_ulong.c ../../lib/mmap/mmap_read.c ../../lib/open/open_read.c ../../lib/scan/scan_utf8.c ../../lib/str/isleap.c ../../lib/str/str_case_diffn.c ../../lib/str/str_chr.c ../../lib/str/str_diff.c ../../lib/str/str_dup.c ../../lib/str/str_len.c ../../lib/str/str_ptime.c ../../lib/str/str_start.c ../../lib/str/time_table_spd.c ../../lib/stralloc/buffer_get_token_sa.c ../../lib/stralloc/buffer_getline_sa.c ../../lib/stralloc/stralloc_append.c ../../lib/stralloc/stralloc_catb.c ../../lib/stralloc/stralloc_catc.c ../../lib/stralloc/stralloc_cats.c ../../lib/stralloc/stralloc_copyb.c ../../lib/stralloc/stralloc_copys.c ../../lib/stralloc/stralloc_free.c ../../lib/stralloc/stralloc_init.c ../../lib/stralloc/stralloc_nul.c ../../lib/stralloc/stralloc_ready.c ../../lib/stralloc/stralloc_readyplus.c ../../lib/stralloc/stralloc_zero.c ../../lib/strlist/strlist_count.c ../../lib/strlist/strlist_dump.c ../../lib/strlist/strlist_fromb.c ../../lib/strlist/strlist_pushb.c ../../lib/strlist/strlist_to_argv.c ../../lib/ucs/scan_latin1_utf8.c ../../lib/ucs/str_utf8_latin.c ../../lib/ucs/ucs_latin1.c ../../lib/unix/getopt.c ../../mediathek-parser.c


DEFINES += io_seek=lseek64

DEFINES += HAVE_ERRNO_H=1
