prefix = /usr/local
bindir = ${prefix}/bin

DEBUG = 1

INSTALL = install

BOOST_INCLUDE_DIR = C:/Boost
BOOST_LIB_DIR = C:/Boost
BOOST_LIBS = boost_random

BUILD := $(shell gcc -dumpmachine)

CC = gcc
CXX = g++

#CXX += -std=c++11
CCVER := $(shell gcc -dumpversion)
CXXVER := $(shell gcc -dumpversion)

ifeq ($(CROSS),)
HOST ?= $(BUILD)
else
HOST := $(shell $(CROSS)gcc -dumpmachine)
endif

ifneq ($(HOST),)
TRIPLET = $(subst -, ,$(HOST))
endif

ifeq ($(CROSS),)
	CROSS := $(HOST)-
endif

#ifneq ($(HOST),$(BUILD))
BUILDDIR = build/$(HOST)/
#endif

ifneq ($(TRIPLET),)
ARCH = $(word 1,$(TRIPLET))
OS = $(word 3,$(TRIPLET))
endif

$(info OS: $(OS))

ifeq ($(ARCH),x86_64)
M64 = 64
endif

ifeq ($(OS),mingw32)
EXEEXT = .exe
LDFLAGS = -s -static
CXXOPTS := -std=c++11 -DCXX11=1
endif

#CXXOPTS := $(shell  $(CXX) -std=c++0x  2>&1 | grep -q 'unrecognized command line option'
#CXXOPTS = $(shell  sh -c "if !  { $(CXX) -std=c++0x  2>&1 | grep -q 'unrecognized command line option'; }; then echo -std=c++0x -D__GXX_EXPERIMENTAL_CXX0X__=1 -D_GLIBCXX_PERMIT_BACKWARD_HASH=1; elif !  { $(CXX) -std=c++11 2>&1 | grep -q 'unrecognized command line option'; }; then echo -std=c++11 -DCXX11=1; fi")
ifeq ($(CXXOPTS),)
CXXOPTS := $(shell  sh -c "if !  { $(CXX) -std=c++11 2>&1 | grep -q 'unrecognized command line option'; }; then echo -std=c++11 -DCXX11=1; elif ! { $(CXX) -std=c++0x  2>&1 | grep -q 'unrecognized command line option'; }; then echo -std=c++0x -D__GXX_EXPERIMENTAL_CXX0X__=1; fi")
endif

#CPPFLAGS = -I/usr/include/libowfat 
#CPPFLAGS = -I. -D__USE_BSD=1
CPPFLAGS = -I. -DINLINE=inline -DPATH_MAX=4096 -D_LARGEFILE_SOURCE=1 -D_GNU_SOURCE=1 -D_FILE_OFFSET_BITS=64
ifeq ($(DEBUG),1)
CFLAGS = -g -ggdb -O0 
else
CFLAGS = -g -O2 -Wall
endif
ifeq ($(STATIC),1)
LDFLAGS += -static
endif
ifeq ($(STRIP),1)
LDFLAGS += -s
endif
CXXFLAGS = $(CFLAGS)
#LIBS = -lowfat
RM = rm -f 

ifneq ($(BOOST_INCLUDE_DIR),)
CXXFLAGS += -I$(BOOST_INCLUDE_DIR)
endif

ifneq ($(BOOST_LIB_DIR),)
LIBS += -L$(BOOST_LIB_DIR) $(patsubst %,-l%,$(BOOST_LIBS))
endif

LIB_OBJ = $(patsubst %.o,$(BUILDDIR)%.o,buffer_close.o buffer_default.o buffer_dump.o buffer_feed.o buffer_flush.o buffer_free.o buffer_fromsa.o buffer_fromstr.o buffer_get.o buffer_get_new_token_sa.o buffer_get_new_token_sa_pred.o buffer_get_token.o buffer_get_token_pred.o buffer_get_token_sa.o buffer_get_token_sa_pred.o buffer_get_until.o buffer_getc.o buffer_getline.o buffer_getline_sa.o buffer_init.o buffer_mmapprivate.o buffer_mmapread.o buffer_mmapread_fd.o buffer_prefetch.o buffer_put.o buffer_putc.o buffer_putflush.o buffer_putm_internal.o buffer_putnlflush.o buffer_putnspace.o buffer_puts.o buffer_putsa.o buffer_putsflush.o buffer_putspace.o buffer_putulong.o buffer_skip_until.o buffer_stubborn.o buffer_stubborn2.o buffer_tosa.o buffer_truncfile.o byte_chr.o byte_copy.o byte_copyr.o byte_fill.o dir_close.o dir_open.o dir_read.o dir_type.o dir_time.o fmt_minus.o fmt_ulong.o fmt_ulong0.o mmap_private.o mmap_read.o mmap_read_fd.o mmap_unmap.o mmap_map.o  open_append.o open_read.o open_rw.o open_trunc.o shell_alloc.o shell_error.o shell_errorn.o shell_init.o shell_realloc.o str_diffn.o str_len.o stralloc_append.o stralloc_cat.o stralloc_catb.o stralloc_catc.o stralloc_catlong0.o stralloc_cats.o stralloc_catulong0.o stralloc_copy.o stralloc_copyb.o stralloc_copys.o stralloc_diffs.o stralloc_free.o stralloc_init.o stralloc_insertb.o stralloc_move.o stralloc_nul.o stralloc_ready.o stralloc_readyplus.o stralloc_remove.o stralloc_trunc.o stralloc_write.o stralloc_zero.o file.o directory_iterator.o)

