TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CXX = gcc

*msvc* {
 DEFINES += INLINE=__inline
 DEFINES += _CRT_NONSTDC_NO_DEPRECATE=1
 DEFINES += _CRT_SECURE_NO_WARNINGS=1

 QMAKE_CFLAGS_WARN_ON = -W3 -wd4101 -wd4005
}

gcc {
gcc: QMAKE_CFLAGS_WARN_ON += -Wno-unused-arguments -Wno-unused-parameter -Wno-unused-but-set-variable
gcc: QMAKE_CFLAGS_WARN_ON += -Wno-unused-variable -Wno-unused-but-set-variable
}



#mingw | mingw32 | mingw64 | msvc {
win32 | win64 {
 DEFINES += USE_READDIR=0 USE_WIDECHAR=1 UNICODE=1

} else {
 DEFINES += USE_READDIR=1
}

#win32|win64|msvc|mingw32:CONFIG(release, debug|release): LIBS += -ladvapi32 -lws2_32
msvc: LIBS += advapi32.lib ws2_32.lib
mingw32|mingw64: LIBS += -ladvapi32 -lws2_32

mingw | mingw32 | mingw64 {
 QMAKE_LFLAGS += -static-libgcc -static-libstdc++
}

DEFINES += DEBUG=1

msvc {
  DEFINES += MSVC=1
} else  {
 DEFINES += INLINE=inline

 #!*mingw*: DEFINES += USE_READDIR=1

}

INCLUDEPATH += .


DEFINES += HAVE_ERRNO_H=1



HEADERS = lib/buffer.h lib/byte.h lib/env.h lib/errmsg.h lib/mmap.h lib/open.h lib/path.h lib/scan.h lib/str.h lib/stralloc.h lib/strlist.h

SOURCES = cc-wrap.c lib/buffer/buffer_2.c lib/buffer/buffer_flush.c lib/buffer/buffer_put.c lib/buffer/buffer_putflush.c lib/buffer/buffer_putnlflush.c lib/buffer/buffer_stubborn.c lib/byte/byte_chr.c lib/byte/byte_copy.c lib/byte/byte_copyr.c lib/byte/byte_diff.c lib/byte/byte_equal.c lib/byte/byte_zero.c lib/dir/dir_close.c lib/dir/dir_open.c lib/dir/dir_read.c lib/env/env_get.c lib/env/env_put.c lib/errmsg/errmsg_iam.c lib/errmsg/errmsg_puts.c lib/errmsg/errmsg_warnsys.c lib/errmsg/errmsg_write.c lib/mmap/mmap_read.c lib/mmap/mmap_unmap.c lib/open/open_read.c lib/path/path_basename.c lib/path/path_exists.c lib/path/path_is_absolute.c lib/scan/scan_line.c lib/scan/scan_lineskip.c lib/str/str_case_diff.c lib/str/str_case_equal.c lib/str/str_copy.c lib/str/str_copyn.c lib/str/str_diffn.c lib/str/str_len.c lib/str/str_rchrs.c lib/stralloc/stralloc_cat.c lib/stralloc/stralloc_catb.c lib/stralloc/stralloc_catc.c lib/stralloc/stralloc_cats.c lib/stralloc/stralloc_contains.c lib/stralloc/stralloc_copy.c lib/stralloc/stralloc_copyb.c lib/stralloc/stralloc_copys.c lib/stralloc/stralloc_endb.c lib/stralloc/stralloc_ends.c lib/stralloc/stralloc_findb.c lib/stralloc/stralloc_finds.c lib/stralloc/stralloc_free.c lib/stralloc/stralloc_init.c lib/stralloc/stralloc_insertb.c lib/stralloc/stralloc_nul.c lib/stralloc/stralloc_ready.c lib/stralloc/stralloc_readyplus.c lib/stralloc/stralloc_zero.c lib/strlist/strlist_count.c lib/strlist/strlist_froms.c lib/strlist/strlist_joins.c lib/strlist/strlist_push.c lib/strlist/strlist_to_argv.c lib/strlist/strlist_unshift.c lib/path/path_find.c lib/dir/dir_name.c lib/str/str_cat.c lib/stralloc/utf8.c lib/str/str_ndup.c

