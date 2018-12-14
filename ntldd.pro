TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

TARGET = ntldd

INCLUDEPATH += .

HEADERS = lib/buffer.h lib/byte.h lib/fmt.h lib/mmap.h lib/open.h lib/path.h lib/pe.h lib/str.h lib/stralloc.h lib/strlist.h


SOURCES = lib/buffer/buffer_1.c lib/buffer/buffer_2.c lib/buffer/buffer_flush.c lib/buffer/buffer_put.c lib/buffer/buffer_putc.c lib/buffer/buffer_putflush.c lib/buffer/buffer_putlong.c lib/buffer/buffer_putm_internal.c lib/buffer/buffer_putnc.c lib/buffer/buffer_putnlflush.c lib/buffer/buffer_putnspace.c lib/buffer/buffer_putptr.c lib/buffer/buffer_puts.c lib/buffer/buffer_putspace.c lib/buffer/buffer_putspad.c lib/buffer/buffer_putulong.c lib/buffer/buffer_putulong0.c lib/buffer/buffer_putxint640.c lib/buffer/buffer_stubborn.c lib/byte/byte_chr.c lib/byte/byte_copy.c lib/byte/byte_copyr.c lib/byte/byte_diff.c lib/byte/byte_equal.c lib/byte/byte_fill.c lib/byte/byte_replace.c lib/byte/byte_zero.c lib/fmt/fmt_long.c lib/fmt/fmt_ulong.c lib/fmt/fmt_xint64.c lib/mmap/mmap_read.c lib/mmap/mmap_unmap.c lib/open/open_read.c lib/path/path_exists.c lib/path/path_getcwd.c lib/path/path_getsep.c lib/path/path_is_absolute.c lib/pe/pe_get_datadir.c lib/pe/pe_header.c lib/pe/pe_rva2offset.c lib/pe/pe_rva2ptr.c lib/pe/pe_rva2section.c lib/str/str_basename.c lib/str/str_case_diff.c lib/str/str_case_diffn.c lib/str/str_copyn.c lib/str/str_diff.c lib/str/str_dup.c lib/str/str_len.c lib/str/str_rchr.c lib/stralloc/stralloc_append.c lib/stralloc/stralloc_cat.c lib/stralloc/stralloc_catb.c lib/stralloc/stralloc_catc.c lib/stralloc/stralloc_cats.c lib/stralloc/stralloc_copyb.c lib/stralloc/stralloc_copys.c lib/stralloc/stralloc_endb.c lib/stralloc/stralloc_free.c lib/stralloc/stralloc_init.c lib/stralloc/stralloc_insertb.c lib/stralloc/stralloc_nul.c lib/stralloc/stralloc_prepend.c lib/stralloc/stralloc_prependb.c lib/stralloc/stralloc_ready.c lib/stralloc/stralloc_readyplus.c lib/stralloc/stralloc_replacec.c lib/stralloc/stralloc_zero.c lib/strlist/strlist_contains.c lib/strlist/strlist_contains_sa.c lib/strlist/strlist_count.c lib/strlist/strlist_dump.c lib/strlist/strlist_froms.c lib/strlist/strlist_push_sa.c lib/strlist/strlist_push_unique_sa.c lib/strlist/strlist_unshift.c lib/unix/getopt.c ntldd.c \
    lib/path/path_find.c \
    lib/dir/dir_read.c \
    lib/dir/dir_open.c \
    lib/dir/dir_close.c \
    lib/str/str_case_equal.c
x
DEFINES += io_seek=lseek64
gcc: QMAKE_CFLAGS_WARN_ON += -Wno-sign-compare -Wno-unused-variable -Wno-unused-parameter -Wno-unused-but-set-variable
