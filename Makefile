DEBUG = 1
LARGEFILE = 1
WARNINGS = all no-unused-variable
#WARNINGS += error

INSTALL = install


ifeq ($SUBLIME_FILENAME),None)
PATH = /c/git-sdk-64/usr/bin
MAKE = c:/git-sdk-64/usr/bin/make
endif

CC = gcc
CXX = g++

BUILD := $(shell $(CC) -dumpmachine)

CCVER := $(shell $(CC) -dumpversion)
CXXVER := $(shell $(CXX) -dumpversion)

ifeq ($(word 1,(CROSS_COMPILE)),diet)
DIET := 1
else
DIET := 0
endif

ifeq ($(word 3,$(subst -, ,$(BUILD))),mingw32)
MINGW := 1
else
MINGW := 0
endif

ifeq ($(DIET),1)
DO_CXX := 0
else
DO_CXX := 1
endif

ifeq ($(CROSS_COMPILE),)
HOST ?= $(BUILD)
else
ifeq ($(DIET),1)
HOST := $(shell set -x; $(CC) -dumpmachine  | sed 's|[-.0-9]*\\\$$|| ;; s|\\r\$$|| ;; s|^\([^-]*\)-\([^-]*\)-\([^-]*\)-gnu|\1-\2-\3-diet| ;; s|^\([^-]*\)-\([^-]*\)-\([^-]*\)|\1-diet-\3|' )
else
HOST := $(shell set -x; $(CROSS_COMPILE)$(CC) -dumpmachine  | sed 's|[-.0-9]*\\\$$|| ;; s|\\r\$$||' )
endif
endif

$(info DIET: $(DIET))
$(info MINGW: $(MINGW))
$(info HOST: $(HOST))

ifeq ($(PREFIX),)
PREFIX := $(shell $(CROSS_COMPILE)$(CC) -print-search-dirs |sed -n 's,.*:\s\+=\?,,; s,/bin.*,,p ; s,/lib.*,,p' |head -n1 )
endif

$(info PREFIX: $(PREFIX))

prefix ?= $(PREFIX)
bindir = ${prefix}/bin


ifneq ($(HOST),)
TRIPLET := $(subst -, ,$(HOST))
endif

ifeq ($(CROSS_COMPILE),)
	CROSS_COMPILE := $(HOST)-
endif

ifeq ($(DEBUG),1)
BUILDTYPE = debug
else
BUILDTYPE = release
DO_STRIP := 1
endif

ifeq ($(HOST),$(BUILD))
CROSS_COMPILE :=
endif

PKG_CONFIG ?= pkg-config

ifneq ($(TRIPLET),)
ARCH := $(word 1,$(TRIPLET))
OS := $(shell echo $(word 3,$(TRIPLET)) |sed "s|[0-9].*||")
KERN := $(word 2,$(TRIPLET))
SYS := $(word 3,$(TRIPLET))
W := 4
ifeq ($(SYS),pc)
SYS := $(word $(W),$(TRIPLET))
W := 5
endif
ifeq ($(SYS),w64)
SYS := $(word $(W),$(TRIPLET))
endif
endif

$(info Arch: $(ARCH))
$(info OS: $(OS))
$(info KERN: $(KERN))
$(info SYS: $(SYS))

ifneq ($(OS),linux)
ifeq ($(ARCH),x86_64)
M64 = 64
endif
endif

ifeq ($(OS),msys)
EXEEXT = .exe
STATIC_LIBGCC := 1
endif

ifeq ($(OS),mingw)
EXEEXT = .exe
STATIC_LIBGCC := 1
endif

ifeq ($(OS),cygwin)
EXEEXT = .exe
endif

ifeq ($(OS),mingw32)
EXEEXT = .exe
endif
BOOST_LIBS = boost_random

ifeq ($(OS),mingw32)
TOOLCHAIN = $(HOST)-$(shell $(CROSS_COMPILE)gcc -dumpversion)
else
TOOLCHAIN = $(HOST)
endif

ifneq ($(HOST),$(BUILD))
BUILDDIR = build/$(TOOLCHAIN)/$(BUILD_TYPE)/
else
ifeq ($(CROSS_COMPILE),)
BUILDDIR = build/$(TOOLCHAIN)/$(BUILD_TYPE)/
else
BUILDDIR = build/$(patsubst %-,%,$(CROSS_COMPILE))/$(BUILD_TYPE)/
endif
endif


