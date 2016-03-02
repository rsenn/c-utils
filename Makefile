prefix = /usr/local
bindir = ${prefix}/bin

DEBUG = 1
LARGEFILE = 1
WARNINGS = all
#WARNINGS += error

INSTALL = install

CC ?= gcc
CXX ?= g++

BUILD := $(shell $(CC) -dumpmachine)

CCVER := $(shell $(CC) -dumpversion)
CXXVER := $(shell $(CXX) -dumpversion)

ifeq ($(CROSS),)
HOST ?= $(BUILD)
else
HOST := $(shell $(CROSS)$(CC) -dumpmachine |sed "s,[-.0-9]*\$,,")
endif

ifneq ($(HOST),)
TRIPLET := $(subst -, ,$(HOST))
endif

ifeq ($(CROSS),)
	CROSS := $(HOST)-
endif

ifeq ($(DEBUG),1)
BUILDTYPE = debug
else
BUILDTYPE = release
endif

ifeq ($(HOST),$(BUILD))
CROSS :=
endif

ifneq ($(TRIPLET),)
ARCH = $(word 1,$(TRIPLET))
OS := $(shell echo $(word 3,$(TRIPLET)) |sed 's,[0-9].*,,')
KERN = $(word 2,$(TRIPLET))
endif

$(info Arch: $(ARCH))
$(info OS: $(OS))

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
endif
BOOST_LIBS = boost_random

ifeq ($(OS),mingw32)
TOOLCHAIN = $(HOST)-$(shell $(CROSS)gcc -dumpversion)
else
TOOLCHAIN = $(HOST)
endif

ifneq ($(HOST),$(BUILD))
BUILDDIR = build/$(TOOLCHAIN)/$(BUILD_TYPE)/
else
ifeq ($(CROSS),)
BUILDDIR = build/$(TOOLCHAIN)/$(BUILD_TYPE)/
else
BUILDDIR = build/$(patsubst %-,%,$(CROSS))/$(BUILD_TYPE)/
endif
endif

ifeq ($(CXXOPTS),)
#$(info OS: '$(OS)')
ifneq ($(OS),msys)
endif
endif

CPPFLAGS := -I.

DEFS += INLINE=inline
DEFS += PATH_MAX=4096

ifeq ($(READDIR),)
DEFS += USE_READDIR=1
endif

ifeq ($(LARGEFILE),1)
DEFS += _FILE_OFFSET_BITS=64
DEFS += _LARGEFILE_SOURCE=1
endif
DEFS += _GNU_SOURCE=1 

WARNINGS += no-strict-aliasing

CFLAGS = -pipe

CFLAGS_Debug = -g -ggdb -O0
CFLAGS_MinSizeRel = -g -fomit-frame-pointer -Os
CFLAGS_RelWithDebInfo = -g -ggdb -O2
CFLAGS_Release = -g -fomit-frame-pointer -O2

CXXFLAGS = -pipe

CXXFLAGS_Debug = -g -ggdb -O0
CXXFLAGS_MinSizeRel = -g -fomit-frame-pointer -Os
CXXFLAGS_RelWithDebInfo = -g -ggdb -O2
CXXFLAGS_Release = -g -fomit-frame-pointer -O2

ifeq ($(BUILD_TYPE),)
ifeq ($(DEBUG),1)
ifeq ($(RELEASE),1)
BUILD_TYPE = RelWithDebInfo
else
BUILD_TYPE = Debug
endif
else
ifeq ($(MINSIZE),1)
BUILD_TYPE = MinSizeRel
else
BUILD_TYPE = Release
endif
endif
endif

ifeq ($(BUILD_TYPE),Debug)
DEBUG := 1
RELEASE := 0
MINSIZE := 0
endif
ifeq ($(BUILD_TYPE),RelWithDebInfo)
DEBUG := 1
RELEASE := 1
MINSIZE := 0
endif
ifeq ($(BUILD_TYPE),MinSizeRel)
DEBUG := 0
RELEASE := 1
MINSIZE := 1
endif
ifeq ($(BUILD_TYPE),Release)
DEBUG := 0
RELEASE := 1
MINSIZE := 0
endif

ifeq ($(DEBUG),1)
DEFS += DEBUG=1
else
DEFS += NDEBUG=1
endif

