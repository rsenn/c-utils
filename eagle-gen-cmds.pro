TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += link_pkgconfig
#PKGCONFIG += libxml-2.0

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

#win32: LIBXML2_DIR = e:/Libraries/libxml2-2.9.4/vs2015-x86
#win64: LIBXML2_DIR = e:/Libraries/libxml2-2.9.4/vs2015-x64

#DEFINES += LIBXML_STATIC=1
#INCLUDEPATH += $$LIBXML2_DIR/include $$LIBXML2_DIR/include/libxml $$PWD/lib
#!unix: LIBS += $$LIBXML2_DIR/lib/xml2.lib

#WIN_ICONV_DIR = e:/Libraries/win-iconv-0.0.6/x86_64-w64-mingw32
#INCLUDEPATH += $$WIN_ICONV_DIR/include $$PWD/lib

DEFINES += io_seek=lseek64 _LARGEFILE64_SOURCE=1

SOURCES = eagle-gen-cmds.c lib/array/array_allocate.c lib/array/array_catb.c lib/array/array_fail.c lib/array/array_get.c lib/array/array_length.c lib/array/array_start.c lib/buffer/buffer_1.c lib/buffer/buffer_2.c lib/buffer/buffer_dummyread.c lib/buffer/buffer_feed.c lib/buffer/buffer_flush.c lib/buffer/buffer_free.c lib/buffer/buffer_get.c lib/buffer/buffer_gettok_sa.c lib/buffer/buffer_mmapprivate.c lib/buffer/buffer_munmap.c lib/buffer/buffer_peek.c lib/buffer/buffer_peekc.c lib/buffer/buffer_prefetch.c lib/buffer/buffer_put.c lib/buffer/buffer_putc.c lib/buffer/buffer_putdouble.c lib/buffer/buffer_putflush.c lib/buffer/buffer_putlong.c lib/buffer/buffer_putlonglong.c lib/buffer/buffer_putm_internal.c lib/buffer/buffer_putnlflush.c lib/buffer/buffer_putnspace.c lib/buffer/buffer_putptr.c lib/buffer/buffer_puts.c lib/buffer/buffer_putsa.c lib/buffer/buffer_putspace.c lib/buffer/buffer_putulong.c lib/buffer/buffer_putulonglong.c lib/buffer/buffer_skip_pred.c lib/buffer/buffer_skip_until.c lib/buffer/buffer_skipc.c lib/buffer/buffer_skipspace.c lib/buffer/buffer_stubborn.c lib/buffer/buffer_stubborn2.c lib/buffer/buffer_truncfile.c lib/byte/byte_case_diff.c lib/byte/byte_chr.c lib/byte/byte_copy.c lib/byte/byte_copyr.c lib/byte/byte_diff.c lib/byte/byte_equal.c lib/byte/byte_fill.c lib/byte/byte_scan.c lib/byte/byte_zero.c lib/cbmap/alloc.c lib/cbmap/cbmap_count.c lib/cbmap/cbmap_get.c lib/cbmap/cbmap_insert.c lib/cbmap/cbmap_internal_node.c lib/cbmap/cbmap_new.c lib/cbmap/cbmap_visit.c lib/cbmap/cbmap_visit_all.c lib/fmt/fmt_double.c lib/fmt/fmt_long.c lib/fmt/fmt_longlong.c lib/fmt/fmt_minus.c lib/fmt/fmt_ulong.c lib/fmt/fmt_ulong0.c lib/fmt/fmt_ulonglong.c lib/fmt/fmt_utf8.c lib/fmt/fmt_xlong.c lib/fmt/fmt_xlonglong.c lib/fmt/fmt_xmlescape.c lib/hmap/hmap_add.c lib/hmap/hmap_destroy.c lib/hmap/hmap_free_data.c lib/hmap/hmap_init.c lib/hmap/hmap_search.c lib/hmap/hmap_set.c lib/hmap/hmap_truncate.c lib/memalign.c lib/mmap/mmap_private.c lib/open/open_read.c lib/open/open_trunc.c lib/scan/scan_double.c lib/scan/scan_fromhex.c lib/scan/scan_long.c lib/scan/scan_longn.c lib/scan/scan_uint.c lib/scan/scan_ulong.c lib/scan/scan_ulongn.c lib/scan/scan_xlong.c lib/scan/scan_xmlescape.c lib/str/str_basename.c lib/str/str_chr.c lib/str/str_diff.c lib/str/str_diffn.c lib/str/str_dup.c lib/str/str_len.c lib/str/str_ndup.c lib/str/str_rchr.c lib/stralloc/stralloc_append.c lib/stralloc/stralloc_case_diff.c lib/stralloc/stralloc_case_equal.c lib/stralloc/stralloc_catb.c lib/stralloc/stralloc_catc.c lib/stralloc/stralloc_catlong0.c lib/stralloc/stralloc_cats.c lib/stralloc/stralloc_copy.c lib/stralloc/stralloc_copyb.c lib/stralloc/stralloc_copys.c lib/stralloc/stralloc_fmt_pred.c lib/stralloc/stralloc_free.c lib/stralloc/stralloc_init.c lib/stralloc/stralloc_insertb.c lib/stralloc/stralloc_nul.c lib/stralloc/stralloc_ready.c lib/stralloc/stralloc_readyplus.c lib/stralloc/stralloc_subst.c lib/stralloc/stralloc_zero.c lib/strarray/strarray_index_of.c lib/strarray/strarray_set.c lib/strlist/strlist_at.c lib/strlist/strlist_contains.c lib/strlist/strlist_froms.c lib/strlist/strlist_push.c lib/umult64.c lib/xml/xml_add_child.c lib/xml/xml_content.c lib/xml/xml_delete.c lib/xml/xml_element.c lib/xml/xml_escape.c lib/xml/xml_find_all_pred.c lib/xml/xml_find_element.c lib/xml/xml_find_element_attr.c lib/xml/xml_find_parent.c lib/xml/xml_find_pred.c lib/xml/xml_find_predicates.c lib/xml/xml_free.c lib/xml/xml_get_attribute.c lib/xml/xml_has_attribute.c lib/xml/xml_newnode.c lib/xml/xml_path.c lib/xml/xml_print.c lib/xml/xml_print_attributes.c lib/xml/xml_read_callback.c lib/xml/xml_read_tree.c lib/xml/xml_reader_init.c lib/xml/xml_set_attribute.c
HEADERS = lib/array.h lib/buffer.h lib/byte.h lib/cbmap.h lib/fmt.h lib/hmap.h lib/mmap.h lib/open.h lib/scan.h lib/str.h lib/stralloc.h lib/strarray.h lib/strlist.h lib/xml.h