vpath lib $(BUILDDIR)
VPATH = lib:$(BUILDDIR)


ifeq ($(CXXOPTS),)
#$(info OS: "$(OS)")
ifneq ($(OS),msys)
endif
endif

CPPFLAGS := -I. -Ilib

DEFS += INLINE=inline
#DEFS += PATH_MAX=4096

ifeq ($(READDIR),)
ifeq ($(SYS),mingw32)
DEFS += USE_READDIR=0
else
ifeq ($(SYS),msys)
DEFS += USE_READDIR=0
else
DEFS += USE_READDIR=1
endif
endif
endif

ifeq ($(LARGEFILE),1)
DEFS += _FILE_OFFSET_BITS=64
DEFS += _LARGEFILE_SOURCE=1
endif
DEFS += _GNU_SOURCE=1

WARNINGS += no-strict-aliasing

STRIP ?= strip

CFLAGS = -pipe

CFLAGS_Debug = -g -ggdb -pg -O0
CFLAGS_MinSizeRel = -g -fomit-frame-pointer -Os
CFLAGS_RelWithDebInfo = -g -ggdb -pg -O2
CFLAGS_Release = -g -fomit-frame-pointer -O2

CXXFLAGS = -pipe

CXXFLAGS += -std=c++14

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
#LDFLAGS += -static
PKG_CONFIG += --static
ifeq ($(MINGW)$(STATIC),1)
LDFLAGS += -static-libgcc -static-libstdc++
else
LDFLAGS += -static
endif
endif


ifeq ($(STATIC_LIBGCC)$(STATIC),1)
LDFLAGS += -static-libgcc
endif

ifeq ($(STATIC_LIBSTDCXX)$(STATIC),1)
LDFLAGS += -static-libstdc++
endif

ifeq ($(CROSS_COMPILE)$(STRIP),1)
LDFLAGS += -s
endif
RM = rm -f

ifneq ($(shell uname -s)-$(shell uname -o),MINGW32_NT-5.1-Msys)
ifeq ($(STATIC),)
LDFLAGS +=  -static-libgcc -static-libstdc++
endif
endif

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

PROGRAMS = $(BUILDDIR)list-r$(M64_)$(EXESUFFIX)$(EXEEXT) $(BUILDDIR)count-depth$(M64_)$(EXESUFFIX)$(EXEEXT) $(BUILDDIR)decode-ls-lR$(M64_)$(EXESUFFIX)$(EXEEXT) $(BUILDDIR)reg2cmd$(M64_)$(EXESUFFIX)$(EXEEXT) $(BUILDDIR)torrent-progress$(M64_)$(EXESUFFIX)$(EXEEXT)  $(BUILDDIR)mediathek-parser$(M64_)$(EXESUFFIX)$(EXEEXT)  $(BUILDDIR)opensearch-dump$(M64_)$(EXESUFFIX)$(EXEEXT)   $(BUILDDIR)xc8-wrapper$(M64_)$(EXESUFFIX)$(EXEEXT) 
  
  
ifeq ($(DO_CXX),1)
PROGRAMS += \
  $(BUILDDIR)piccfghex$(M64_)$(EXESUFFIX)$(EXEEXT)   \
  $(BUILDDIR)mediathek-parser-cpp$(M64_)$(EXESUFFIX)$(EXEEXT)
endif
OBJECTS = $(PROGRAMS:%=%.o) $(LIB_OBJ)



vpath $(BUILDDIR)

VPATH = $(BUILDDIR):.

$(info Programs: $(PROGRAMS))

#$(info ARCH: $(ARCH))
#$(info BUILD: $(BUILD))
#$(info BUILDDIR: $(BUILDDIR))
#$(info BUILDTYPE: $(BUILDTYPE))
#$(info CCVER: $(CCVER))
#$(info CROSS_COMPILE: $(CROSS_COMPILE))
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
	
$(OBJDIR):
	-mkdir -p $(OBJDIR) || mkdir $(OBJDIR)
	-md $(subst /,\,$(OBJDIR))

$(BUILDDIR)scan.a: $(BUILDDIR)scan_fromhex.o $(BUILDDIR)scan_xlong.o $(BUILDDIR)scan_xlonglong.o
	$(AR) rcs $@ $^
