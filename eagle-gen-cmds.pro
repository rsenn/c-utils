TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += link_pkgconfig
PKGCONFIG += libxml-2.0

contains(CONFIG,debug) {
  DEFINES += DEBUG=1
}

unix|linux {
  DEFINES += HAVE_POSIX_MEMALIGN=1
}

*msvc* {
  DEFINES += INLINE=__inline
  DEFINES += _CRT_NONSTDC_NO_DEPRECATE=1
  DEFINES += _CRT_SECURE_NO_WARNINGS=1

  QMAKE_CFLAGS_WARN_ON = -W3
}

gcc|linux {
  QMAKE_CFLAGS_WARN_ON += -Wno-unused-parameter
  QMAKE_CFLAGS_WARN_ON += -Wno-unused-variable
}

mingw | mingw32 | mingw64 | msvc {
  DEFINES += USE_READDIR=0
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

win32: LIBXML2_DIR = e:/Libraries/libxml2-2.9.4/vs2015-x86
win64: LIBXML2_DIR = e:/Libraries/libxml2-2.9.4/vs2015-x64

DEFINES += LIBXML_STATIC=1
INCLUDEPATH += $$LIBXML2_DIR/include $$LIBXML2_DIR/include/libxml $$PWD/lib
!unix: LIBS += $$LIBXML2_DIR/lib/xml2.lib

#WIN_ICONV_DIR = e:/Libraries/win-iconv-0.0.6/x86_64-w64-mingw32
#INCLUDEPATH += $$WIN_ICONV_DIR/include $$PWD/lib

HEADERS = \
  lib/byte.h  lib/buffer.h lib/hmap.h lib/stralloc.h lib/str.h \
    lib/cbmap/alloc.h \
    lib/cbmap/cbmap.h \
    lib/cbmap/alloc.h \
    lib/cbmap/cbmap.h \
    lib/cbmap.h \
    lib/cb_internal.h \
    lib/cb.h \
    lib/cb_internal.h \
    lib/cbmap_internal.h \
    lib/cb.h \
    lib/iterator.h

DEFINES += LSEEK=lseek64 _LARGEFILE64_SOURCE=1


include(deployment.pri)
qtcAddDeployment()

SOURCES = eagle-gen-cmds.c lib/umult64.c lib/array/array_catb.c lib/array/array_fail.c  lib/array/array_length.c lib/array/array_get.c    lib/array/array_allocate.c lib/buffer.h lib/buffer/buffer_0.c lib/buffer/buffer_1.c lib/buffer/buffer_2.c lib/buffer/buffer_close.c lib/buffer/buffer_default.c lib/buffer/buffer_feed.c lib/buffer/buffer_flush.c lib/buffer/buffer_get.c lib/buffer/buffer_mmapprivate.c lib/buffer/buffer_mmapread.c lib/buffer/buffer_munmap.c lib/buffer/buffer_put.c lib/buffer/buffer_putc.c lib/buffer/buffer_putdouble.c lib/buffer/buffer_putflush.c lib/buffer/buffer_putlong.c lib/buffer/buffer_putm_internal.c lib/buffer/buffer_putm_internal_flush.c lib/buffer/buffer_putnlflush.c lib/buffer/buffer_puts.c lib/buffer/buffer_putulong.c lib/buffer/buffer_putxlong.c lib/buffer/buffer_skip_until.c lib/buffer/buffer_stubborn.c lib/buffer/buffer_stubborn2.c lib/byte.h lib/byte/byte_chr.c lib/byte/byte_copy.c lib/byte/byte_copyr.c lib/byte/byte_diff.c lib/byte/byte_fill.c lib/byte/byte_rchr.c lib/byte/byte_zero.c lib/cb.h lib/cb/cb_clear.c lib/cb/cb_erase.c lib/cb/cb_find.c lib/cb/cb_find_prefix.c lib/cb/cb_foreach.c lib/cb/cb_get_kv.c lib/cb/cb_get_kv_ex.c lib/cb/cb_insert.c lib/cb/cb_new_kv.c lib/cb_internal.h lib/cbmap.h lib/cbmap/alloc.c lib/cbmap/alloc.h lib/cbmap/cbmap_count.c lib/cbmap/cbmap_data_node_destroy.c lib/cbmap/cbmap_delete.c lib/cbmap/cbmap_destroy.c lib/cbmap/cbmap_get.c lib/cbmap/cbmap_insert.c lib/cbmap/cbmap_internal_node.c lib/cbmap/cbmap_new.c lib/cbmap/cbmap_visit.c lib/cbmap/cbmap_visit_all.c lib/cbmap_internal.h lib/fmt/fmt_double.c lib/fmt/fmt_escapecharquotedprintable.c lib/fmt/fmt_escapecharquotedprintableutf8.c lib/fmt/fmt_long.c lib/fmt/fmt_minus.c lib/fmt/fmt_tohex.c lib/fmt/fmt_ulong.c lib/fmt/fmt_ulong0.c lib/fmt/fmt_utf8.c lib/fmt/fmt_xlong.c lib/hmap.h lib/hmap/hmap_add.c lib/hmap/hmap_destroy.c lib/hmap/hmap_free_data.c lib/hmap/hmap_init.c lib/hmap/hmap_search.c lib/hmap/hmap_truncate.c lib/mmap/mmap_private.c lib/mmap/mmap_read.c lib/mmap/mmap_unmap.c lib/open/open_read.c lib/scan/scan_double.c lib/scan/scan_uint.c lib/scan/scan_ulong.c lib/scan/scan_ulongn.c lib/scan/scan_long.c lib/scan/scan_longn.c  lib/str.h lib/str/str_chr.c lib/str.h lib/str/str_tok.c lib/str/str_diff.c lib/str/str_diffn.c lib/str/str_dup.c lib/str/str_len.c lib/str/str_lower.c lib/stralloc.h lib/stralloc/stralloc_append.c lib/stralloc/stralloc_append_sa.c lib/stralloc/stralloc_cat.c lib/stralloc/stralloc_catb.c lib/stralloc/stralloc_catc.c lib/stralloc/stralloc_catlong0.c lib/stralloc/stralloc_catm_internal.c lib/stralloc/stralloc_cats.c lib/stralloc/stralloc_catulong0.c lib/stralloc/stralloc_copy.c lib/stralloc/stralloc_copyb.c lib/stralloc/stralloc_copys.c lib/stralloc/stralloc_decamelize.c lib/stralloc/stralloc_diffs.c lib/stralloc/stralloc_free.c lib/stralloc/stralloc_init.c lib/stralloc/stralloc_insertb.c lib/stralloc/stralloc_move.c lib/stralloc/stralloc_nul.c lib/stralloc/stralloc_ready.c lib/stralloc/stralloc_readyplus.c lib/stralloc/stralloc_remove.c lib/stralloc/stralloc_trunc.c lib/stralloc/stralloc_write.c lib/stralloc/stralloc_zero.c lib/strlist/strlist_at.c lib/strlist/strlist_cat.c lib/strlist/strlist_count.c lib/strlist/strlist_dump.c lib/strlist/strlist_index_of.c lib/strlist/strlist_join.c lib/strlist/strlist_push.c lib/strlist/strlist_push_sa.c lib/strlist/strlist_push_tokens.c lib/strlist/strlist_push_unique.c lib/strlist/strlist_pushb.c lib/strlist/strlist_pushm_internal.c lib/strlist/strlist_pushsa.c lib/strlist/strlist_range.c lib/strlist/strlist_shift.c lib/strlist/strlist_sort.c lib/strlist/strlist_to_argv.c lib/strlist/strlist_unshift.c \
    lib/buffer/buffer_putsa.c \
    lib/buffer/buffer_putspace.c \
    lib/str/str_ndup.c
