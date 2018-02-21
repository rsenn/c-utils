
INSTALL = install
CC = gcc
CXX = g++
#CPPFLAGS = -I/usr/include/libowfat
#CPPFLAGS = -I. -D__USE_BSD=1 
CPPFLAGS = -I.  -DPATHLEN=4096 -DUSE_READDIR=1 -DINLINE=__inline__ -DUSE_READDIR=1
CFLAGS = -g -O2 -Wall
CXXFLAGS = $(CFLAGS)
#LIBS = -lowfat
EXEEXT =
HOST = $(shell $(CC) -dumpmachine |sed 's,.*-,,')
RM = rm -f

prefix := `$(CC)  -print-search-dirs|sed -n "s|^[^:]*: =\?\(/\?[^/]\+\)/.*|\1|p" | head -n1`
bindir = ${prefix}/bin

#EXTRA_DEP = $(EXTRA_MODULES:%=%.o)
EXTRA_MODULES = 
SOURCES = array_allocate.c array_bytes.c array_cat.c array_cat0.c array_catb.c array_cate.c array_cats.c array_cats0.c array_equal.c array_fail.c array_get.c array_length.c array_reset.c array_start.c array_trunc.c array_truncate.c buffer_0.c buffer_1.c buffer_2.c buffer_close.c buffer_default.c buffer_feed.c buffer_flush.c buffer_free.c buffer_fromsa.c buffer_fromstr.c buffer_get.c buffer_get_new_token_sa.c buffer_get_new_token_sa_pred.c buffer_get_token.c buffer_get_token_pred.c buffer_get_token_sa.c buffer_get_token_sa_pred.c buffer_get_until.c buffer_getc.c buffer_getline.c buffer_getline_sa.c buffer_init.c buffer_mmapprivate.c buffer_mmapread.c buffer_mmapread_fd.c buffer_prefetch.c buffer_put.c buffer_putc.c buffer_putflush.c buffer_putm_internal.c buffer_putnlflush.c buffer_putnspace.c buffer_puts.c buffer_putsa.c buffer_putsflush.c buffer_putspace.c buffer_putuint64.c buffer_putulong.c buffer_putulonglong.c buffer_putxlong.c buffer_skip_until.c buffer_stubborn.c buffer_stubborn2.c buffer_tosa.c buffer_truncfile.c byte_chr.c byte_copy.c byte_copyr.c byte_diff.c byte_fill.c byte_zero.c dir_close.c dir_open.c dir_read.c dir_time.c dir_type.c fmt_minus.c fmt_uint64.c fmt_ulong.c fmt_ulong0.c fmt_ulonglong.c fmt_xlong.c fmt_xlonglong.c mmap_map.c mmap_private.c mmap_read.c mmap_read_fd.c mmap_unmap.c open_append.c open_read.c open_rw.c open_trunc.c scan_fromhex.c scan_xlong.c scan_xlonglong.c str_diffn.c str_len.c stralloc_append.c stralloc_cat.c stralloc_catb.c stralloc_catc.c stralloc_catlong0.c stralloc_cats.c stralloc_catulong0.c stralloc_copy.c stralloc_copyb.c stralloc_copys.c stralloc_diffs.c stralloc_free.c stralloc_init.c stralloc_insertb.c stralloc_move.c stralloc_nul.c stralloc_ready.c stralloc_readyplus.c stralloc_remove.c stralloc_trunc.c stralloc_write.c stralloc_zero.c strlist_append.c strlist_append_unique.c strlist_at.c strlist_count.c strlist_index_of.c strlist_sort.c
LIB_SOURCES = array_allocate.c array_bytes.c array_cat.c array_cat0.c array_catb.c array_cate.c array_cats.c array_cats0.c array_equal.c array_fail.c array_get.c array_length.c array_reset.c array_start.c array_trunc.c array_truncate.c buffer_0.c buffer_1.c buffer_2.c buffer_close.c buffer_default.c buffer_feed.c buffer_flush.c buffer_free.c buffer_fromsa.c buffer_fromstr.c buffer_get.c buffer_get_new_token_sa.c buffer_get_new_token_sa_pred.c buffer_get_token.c buffer_get_token_pred.c buffer_get_token_sa.c buffer_get_token_sa_pred.c buffer_get_until.c buffer_getc.c buffer_getline.c buffer_getline_sa.c buffer_init.c buffer_mmapprivate.c buffer_mmapread.c buffer_mmapread_fd.c buffer_prefetch.c buffer_put.c buffer_putc.c buffer_putflush.c buffer_putm_internal.c buffer_putnlflush.c buffer_putnspace.c buffer_puts.c buffer_putsa.c buffer_putsflush.c buffer_putspace.c buffer_putuint64.c buffer_putulong.c buffer_putulonglong.c buffer_putxlong.c buffer_skip_until.c buffer_stubborn.c buffer_stubborn2.c buffer_tosa.c buffer_truncfile.c byte_chr.c byte_copy.c byte_copyr.c byte_diff.c byte_fill.c byte_zero.c dir_close.c dir_open.c dir_read.c dir_time.c dir_type.c fmt_minus.c fmt_uint64.c fmt_ulong.c fmt_ulong0.c fmt_ulonglong.c fmt_xlong.c fmt_xlonglong.c mmap_map.c mmap_private.c mmap_read.c mmap_read_fd.c mmap_unmap.c open_append.c open_read.c open_rw.c open_trunc.c scan_fromhex.c scan_xlong.c scan_xlonglong.c str_diffn.c str_len.c stralloc_append.c stralloc_cat.c stralloc_catb.c stralloc_catc.c stralloc_catlong0.c stralloc_cats.c stralloc_catulong0.c stralloc_copy.c stralloc_copyb.c stralloc_copys.c stralloc_diffs.c stralloc_free.c stralloc_init.c stralloc_insertb.c stralloc_move.c stralloc_nul.c stralloc_ready.c stralloc_readyplus.c stralloc_remove.c stralloc_trunc.c stralloc_write.c stralloc_zero.c strlist_append.c strlist_append_unique.c strlist_at.c strlist_count.c strlist_index_of.c strlist_sort.c