$(BUILDDIR)open.a: $(BUILDDIR)open_append.o $(BUILDDIR)open_read.o $(BUILDDIR)open_rw.o $(BUILDDIR)open_trunc.o
	$(AR) rcs $@ $^
$(BUILDDIR)str.a: $(BUILDDIR)str_chr.o $(BUILDDIR)str_diff.o $(BUILDDIR)str_diffn.o $(BUILDDIR)str_len.o
	$(AR) rcs $@ $^
$(BUILDDIR)dir.a: $(BUILDDIR)dir_close.o $(BUILDDIR)dir_open.o $(BUILDDIR)dir_read.o $(BUILDDIR)dir_time.o $(BUILDDIR)dir_type.o
	$(AR) rcs $@ $^
$(BUILDDIR)mmap.a: $(BUILDDIR)mmap_map.o $(BUILDDIR)mmap_private.o $(BUILDDIR)mmap_read.o $(BUILDDIR)mmap_read_fd.o $(BUILDDIR)mmap_unmap.o
	$(AR) rcs $@ $^
$(BUILDDIR)byte.a: $(BUILDDIR)byte_chr.o $(BUILDDIR)byte_copy.o $(BUILDDIR)byte_copyr.o $(BUILDDIR)byte_diff.o $(BUILDDIR)byte_fill.o $(BUILDDIR)byte_rchr.o $(BUILDDIR)byte_zero.o
	$(AR) rcs $@ $^
$(BUILDDIR)strlist.a: $(BUILDDIR)strlist_at.o $(BUILDDIR)strlist_copy.o $(BUILDDIR)strlist_count.o $(BUILDDIR)strlist_dump.o $(BUILDDIR)strlist_index_of.o $(BUILDDIR)strlist_push.o $(BUILDDIR)strlist_push_sa.o $(BUILDDIR)strlist_pushb.o $(BUILDDIR)strlist_pushm_internal.o $(BUILDDIR)strlist_pushsa.o $(BUILDDIR)strlist_pushunique.o $(BUILDDIR)strlist_shift.o $(BUILDDIR)strlist_sort.o $(BUILDDIR)strlist_to_argv.o $(BUILDDIR)strlist_unshift.o
	$(AR) rcs $@ $^
$(BUILDDIR)array.a: $(BUILDDIR)array_allocate.o $(BUILDDIR)array_bytes.o $(BUILDDIR)array_cat.o $(BUILDDIR)array_cat0.o $(BUILDDIR)array_catb.o $(BUILDDIR)array_cate.o $(BUILDDIR)array_cats.o $(BUILDDIR)array_cats0.o $(BUILDDIR)array_equal.o $(BUILDDIR)array_fail.o $(BUILDDIR)array_get.o $(BUILDDIR)array_length.o $(BUILDDIR)array_reset.o $(BUILDDIR)array_start.o $(BUILDDIR)array_trunc.o $(BUILDDIR)array_truncate.o  $(BUILDDIR)umult64.o
	$(AR) rcs $@ $^
$(BUILDDIR)stralloc.a: $(BUILDDIR)stralloc_append.o $(BUILDDIR)stralloc_append_sa.o $(BUILDDIR)stralloc_cat.o $(BUILDDIR)stralloc_catb.o $(BUILDDIR)stralloc_catc.o $(BUILDDIR)stralloc_catlong0.o $(BUILDDIR)stralloc_cats.o $(BUILDDIR)stralloc_catulong0.o $(BUILDDIR)stralloc_copy.o $(BUILDDIR)stralloc_copyb.o $(BUILDDIR)stralloc_copys.o $(BUILDDIR)stralloc_diffs.o $(BUILDDIR)stralloc_free.o $(BUILDDIR)stralloc_init.o $(BUILDDIR)stralloc_insertb.o $(BUILDDIR)stralloc_move.o $(BUILDDIR)stralloc_nul.o $(BUILDDIR)stralloc_ready.o $(BUILDDIR)stralloc_readyplus.o $(BUILDDIR)stralloc_remove.o $(BUILDDIR)stralloc_trunc.o $(BUILDDIR)stralloc_write.o $(BUILDDIR)stralloc_zero.o
	$(AR) rcs $@ $^