CFLAGS += $(CFLAGS_$(BUILD_TYPE))
CXXFLAGS += $(CXXFLAGS_$(BUILD_TYPE))

ifeq ($(STATIC),1)
LDFLAGS += -static
endif
ifeq ($(STRIP),1)
LDFLAGS += -s
endif
RM = rm -f

ifneq ($(BOOST_INCLUDE_DIR),)
CXXFLAGS += -I$(BOOST_INCLUDE_DIR)
endif

ifneq ($(BOOST_LIB_DIR),)
LIBS += -L$(BOOST_LIB_DIR) $(patsubst %,-l%,$(BOOST_LIBS))
endif

CFLAGS += $(patsubst %,-W%,$(WARNINGS))
CPPFLAGS += $(patsubst %,-D%,$(DEFS))

LIB_SRC = $(wildcard *_*.c umult*.c)
LIB_OBJ = $(patsubst %.o,$(BUILDDIR)%.o,$(patsubst %.c,%.o,$(LIB_SRC)))

LIBS += -lstdc++

PROGRAMS = $(BUILDDIR)list-r$(M64_)$(EXEEXT) $(BUILDDIR)count-depth$(M64_)$(EXEEXT) $(BUILDDIR)decode-ls-lR$(M64_)$(EXEEXT) $(BUILDDIR)reg2cmd$(M64_)$(EXEEXT) $(BUILDDIR)torrent-progress$(M64_)$(EXEEXT)  #kbd-adjacency$(M64_)$(EXEEXT)
OBJECTS = $(PROGRAMS:%=%.o) $(LIB_OBJ)

vpath $(BUILDDIR)

VPATH = $(BUILDDIR):.

#$(info ARCH: $(ARCH))
#$(info BUILD: $(BUILD))
#$(info BUILDDIR: $(BUILDDIR))
#$(info BUILDTYPE: $(BUILDTYPE))
#$(info CCVER: $(CCVER))
#$(info CROSS: $(CROSS))
#$(info CXXVER: $(CXXVER))
#$(info HOST: $(HOST))
#$(info TOOLCHAIN: $(TOOLCHAIN))
#$(info KERN: $(KERN))
#$(info M64: $(M64))
#$(info OS: $(OS))
#$(info STATIC: $(STATIC))
#$(info TRIPLET: $(TRIPLET))
ifeq ($(OS),darwin)
#DEFINES += USE_READDIR=1
CFLAGS += -DUSE_READDIR=1
CPPFLAGS += -DUSE_READDIR=1
endif

all: $(BUILDDIR) $(PROGRAMS)

$(BUILDDIR):
	-mkdir -p $(BUILDDIR) || mkdir $(BUILDDIR)
	-md $(subst /,\,$(BUILDDIR))

$(BUILDDIR)decode-ls-lR.o: decode-ls-lR.c
$(BUILDDIR)decode-ls-lR$(M64_)$(EXEEXT): $(BUILDDIR)decode-ls-lR.o $(BUILDDIR)buffer_stubborn2.o $(BUILDDIR)buffer_feed.o $(BUILDDIR)buffer_getc.o $(BUILDDIR)stralloc_ready.o $(BUILDDIR)buffer_get_token.o $(BUILDDIR)buffer_stubborn.o $(BUILDDIR)byte_copy.o $(BUILDDIR)str_len.o $(BUILDDIR)stralloc_copyb.o $(BUILDDIR)stralloc_readyplus.o $(BUILDDIR)buffer_0.o $(BUILDDIR)buffer_1.o $(BUILDDIR)buffer_2.o $(BUILDDIR)buffer_flush.o $(BUILDDIR)buffer_getline.o $(BUILDDIR)buffer_put.o $(BUILDDIR)buffer_puts.o $(BUILDDIR)byte_chr.o $(BUILDDIR)str_diffn.o $(BUILDDIR)stralloc_catb.o $(BUILDDIR)stralloc_copys.o
	$(CROSS)$(CC) $(LDFLAGS) $(CFLAGS) -o $@ $^ $(LIBS)