LIBS += -lstdc++

PROGRAMS = $(BUILDDIR)list-r$(M64)$(EXEEXT) $(BUILDDIR)count-depth$(M64)$(EXEEXT) $(BUILDDIR)decode-ls-lR$(M64)$(EXEEXT) $(BUILDDIR)torrent-progress$(M64)$(EXEEXT)  kbd-adjacency$(M64)$(EXEEXT)
OBJECTS = $(PROGRAMS:%=%.o) $(LIB_OBJ)

vpath $(BUILDDIR)

VPATH = $(BUILDDIR):.

all: $(BUILDDIR) $(PROGRAMS) 

$(BUILDDIR):
	-mkdir $(BUILDDIR)
	-md $(subst /,\,$(BUILDDIR))

$(BUILDDIR)decode-ls-lR.o: decode-ls-lR.c
$(BUILDDIR)decode-ls-lR$(M64)$(EXEEXT): $(BUILDDIR)decode-ls-lR.o $(LIB_OBJ)
	$(CROSS)$(CC) $(LDFLAGS) $(CFLAGS) -o $@ $(BUILDDIR)decode-ls-lR.o $(LIB_OBJ) $(LIBS)

$(BUILDDIR)count-depth.o: count-depth.c
$(BUILDDIR)count-depth$(M64)$(EXEEXT): $(BUILDDIR)count-depth.o $(LIB_OBJ)
	$(CROSS)$(CC) $(LDFLAGS) $(CFLAGS) -o $@ $(BUILDDIR)count-depth.o $(LIB_OBJ) $(LIBS)

$(BUILDDIR)list-r.o: list-r.c
$(BUILDDIR)list-r$(M64)$(EXEEXT): $(BUILDDIR)list-r.o $(LIB_OBJ)
	$(CROSS)$(CXX) $(CXXOPTS) $(LDFLAGS) $(CFLAGS) -o $@ $(BUILDDIR)list-r.o $(LIB_OBJ) $(LIBS)

$(BUILDDIR)torrent-progress.o: torrent-progress.c
$(BUILDDIR)torrent-progress$(M64)$(EXEEXT): $(BUILDDIR)torrent-progress.o $(LIB_OBJ)
	$(CROSS)$(CC) $(LDFLAGS) $(CFLAGS) -o $@ $(BUILDDIR)torrent-progress.o $(LIB_OBJ) $(LIBS)

$(BUILDDIR)kbd-adjacency.o: kbd-adjacency.cpp
$(BUILDDIR)kbd-adjacency$(M64)$(EXEEXT): $(BUILDDIR)kbd-adjacency.o $(LIB_OBJ)
	$(CROSS)$(CC) $(LDFLAGS) $(CFLAGS) -o $@ $(BUILDDIR)kbd-adjacency.o $(LIB_OBJ) $(LIBS)

.c.o:
	$(CROSS)$(CC) $(CPPFLAGS) $(CFLAGS) -c -o $@ $<

#%.o: %.c
#	$(CROSS)$(CC) $(CPPFLAGS) $(CFLAGS) -c -o $@ $<

$(BUILDDIR)%.o: %.c
	$(CROSS)$(CC) $(CPPFLAGS) $(CFLAGS) -c -o $@ $<

.cpp.o:
	$(CROSS)$(CXX) $(CXXOPTS) $(CPPFLAGS) $(CXXFLAGS) -c -o $(BUILDDIR)$(patsubst %.cpp,%.o,$<) $<

#%.o: %.cpp
#	$(CROSS)$(CXX) $(CXXOPTS) $(CPPFLAGS) $(CXXFLAGS) -c -o $(BUILDDIR)$(patsubst %.cpp,%.o,$<) $<

$(BUILDDIR)%.o: %.cpp
	$(CROSS)$(CXX) $(CXXOPTS) $(CPPFLAGS) $(CXXFLAGS) -c -o $(BUILDDIR)$(patsubst %.cpp,%.o,$<) $<

clean:
	$(RM) -f $(OBJECTS) list-r.o list-r$(M64)$(EXEEXT)

install: $(PROGRAMS)
	$(INSTALL) -d $(DESTDIR)$(bindir)
	$(INSTALL) -m 755 $(PROGRAMS) $(DESTDIR)$(bindir)
	$(INSTALL) -d $(DESTDIR)$(bindir)

uninstall: 
	@for PROGRAM in $(PROGRAMS); do \
		echo $(RM) $(DESTDIR)$(bindir)/$$PROGRAM; \
		$(RM) $(DESTDIR)$(bindir)/$$PROGRAM; \
  done