$(BUILDDIR)fmt.a: $(BUILDDIR)fmt_8long.o $(BUILDDIR)fmt_8longlong.o $(BUILDDIR)fmt_asn1derlength.o $(BUILDDIR)fmt_asn1dertag.o $(BUILDDIR)fmt_double.o $(BUILDDIR)fmt_escapecharc.o $(BUILDDIR)fmt_escapecharhtml.o $(BUILDDIR)fmt_escapecharjson.o $(BUILDDIR)fmt_escapecharquotedprintable.o $(BUILDDIR)fmt_escapecharquotedprintableutf8.o $(BUILDDIR)fmt_escapecharxml.o $(BUILDDIR)fmt_fill.o $(BUILDDIR)fmt_httpdate.o $(BUILDDIR)fmt_human.o $(BUILDDIR)fmt_humank.o $(BUILDDIR)fmt_iso8601.o $(BUILDDIR)fmt_long.o $(BUILDDIR)fmt_longlong.o $(BUILDDIR)fmt_minus.o $(BUILDDIR)fmt_pad.o  $(BUILDDIR)fmt_plusminus.o $(BUILDDIR)fmt_str.o $(BUILDDIR)fmt_strm_internal.o $(BUILDDIR)fmt_strn.o $(BUILDDIR)fmt_tohex.o $(BUILDDIR)fmt_uint64.o $(BUILDDIR)fmt_ulong.o $(BUILDDIR)fmt_ulong0.o $(BUILDDIR)fmt_ulonglong.o $(BUILDDIR)fmt_utf8.o $(BUILDDIR)fmt_varint.o $(BUILDDIR)fmt_xlong.o $(BUILDDIR)fmt_xlonglong.o $(BUILDDIR)fmt_xmlescape.o
	$(AR) rcs $@ $^
$(BUILDDIR)buffer.a: $(BUILDDIR)buffer_0.o $(BUILDDIR)buffer_0small.o $(BUILDDIR)buffer_1.o $(BUILDDIR)buffer_1small.o $(BUILDDIR)buffer_2.o $(BUILDDIR)buffer_close.o $(BUILDDIR)buffer_default.o $(BUILDDIR)buffer_dummyread.o $(BUILDDIR)buffer_dummyreadmmap.o $(BUILDDIR)buffer_dump.o $(BUILDDIR)buffer_feed.o $(BUILDDIR)buffer_flush.o $(BUILDDIR)buffer_free.o $(BUILDDIR)buffer_fromarray.o $(BUILDDIR)buffer_frombuf.o $(BUILDDIR)buffer_fromsa.o $(BUILDDIR)buffer_fromstr.o $(BUILDDIR)buffer_get.o $(BUILDDIR)buffer_get_new_token_sa.o $(BUILDDIR)buffer_get_new_token_sa_pred.o $(BUILDDIR)buffer_get_token.o $(BUILDDIR)buffer_get_token_pred.o $(BUILDDIR)buffer_get_token_sa.o $(BUILDDIR)buffer_get_token_sa_pred.o $(BUILDDIR)buffer_get_until.o $(BUILDDIR)buffer_getc.o $(BUILDDIR)buffer_getline.o $(BUILDDIR)buffer_getline_sa.o $(BUILDDIR)buffer_getn.o $(BUILDDIR)buffer_getnewline_sa.o $(BUILDDIR)buffer_init.o $(BUILDDIR)buffer_init_free.o $(BUILDDIR)buffer_mmapprivate.o $(BUILDDIR)buffer_mmapread.o $(BUILDDIR)buffer_mmapread_fd.o $(BUILDDIR)buffer_munmap.o $(BUILDDIR)buffer_peek.o $(BUILDDIR)buffer_prefetch.o $(BUILDDIR)buffer_put.o $(BUILDDIR)buffer_put8long.o $(BUILDDIR)buffer_putalign.o $(BUILDDIR)buffer_putc.o $(BUILDDIR)buffer_puterror.o $(BUILDDIR)buffer_puterror2.o $(BUILDDIR)buffer_putflush.o $(BUILDDIR)buffer_putlong.o $(BUILDDIR)buffer_putlonglong.o $(BUILDDIR)buffer_putm_internal.o $(BUILDDIR)buffer_putm_internal_flush.o $(BUILDDIR)buffer_putnlflush.o $(BUILDDIR)buffer_putnspace.o $(BUILDDIR)buffer_puts.o $(BUILDDIR)buffer_putsa.o $(BUILDDIR)buffer_putsaflush.o $(BUILDDIR)buffer_putsalign.o $(BUILDDIR)buffer_putsflush.o $(BUILDDIR)buffer_putspace.o $(BUILDDIR)buffer_putuint64.o $(BUILDDIR)buffer_putulong.o $(BUILDDIR)buffer_putulonglong.o $(BUILDDIR)buffer_putxlong.o $(BUILDDIR)buffer_seek.o $(BUILDDIR)buffer_skip_until.o $(BUILDDIR)buffer_stubborn.o $(BUILDDIR)buffer_stubborn2.o $(BUILDDIR)buffer_tosa.o $(BUILDDIR)buffer_truncfile.o
	$(AR) rcs $@ $^


