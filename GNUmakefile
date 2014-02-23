prefix = /usr/local
bindir = ${prefix}/bin

INSTALL = install
CC = gcc
#CPPFLAGS = -I/usr/include/libowfat 
#CPPFLAGS = -I. -D__USE_BSD=1
CPPFLAGS = -I.  -DPATH_MAX=4096
CFLAGS = -g -O2 -Wall
#LIBS = -lowfat
EXEEXT =
HOST = $(shell $(CC) -dumpmachine |sed 's,.*-,,')
RM = rm -f 


EXTRA_DEP = $(EXTRA_MODULES:%=%.o)
EXTRA_MODULES = \
  stralloc_catc \
	stralloc_nul \
  dir_close \
  dir_open \
  dir_read \
  dir_type \
  dir_time


LIB_DEP = $(LIB_MODULES:%=%.o)
LIB_MODULES = \
  buffer_close \
  buffer_default \
  buffer_dump \
  buffer_feed \
  buffer_flush \
  buffer_free \
  buffer_fromsa \
  buffer_fromstr \
  buffer_get \
  buffer_get_new_token_sa \
  buffer_get_new_token_sa_pred \
  buffer_get_token \
  buffer_get_token_pred \
  buffer_get_token_sa \
  buffer_get_token_sa_pred \
  buffer_get_until \
  buffer_getc \
  buffer_getline \
  buffer_getline_sa \
  buffer_init \
  buffer_mmapprivate \
  buffer_mmapread \
  buffer_mmapread_fd \
  buffer_prefetch \
  buffer_put \
  buffer_putc \
  buffer_putflush \
  buffer_putm_internal \
  buffer_putnlflush \
  buffer_putnspace \
  buffer_puts \
  buffer_putsa \
  buffer_putsflush \
  buffer_putspace \
  buffer_putulong \
  buffer_skip_until \
  buffer_stubborn \
  buffer_stubborn2 \
  buffer_tosa \
  buffer_truncfile \
  byte_chr \
  byte_copy \
  byte_copyr \
  byte_fill \
  fmt_minus \
  fmt_ulong \
  fmt_ulong0 \
  mmap_private \
  mmap_read \
  mmap_read_fd \
  mmap_unmap \
  open_read \
  open_trunc \
  shell_alloc \
  shell_error \
  shell_errorn \
  shell_init \
  shell_realloc \
  str_len \
  stralloc_append \
  stralloc_cat \
  stralloc_catb \
  stralloc_catlong0 \
  stralloc_cats \
  stralloc_catulong0 \
  stralloc_copy \
  stralloc_copyb \
  stralloc_copys \
  stralloc_diffs \
  stralloc_free \
  stralloc_init \
  stralloc_insertb \
  stralloc_move \
  stralloc_ready \
  stralloc_readyplus \
  stralloc_remove \
  stralloc_trunc \
  stralloc_write \
  stralloc_zero

PROGRAMS = list-r$(EXEEXT) count-depth$(EXEEXT) decode-ls-lR$(EXEEXT) torrent-progress$(EXEEXT)
all: $(PROGRAMS) 

decode-ls-lR.o: decode-ls-lR.c
decode-ls-lR$(EXEEXT): decode-ls-lR.o $(EXTRA_DEP) $(LIB_DEP)
	$(CC) $(LDFLAGS) $(CFLAGS) -o $@ $^ $(LIBS)

count-depth.o: count-depth.c
count-depth$(EXEEXT): count-depth.o $(EXTRA_DEP) $(LIB_DEP)
	$(CC) $(LDFLAGS) $(CFLAGS) -o $@ $^ $(LIBS)

list-r.o: list-r.c
list-r$(EXEEXT): list-r.o $(EXTRA_DEP) $(LIB_DEP)
	$(CC) $(LDFLAGS) $(CFLAGS) -o $@ $^ $(LIBS)

torrent-progress.o: torrent-progress.c
torrent-progress$(EXEEXT): torrent-progress.o $(EXTRA_DEP) $(LIB_DEP)
	$(CC) $(LDFLAGS) $(CFLAGS) -o $@ $^ $(LIBS)

.c.o:
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $<

clean:
	$(RM) -f $(EXTRA_MODULES:%=%.o) $(LIB_MODULES:%=%.o) list-r.o list-r$(EXEEXT)

install: $(PROGRAMS)
	$(INSTALL) -d $(DESTDIR)$(bindir)
	$(INSTALL) -m 755 $(PROGRAMS) $(DESTDIR)$(bindir)
	$(INSTALL) -d $(DESTDIR)$(bindir)