$(BUILDDIR)count-depth.o: count-depth.c
$(BUILDDIR)count-depth$(M64_)$(EXEEXT): $(BUILDDIR)count-depth.o $(BUILDDIR)buffer_stubborn2.o $(BUILDDIR)buffer_feed.o $(BUILDDIR)buffer_getc.o $(BUILDDIR)byte_chr.o $(BUILDDIR)buffer_get_token.o $(BUILDDIR)buffer_stubborn.o $(BUILDDIR)byte_copy.o $(BUILDDIR)buffer_0.o $(BUILDDIR)buffer_1.o $(BUILDDIR)buffer_flush.o $(BUILDDIR)buffer_getline.o $(BUILDDIR)buffer_put.o $(BUILDDIR)buffer_putulong.o $(BUILDDIR)fmt_ulong.o
	$(CROSS)$(CC) $(LDFLAGS) $(CFLAGS) -o $@ $^ $(LIBS)

$(BUILDDIR)list-r.o: list-r.c
$(BUILDDIR)list-r$(M64_)$(EXEEXT): $(BUILDDIR)list-r.o $(BUILDDIR)buffer_1.o $(BUILDDIR)buffer_2.o $(BUILDDIR)buffer_flush.o $(BUILDDIR)buffer_put.o $(BUILDDIR)buffer_puts.o $(BUILDDIR)buffer_putsa.o $(BUILDDIR)buffer_stubborn.o $(BUILDDIR)byte_copy.o $(BUILDDIR)dir_close.o $(BUILDDIR)dir_open.o $(BUILDDIR)dir_read.o $(BUILDDIR)dir_type.o $(BUILDDIR)fmt_ulong.o $(BUILDDIR)fmt_uint64.o  $(BUILDDIR)str_len.o $(BUILDDIR)stralloc_catb.o $(BUILDDIR)stralloc_cats.o $(BUILDDIR)stralloc_copyb.o $(BUILDDIR)stralloc_copys.o $(BUILDDIR)stralloc_init.o $(BUILDDIR)stralloc_nul.o $(BUILDDIR)stralloc_ready.o $(BUILDDIR)stralloc_readyplus.o
	$(CROSS)$(CXX) $(CXXOPTS) $(LDFLAGS) $(CFLAGS) -o $@ $^ $(LIBS)

$(BUILDDIR)reg2cmd.o: reg2cmd.c
$(BUILDDIR)reg2cmd$(M64_)$(EXEEXT): $(BUILDDIR)reg2cmd.o $(BUILDDIR)buffer_0.o $(BUILDDIR)buffer_1.o $(BUILDDIR)buffer_2.o $(BUILDDIR)buffer_feed.o $(BUILDDIR)buffer_flush.o $(BUILDDIR)buffer_get_token.o $(BUILDDIR)buffer_getc.o $(BUILDDIR)buffer_getline.o $(BUILDDIR)buffer_put.o $(BUILDDIR)buffer_putc.o $(BUILDDIR)buffer_puts.o $(BUILDDIR)buffer_putuint64.o $(BUILDDIR)buffer_stubborn.o $(BUILDDIR)buffer_stubborn2.o $(BUILDDIR)byte_chr.o $(BUILDDIR)byte_copy.o $(BUILDDIR)fmt_uint64.o $(BUILDDIR)scan_fromhex.o $(BUILDDIR)scan_xlong.o $(BUILDDIR)scan_xlonglong.o $(BUILDDIR)str_len.o $(BUILDDIR)stralloc_catb.o $(BUILDDIR)stralloc_init.o $(BUILDDIR)stralloc_ready.o $(BUILDDIR)stralloc_readyplus.o $(BUILDDIR)stralloc_zero.o
	$(CROSS)$(CXX) $(CXXOPTS) $(LDFLAGS) $(CFLAGS) -o $@ $^ $(LIBS)

$(BUILDDIR)torrent-progress.o: torrent-progress.c
$(BUILDDIR)torrent-progress$(M64_)$(EXEEXT): $(BUILDDIR)torrent-progress.o $(BUILDDIR)buffer_flush.o $(BUILDDIR)buffer_stubborn.o $(BUILDDIR)buffer_stubborn2.o $(BUILDDIR)buffer_feed.o $(BUILDDIR)buffer_put.o $(BUILDDIR)buffer_putflush.o $(BUILDDIR)byte_copy.o $(BUILDDIR)str_len.o $(BUILDDIR)buffer_1.o $(BUILDDIR)buffer_get.o $(BUILDDIR)buffer_putnlflush.o $(BUILDDIR)buffer_puts.o $(BUILDDIR)buffer_putulong.o $(BUILDDIR)fmt_ulong.o $(BUILDDIR)mmap_map.o $(BUILDDIR)mmap_unmap.o $(BUILDDIR)open_read.o
	$(CROSS)$(CC) $(LDFLAGS) $(CFLAGS) -o $@ $^ $(LIBS)