$(BUILDDIR)decode-ls-lR.o: decode-ls-lR.c
$(BUILDDIR)decode-ls-lR$(M64_)$(EXESUFFIX)$(EXEEXT): $(BUILDDIR)decode-ls-lR.o buffer.a str.a stralloc.a byte.a
	$(CROSS_COMPILE)$(CC) $(LDFLAGS) $(CFLAGS) -o $@ $^ $(LIBS)

$(BUILDDIR)count-depth.o: count-depth.c
$(BUILDDIR)count-depth$(M64_)$(EXESUFFIX)$(EXEEXT): $(BUILDDIR)count-depth.o buffer.a byte.a fmt.a
	$(CROSS_COMPILE)$(CC) $(LDFLAGS) $(CFLAGS) -o $@ $^ $(LIBS)
ifeq ($(DO_STRIP),1)
	$(CROSS_COMPILE)$(STRIP) --strip-all $@
endif

$(BUILDDIR)list-r.o: list-r.c
$(BUILDDIR)list-r$(M64_)$(EXESUFFIX)$(EXEEXT): $(BUILDDIR)list-r.o buffer.a stralloc.a byte.a dir.a fmt.a str.a
	$(CROSS_COMPILE)$(CC) $(LDFLAGS) $(CFLAGS) -o $@ $^ $(LIBS)
ifeq ($(DO_STRIP),1)
	$(CROSS_COMPILE)$(STRIP) --strip-all $@
endif

$(BUILDDIR)reg2cmd.o: reg2cmd.c
$(BUILDDIR)reg2cmd$(M64_)$(EXESUFFIX)$(EXEEXT): $(BUILDDIR)reg2cmd.o buffer.a fmt.a scan.a str.a stralloc.a byte.a
	$(CROSS_COMPILE)$(CC) $(LDFLAGS) $(CFLAGS) -o $@ $^ $(LIBS)
ifeq ($(DO_STRIP),1)
	$(CROSS_COMPILE)$(STRIP) --strip-all $@
endif

$(BUILDDIR)torrent-progress.o: torrent-progress.c
$(BUILDDIR)torrent-progress$(M64_)$(EXESUFFIX)$(EXEEXT): $(BUILDDIR)torrent-progress.o buffer.a fmt.a mmap.a open.a str.a byte.a
	$(CROSS_COMPILE)$(CC) $(LDFLAGS) $(CFLAGS) -o $@ $^ $(LIBS)
ifeq ($(DO_STRIP),1)
	$(CROSS_COMPILE)$(STRIP) --strip-all $@
endif

$(BUILDDIR)mediathek-parser$(M64_)$(EXESUFFIX)$(EXEEXT): $(BUILDDIR)mediathek-parser.o array.a buffer.a fmt.a mmap.a open.a str.a stralloc.a strlist.a  byte.a
	$(CROSS_COMPILE)$(CC) $(LDFLAGS) $(CFLAGS) -o $@ $^ $(LIBS)  
ifeq ($(DO_STRIP),1)
	$(CROSS_COMPILE)$(STRIP) --strip-all $@
endif

$(BUILDDIR)opensearch-dump$(M64_)$(EXESUFFIX)$(EXEEXT): CFLAGS += $(shell $(PKG_CONFIG) --cflags libxml-2.0)
$(BUILDDIR)opensearch-dump$(M64_)$(EXESUFFIX)$(EXEEXT): LIBS += $(shell $(PKG_CONFIG) --libs libxml-2.0)
$(BUILDDIR)opensearch-dump$(M64_)$(EXESUFFIX)$(EXEEXT): $(BUILDDIR)opensearch-dump.o buffer.a str.a stralloc.a byte.a
	$(CROSS_COMPILE)$(CC) $(LDFLAGS) $(CFLAGS) -o $@ $^ $(LIBS)  
