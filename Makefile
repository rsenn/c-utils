prefix = /usr/local
bindir = ${prefix}/bin

DEBUG = 1
LARGEFILE = 1
WARNINGS = all
WARNINGS += error

INSTALL = install

CC ?= gcc
CXX ?= g++

BUILD := $(shell $(CC) -dumpmachine)

CCVER := $(shell $(CC) -dumpversion)
CXXVER := $(shell $(CXX) -dumpversion)

ifeq ($(CROSS),)
HOST ?= $(BUILD)
else
HOST := $(shell $(CROSS)$(CC) -dumpmachine)
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

all: $(BUILDDIR) $(PROGRAMS)

$(BUILDDIR):
	-mkdir -p $(BUILDDIR) || mkdir $(BUILDDIR)
	-md $(subst /,\,$(BUILDDIR))

$(BUILDDIR)decode-ls-lR.o: decode-ls-lR.c
$(BUILDDIR)decode-ls-lR$(M64_)$(EXEEXT): $(BUILDDIR)decode-ls-lR.o $(BUILDDIR)buffer_stubborn2.o $(BUILDDIR)buffer_feed.o $(BUILDDIR)buffer_getc.o $(BUILDDIR)stralloc_ready.o $(BUILDDIR)buffer_get_token.o $(BUILDDIR)buffer_stubborn.o $(BUILDDIR)byte_copy.o $(BUILDDIR)str_len.o $(BUILDDIR)stralloc_copyb.o $(BUILDDIR)stralloc_readyplus.o $(BUILDDIR)buffer_0.o $(BUILDDIR)buffer_1.o $(BUILDDIR)buffer_2.o $(BUILDDIR)buffer_flush.o $(BUILDDIR)buffer_getline.o $(BUILDDIR)buffer_put.o $(BUILDDIR)buffer_puts.o $(BUILDDIR)byte_chr.o $(BUILDDIR)str_diffn.o $(BUILDDIR)stralloc_catb.o $(BUILDDIR)stralloc_copys.o
	$(CROSS)$(CC) $(LDFLAGS) $(CFLAGS) -o $@ $^ $(LIBS)

$(BUILDDIR)count-depth.o: count-depth.c
$(BUILDDIR)count-depth$(M64_)$(EXEEXT): $(BUILDDIR)count-depth.o $(BUILDDIR)buffer_stubborn2.o $(BUILDDIR)buffer_feed.o $(BUILDDIR)buffer_getc.o $(BUILDDIR)byte_chr.o $(BUILDDIR)buffer_get_token.o $(BUILDDIR)buffer_stubborn.o $(BUILDDIR)byte_copy.o $(BUILDDIR)fmt_ulong.o $(BUILDDIR)buffer_0.o $(BUILDDIR)buffer_1.o $(BUILDDIR)buffer_flush.o $(BUILDDIR)buffer_getline.o $(BUILDDIR)buffer_put.o $(BUILDDIR)buffer_putulong.o
	$(CROSS)$(CC) $(LDFLAGS) $(CFLAGS) -o $@ $^ $(LIBS)

$(BUILDDIR)list-r.o: list-r.c
$(BUILDDIR)list-r$(M64_)$(EXEEXT): $(BUILDDIR)list-r.o $(BUILDDIR)buffer_1.o $(BUILDDIR)buffer_2.o $(BUILDDIR)buffer_flush.o $(BUILDDIR)buffer_put.o $(BUILDDIR)buffer_puts.o $(BUILDDIR)buffer_putsa.o $(BUILDDIR)buffer_stubborn.o $(BUILDDIR)byte_copy.o $(BUILDDIR)dir_close.o $(BUILDDIR)dir_open.o $(BUILDDIR)dir_read.o $(BUILDDIR)dir_type.o $(BUILDDIR)fmt_uint64.o $(BUILDDIR)str_len.o $(BUILDDIR)stralloc_catb.o $(BUILDDIR)stralloc_cats.o $(BUILDDIR)stralloc_copyb.o $(BUILDDIR)stralloc_copys.o $(BUILDDIR)stralloc_init.o $(BUILDDIR)stralloc_nul.o $(BUILDDIR)stralloc_ready.o $(BUILDDIR)stralloc_readyplus.o
	$(CROSS)$(CXX) $(CXXOPTS) $(LDFLAGS) $(CFLAGS) -o $@ $^ $(LIBS)

$(BUILDDIR)reg2cmd.o: reg2cmd.c
$(BUILDDIR)reg2cmd$(M64_)$(EXEEXT): $(BUILDDIR)reg2cmd.o $(BUILDDIR)buffer_0.o $(BUILDDIR)buffer_1.o $(BUILDDIR)buffer_2.o $(BUILDDIR)buffer_feed.o $(BUILDDIR)buffer_flush.o $(BUILDDIR)buffer_get_token.o $(BUILDDIR)buffer_getc.o $(BUILDDIR)buffer_getline.o $(BUILDDIR)buffer_put.o $(BUILDDIR)buffer_putc.o $(BUILDDIR)buffer_puts.o $(BUILDDIR)buffer_putuint64.o $(BUILDDIR)buffer_stubborn.o $(BUILDDIR)buffer_stubborn2.o $(BUILDDIR)byte_chr.o $(BUILDDIR)byte_copy.o $(BUILDDIR)fmt_uint64.o $(BUILDDIR)scan_fromhex.o $(BUILDDIR)scan_xlong.o $(BUILDDIR)scan_xlonglong.o $(BUILDDIR)str_len.o $(BUILDDIR)stralloc_catb.o $(BUILDDIR)stralloc_init.o $(BUILDDIR)stralloc_ready.o $(BUILDDIR)stralloc_readyplus.o $(BUILDDIR)stralloc_zero.o
	$(CROSS)$(CXX) $(CXXOPTS) $(LDFLAGS) $(CFLAGS) -o $@ $^ $(LIBS)

$(BUILDDIR)torrent-progress.o: torrent-progress.c
$(BUILDDIR)torrent-progress$(M64_)$(EXEEXT): $(BUILDDIR)torrent-progress.o $(BUILDDIR)buffer_flush.o $(BUILDDIR)buffer_stubborn.o $(BUILDDIR)buffer_stubborn2.o $(BUILDDIR)buffer_feed.o $(BUILDDIR)buffer_put.o $(BUILDDIR)buffer_putflush.o $(BUILDDIR)byte_copy.o $(BUILDDIR)fmt_ulong.o $(BUILDDIR)str_len.o $(BUILDDIR)buffer_1.o $(BUILDDIR)buffer_get.o $(BUILDDIR)buffer_putnlflush.o $(BUILDDIR)buffer_puts.o $(BUILDDIR)buffer_putulong.o $(BUILDDIR)mmap_map.o $(BUILDDIR)mmap_unmap.o $(BUILDDIR)open_read.o
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
