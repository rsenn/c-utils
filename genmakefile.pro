TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CXX = gcc

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

INCLUDEPATH += .

HEADERS =


SOURCES = \ 
    genmakefile.c \
    lib/buffer/buffer_0.c \
    lib/buffer/buffer_1.c \
    lib/buffer/buffer_2.c \
    lib/buffer/buffer_bz2.c \
    lib/buffer/buffer_bzip.c \
    lib/buffer/buffer_close.c \
    lib/buffer/buffer_copy.c \
    lib/buffer/buffer_default.c \
    lib/buffer/buffer_deflate.c \
    lib/buffer/buffer_dummyread.c \
    lib/buffer/buffer_dummyreadmmap.c \
    lib/buffer/buffer_dump.c \
    lib/buffer/buffer_feed.c \
    lib/buffer/buffer_flush.c \
    lib/buffer/buffer_free.c \
    lib/buffer/buffer_freshen.c \
    lib/buffer/buffer_fromarray.c \
    lib/buffer/buffer_frombuf.c \
    lib/buffer/buffer_fromsa.c \
    lib/buffer/buffer_fromstr.c \
    lib/buffer/buffer_get.c \
    lib/buffer/buffer_get_new_token_sa.c \
    lib/buffer/buffer_get_new_token_sa_pred.c \
    lib/buffer/buffer_get_token.c \
    lib/buffer/buffer_get_token_pred.c \
    lib/buffer/buffer_get_token_sa.c \
    lib/buffer/buffer_get_token_sa_pred.c \
    lib/buffer/buffer_get_until.c \
    lib/buffer/buffer_getc.c \
    lib/buffer/buffer_getline.c \
    lib/buffer/buffer_getline_sa.c \
    lib/buffer/buffer_getn.c \
    lib/buffer/buffer_getnewline_sa.c \
    lib/buffer/buffer_gettok_sa.c \
    lib/buffer/buffer_gzip.c \
    lib/buffer/buffer_inflate.c \
    lib/buffer/buffer_init.c \
    lib/buffer/buffer_init_free.c \
    lib/buffer/buffer_lzma.c \
    lib/buffer/buffer_mmapprivate.c \
    lib/buffer/buffer_mmapprivate_fd.c \
    lib/buffer/buffer_mmapread.c \
    lib/buffer/buffer_mmapread_fd.c \
    lib/buffer/buffer_mmapshared_fd.c \
    lib/buffer/buffer_munmap.c \
    lib/buffer/buffer_peek.c \
    lib/buffer/buffer_peekc.c \
    lib/buffer/buffer_prefetch.c \
    lib/buffer/buffer_put.c \
    lib/buffer/buffer_put_escaped.c \
    lib/buffer/buffer_put8long.c \
    lib/buffer/buffer_putalign.c \
    lib/buffer/buffer_putc.c \
    lib/buffer/buffer_putdouble.c \
    lib/buffer/buffer_puterror.c \
    lib/buffer/buffer_puterror2.c \
    lib/buffer/buffer_putflush.c \
    lib/buffer/buffer_putint64.c \
    lib/buffer/buffer_putlong.c \
    lib/buffer/buffer_putlonglong.c \
    lib/buffer/buffer_putm_internal.c \
    lib/buffer/buffer_putm_internal_flush.c \
    lib/buffer/buffer_putnc.c \
    lib/buffer/buffer_putnlflush.c \
    lib/buffer/buffer_putnspace.c \
    lib/buffer/buffer_putptr.c \
    lib/buffer/buffer_puts.c \
    lib/buffer/buffer_puts_escaped.c \
    lib/buffer/buffer_putsa.c \
    lib/buffer/buffer_putsaflush.c \
    lib/buffer/buffer_putsalign.c \
    lib/buffer/buffer_putsflush.c \
    lib/buffer/buffer_putspace.c \
    lib/buffer/buffer_putspad.c \
    lib/buffer/buffer_putuint64.c \
    lib/buffer/buffer_putulong.c \
    lib/buffer/buffer_putulong0.c \
    lib/buffer/buffer_putulonglong.c \
    lib/buffer/buffer_putxlong.c \
    lib/buffer/buffer_putxlong0.c \
    lib/buffer/buffer_putxlonglong.c \
    lib/buffer/buffer_putxlonglong0.c \
    lib/buffer/buffer_read.c \
    lib/buffer/buffer_read_fd.c \
    lib/buffer/buffer_seek.c \
    lib/buffer/buffer_skip_pred.c \
    lib/buffer/buffer_skip_until.c \
    lib/buffer/buffer_skipc.c \
    lib/buffer/buffer_skipn.c \
    lib/buffer/buffer_skipspace.c \
    lib/buffer/buffer_stubborn.c \
    lib/buffer/buffer_stubborn2.c \
    lib/buffer/buffer_tosa.c \
    lib/buffer/buffer_truncfile.c \
    lib/buffer/buffer_write.c \
    lib/buffer/buffer_write_fd.c \
    lib/strarray/strarray_appends.c \
    lib/strarray/strarray_from_argv.c \
    lib/strarray/strarray_index_of.c \
    lib/strarray/strarray_joins.c \
    lib/strarray/strarray_pop.c \
    lib/strarray/strarray_prepends.c \
    lib/strarray/strarray_push.c \
    lib/strarray/strarray_push_sa.c \
    lib/strarray/strarray_pushb.c \
    lib/strarray/strarray_pushd.c \
    lib/strarray/strarray_removeprefixs.c \
    lib/strarray/strarray_removesuffixs.c \
    lib/strarray/strarray_set.c \
    lib/strarray/strarray_sort.c \
    lib/strarray/strarray_to_argv.c \
    lib/rdir/rdir_close.c \
    lib/rdir/rdir_open.c \
    lib/rdir/rdir_read.c \
    lib/byte/byte_case_diff.c \
    lib/byte/byte_case_equal.c \
    lib/byte/byte_case_start.c \
    lib/byte/byte_chr.c \
    lib/byte/byte_copy.c \
    lib/byte/byte_copyr.c \
    lib/byte/byte_count.c \
    lib/byte/byte_diff.c \
    lib/byte/byte_equal.c \
    lib/byte/byte_fill.c \
    lib/byte/byte_fmt.c \
    lib/byte/byte_lower.c \
    lib/byte/byte_rchr.c \
    lib/byte/byte_scan.c \
    lib/byte/byte_upper.c \
    lib/byte/byte_zero.c

DEFINES += HAVE_ERRNO_H=1