ifeq ($(DO_STRIP),1)
	$(CROSS_COMPILE)$(STRIP) --strip-all $@
endif

$(BUILDDIR)xc8-wrapper/:
	mkdir -p $@
	
#$(BUILDDIR)xc8-wrapper$(M64_)$(EXESUFFIX)$(EXEEXT): CFLAGS += $(shell $(PKG_CONFIG) --cflags libxml-2.0)
#$(BUILDDIR)xc8-wrapper$(M64_)$(EXESUFFIX)$(EXEEXT): LIBS += $(shell $(PKG_CONFIG) --libs libxml-2.0)
$(BUILDDIR)xc8-wrapper$(M64_)$(EXESUFFIX)$(EXEEXT): DEFS += -DXC8_WRAPPER=1
#$(BUILDDIR)xc8-wrapper$(M64_)$(EXESUFFIX)$(EXEEXT): OBJDIR := $(BUILDDIR)xc8-wrapper/
$(BUILDDIR)xc8-wrapper$(M64_)$(EXESUFFIX)$(EXEEXT): $(BUILDDIR)compiler-wrapper.o strlist.a stralloc.a buffer.a str.a byte.a fmt.a

	$(CROSS_COMPILE)$(CC) $(LDFLAGS) $(CFLAGS) -o $@ $^ $(LIBS)  
ifeq ($(DO_STRIP),1)
	$(CROSS_COMPILE)$(STRIP) --strip-all $@
endif

ifeq ($(DO_CXX),1)
$(BUILDDIR)piccfghex.o: piccfghex.cpp
$(BUILDDIR)piccfghex$(M64_)$(EXESUFFIX)$(EXEEXT): $(BUILDDIR)piccfghex.o $(BUILDDIR)intelhex.o
	$(CROSS_COMPILE)$(CXX) $(LDFLAGS) $(CXXFLAGS) -o $@ $^ $(LIBS)
ifeq ($(DO_STRIP),1)
	$(CROSS_COMPILE)$(STRIP) --strip-all $@
endif

$(BUILDDIR)mediathek-parser-cpp.o: mediathek-parser.cpp
	$(CROSS_COMPILE)$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c -o $@ $<

$(BUILDDIR)mediathek-parser-cpp$(M64_)$(EXESUFFIX)$(EXEEXT): $(BUILDDIR)mediathek-parser-cpp.o
	$(CROSS_COMPILE)$(CXX) $(LDFLAGS) $(CXXFLAGS) -o $@ $^ $(LIBS)
ifeq ($(DO_STRIP),1)
	$(CROSS_COMPILE)$(STRIP) --strip-all $@
endif

	$(CROSS_COMPILE)$(CC) $(LDFLAGS) $(CFLAGS) -o $@ $^ $(LIBS)
$(BUILDDIR)kbd-adjacency.o: kbd-adjacency.cpp
	$(CROSS_COMPILE)$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c -o  $(BUILDDIR)$(patsubst %.cpp,%.o,$(notdir $<))  $<

$(BUILDDIR)kbd-adjacency$(M64_)$(EXESUFFIX)$(EXEEXT): $(BUILDDIR)kbd-adjacency.o $(LIB_OBJ)
	$(CROSS_COMPILE)$(CXX) $(LDFLAGS) $(CXXFLAGS) -o $@ $^ $(LIBS)
ifeq ($(DO_STRIP),1)
	$(CROSS_COMPILE)$(STRIP) --strip-all $@
endif
endif

ifeq ($(BUILDDIR),)
.c.o:
	$(CROSS_COMPILE)$(CC) $(CPPFLAGS) $(CFLAGS) -c $<

%.o: %.c
	$(CROSS_COMPILE)$(CC) $(CPPFLAGS) $(CFLAGS) -c $<

.cpp.o:
	$(CROSS_COMPILE)$(CXX) $(CXXOPTS) $(CPPFLAGS) $(CXXFLAGS) -c $<

%.o: %.cpp
	$(CROSS_COMPILE)$(CXX) $(CXXOPTS) $(CPPFLAGS) $(CXXFLAGS) -c $<
else
.c.o:
	$(CROSS_COMPILE)$(CC) $(CPPFLAGS) $(CFLAGS) -c $<

