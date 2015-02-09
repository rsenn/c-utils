prefix = /usr/local
bindir = ${prefix}/bin

DEBUG = 1
LARGEFILE = 1
WARNINGS = all
WARNINGS += error

INSTALL = install

BUILD := $(shell gcc -dumpmachine)

CC = gcc
CXX = g++

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

ifeq ($(DEBUG),1)
BUILDTYPE = debug
else
BUILDTYPE = release
endif

ifneq ($(HOST),$(BUILD))
BUILDDIR = build/$(HOST)/$(BUILDTYPE)/
else
ifneq ($(CROSS),)
BUILDDIR = build/$(HOST)/$(BUILDTYPE)/
endif
endif

ifeq ($(HOST),$(BUILD))
CROSS :=
endif

ifneq ($(TRIPLET),)
ARCH = $(word 1,$(TRIPLET))
OS = $(word 3,$(TRIPLET))
KERN = $(word 2,$(TRIPLET))
endif

ifneq ($(OS),linux)
ifeq ($(ARCH),x86_64)
M64 = 64
endif
endif

ifeq ($(OS),msys)
EXEEXT = .exe
STATIC := 1
endif

ifeq ($(OS),cygwin)
EXEEXT = .exe
endif

ifeq ($(OS),mingw32)
EXEEXT = .exe
<<<<<<< HEAD
LDFLAGS = -s -static
=======
STATIC := 1
#LDFLAGS = -s -static
CXXOPTS := -std=c++11
DEFS += CXX11=1
endif

ifneq ($(OS),linux)
BOOST_INCLUDE_DIR = C:/Boost
BOOST_LIB_DIR = C:/Boost
>>>>>>> 30dc815864d44ccc70f9aadaaa58dc05de955624
endif
BOOST_LIBS = boost_random

##CXXOPTS := $(shell  $(CXX) -std=c++0x  2>&1 | grep -q 'unrecognized command line option'
##CXXOPTS = $(shell  sh -c "if !  { $(CXX) -std=c++0x  2>&1 | grep -q 'unrecognized command line option'; }; then echo -std=c++0x -D__GXX_EXPERIMENTAL_CXX0X__=1 -D_GLIBCXX_PERMIT_BACKWARD_HASH=1; elif !  { $(CXX) -std=c++11 2>&1 | grep -q 'unrecognized command line option'; }; then echo -std=c++11 -DCXX11=1; fi")
ifeq ($(CXXOPTS),)
#CXXOPTS += $(shell  sh -c "if !  { $(CXX) -std=c++11 2>&1 | grep -q 'unrecognized command line option'; }; then echo -std=c++11 -DCXX11=1; elif ! { $(CXX) -std=c++0x  2>&1 | grep -q 'unrecognized command line option'; }; then echo -std=c++0x -D__GXX_EXPERIMENTAL_CXX0X__=1; fi")
$(info OS: '$(OS)')
ifneq ($(OS),msys)
##CXXOPTS += -std=c++11 -DCXX11=1
endif
endif

#CPPFLAGS = -I/usr/include/libowfat
#CPPFLAGS = -I. -D__USE_BSD=1
CPPFLAGS := -I.

DEFS += INLINE=inline
DEFS += PATH_MAX=4096

ifeq ($(LARGEFILE),1)
DEFS += _FILE_OFFSET_BITS=64
DEFS += _LARGEFILE_SOURCE=1
endif
DEFS += _GNU_SOURCE=1 

WARNINGS += no-strict-aliasing

ifeq ($(DEBUG),1)
CFLAGS = -g -ggdb -O0
DEFS += DEBUG=1
else
CFLAGS = -g -O2
DEFS += NDEBUG=1
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

CFLAGS += $(patsubst %,-W%,$(WARNINGS))
CPPFLAGS += $(patsubst %,-D%,$(DEFS))

#LIB_SRC = buffer_1.c buffer_close.c buffer_default.c buffer_dump.c buffer_feed.c buffer_flush.c buffer_free.c buffer_fromsa.c buffer_fromstr.c buffer_get.c buffer_get_new_token_sa.c buffer_get_new_token_sa_pred.c buffer_get_token.c buffer_get_token_pred.c buffer_get_token_sa.c buffer_get_token_sa_pred.c buffer_get_until.c buffer_getc.c buffer_getline.c buffer_getline_sa.c buffer_init.c buffer_mmapprivate.c buffer_mmapread.c buffer_mmapread_fd.c buffer_prefetch.c buffer_put.c buffer_putc.c buffer_putflush.c buffer_putm_internal.c buffer_putnlflush.c buffer_putnspace.c buffer_puts.c buffer_putsa.c buffer_putsflush.c buffer_putspace.c buffer_putulong.c buffer_putulonglong.c buffer_putxlong.c buffer_skip_until.c buffer_stubborn.c buffer_stubborn2.c buffer_tosa.c buffer_truncfile.c byte_chr.c byte_copy.c byte_copyr.c byte_diff.c byte_fill.c byte_rchr.c byte_zero.c dir_close.c dir_open.c dir_read.c dir_time.c dir_type.c fmt_minus.c fmt_ulong.c fmt_ulong0.c fmt_ulonglong.c fmt_xlong.c fmt_xlonglong.c mmap_map.c mmap_private.c mmap_read.c mmap_read_fd.c mmap_unmap.c open_append.c open_read.c open_rw.c open_trunc.c scan_fromhex.c scan_xlong.c scan_xlonglong.c shell_alloc.c shell_error.c shell_errorn.c shell_init.c shell_realloc.c str_diffn.c str_len.c stralloc_append.c stralloc_cat.c stralloc_catb.c stralloc_catc.c stralloc_catlong0.c stralloc_cats.c stralloc_catulong0.c stralloc_chop.c stralloc_copy.c stralloc_copyb.c stralloc_copys.c stralloc_diffs.c stralloc_free.c stralloc_init.c stralloc_insertb.c stralloc_move.c stralloc_nul.c stralloc_ready.c stralloc_readyplus.c stralloc_remove.c stralloc_trunc.c stralloc_write.c stralloc_zero.c
LIB_SRC = $(wildcard *_*.c)
LIB_OBJ = $(patsubst %.o,$(BUILDDIR)%.o,$(patsubst %.c,%.o,$(LIB_SRC)))

