TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += link_pkgconfig
PKGCONFIG += libxml-2.0


TARGET = opensearch-dump

INCLUDEPATH += . $$PWD/lib

SOURCES = lib/buffer/buffer_0.c lib/buffer/buffer_0small.c lib/buffer/buffer_1.c lib/buffer/buffer_1small.c lib/buffer/buffer_2.c lib/buffer/buffer_close.c lib/buffer/buffer_default.c lib/buffer/buffer_dummyread.c lib/buffer/buffer_dummyreadmmap.c lib/buffer/buffer_dump.c lib/buffer/buffer_feed.c lib/buffer/buffer_flush.c lib/buffer/buffer_free.c lib/buffer/buffer_fromarray.c lib/buffer/buffer_frombuf.c lib/buffer/buffer_fromsa.c lib/buffer/buffer_fromstr.c lib/buffer/buffer_get.c lib/buffer/buffer_getc.c lib/buffer/buffer_getline.c lib/buffer/buffer_getline_sa.c lib/buffer/buffer_getn.c lib/buffer/buffer_getnewline_sa.c lib/buffer/buffer_get_new_token_sa.c lib/buffer/buffer_get_new_token_sa_pred.c lib/buffer/buffer_get_token.c lib/buffer/buffer_get_token_pred.c lib/buffer/buffer_get_token_sa.c lib/buffer/buffer_get_token_sa_pred.c lib/buffer/buffer_get_until.c lib/buffer/buffer_init.c lib/buffer/buffer_init_free.c lib/buffer/buffer_mmapprivate.c lib/buffer/buffer_mmapread.c lib/buffer/buffer_mmapread_fd.c lib/buffer/buffer_munmap.c lib/buffer/buffer_peek.c lib/buffer/buffer_prefetch.c lib/buffer/buffer_put.c lib/buffer/buffer_put8long.c lib/buffer/buffer_putalign.c lib/buffer/buffer_putc.c lib/buffer/buffer_puterror.c lib/buffer/buffer_puterror2.c lib/buffer/buffer_putflush.c lib/buffer/buffer_putlong.c lib/buffer/buffer_putlonglong.c lib/buffer/buffer_putm_internal.c lib/buffer/buffer_putm_internal_flush.c lib/buffer/buffer_putnlflush.c lib/buffer/buffer_putnspace.c lib/buffer/buffer_puts.c lib/buffer/buffer_putsa.c lib/buffer/buffer_putsaflush.c lib/buffer/buffer_putsalign.c lib/buffer/buffer_putsflush.c lib/buffer/buffer_putspace.c lib/buffer/buffer_putuint64.c lib/buffer/buffer_putulong.c lib/buffer/buffer_putulonglong.c lib/buffer/buffer_putxlong.c lib/buffer/buffer_read_fd.c lib/buffer/buffer_seek.c lib/buffer/buffer_skip_until.c lib/buffer/buffer_stubborn.c lib/buffer/buffer_stubborn2.c lib/buffer/buffer_tosa.c lib/buffer/buffer_truncfile.c lib/byte/byte_chr.c lib/byte/byte_copy.c lib/byte/byte_copyr.c lib/byte/byte_diff.c lib/byte/byte_fill.c lib/byte/byte_rchr.c lib/byte/byte_zero.c lib/stralloc/stralloc_append.c lib/stralloc/stralloc_append_sa.c lib/stralloc/stralloc_cat.c lib/stralloc/stralloc_catb.c lib/stralloc/stralloc_catc.c lib/stralloc/stralloc_catlong0.c lib/stralloc/stralloc_cats.c lib/stralloc/stralloc_catulong0.c lib/stralloc/stralloc_copy.c lib/stralloc/stralloc_copyb.c lib/stralloc/stralloc_copys.c lib/stralloc/stralloc_decamelize.c lib/stralloc/stralloc_diffs.c lib/stralloc/stralloc_free.c lib/stralloc/stralloc_init.c lib/stralloc/stralloc_insertb.c lib/stralloc/stralloc_move.c lib/stralloc/stralloc_nul.c lib/stralloc/stralloc_ready.c lib/stralloc/stralloc_readyplus.c lib/stralloc/stralloc_remove.c lib/stralloc/stralloc_trunc.c lib/stralloc/stralloc_write.c lib/stralloc/stralloc_zero.c lib/str/str_basename.c lib/str/str_case_diff.c lib/str/str_case_diffn.c lib/str/str_case_equal.c lib/str/str_case_start.c lib/str/str_cat.c lib/str/str_chr.c lib/str/str_copy.c lib/str/str_copyb.c lib/str/str_decamelize.c lib/str/str_diff.c lib/str/str_diffn.c lib/str/str_dup.c lib/str/str_equal.c lib/str/str_find.c lib/str/str_findb.c lib/str/str_istr.c lib/str/str_len.c lib/str/str_lower.c lib/str/str_ptime.c lib/str/str_rchr.c lib/str/str_start.c lib/str/str_tok.c opensearch-dump.c

HEADERS = lib/buffer.h lib/stralloc.h 


include(deployment.pri)
qtcAddDeployment()