$(BUILDDIR)%.o: lib/%.c
	$(CROSS_COMPILE)$(CC) $(CPPFLAGS) $(CFLAGS) -c -o $(BUILDDIR)$(patsubst lib/%.c,%.o,$<) $<

$(BUILDDIR)%.o: %.c
	$(CROSS_COMPILE)$(CC) $(CPPFLAGS) $(CFLAGS) -c -o $(BUILDDIR)$(patsubst %.c,%.o,$<) $<

.cpp.o:
	$(CROSS_COMPILE)$(CXX) $(CXXOPTS) $(CPPFLAGS) $(CXXFLAGS) -c $<

$(BUILDDIR)%.o: %.cpp
	$(CROSS_COMPILE)$(CXX) $(CXXOPTS) $(CPPFLAGS) $(CXXFLAGS) -c -o $(BUILDDIR)$(patsubst %.cpp,%.o,$<) $<
endif

clean:
	$(RM) -f $(OBJECTS) list-r.o list-r$(M64_)$(EXESUFFIX)$(EXEEXT)

install: $(PROGRAMS)
	$(INSTALL) -d $(DESTDIR)$(bindir)
	$(INSTALL) -m 755 $(PROGRAMS) $(DESTDIR)$(bindir)
	$(INSTALL) -d $(DESTDIR)$(bindir)

uninstall:
	@for PROGRAM in $(PROGRAMS); do \
		echo $(RM) $(DESTDIR)$(bindir)/$$PROGRAM; \
		$(RM) $(DESTDIR)$(bindir)/$$PROGRAM; \
  done
LIBSOURCES = array.h array_allocate.c array_bytes.c array_cat.c array_cat0.c array_catb.c array_cate.c array_cats.c array_cats0.c array_equal.c array_fail.c array_get.c array_length.c array_reset.c array_start.c array_trunc.c array_truncate.c buffer.h buffer_0.c buffer_1.c buffer_2.c buffer_close.c buffer_default.c buffer_feed.c buffer_flush.c buffer_free.c buffer_fromsa.c buffer_fromstr.c buffer_get.c buffer_get_new_token_sa.c buffer_get_new_token_sa_pred.c buffer_get_token.c buffer_get_token_pred.c buffer_get_token_sa.c buffer_get_token_sa_pred.c buffer_get_until.c buffer_getc.c buffer_getline.c buffer_getline_sa.c buffer_init.c buffer_mmapprivate.c buffer_mmapread.c buffer_mmapread_fd.c buffer_prefetch.c buffer_put.c buffer_putc.c buffer_putflush.c buffer_putm_internal.c buffer_putnlflush.c buffer_putnspace.c buffer_puts.c buffer_putsa.c buffer_putsflush.c buffer_putspace.c buffer_putuint64.c buffer_putulong.c buffer_putulonglong.c buffer_putxlong.c buffer_skip_until.c buffer_stubborn.c buffer_stubborn2.c buffer_tosa.c buffer_truncfile.c byte.h byte_chr.c byte_copy.c byte_copyr.c byte_diff.c byte_fill.c byte_zero.c dir.h dir_close.c dir_internal.h dir_open.c dir_read.c dir_time.c dir_type.c fmt.h fmt_minus.c fmt_uint64.c fmt_ulong.c fmt_ulong0.c fmt_ulonglong.c fmt_xlong.c fmt_xlonglong.c mmap.h mmap_map.c mmap_private.c mmap_read.c mmap_read_fd.c mmap_unmap.c open.h open_append.c open_read.c open_rw.c open_trunc.c scan.h scan_fromhex.c scan_xlong.c scan_xlonglong.c str.h str_diffn.c str_len.c stralloc.h stralloc_append.c stralloc_cat.c stralloc_catb.c stralloc_catc.c stralloc_catlong0.c stralloc_cats.c stralloc_catulong0.c stralloc_copy.c stralloc_copyb.c stralloc_copys.c stralloc_diffs.c stralloc_free.c stralloc_init.c stralloc_insertb.c stralloc_move.c stralloc_nul.c stralloc_ready.c stralloc_readyplus.c stralloc_remove.c stralloc_trunc.c stralloc_write.c stralloc_zero.c strlist.h strlist_push.c strlist_pushunique.c strlist_at.c strlist_count.c strlist_index_of.c strlist_sort.c



