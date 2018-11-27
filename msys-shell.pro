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
  QMAKE_CFLAGS_WARN_ON += -Wno-unused-arguments -Wno-unused-parameter
  QMAKE_CFLAGS_WARN_ON += -Wno-unused-variable
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

 #QMAKE_CFLAGS_WARN_ON += -Wno-sign-compare -Wno-unused-parameter
}

INCLUDEPATH += .

HEADERS = lib/buffer.h lib/byte.h lib/env.h lib/errmsg.h lib/path.h lib/str.h lib/stralloc.h lib/strlist.h


DEFINES += HAVE_ERRNO_H=1

SOURCES = \
  lib/buffer/buffer_0.c \
  lib/buffer/buffer_0small.c \
  lib/buffer/buffer_1.c \
  lib/buffer/buffer_1small.c \
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
  lib/buffer/buffer_getc.c \
  lib/buffer/buffer_getline.c \
  lib/buffer/buffer_getn.c \
  lib/buffer/buffer_get_token.c \
  lib/buffer/buffer_get_token_pred.c \
  lib/buffer/buffer_get_until.c \
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
  lib/buffer/buffer_put8long.c \
  lib/buffer/buffer_putalign.c \
  lib/buffer/buffer_putc.c \
  lib/buffer/buffer_putdouble.c \
  lib/buffer/buffer_puterror.c \
  lib/buffer/buffer_puterror2.c \
  lib/buffer/buffer_putflush.c \
  lib/buffer/buffer_putint64.c \
  lib/buffer/buffer_putlong.c \
  lib/buffer/buffer_putlong0.c \
  lib/buffer/buffer_putm_internal.c \
  lib/buffer/buffer_putm_internal_flush.c \
  lib/buffer/buffer_putnc.c \
  lib/buffer/buffer_putnlflush.c \
  lib/buffer/buffer_putnspace.c \
  lib/buffer/buffer_putptr.c \
  lib/buffer/buffer_puts.c \
  lib/buffer/buffer_putsa.c \
  lib/buffer/buffer_putsaflush.c \
  lib/buffer/buffer_putsalign.c \
  lib/buffer/buffer_putsflush.c \
  lib/buffer/buffer_putspace.c \
  lib/buffer/buffer_putspad.c \
  lib/buffer/buffer_puts_escaped.c \
  lib/buffer/buffer_putuint64.c \
  lib/buffer/buffer_putulong.c \
  lib/buffer/buffer_putulong0.c \
  lib/buffer/buffer_putxint64.c \
  lib/buffer/buffer_putxint640.c \
  lib/buffer/buffer_putxlong.c \
  lib/buffer/buffer_putxlong0.c \
  lib/buffer/buffer_read.c \
  lib/buffer/buffer_read_fd.c \
  lib/buffer/buffer_seek.c \
  lib/buffer/buffer_skipc.c \
  lib/buffer/buffer_skipn.c \
  lib/buffer/buffer_skipspace.c \
  lib/buffer/buffer_skip_pred.c \
  lib/buffer/buffer_skip_until.c \
  lib/buffer/buffer_stubborn.c \
  lib/buffer/buffer_stubborn2.c \
  lib/buffer/buffer_truncfile.c \
  lib/buffer/buffer_write.c \
  lib/buffer/buffer_write_fd.c \
  lib/buffer/stralloc_write.c \
  lib/byte/byte_case_diff.c \
  lib/byte/byte_case_equal.c \
  lib/byte/byte_case_start.c \
  lib/byte/byte_ccopy.c \
  lib/byte/byte_chr.c \
  lib/byte/byte_copy.c \
  lib/byte/byte_copyr.c \
  lib/byte/byte_count.c \
  lib/byte/byte_diff.c \
  lib/byte/byte_equal.c \
  lib/byte/byte_fill.c \
  lib/byte/byte_findb.c \
  lib/byte/byte_finds.c \
  lib/byte/byte_fmt.c \
  lib/byte/byte_lower.c \
  lib/byte/byte_rchr.c \
  lib/byte/byte_replace.c \
  lib/byte/byte_scan.c \
  lib/byte/byte_upper.c \
  lib/byte/byte_zero.c \
  lib/env/env_addmodif.c \
  lib/env/env_get.c \
  lib/env/env_get2.c \
  lib/env/env_len.c \
  lib/env/env_make.c \
  lib/env/env_merge.c \
  lib/env/env_pick.c \
  lib/env/env_put.c \
  lib/env/env_set.c \
  lib/env/env_string.c \
  lib/env/env_unset.c \
  lib/errmsg/errmsg_iam.c \
  lib/errmsg/errmsg_info.c \
  lib/errmsg/errmsg_infosys.c \
  lib/errmsg/errmsg_puts.c \
  lib/errmsg/errmsg_warn.c \
  lib/errmsg/errmsg_warnsys.c \
  lib/errmsg/errmsg_write.c \
  lib/path/path_absolute.c \
  lib/path/path_absolute_sa.c \
  lib/path/path_basename.c \
  lib/path/path_canonical.c \
  lib/path/path_canonicalize.c \
  lib/path/path_canonical_sa.c \
  lib/path/path_collapse.c \
  lib/path/path_dirname.c \
  lib/path/path_exists.c \
  lib/path/path_fnmatch.c \
  lib/path/path_getcwd.c \
  lib/path/path_gethome.c \
  lib/path/path_getsep.c \
  lib/path/path_is_absolute.c \
  lib/path/path_is_separator.c \
  lib/path/path_len.c \
  lib/path/path_len_s.c \
  lib/path/path_num.c \
  lib/path/path_num_sa.c \
  lib/path/path_readlink.c \
  lib/path/path_realpath.c \
  lib/path/path_relative.c \
  lib/path/path_right.c \
  lib/path/path_skip.c \
  lib/path/path_skips.c \
  lib/path/path_skip_separator.c \
  lib/path/path_split.c \
  lib/str/str_basename.c \
  lib/str/str_case_diff.c \
  lib/str/str_case_diffn.c \
  lib/str/str_case_equal.c \
  lib/str/str_case_start.c \
  lib/str/str_cat.c \
  lib/str/str_chr.c \
  lib/str/str_copy.c \
  lib/str/str_copyb.c \
  lib/str/str_copyn.c \
  lib/str/str_diff.c \
  lib/str/str_diffn.c \
  lib/str/str_dup.c \
  lib/str/str_end.c \
  lib/str/str_endb.c \
  lib/str/str_equal.c \
  lib/str/str_find.c \
  lib/str/str_findb.c \
  lib/str/str_istr.c \
  lib/str/str_len.c \
  lib/str/str_lower.c \
  lib/str/str_ndup.c \
  lib/str/str_ptime.c \
  lib/str/str_rchr.c \
  lib/str/str_rchrs.c \
  lib/str/str_rfind.c \
  lib/str/str_rfindb.c \
  lib/str/str_start.c \
  lib/str/str_tok.c \
  lib/stralloc/buffer_getline_sa.c \
  lib/stralloc/buffer_getnewline_sa.c \
  lib/stralloc/buffer_gettok_sa.c \
  lib/stralloc/buffer_get_new_token_sa.c \
  lib/stralloc/buffer_get_new_token_sa_pred.c \
  lib/stralloc/buffer_get_token_sa.c \
  lib/stralloc/buffer_get_token_sa_pred.c \
  lib/stralloc/buffer_put_escaped.c \
  lib/stralloc/buffer_tosa.c \
  lib/stralloc/stralloc_0.c \
  lib/stralloc/stralloc_append.c \
  lib/stralloc/stralloc_case_diff.c \
  lib/stralloc/stralloc_case_diffs.c \
  lib/stralloc/stralloc_case_end.c \
  lib/stralloc/stralloc_case_equal.c \
  lib/stralloc/stralloc_case_equals.c \
  lib/stralloc/stralloc_case_start.c \
  lib/stralloc/stralloc_case_starts.c \
  lib/stralloc/stralloc_cat.c \
  lib/stralloc/stralloc_catb.c \
  lib/stralloc/stralloc_catc.c \
  lib/stralloc/stralloc_catdouble.c \
  lib/stralloc/stralloc_cathexb.c \
  lib/stralloc/stralloc_catint.c \
  lib/stralloc/stralloc_catint0.c \
  lib/stralloc/stralloc_catlong.c \
  lib/stralloc/stralloc_catlong0.c \
  lib/stralloc/stralloc_catm_internal.c \
  lib/stralloc/stralloc_cats.c \
  lib/stralloc/stralloc_catuint.c \
  lib/stralloc/stralloc_catuint0.c \
  lib/stralloc/stralloc_catulong.c \
  lib/stralloc/stralloc_catulong0.c \
  lib/stralloc/stralloc_catwcb.c \
  lib/stralloc/stralloc_catwcs.c \
  lib/stralloc/stralloc_catxlong.c \
  lib/stralloc/stralloc_chomp.c \
  lib/stralloc/stralloc_chop.c \
  lib/stralloc/stralloc_chr.c \
  lib/stralloc/stralloc_contains.c \
  lib/stralloc/stralloc_copy.c \
  lib/stralloc/stralloc_copyb.c \
  lib/stralloc/stralloc_copys.c \
  lib/stralloc/stralloc_copywcb.c \
  lib/stralloc/stralloc_copywcs.c \
  lib/stralloc/stralloc_count.c \
  lib/stralloc/stralloc_decamelize.c \
  lib/stralloc/stralloc_diff.c \
  lib/stralloc/stralloc_diffb.c \
  lib/stralloc/stralloc_diffs.c \
  lib/stralloc/stralloc_endb.c \
  lib/stralloc/stralloc_ends.c \
  lib/stralloc/stralloc_equal.c \
  lib/stralloc/stralloc_equalb.c \
  lib/stralloc/stralloc_equals.c \
  lib/stralloc/stralloc_erase.c \
  lib/stralloc/stralloc_expand.c \
  lib/stralloc/stralloc_find.c \
  lib/stralloc/stralloc_findb.c \
  lib/stralloc/stralloc_finds.c \
  lib/stralloc/stralloc_fmt.c \
  lib/stralloc/stralloc_fmt_call.c \
  lib/stralloc/stralloc_fmt_pred.c \
  lib/stralloc/stralloc_free.c \
  lib/stralloc/stralloc_init.c \
  lib/stralloc/stralloc_insert.c \
  lib/stralloc/stralloc_insertb.c \
  lib/stralloc/stralloc_inserts.c \
  lib/stralloc/stralloc_lower.c \
  lib/stralloc/stralloc_move.c \
  lib/stralloc/stralloc_nul.c \
  lib/stralloc/stralloc_prepend.c \
  lib/stralloc/stralloc_prependb.c \
  lib/stralloc/stralloc_prepends.c \
  lib/stralloc/stralloc_rchr.c \
  lib/stralloc/stralloc_ready.c \
  lib/stralloc/stralloc_readyplus.c \
  lib/stralloc/stralloc_ready_tuned.c \
  lib/stralloc/stralloc_remove.c \
  lib/stralloc/stralloc_removesuffixb.c \
  lib/stralloc/stralloc_removesuffixs.c \
  lib/stralloc/stralloc_remove_all.c \
  lib/stralloc/stralloc_replace.c \
  lib/stralloc/stralloc_replacec.c \
  lib/stralloc/stralloc_replaces.c \
  lib/stralloc/stralloc_replace_non_printable.c \
  lib/stralloc/stralloc_reverse.c \
  lib/stralloc/stralloc_reverse_blocks.c \
  lib/stralloc/stralloc_scan.c \
  lib/stralloc/stralloc_shrink.c \
  lib/stralloc/stralloc_start.c \
  lib/stralloc/stralloc_startb.c \
  lib/stralloc/stralloc_starts.c \
  lib/stralloc/stralloc_subst.c \
  lib/stralloc/stralloc_trim.c \
  lib/stralloc/stralloc_trunc.c \
  lib/stralloc/stralloc_upper.c \
  lib/stralloc/stralloc_zero.c \
  lib/stralloc/str_decamelize.c \
  lib/strarray/strarray_appends.c \
  lib/strarray/strarray_each.c \
  lib/strarray/strarray_from_argv.c \
  lib/strarray/strarray_glob.c \
  lib/strarray/strarray_index_of.c \
  lib/strarray/strarray_joins.c \
  lib/strarray/strarray_pop.c \
  lib/strarray/strarray_prepends.c \
  lib/strarray/strarray_push.c \
  lib/strarray/strarray_pushb.c \
  lib/strarray/strarray_pushd.c \
  lib/strarray/strarray_push_sa.c \
  lib/strarray/strarray_removeprefixs.c \
  lib/strarray/strarray_removesuffixs.c \
  lib/strarray/strarray_set.c \
  lib/strarray/strarray_sort.c \
  lib/strarray/strarray_to_argv.c \
  lib/strarray/strarray_transform.c \
  lib/strlist/strlist_append_sa.c \
  lib/strlist/strlist_at.c \
  lib/strlist/strlist_at_n.c \
  lib/strlist/strlist_at_sa.c \
  lib/strlist/strlist_cat.c \
  lib/strlist/strlist_contains.c \
  lib/strlist/strlist_containsb.c \
  lib/strlist/strlist_contains_sa.c \
  lib/strlist/strlist_count.c \
  lib/strlist/strlist_dump.c \
  lib/strlist/strlist_fromb.c \
  lib/strlist/strlist_froms.c \
  lib/strlist/strlist_fromv.c \
  lib/strlist/strlist_indexofb.c \
  lib/strlist/strlist_index_of.c \
  lib/strlist/strlist_join.c \
  lib/strlist/strlist_joins.c \
  lib/strlist/strlist_pop.c \
  lib/strlist/strlist_prependb.c \
  lib/strlist/strlist_prepends.c \
  lib/strlist/strlist_prepend_sa.c \
  lib/strlist/strlist_push.c \
  lib/strlist/strlist_pushb.c \
  lib/strlist/strlist_pushb_unique.c \
  lib/strlist/strlist_pushm_internal.c \
  lib/strlist/strlist_push_sa.c \
  lib/strlist/strlist_push_tokens.c \
  lib/strlist/strlist_push_unique.c \
  lib/strlist/strlist_push_unique_sa.c \
  lib/strlist/strlist_range.c \
  lib/strlist/strlist_removeb.c \
  lib/strlist/strlist_removes.c \
  lib/strlist/strlist_shift.c \
  lib/strlist/strlist_shift_n.c \
  lib/strlist/strlist_sort.c \
  lib/strlist/strlist_sub.c \
  lib/strlist/strlist_to_argv.c \
  lib/strlist/strlist_trunc.c \
  lib/strlist/strlist_unshift.c \
  msys-shell.c
