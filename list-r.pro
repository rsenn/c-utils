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

mingw | mingw32 | mingw64 | msvc {
  DEFINES += USE_READDIR=0
}


mingw | mingw32 | mingw64 {
  QMAKE_LFLAGS += -static-libgcc -static-libstdc++
}

!*msvc* {
  DEFINES += INLINE=inline

  #!*mingw*: DEFINES += USE_READDIR=1

  #QMAKE_CFLAGS_WARN_ON += -Wno-sign-compare -Wno-unused-parameter
}

INCLUDEPATH += . $$PWD/lib

HEADERS =   lib/array.h \
  lib/buffer.h \
  lib/byte.h \
  lib/dir.h \
  lib/fmt.h \
lib/mmap.h \
  lib/str.h \
  lib/stralloc.h \
  lib/fnmatch.h \
lib/safemult.h


SOURCES = list-r.c \
  lib/fnmatch.c \
lib/umult64.c \
  lib/array_allocate.c \
  lib/array_bytes.c \
  lib/array_cat.c \
  lib/array_cat0.c \
  lib/array_catb.c \
  lib/array_cate.c \
  lib/array_cats.c \
  lib/array_cats0.c \
  lib/array_equal.c \
  lib/array_fail.c \
  lib/array_get.c \
  lib/array_length.c \
  lib/array_reset.c \
  lib/array_start.c \
  lib/array_trunc.c \
  lib/array_truncate.c \
  lib/buffer_0.c \
  lib/buffer_0small.c \
  lib/buffer_1.c \
  lib/buffer_1small.c \
  lib/buffer_2.c \
  lib/buffer_close.c \
  lib/buffer_default.c \
  lib/buffer_dump.c \
  lib/buffer_feed.c \
  lib/buffer_flush.c \
  lib/buffer_free.c \
  lib/buffer_fromarray.c \
  lib/buffer_frombuf.c \
  lib/buffer_fromsa.c \
  lib/buffer_fromstr.c \
  lib/buffer_get.c \
  lib/buffer_getc.c \
  lib/buffer_getline.c \
  lib/buffer_getline_sa.c \
  lib/buffer_getn.c \
  lib/buffer_getnewline_sa.c \
  lib/buffer_get_new_token_sa.c \
  lib/buffer_get_new_token_sa_pred.c \
  lib/buffer_get_token.c \
  lib/buffer_get_token_pred.c \
  lib/buffer_get_token_sa.c \
  lib/buffer_get_token_sa_pred.c \
  lib/buffer_get_until.c \
  lib/buffer_init.c \
  lib/buffer_init_free.c \
  lib/buffer_mmapprivate.c \
  lib/buffer_mmapread.c \
  lib/buffer_mmapread_fd.c \
  lib/buffer_munmap.c \
  lib/buffer_peek.c \
  lib/buffer_prefetch.c \
  lib/buffer_put.c \
  lib/buffer_put8long.c \
  lib/buffer_putalign.c \
  lib/buffer_putc.c \
  lib/buffer_puterror.c \
  lib/buffer_puterror2.c \
  lib/buffer_putflush.c \
  lib/buffer_putlong.c \
  lib/buffer_putlonglong.c \
  lib/buffer_putm_internal.c \
  lib/buffer_putm_internal_flush.c \
  lib/buffer_putnlflush.c \
  lib/buffer_putnspace.c \
  lib/buffer_puts.c \
  lib/buffer_putsa.c \
  lib/buffer_putsaflush.c \
  lib/buffer_putsalign.c \
  lib/buffer_putsflush.c \
  lib/buffer_putspace.c \
  lib/buffer_putuint64.c \
  lib/buffer_putulong.c \
  lib/buffer_putulonglong.c \
  lib/buffer_putxlong.c \
  lib/buffer_seek.c \
  lib/buffer_skip_until.c \
  lib/buffer_stubborn.c \
  lib/buffer_stubborn2.c \
  lib/buffer_tosa.c \
  lib/buffer_truncfile.c \
  lib/byte_chr.c \
  lib/byte_copy.c \
  lib/byte_copyr.c \
  lib/byte_diff.c \
  lib/byte_fill.c \
  lib/byte_rchr.c \
  lib/byte_zero.c \
  lib/dir_close.c \
  lib/dir_name.c \
  lib/dir_open.c \
  lib/dir_read.c \
  lib/dir_time.c \
  lib/dir_type.c \
  lib/fmt_8long.c \
  lib/fmt_8longlong.c \
  lib/fmt_asn1derlength.c \
  lib/fmt_asn1dertag.c \
  lib/fmt_double.c \
  lib/fmt_escapecharc.c \
  lib/fmt_escapecharhtml.c \
  lib/fmt_escapecharjson.c \
  lib/fmt_escapecharquotedprintable.c \
  lib/fmt_escapecharquotedprintableutf8.c \
  lib/fmt_escapecharxml.c \
  lib/fmt_fill.c \
  lib/fmt_httpdate.c \
  lib/fmt_human.c \
  lib/fmt_humank.c \
  lib/fmt_iso8601.c \
  lib/fmt_long.c \
  lib/fmt_longlong.c \
  lib/fmt_minus.c \
  lib/fmt_pad.c \
  lib/fmt_plusminus.c \
  lib/fmt_str.c \
  lib/fmt_strm_internal.c \
  lib/fmt_strn.c \
  lib/fmt_tohex.c \
  lib/fmt_uint64.c \
  lib/fmt_ulong.c \
  lib/fmt_ulong0.c \
  lib/fmt_ulonglong.c \
  lib/fmt_utf8.c \
  lib/fmt_varint.c \
  lib/fmt_xlong.c \
  lib/fmt_xlonglong.c \
  lib/fmt_xmlescape.c \
  lib/open_append.c \
  lib/open_read.c \
  lib/open_rw.c \
  lib/open_trunc.c \
  lib/mmap_map.c \
  lib/mmap_private.c \
  lib/mmap_read.c \
  lib/mmap_read_fd.c \
  lib/mmap_unmap.c \
  lib/stralloc_append.c \
  lib/stralloc_append_sa.c \
  lib/stralloc_cat.c \
  lib/stralloc_catb.c \
  lib/stralloc_catc.c \
  lib/stralloc_catlong0.c \
  lib/stralloc_cats.c \
  lib/stralloc_catulong0.c \
  lib/stralloc_copy.c \
  lib/stralloc_copyb.c \
  lib/stralloc_copys.c \
  lib/stralloc_diffs.c \
  lib/stralloc_free.c \
  lib/stralloc_init.c \
  lib/stralloc_insertb.c \
  lib/stralloc_move.c \
  lib/stralloc_nul.c \
  lib/stralloc_ready.c \
  lib/stralloc_readyplus.c \
  lib/stralloc_remove.c \
  lib/stralloc_trunc.c \
  lib/stralloc_write.c \
  lib/stralloc_zero.c \
  lib/str_chr.c \
  lib/str_diff.c \
  lib/str_diffn.c \
  lib/str_len.c

include(deployment.pri)
qtcAddDeployment()

