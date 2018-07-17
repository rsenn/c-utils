TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += link_pkgconfig
PKGCONFIG += libxml-2.0

*msvc* {
  DEFINES += INLINE=__inline
  DEFINES += _CRT_NONSTDC_NO_DEPRECATE=1
  DEFINES += _CRT_SECURE_NO_WARNINGS=1

  QMAKE_CFLAGS_WARN_ON = -W3
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
  lib/byte.h lib/hmap.h lib/stralloc.h lib/str.h 

SOURCES = eagle-init-brd.c lib/byte/byte_chr.c lib/byte/byte_copy.c lib/byte/byte_copyr.c lib/byte/byte_diff.c lib/byte/byte_fill.c lib/byte/byte_rchr.c lib/byte/byte_zero.c lib/hmap/hmap_add.c lib/hmap/hmap_add_tuple_with_data.c lib/hmap/hmap_delete.c lib/hmap/hmap_destroy.c lib/hmap/hmap_free_data.c lib/hmap/hmap_init.c lib/hmap/hmap_internal.c lib/hmap/hmap_is_locate.c lib/hmap/hmap_print_list.c lib/hmap/hmap_print_table.c lib/hmap/hmap_print_tree.c lib/hmap/hmap_search.c lib/hmap/hmap_set.c lib/hmap/hmap_truncate.c lib/stralloc/stralloc_append.c lib/stralloc/stralloc_append_sa.c lib/stralloc/stralloc_cat.c lib/stralloc/stralloc_catb.c lib/stralloc/stralloc_catc.c lib/stralloc/stralloc_catlong0.c lib/stralloc/stralloc_cats.c lib/stralloc/stralloc_catulong0.c lib/stralloc/stralloc_copy.c lib/stralloc/stralloc_copyb.c lib/stralloc/stralloc_copys.c lib/stralloc/stralloc_decamelize.c lib/stralloc/stralloc_diffs.c lib/stralloc/stralloc_free.c lib/stralloc/stralloc_init.c lib/stralloc/stralloc_insertb.c lib/stralloc/stralloc_move.c lib/stralloc/stralloc_nul.c lib/stralloc/stralloc_ready.c lib/stralloc/stralloc_readyplus.c lib/stralloc/stralloc_remove.c lib/stralloc/stralloc_trunc.c lib/stralloc/stralloc_write.c lib/stralloc/stralloc_zero.c lib/strarray/strarray_push.c lib/strarray/strarray_pushd.c lib/strlist/strlist_at.c lib/strlist/strlist_cat.c lib/strlist/strlist_count.c lib/strlist/strlist_dump.c lib/strlist/strlist_index_of.c lib/strlist/strlist_join.c lib/strlist/strlist_push.c lib/strlist/strlist_pushb.c lib/strlist/strlist_pushm_internal.c lib/strlist/strlist_pushsa.c lib/strlist/strlist_push_sa.c lib/strlist/strlist_push_tokens.c lib/strlist/strlist_push_unique.c lib/strlist/strlist_range.c lib/strlist/strlist_shift.c lib/strlist/strlist_sort.c lib/strlist/strlist_to_argv.c lib/strlist/strlist_unshift.c lib/str/str_basename.c lib/str/str_case_diff.c lib/str/str_case_diffn.c lib/str/str_case_equal.c lib/str/str_case_start.c lib/str/str_cat.c lib/str/str_chr.c lib/str/str_copy.c lib/str/str_copyb.c lib/str/str_decamelize.c lib/str/str_diff.c lib/str/str_diffn.c lib/str/str_dup.c lib/str/str_equal.c lib/str/str_find.c lib/str/str_findb.c lib/str/str_istr.c lib/str/str_len.c lib/str/str_lower.c lib/str/str_ptime.c lib/str/str_rchr.c lib/str/str_start.c lib/str/str_tok.c

include(deployment.pri)
qtcAddDeployment()