$(BUILDDIR)kbd-adjacency.o: kbd-adjacency.cpp
$(BUILDDIR)kbd-adjacency$(M64_)$(EXEEXT): $(BUILDDIR)kbd-adjacency.o $(LIB_OBJ)
	$(CROSS)$(CC) $(LDFLAGS) $(CFLAGS) -o $@ $^ $(LIBS)

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
	$(RM) -f $(OBJECTS) list-r.o list-r$(M64_)$(EXEEXT)

install: $(PROGRAMS)
	$(INSTALL) -d $(DESTDIR)$(bindir)
	$(INSTALL) -m 755 $(PROGRAMS) $(DESTDIR)$(bindir)
	$(INSTALL) -d $(DESTDIR)$(bindir)

uninstall:
	@for PROGRAM in $(PROGRAMS); do \
		echo $(RM) $(DESTDIR)$(bindir)/$$PROGRAM; \
		$(RM) $(DESTDIR)$(bindir)/$$PROGRAM; \
  done
LIBSOURCES = array.h array_allocate.c array_bytes.c array_cat.c array_cat0.c array_catb.c array_cate.c array_cats.c array_cats0.c array_equal.c array_fail.c array_get.c array_length.c array_reset.c array_start.c array_trunc.c array_truncate.c buffer.h buffer_0.c buffer_1.c buffer_2.c buffer_close.c buffer_default.c buffer_feed.c buffer_flush.c buffer_free.c buffer_fromsa.c buffer_fromstr.c buffer_get.c buffer_get_new_token_sa.c buffer_get_new_token_sa_pred.c buffer_get_token.c buffer_get_token_pred.c buffer_get_token_sa.c buffer_get_token_sa_pred.c buffer_get_until.c buffer_getc.c buffer_getline.c buffer_getline_sa.c buffer_init.c buffer_mmapprivate.c buffer_mmapread.c buffer_mmapread_fd.c buffer_prefetch.c buffer_put.c buffer_putc.c buffer_putflush.c buffer_putm_internal.c buffer_putnlflush.c buffer_putnspace.c buffer_puts.c buffer_putsa.c buffer_putsflush.c buffer_putspace.c buffer_putuint64.c buffer_putulong.c buffer_putulonglong.c buffer_putxlong.c buffer_skip_until.c buffer_stubborn.c buffer_stubborn2.c buffer_tosa.c buffer_truncfile.c byte.h byte_chr.c byte_copy.c byte_copyr.c byte_diff.c byte_fill.c byte_zero.c dir.h dir_close.c dir_internal.h dir_open.c dir_read.c dir_time.c dir_type.c fmt.h fmt_minus.c fmt_uint64.c fmt_ulong.c fmt_ulong0.c fmt_ulonglong.c fmt_xlong.c fmt_xlonglong.c mmap.h mmap_map.c mmap_private.c mmap_read.c mmap_read_fd.c mmap_unmap.c open.h open_append.c open_read.c open_rw.c open_trunc.c scan.h scan_fromhex.c scan_xlong.c scan_xlonglong.c str.h str_diffn.c str_len.c stralloc.h stralloc_append.c stralloc_cat.c stralloc_catb.c stralloc_catc.c stralloc_catlong0.c stralloc_cats.c stralloc_catulong0.c stralloc_copy.c stralloc_copyb.c stralloc_copys.c stralloc_diffs.c stralloc_free.c stralloc_init.c stralloc_insertb.c stralloc_move.c stralloc_nul.c stralloc_ready.c stralloc_readyplus.c stralloc_remove.c stralloc_trunc.c stralloc_write.c stralloc_zero.c strlist.h strlist_append.c strlist_append_unique.c strlist_at.c strlist_count.c strlist_index_of.c strlist_sort.c