#LIB_DEP = $(LIB_MODULES:%=%.o)
LIB_MODULES = $(LIB_SOURCES:%.c=%)

LIB_DEP = libowfat.a

PROGRAMS = list-r$(EXESUFFIX)$(EXEEXT) count-depth$(EXESUFFIX)$(EXEEXT) decode-ls-lR$(EXESUFFIX)$(EXEEXT) torrent-progress$(EXESUFFIX)$(EXEEXT)
all: $(LIB_DEP) $(PROGRAMS)

libowfat.a: $(LIB_MODULES:%=%.o)
	ar rcs $@ $^

decode-ls-lR.o: decode-ls-lR.c
decode-ls-lR$(EXESUFFIX)$(EXEEXT): decode-ls-lR.o $(EXTRA_DEP) $(LIB_DEP)
	$(CC) $(LDFLAGS) $(CFLAGS) -o $@ $^ $(LIBS)

count-depth.o: count-depth.c
count-depth$(EXESUFFIX)$(EXEEXT): count-depth.o $(EXTRA_DEP) $(LIB_DEP)
	$(CC) $(LDFLAGS) $(CFLAGS) -o $@ $^ $(LIBS)

list-r.o: list-r.c libowfat.a
list-r$(EXESUFFIX)$(EXEEXT): list-r.o $(EXTRA_DEP) $(LIB_DEP)
	$(CXX) $(LDFLAGS) $(CFLAGS) -o $@ $^ $(LIBS)

#list-r$(EXESUFFIX)$(EXEEXT): LIB_MODULES += file directory_iterator

torrent-progress.o: torrent-progress.c
torrent-progress$(EXESUFFIX)$(EXEEXT): torrent-progress.o $(EXTRA_DEP) $(LIB_DEP)
	$(CC) $(LDFLAGS) $(CFLAGS) -o $@ $^ $(LIBS)

.c.o:
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $<

.cpp.o:
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $<

clean:
	$(RM) -f $(EXTRA_MODULES:%=%.o) $(LIB_MODULES:%=%.o) list-r.o list-r$(EXESUFFIX)$(EXEEXT)

install: $(PROGRAMS)
	$(INSTALL) -d $(DESTDIR)$(bindir)
	$(INSTALL) -m 755 $(PROGRAMS) $(DESTDIR)$(bindir)
	$(INSTALL) -d $(DESTDIR)$(bindir)