LIBS += -lstdc++

<<<<<<< HEAD
PROGRAMS = $(BUILDDIR)list-r$(M64)$(EXEEXT) $(BUILDDIR)count-depth$(M64)$(EXEEXT) $(BUILDDIR)decode-ls-lR$(M64)$(EXEEXT) $(BUILDDIR)torrent-progress$(M64)$(EXEEXT) #kbd-adjacency$(M64)$(EXEEXT)
=======
PROGRAMS = $(BUILDDIR)list-r$(M64)$(EXEEXT) $(BUILDDIR)count-depth$(M64)$(EXEEXT) $(BUILDDIR)decode-ls-lR$(M64)$(EXEEXT) $(BUILDDIR)torrent-progress$(M64)$(EXEEXT)  #kbd-adjacency$(M64)$(EXEEXT)
>>>>>>> 30dc815864d44ccc70f9aadaaa58dc05de955624
OBJECTS = $(PROGRAMS:%=%.o) $(LIB_OBJ)

vpath $(BUILDDIR)

VPATH = $(BUILDDIR):.

$(info ARCH: $(ARCH))
#$(info BOOST_INCLUDE_DIR: $(BOOST_INCLUDE_DIR))
#$(info BOOST_LIBS: $(BOOST_LIBS))
#$(info BOOST_LIB_DIR: $(BOOST_LIB_DIR))
$(info BUILD: $(BUILD))
$(info BUILDDIR: $(BUILDDIR))
$(info BUILDTYPE: $(BUILDTYPE))
#$(info CC: $(CC))
$(info CCVER: $(CCVER))
#$(info CFLAGS: $(CFLAGS))
#$(info CPPFLAGS: $(CPPFLAGS))
$(info CROSS: $(CROSS))
#$(info CXX: $(CXX))
#$(info CXXFLAGS: $(CXXFLAGS))
#$(info CXXOPTS: $(CXXOPTS))
$(info CXXVER: $(CXXVER))
#$(info DEBUG: $(DEBUG))
#$(info DESTDIR: $(DESTDIR))
#$(info EXEEXT: $(EXEEXT))
$(info HOST: $(HOST))
#$(info INSTALL: $(INSTALL))
$(info KERN: $(KERN))
#$(info LDFLAGS: $(LDFLAGS))
#$(info LIBS: $(LIBS))
#$(info LIB_OBJ: $(LIB_OBJ))
#$(info LIB_SRC: $(LIB_SRC))
$(info M64: $(M64))
#$(info OBJECTS: $(OBJECTS))
$(info OS: $(OS))
#$(info PROGRAM: $(PROGRAM))
#$(info PROGRAMS: $(PROGRAMS))
#$(info RM: $(RM))
$(info STATIC: $(STATIC))
#$(info STRIP: $(STRIP))
$(info TRIPLET: $(TRIPLET))
#$(info VPATH: $(VPATH))

all: $(BUILDDIR) $(PROGRAMS)

$(BUILDDIR):
	-mkdir -p $(BUILDDIR) || mkdir $(BUILDDIR)
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

ifeq ($(BUILDDIR),)
.c.o:
	$(CROSS)$(CC) $(CPPFLAGS) $(CFLAGS) -c $<

$(BUILDDIR)%.o: %.c
	$(CROSS)$(CC) $(CPPFLAGS) $(CFLAGS) -c $<

.cpp.o:
	$(CROSS)$(CXX) $(CXXOPTS) $(CPPFLAGS) $(CXXFLAGS) -c $<

$(BUILDDIR)%.o: %.cpp
	$(CROSS)$(CXX) $(CXXOPTS) $(CPPFLAGS) $(CXXFLAGS) -c $<
else
.c.o:
	$(CROSS)$(CC) $(CPPFLAGS) $(CFLAGS) -c $<

$(BUILDDIR)%.o: %.c
	$(CROSS)$(CC) $(CPPFLAGS) $(CFLAGS) -c -o $(BUILDDIR)$(patsubst %.c,%.o,$<) $<

.cpp.o:
	$(CROSS)$(CXX) $(CXXOPTS) $(CPPFLAGS) $(CXXFLAGS) -c $<

$(BUILDDIR)%.o: %.cpp
	$(CROSS)$(CXX) $(CXXOPTS) $(CPPFLAGS) $(CXXFLAGS) -c -o $(BUILDDIR)$(patsubst %.cpp,%.o,$<) $<
endif

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


