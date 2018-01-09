DEBUG = 1
LARGEFILE = 1
WARNINGS = all no-unused-variable
#WARNINGS += error

INSTALL = install

ROOT := $(shell cygpath -am / 2>/dev/null | sed "s|/$$||")
ROOTNAME := $(shell basename "$(ROOT)" | tr "[[:upper:]]" "[[:lower:]]")

ifneq ($(ROOTNAME),$(subst msys,,$(subst git-sdk,,$(subst cygwin,,$(ROOTNAME)))))
SYSNAME := $(subst git-sdk-,gitsdk,$(ROOTNAME))
endif

ifneq ($(CROSS_COMPILE),$(subst /,-,$(CROSS_COMPILE)))
PKG_CONFIG_PATH := $(subst /bin,/lib/pkgconfig,$(CROSS_COMPILE))
endif
$(info PKG_CONFIG_PATH=$(PKG_CONFIG_PATH))

#$(info ROOTNAME=$(ROOTNAME))
#$(info SYSNAME=$(SYSNAME))

OS ?= $(shell uname -o | tr "[[:upper:]]" "[[:lower:]]")

ifeq ($SUBLIME_FILENAME),None)
PATH = /c/git-sdk-64/usr/bin
MAKE = c:/git-sdk-64/usr/bin/make
endif

CC = gcc
CXX = g++

BUILD := $(shell $(CROSS_COMPILE)$(CC) -dumpmachine)

ifneq ($(CC),$(subst m32,,$(CC)))
BUILD := $(subst x86_64,i386,$(BUILD))
endif

CCVER := $(shell $(CROSS_COMPILE)$(CC) -dumpversion)
CXXVER := $(shell $(CROSS_COMPILE)$(CXX) -dumpversion)

ifneq ($(DIET),0)
ifneq ($(DIET),1)
ifneq ($(DIET),)
CC := $(DIET) $(CC)
USE_DIET := 1
endif
endif
endif

$(info DIET: $(DIET))
$(info USE_DIET: $(USE_DIET))

ifneq ($(USE_DIET),1)
ifeq ($(word 1,(CROSS_COMPILE)),diet)
USE_DIET := 1
else
USE_DIET := 0
endif
endif

ifeq ($(word 3,$(subst -, ,$(BUILD))),mingw32)
MINGW := 1
else
MINGW := 0
endif
ifeq ($(word 3,$(subst -, ,$(BUILD))),cygwin)
CYGWIN := 1
else
CYGWIN := 0
endif

ifeq ($(USE_DIET),1)
DO_CXX := 0
else
DO_CXX := 0
endif

ifeq ($(CROSS_COMPILE),)
HOST ?= $(BUILD)
else
ifeq ($(USE_DIET),1)
HOST := $(shell set -x; $(CC) -dumpmachine  | sed 's|[-.0-9]*\\\$$|| ;; s|\\r\$$|| ;; s|^\([^-]*\)-\([^-]*\)-\([^-]*\)-gnu|\1-\2-\3-diet| ;; s|^\([^-]*\)-\([^-]*\)-\([^-]*\)|\1-diet-\3|' )
else
HOST := $(shell set -x; $(CROSS_COMPILE)$(CC) -dumpmachine  | sed 's|[-.0-9]*\\\$$|| ;; s|\\r\$$||' )
endif
endif

ifneq ($(CC),$(subst m32,,$(CC)))
HOST := $(subst x86_64,i386,$(HOST))
endif

ifeq ($(USE_DIET),1)
HOST := $(subst gnu,dietlibc,$(HOST))
endif
#$(info USE_DIET: $(USE_DIET))
#$(info MINGW: $(MINGW))
$(info HOST: $(HOST))

ifeq ($(PREFIX),)
PREFIX := $(shell $(CROSS_COMPILE)$(CC) -print-search-dirs |sed -n 's,.*:\s\+=\?,,; s,/bin.*,,p ; s,/lib.*,,p' |head -n1 )
endif
#$(info PREFIX: $(PREFIX))

ifneq ($(CROSS_COMPILE),$(subst /,-,$(CROSS_COMPILE)))
SYSROOT := $(subst /bin/,,$(CROSS_COMPILE))
else
SYSROOT := $(shell $(CROSS_COMPILE)$(CC) -print-search-dirs|sed -n "/^lib/ { s|.*:\s\+|| ; s|^=|| ; /;/ { s|.*;|;| }; /;/! { s|.*:|| } ; s|^;|| ; s|/lib.*|| ; s|/mingw$$|| ; s|/usr$$|| ; p }")
endif
$(info SYSROOT: $(SYSROOT))


prefix ?= $(PREFIX)
bindir = ${prefix}/bin


ifneq ($(HOST),)
TRIPLET := $(subst -, ,$(HOST))
endif

ifneq ($(USE_DIET),1)
ifneq ($(HOST),$(BUILD))
ifeq ($(CROSS_COMPILE),)
	CROSS_COMPILE := $(HOST)-
endif
endif
endif

ifeq ($(DEBUG),1)
BUILDTYPE = debug
else
BUILDTYPE = release
DO_STRIP := 1
endif

ifeq ($(PROF),1)
BUILDTYPE := prof
endif

ifeq ($(HOST),$(BUILD))
CROSS_COMPILE :=
endif

PKG_CONFIG ?= $(CROSS_COMPILE)pkg-config

$(info PKG_CONFIG: $(PKG_CONFIG))

PKG_CONFIG_CMD := $(if $(SYSROOT)$(PKG_CONFIG_PATH),env $(if $(SYSROOT),PKG_CONFIG_SYSROOT_DIR=$(SYSROOT) ,)$(if $(PKG_CONFIG_PATH),PKG_CONFIG_PATH=$(PKG_CONFIG_PATH) ,),)$(PKG_CONFIG)
$(info PKG_CONFIG_CMD: $(PKG_CONFIG_CMD))

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

#$(info Arch: $(ARCH))
#$(info OS: $(OS))
#$(info KERN: $(KERN))
#$(info SYS: $(SYS))

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

$(info Host: $(HOST))
$(info Build: $(BUILD))

HOST1 := $(word 1,$(subst -, ,$(HOST)))
HOST2 := $(word 2,$(subst -, ,$(HOST)))
HOST3 := $(subst $(HOST1)-$(HOST2)-,,$(HOST))



#$(info HOST1=$(HOST1), HOST2=$(HOST2), HOST3=$(HOST3))

#ifneq ($(SYSNAME),)
#BUILDDIR := $(subst w64,$(SYSNAME),$(BUILDDIR))
#endif

ifeq ($(HOST2),pc)
HOST2 := $(SYSNAME)
endif
ifeq ($(HOST2),w64)
HOST2 := $(SYSNAME)
endif


ifeq ($(HOST2),)
HOST2 := pc
endif
#ifneq ($(SYSNAME),)
#HOST := $(subst w64,$(SYSNAME),$(HOST))
#endif
TOOLCHAIN := $(HOST1)-$(HOST2)-$(HOST3)

#$(info HOST: $(HOST))

#ifeq ($(OS),mingw32)
#TOOLCHAIN = $(HOST)-$(shell $(CROSS_COMPILE)gcc -dumpversion)
#else
#TOOLCHAIN = $(HOST)
#endif
#
#$(info TOOLCHAIN: $(TOOLCHAIN))

ifneq (${builddir},)
BUILDDIR = ${builddir}/$(BUILD_TYPE)/
else
  ifneq ($(HOST),$(BUILD))
  BUILDDIR = build/$(TOOLCHAIN)/$(BUILD_TYPE)/
  else
#	ifeq ($(CROSS_COMPILE),)
	BUILDDIR = build/$(TOOLCHAIN)/$(BUILD_TYPE)/
#	else
#	BUILDDIR = build/$(patsubst %-,%,$(CROSS_COMPILE))/$(BUILD_TYPE)/
#	endif
  endif
endif

vpath lib $(BUILDDIR)
VPATH = lib:$(BUILDDIR)

ifeq ($(CXXOPTS),)
##$(info OS: "$(OS)")
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
#ifeq ($(SYS),msys)	
#DEFS += USE_READDIR=0
#else
DEFS += USE_READDIR=1
#endif
endif
endif

ifeq ($(WIN32),)
ifeq ($(SYS),mingw32)
WIN32 := 1
else
ifeq ($(SYS),msys)
WIN32 := 1
endif
endif
endif
ifeq ($(WIN32),)
WIN32 := 0
endif

ifeq ($(WIN32),1)
OTHERLIBS := -lws2_32
endif

ifeq ($(WIN32),1)
WIDECHAR := 1
endif
ifeq ($(WIDECHAR),)
WIDECHAR := 0
endif

DEFS += USE_WIDECHAR=$(WIDECHAR)

ifeq ($(LARGEFILE),1)
DEFS += _FILE_OFFSET_BITS=64
DEFS += _LARGEFILE_SOURCE=1
endif
DEFS += _GNU_SOURCE=1

WARNINGS += no-strict-aliasing

STRIP ?= strip

CFLAGS = -pipe

CFLAGS_Prof = -pg -O2 
CFLAGS_Debug = -g -ggdb -O0
CFLAGS_MinSizeRel = -g -fomit-frame-pointer -Os
CFLAGS_RelWithDebInfo = -g -ggdb -O2
CFLAGS_Release = -g -fomit-frame-pointer -O2

CXXFLAGS = -pipe

CXXFLAGS += -std=c++11

CXXFLAGS_Prof = -pg -O2 
CXXFLAGS_Debug = -g -ggdb -O0
CXXFLAGS_MinSizeRel = -g -fomit-frame-pointer -Os
CXXFLAGS_RelWithDebInfo = -g -ggdb -O2
CXXFLAGS_Release = -g -fomit-frame-pointer -O2

ifeq ($(BUILD_TYPE),)
ifeq ($(PROF),1)
BUILD_TYPE = Prof
else
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
endif

$(info BUILDDIR: \
  )
#$(info builddir: $(builddir))


ifeq ($(BUILD_TYPE),Prof)
DEBUG := 0
RELEASE := 1
MINSIZE := 0
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

ifneq ($(STATIC),1)
STATIC := 0
endif

ifeq ($(HOST),)
HOST := $(shell $(CROSS_COMPILE)$(CC) -dumpmachine)
endif

ifneq ($(HOST),$(subst mingw,,$(HOST)))
MINGW := 1
endif

ifneq ($(MINGW),1)
MINGW := 0
endif

ifeq ($(MINGW),1)
LDFLAGS += -static-libgcc -static-lib{asan,gfortran,lsan,tsan,ubsan}
endif

ifeq ($(STATIC),1)
#LDFLAGS += -static
PKG_CONFIG += --static
ifeq ($(MINGW)$(STATIC)$(CYGWIN),10)
LDFLAGS += -static-libgcc -static-libstdc++
else
LDFLAGS += -static
endif
endif


ifeq ($(STATIC_LIBGCC),1)
LDFLAGS += -static-libgcc
endif

ifeq ($(STATIC_LIBSTDCXX),1)
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

WARNINGS += no-unused-function

CFLAGS += $(patsubst %,-W%,$(WARNINGS))
CPPFLAGS += $(patsubst %,-D%,$(DEFS))

LIB_SRC = $(wildcard *_*.c umult*.c)
LIB_OBJ = $(patsubst %.o,$(BUILDDIR)%.o,$(patsubst %.c,%.o,$(LIB_SRC)))

pkg-conf = $(foreach L,$(2),$(shell $(PKG_CONFIG_CMD) $(1) $(L) |sed "s,\([[:upper:]]:/\),\n-I\1,g ; /^-I$$/d"))

ifneq ($(shell uname -o),GNU/Linux)
ICONV_CFLAGS := $(shell $(PKG_CONFIG_CMD) --cflags libiconv 2>/dev/null || echo)
ICONV_LIBS := $(shell $(PKG_CONFIG_CMD) --libs libiconv 2>/dev/null || echo -liconv)
endif

#$(info ICONV_CFLAGS: $(ICONV_CFLAGS))
#$(info ICONV_LIBS: $(ICONV_LIBS))

LIBXML2_CFLAGS := $(call pkg-conf,--cflags,libxml-2.0 liblzma zlib)
LIBXML2_LIBS := $(call pkg-conf,--libs,libxml-2.0 liblzma zlib)

ifeq ($(STATIC),1)
LIBXML2_LIBS += $(OTHERLIBS)
LIBXML2_LIBS += -liconv
endif

$(info LIBXML2_CFLAGS: $(LIBXML2_CFLAGS))
$(info LIBXML2_LIBS: $(LIBXML2_LIBS))

#LIBS += -lstdc++


PROGRAMS = $(patsubst %,$(BUILDDIR)%$(M64_)$(EXESUFFIX)$(EXEEXT),list-r count-depth decode-ls-lR reg2cmd regfilter torrent-progress mediathek-parser mediathek-list xc8-wrapper picc-wrapper picc18-wrapper sdcc-wrapper rdir-test httptest xmltest xmltest2 xmltest3 plstest compiler-wrapper) 
 #opensearch-dump eagle-init-brd)
  
ifeq ($(DO_CXX),1)
PROGRAMS += \
  piccfghex$(M64_)$(EXESUFFIX)$(EXEEXT)   
#  $(BUILDDIR)mediathek-parser-cpp$(M64_)$(EXESUFFIX)$(EXEEXT)
endif
OBJECTS = $(PROGRAMS:%=%.o) $(LIB_OBJ)

vpath $(BUILDDIR) lib src

VPATH = $(BUILDDIR):.:lib:src

#$(info Programs: $(PROGRAMS))

##$(info ARCH: $(ARCH))
##$(info BUILD: $(BUILD))
##$(info BUILDDIR: \
  )
##$(info BUILDTYPE: $(BUILDTYPE))
##$(info CCVER: $(CCVER))
##$(info CROSS_COMPILE: $(CROSS_COMPILE))
##$(info CXXVER: $(CXXVER))
##$(info HOST: $(HOST))
##$(info TOOLCHAIN: $(TOOLCHAIN))
##$(info KERN: $(KERN))
##$(info M64: $(M64))
##$(info OS: $(OS))
##$(info STATIC: $(STATIC))
##$(info TRIPLET: $(TRIPLET))
ifeq ($(OS),darwin)
DEFS += USE_READDIR=1
#CFLAGS += -DUSE_READDIR=1
#CPPFLAGS += -DUSE_READDIR=1
endif

all: \
   $(BUILDDIR) $(PROGRAMS)
all-release:
	$(MAKE) DEBUG=0 all

$(BUILDDIR):
	-mkdir -p $(BUILDDIR) || mkdir $(BUILDDIR)
	-md $(subst /,\,$(BUILDDIR))
	
$(OBJDIR):
	-mkdir -p $(OBJDIR) || mkdir $(OBJDIR)
	-md $(subst /,\,$(OBJDIR))

$(BUILDDIR)array.a: \
  array_allocate.o array_bytes.o array_cat0.o array_catb.o array_cat.o array_cate.o array_cats0.o array_cats.o array_equal.o array_fail.o array_get.o array_length.o array_reset.o array_start.o array_truncate.o array_trunc.o umult64.o
	$(CROSS_COMPILE)$(AR) rcs $@ $^

$(BUILDDIR)buffer.a: \
  buffer_0.o buffer_0small.o buffer_1.o buffer_1small.o buffer_2.o buffer_close.o buffer_default.o buffer_dummyread.o buffer_dummyreadmmap.o buffer_dump.o buffer_feed.o buffer_flush.o buffer_free.o buffer_fromarray.o buffer_frombuf.o buffer_fromsa.o buffer_fromstr.o buffer_get.o buffer_getc.o buffer_getline.o buffer_getline_sa.o buffer_getn.o buffer_getnewline_sa.o buffer_get_new_token_sa.o buffer_get_new_token_sa_pred.o buffer_get_token.o buffer_get_token_pred.o buffer_get_token_sa.o buffer_get_token_sa_pred.o buffer_get_until.o buffer_init.o buffer_init_free.o buffer_mmapprivate.o buffer_mmapread.o buffer_mmapread_fd.o buffer_munmap.o buffer_peek.o buffer_prefetch.o buffer_put8long.o buffer_putalign.o buffer_put.o buffer_putc.o buffer_puterror2.o buffer_puterror.o buffer_putflush.o buffer_putlong.o buffer_putlonglong.o buffer_putm_internal.o buffer_putm_internal_flush.o buffer_putnlflush.o buffer_putnspace.o buffer_putsa.o buffer_putsaflush.o buffer_putsalign.o buffer_puts.o buffer_putsflush.o buffer_putspace.o buffer_putuint64.o buffer_putulong.o buffer_putulonglong.o buffer_putxlong.o buffer_seek.o buffer_skip_until.o buffer_stubborn2.o buffer_stubborn.o buffer_tosa.o buffer_truncfile.o 
	$(CROSS_COMPILE)$(AR) rcs $@ $^

$(BUILDDIR)byte.a: \
  byte_chr.o byte_copy.o byte_copyr.o byte_diff.o byte_fill.o byte_rchr.o byte_zero.o 
	$(CROSS_COMPILE)$(AR) rcs $@ $^

$(BUILDDIR)dir.a: \
  dir_close.o dir_name.o dir_open.o dir_read.o dir_time.o dir_type.o  utf8.o
	$(CROSS_COMPILE)$(AR) rcs $@ $^

$(BUILDDIR)fmt.a: \
  fmt_8long.o fmt_8longlong.o fmt_asn1derlength.o fmt_asn1dertag.o fmt_double.o fmt_escapecharc.o fmt_escapecharhtml.o fmt_escapecharquotedprintable.o fmt_escapecharquotedprintableutf8.o fmt_escapecharxml.o fmt_fill.o fmt_httpdate.o fmt_human.o fmt_humank.o fmt_iso8601.o fmt_long.o fmt_longlong.o fmt_minus.o fmt_pad.o fmt_plusminus.o fmt_str.o fmt_strm_internal.o fmt_strn.o fmt_tohex.o fmt_uint64.o fmt_ulong0.o fmt_ulong.o fmt_ulonglong.o fmt_utf8.o fmt_xlong.o fmt_xlonglong.o fmt_xmlescape.o 
	$(CROSS_COMPILE)$(AR) rcs $@ $^

$(BUILDDIR)hmap.a: \
  hmap_add.o hmap_add_tuple_with_data.o hmap_delete.o hmap_destroy.o hmap_free_data.o hmap_init.o hmap_internal.o hmap_is_locate.o hmap_print_list.o hmap_print_table.o hmap_print_tree.o hmap_search.o hmap_set.o hmap_truncate.o 
	$(CROSS_COMPILE)$(AR) rcs $@ $^

$(BUILDDIR)iarray.a: \
  iarray_allocate.o iarray_get.o iarray_init.o 
	$(CROSS_COMPILE)$(AR) rcs $@ $^

$(BUILDDIR)io.a: \
  io_appendfile.o io_block.o io_canread.o io_canwrite.o io_check.o io_close.o io_closeonexec.o io_createfile.o io_debugstring.o io_dontwantread.o io_dontwantwrite.o io_eagain.o io_eagain_read.o io_eagain_write.o io_fd.o io_finishandshutdown.o io_getcookie.o io_mmapwritefile.o io_nonblock.o io_passfd.o io_pipe.o io_readfile.o io_readwritefile.o io_receivefd.o io_sendfile.o io_setcookie.o io_sigpipe.o io_socketpair.o io_timedout.o io_timeout.o io_timeouted.o io_tryread.o io_tryreadtimeout.o io_trywrite.o io_trywritetimeout.o io_wait.o io_waitread.o io_waituntil2.o io_waituntil.o io_waitwrite.o io_wantread.o io_wantwrite.o  iopause.o 
	$(CROSS_COMPILE)$(AR) rcs $@ $^

$(BUILDDIR)mmap.a: \
  mmap_map.o mmap_private.o mmap_read.o mmap_read_fd.o mmap_unmap.o 
	$(CROSS_COMPILE)$(AR) rcs $@ $^

$(BUILDDIR)ndelay.a: \
  ndelay_on.o 
	$(CROSS_COMPILE)$(AR) rcs $@ $^

$(BUILDDIR)open.a: \
  open_append.o open_read.o open_rw.o open_trunc.o 
	$(CROSS_COMPILE)$(AR) rcs $@ $^

$(BUILDDIR)rdir.a: \
  rdir_close.o rdir_open.o rdir_read.o 
	$(CROSS_COMPILE)$(AR) rcs $@ $^

$(BUILDDIR)scan.a: \
  scan_double.o scan_fromhex.o scan_uint.o scan_ulong.o scan_ulonglong.o scan_ulongn.o scan_ushort.o scan_xlong.o scan_xlonglong.o 
	$(CROSS_COMPILE)$(AR) rcs $@ $^

$(BUILDDIR)socket.a: \
  socket_connect4.o socket_connect6.o socket_connected.o socket_ip4loopback.o socket_noipv6.o socket_tcp4b.o socket_tcp4.o socket_tcp6b.o socket_tcp6.o socket_v4mappedprefix.o socket_v6loopback.o  winsock2errno.o winsock_init.o
	$(CROSS_COMPILE)$(AR) rcs $@ $^

$(BUILDDIR)stralloc.a: \
  stralloc_append.o stralloc_append_sa.o stralloc_catb.o stralloc_cat.o stralloc_catc.o stralloc_catlong0.o stralloc_cats.o stralloc_catulong0.o stralloc_copyb.o stralloc_copy.o stralloc_copys.o stralloc_diffs.o stralloc_free.o stralloc_init.o stralloc_insertb.o stralloc_move.o stralloc_nul.o stralloc_ready.o stralloc_readyplus.o stralloc_remove.o stralloc_trunc.o stralloc_write.o stralloc_zero.o 
	$(CROSS_COMPILE)$(AR) rcs $@ $^

$(BUILDDIR)strarray.a: \
  strarray_push.o strarray_pushd.o 
	$(CROSS_COMPILE)$(AR) rcs $@ $^

$(BUILDDIR)str.a: \
  str_basename.o str_chr.o str_diff.o str_diffn.o str_dup.o str_istr.o str_len.o str_ptime.o str_rchr.o str_tok.o 
	$(CROSS_COMPILE)$(AR) rcs $@ $^

$(BUILDDIR)strlist.a: \
  strlist_at.o strlist_cat.o strlist_count.o strlist_dump.o strlist_index_of.o strlist_join.o strlist_pushb.o strlist_push.o strlist_pushm_internal.o strlist_push_sa.o strlist_pushsa.o strlist_push_tokens.o strlist_push_unique.o strlist_shift.o strlist_sort.o strlist_to_argv.o strlist_unshift.o 
	$(CROSS_COMPILE)$(AR) rcs $@ $^

$(BUILDDIR)time.a: \
  time_table_spd.o 
	$(CROSS_COMPILE)$(AR) rcs $@ $^

$(BUILDDIR)uint16.a: \
  uint16_pack_big.o 
	$(CROSS_COMPILE)$(AR) rcs $@ $^

$(BUILDDIR)uint32.a: \
  uint32_pack_big.o uint32_pack.o uint32_unpack_big.o uint32_unpack.o 
	$(CROSS_COMPILE)$(AR) rcs $@ $^
$(BUILDDIR)tai.a: \
  tai_add.o tai_now.o tai_pack.o tai_sub.o tai_uint.o tai_unpack.o 
	$(CROSS_COMPILE)$(AR) rcs $@ $^

$(BUILDDIR)taia.a: \
  taia_add.o taia_addsec.o taia_approx.o taia_frac.o taia_half.o taia_less.o taia_now.o taia_pack.o taia_sub.o taia_tai.o taia_uint.o taia_unpack.o 
	$(CROSS_COMPILE)$(AR) rcs $@ $^

$(BUILDDIR)http.a: \
  http_get.o http_init.o http_readable.o http_sendreq.o http_writeable.o 
	$(CROSS_COMPILE)$(AR) rcs $@ $^


$(BUILDDIR)list.a: \
  $(patsubst %.c,$(BUILDDIR)%.o,$(notdir $(wildcard lib/list_*.c)))
	$(CROSS_COMPILE)$(AR) rcs $@ $^

$(BUILDDIR)playlist.a: \
  $(patsubst %.c,$(BUILDDIR)%.o,$(notdir $(wildcard lib/playlist_*.c)))
	$(CROSS_COMPILE)$(AR) rcs $@ $^

$(BUILDDIR)decode-ls-lR.o: decode-ls-lR.c
$(BUILDDIR)decode-ls-lR$(M64_)$(EXESUFFIX)$(EXEEXT): \
  decode-ls-lR.o buffer.a str.a stralloc.a byte.a
	$(CROSS_COMPILE)$(CC) $(LDFLAGS) $(CFLAGS) -o $@ $^ $(LIBS)

$(BUILDDIR)count-depth.o: count-depth.c
$(BUILDDIR)count-depth$(M64_)$(EXESUFFIX)$(EXEEXT): \
  count-depth.o buffer.a byte.a fmt.a
	$(CROSS_COMPILE)$(CC) $(LDFLAGS) $(CFLAGS) -o $@ $^ $(LIBS)
ifeq ($(DO_STRIP),1)
	$(CROSS_COMPILE)$(STRIP) --strip-all $@
endif

$(BUILDDIR)fnmatch.o: fnmatch.c


$(BUILDDIR)list-r.o: list-r.c
$(BUILDDIR)list-r$(M64_)$(EXESUFFIX)$(EXEEXT): \
  list-r.o fnmatch.o array.a buffer.a  stralloc.a byte.a rdir.a dir.a fmt.a str.a
	$(CROSS_COMPILE)$(CC) $(LDFLAGS) $(CFLAGS) -o $@ $^ $(LIBS)
ifeq ($(DO_STRIP),1)
	$(CROSS_COMPILE)$(STRIP) --strip-all $@
endif

$(BUILDDIR)rdir-test.o: rdir-test.c
$(BUILDDIR)rdir-test$(M64_)$(EXESUFFIX)$(EXEEXT): \
  rdir-test.o fnmatch.o rdir.a dir.a array.a buffer.a  stralloc.a byte.a fmt.a str.a
	$(CROSS_COMPILE)$(CC) $(LDFLAGS) $(CFLAGS) -o $@ $^ $(LIBS)
ifeq ($(DO_STRIP),1)
	$(CROSS_COMPILE)$(STRIP) --strip-all $@
endif

$(BUILDDIR)reg2cmd.o: reg2cmd.c
$(BUILDDIR)reg2cmd$(M64_)$(EXESUFFIX)$(EXEEXT): \
  reg2cmd.o buffer.a fmt.a scan.a str.a stralloc.a byte.a
	$(CROSS_COMPILE)$(CC) $(LDFLAGS) $(CFLAGS) -o $@ $^ $(LIBS)
ifeq ($(DO_STRIP),1)
	$(CROSS_COMPILE)$(STRIP) --strip-all $@
endif

$(BUILDDIR)regfilter.o: regfilter.c
$(BUILDDIR)regfilter$(M64_)$(EXESUFFIX)$(EXEEXT): \
  regfilter.o buffer.a fmt.a scan.a str.a stralloc.a byte.a
	$(CROSS_COMPILE)$(CC) $(LDFLAGS) $(CFLAGS) -o $@ $^ $(LIBS)
ifeq ($(DO_STRIP),1)
	$(CROSS_COMPILE)$(STRIP) --strip-all $@
endif

$(BUILDDIR)torrent-progress.o: torrent-progress.c
$(BUILDDIR)torrent-progress$(M64_)$(EXESUFFIX)$(EXEEXT): \
  torrent-progress.o buffer.a fmt.a mmap.a open.a str.a byte.a
	$(CROSS_COMPILE)$(CC) $(LDFLAGS) $(CFLAGS) -o $@ $^ $(LIBS)
ifeq ($(DO_STRIP),1)
	$(CROSS_COMPILE)$(STRIP) --strip-all $@
endif

$(BUILDDIR)mediathek-parser$(M64_)$(EXESUFFIX)$(EXEEXT): \
  mediathek-parser.o array.a buffer.a fmt.a mmap.a open.a  str.a strlist.a stralloc.a byte.a isleap.o time_table_spd.o
	$(CROSS_COMPILE)$(CC) $(LDFLAGS) $(CFLAGS) -o $@ $^ $(LIBS)  
ifeq ($(DO_STRIP),1)
	$(CROSS_COMPILE)$(STRIP) --strip-all $@
endif

$(BUILDDIR)mediathek-list$(M64_)$(EXESUFFIX)$(EXEEXT): \
  mediathek-list.o array.a strlist.a buffer.a fmt.a mmap.a open.a  scan.a str.a stralloc.a byte.a isleap.o time_table_spd.o
	$(CROSS_COMPILE)$(CC) $(LDFLAGS) $(CFLAGS) -o $@ $^ $(LIBS)  
ifeq ($(DO_STRIP),1)
	$(CROSS_COMPILE)$(STRIP) --strip-all $@
endif

$(BUILDDIR)httptest$(M64_)$(EXESUFFIX)$(EXEEXT): LIBS += $(OTHERLIBS)
$(BUILDDIR)httptest$(M64_)$(EXESUFFIX)$(EXEEXT): \
  httptest.o http.a socket.a io.a ndelay.a uint16.a iarray.a array.a buffer.a fmt.a mmap.a open.a  scan.a stralloc.a str.a byte.a   taia.a  uint32.a  
	$(CROSS_COMPILE)$(CC) $(LDFLAGS) $(CFLAGS) -o $@ $^ $(LIBS) 
ifeq ($(DO_STRIP),1)
	$(CROSS_COMPILE)$(STRIP) --strip-all $@
endif

$(BUILDDIR)xmltest$(M64_)$(EXESUFFIX)$(EXEEXT): LIBS += $(LIBXML2_LIBS)
$(BUILDDIR)xmltest$(M64_)$(EXESUFFIX)$(EXEEXT): CFLAGS += $(LIBXML2_CFLAGS)
$(BUILDDIR)xmltest$(M64_)$(EXESUFFIX)$(EXEEXT): \
  xmltest.o 
	$(CROSS_COMPILE)$(CC) $(LDFLAGS) $(CFLAGS) -o $@ $^ $(LIBS) 
ifeq ($(DO_STRIP),1)
	$(CROSS_COMPILE)$(STRIP) --strip-all $@
endif

$(BUILDDIR)xmltest2$(M64_)$(EXESUFFIX)$(EXEEXT): LIBS += $(LIBXML2_LIBS)
$(BUILDDIR)xmltest2$(M64_)$(EXESUFFIX)$(EXEEXT): CFLAGS += $(LIBXML2_CFLAGS)
$(BUILDDIR)xmltest2$(M64_)$(EXESUFFIX)$(EXEEXT): \
  xmltest2.o 
	$(CROSS_COMPILE)$(CC) $(LDFLAGS) $(CFLAGS) -o $@ $^ $(LIBS) 
ifeq ($(DO_STRIP),1)
	$(CROSS_COMPILE)$(STRIP) --strip-all $@
endif

$(BUILDDIR)xmltest3$(M64_)$(EXESUFFIX)$(EXEEXT): LIBS += $(LIBXML2_LIBS)
$(BUILDDIR)xmltest3$(M64_)$(EXESUFFIX)$(EXEEXT): CFLAGS += $(LIBXML2_CFLAGS)
$(BUILDDIR)xmltest3$(M64_)$(EXESUFFIX)$(EXEEXT): \
  xmltest3.o 
	$(CROSS_COMPILE)$(CC) $(LDFLAGS) $(CFLAGS) -o $@ $^ $(LIBS) 
ifeq ($(DO_STRIP),1)
	$(CROSS_COMPILE)$(STRIP) --strip-all $@
endif

$(BUILDDIR)plstest$(M64_)$(EXESUFFIX)$(EXEEXT): LIBS += $(LIBXML2_LIBS)
$(BUILDDIR)plstest$(M64_)$(EXESUFFIX)$(EXEEXT): CFLAGS += $(LIBXML2_CFLAGS)
$(BUILDDIR)plstest$(M64_)$(EXESUFFIX)$(EXEEXT): \
  plstest.o  playlist.a stralloc.a  buffer.a mmap.a open.a str.a fmt.a scan.a  byte.a 
	$(CROSS_COMPILE)$(CC) $(LDFLAGS) $(CFLAGS) -o $@ $^ $(LIBS) 
ifeq ($(DO_STRIP),1)
	$(CROSS_COMPILE)$(STRIP) --strip-all $@
endif

$(BUILDDIR)opensearch-dump$(M64_)$(EXESUFFIX)$(EXEEXT): INCLUDES += $(LIBXML2_CFLAGS) $(ICONV_CFLAGS)
$(BUILDDIR)opensearch-dump$(M64_)$(EXESUFFIX)$(EXEEXT): LIBS += $(LIBXML2_LIBS) $(ICONV_LIBS) $(OTHERLIBS)
$(BUILDDIR)opensearch-dump$(M64_)$(EXESUFFIX)$(EXEEXT): \
  opensearch-dump.o buffer.a str.a stralloc.a byte.a
	$(CROSS_COMPILE)$(CC) $(LDFLAGS) $(CFLAGS) -o $@ $^ $(LIBS)  
ifeq ($(DO_STRIP),1)
	$(CROSS_COMPILE)$(STRIP) --strip-all $@
endif

$(BUILDDIR)eagle-init-brd$(M64_)$(EXESUFFIX)$(EXEEXT): INCLUDES += $(LIBXML2_CFLAGS) $(ICONV_CFLAGS)
$(BUILDDIR)eagle-init-brd$(M64_)$(EXESUFFIX)$(EXEEXT): LIBS += $(LIBXML2_LIBS) $(ICONV_LIBS) $(OTHERLIBS) -lm
$(BUILDDIR)eagle-init-brd$(M64_)$(EXESUFFIX)$(EXEEXT): \
  eagle-init-brd.o buffer.a str.a stralloc.a byte.a scan.a hmap.a
	$(CROSS_COMPILE)$(CC) $(LDFLAGS) $(CFLAGS) -o $@ $^ $(LIBS)  
ifeq ($(DO_STRIP),1)
	#$(CROSS_COMPILE)$(STRIP) --strip-all $@
endif



$(BUILDDIR)xc8-wrapper/:
	mkdir -p $@

$(BUILDDIR)compiler-wrapper$(M64_)$(EXESUFFIX)$(EXEEXT): \
  compiler-wrapper.o dir.a strlist.a stralloc.a buffer.a fmt.a str.a  byte.a 
	$(CROSS_COMPILE)$(CC) $(LDFLAGS) $(CFLAGS) -o $@ $^ $(LIBS)  
ifeq ($(DO_STRIP),1)
	$(CROSS_COMPILE)$(STRIP) --strip-all $@
endif

$(BUILDDIR)xc8-wrapper$(M64_)$(EXESUFFIX)$(EXEEXT): DEFS += -DXC8_WRAPPER=1
$(BUILDDIR)xc8-wrapper$(M64_)$(EXESUFFIX)$(EXEEXT): \
  compiler-wrapper.o strlist.a stralloc.a buffer.a str.a byte.a fmt.a dir.a 
	$(CROSS_COMPILE)$(CC) $(LDFLAGS) $(CFLAGS) -o $@ $^ $(LIBS)  
ifeq ($(DO_STRIP),1)
	$(CROSS_COMPILE)$(STRIP) --strip-all $@
endif

$(BUILDDIR)picc-wrapper$(M64_)$(EXESUFFIX)$(EXEEXT): DEFS += -DPICC_WRAPPER=1
$(BUILDDIR)picc-wrapper$(M64_)$(EXESUFFIX)$(EXEEXT): \
  compiler-wrapper.o strlist.a stralloc.a buffer.a str.a byte.a fmt.a dir.a 
	$(CROSS_COMPILE)$(CC) $(LDFLAGS) $(CFLAGS) -o $@ $^ $(LIBS)  
ifeq ($(DO_STRIP),1)
	$(CROSS_COMPILE)$(STRIP) --strip-all $@
endif

$(BUILDDIR)picc18-wrapper$(M64_)$(EXESUFFIX)$(EXEEXT): DEFS += -DPICC18_WRAPPER=1
$(BUILDDIR)picc18-wrapper$(M64_)$(EXESUFFIX)$(EXEEXT): \
  compiler-wrapper.o strlist.a stralloc.a buffer.a str.a byte.a fmt.a dir.a 
	$(CROSS_COMPILE)$(CC) $(LDFLAGS) $(CFLAGS) -o $@ $^ $(LIBS)  
ifeq ($(DO_STRIP),1)
	$(CROSS_COMPILE)$(STRIP) --strip-all $@
endif

$(BUILDDIR)sdcc-wrapper$(M64_)$(EXESUFFIX)$(EXEEXT): DEFS += -DSDCC_WRAPPER=1
$(BUILDDIR)sdcc-wrapper$(M64_)$(EXESUFFIX)$(EXEEXT): \
  compiler-wrapper.o strlist.a stralloc.a buffer.a str.a byte.a fmt.a dir.a 
	$(CROSS_COMPILE)$(CC) $(LDFLAGS) $(CFLAGS) -o $@ $^ $(LIBS)  
ifeq ($(DO_STRIP),1)
	$(CROSS_COMPILE)$(STRIP) --strip-all $@
endif

ifeq ($(DO_CXX),1)
$(BUILDDIR)piccfghex.o: piccfghex.cpp
$(BUILDDIR)piccfghex$(M64_)$(EXESUFFIX)$(EXEEXT): \
  piccfghex.o intelhex.o
	$(CROSS_COMPILE)$(CXX) $(LDFLAGS) $(CXXFLAGS) -o $@ $^ $(LIBS)
ifeq ($(DO_STRIP),1)
	$(CROSS_COMPILE)$(STRIP) --strip-all $@
endif

$(BUILDDIR)mediathek-parser-cpp.o: mediathek-parser.cpp
	$(CROSS_COMPILE)$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(INCLUDES) -c -o $@ $<

$(BUILDDIR)mediathek-parser-cpp$(M64_)$(EXESUFFIX)$(EXEEXT): \
  mediathek-parser-cpp.o
	$(CROSS_COMPILE)$(CXX) $(LDFLAGS) $(CXXFLAGS) -o $@ $^ $(LIBS)
ifeq ($(DO_STRIP),1)
	$(CROSS_COMPILE)$(STRIP) --strip-all $@
endif

	$(CROSS_COMPILE)$(CC) $(LDFLAGS) $(CFLAGS) -o $@ $^ $(LIBS)
$(BUILDDIR)kbd-adjacency.o: kbd-adjacency.cpp
	$(CROSS_COMPILE)$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(INCLUDES) -c -o  $(BUILDDIR)$(patsubst %.cpp,%.o,$(notdir $<))  $<

$(BUILDDIR)kbd-adjacency$(M64_)$(EXESUFFIX)$(EXEEXT): \
  kbd-adjacency.o $(LIB_OBJ)
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
	$(CROSS_COMPILE)$(CC) $(CPPFLAGS) $(CFLAGS) $(INCLUDES) -c -o $(BUILDDIR)$(patsubst lib/%.c,%.o,$<) $<

$(BUILDDIR)%.o: %.c
	$(CROSS_COMPILE)$(CC) $(CPPFLAGS) $(CFLAGS) $(INCLUDES) -c -o $(BUILDDIR)$(patsubst %.c,%.o,$<) $<

.cpp.o:
	$(CROSS_COMPILE)$(CXX) $(CXXOPTS) $(CPPFLAGS) $(CXXFLAGS) -c $<

$(BUILDDIR)%.o: %.cpp
	$(CROSS_COMPILE)$(CXX) $(CXXOPTS) $(CPPFLAGS) $(CXXFLAGS) $(INCLUDES) -c -o $(BUILDDIR)$(patsubst %.cpp,%.o,$<) $<
endif

clean:
	$(RM) -f $(OBJECTS) list-r.o list-r$(M64_)$(EXESUFFIX)$(EXEEXT)

install: $(PROGRAMS)
	$(INSTALL) -d $(DESTDIR)$(bindir)
	$(INSTALL) -m 755 $(PROGRAMS) $(DESTDIR)$(bindir)
	$(INSTALL) -d $(DESTDIR)$(bindir)

install-release:
	$(MAKE) DEBUG=0 install

uninstall:
	@for PROGRAM in $(PROGRAMS); do \
		echo $(RM) $(DESTDIR)$(bindir)/$$PROGRAM; \
		$(RM) $(DESTDIR)$(bindir)/$$PROGRAM; \
  done
LIBSOURCES = array.h array_allocate.c array_bytes.c array_cat.c array_cat0.c array_catb.c array_cate.c array_cats.c array_cats0.c array_equal.c array_fail.c array_get.c array_length.c array_reset.c array_start.c array_trunc.c array_truncate.c buffer.h buffer_0.c buffer_1.c buffer_2.c buffer_close.c buffer_default.c buffer_feed.c buffer_flush.c buffer_free.c buffer_fromsa.c buffer_fromstr.c buffer_get.c buffer_get_new_token_sa.c buffer_get_new_token_sa_pred.c buffer_get_token.c buffer_get_token_pred.c buffer_get_token_sa.c buffer_get_token_sa_pred.c buffer_get_until.c buffer_getc.c buffer_getline.c buffer_getline_sa.c buffer_init.c buffer_mmapprivate.c buffer_mmapread.c buffer_mmapread_fd.c buffer_prefetch.c buffer_put.c buffer_putc.c buffer_putflush.c buffer_putm_internal.c buffer_putnlflush.c buffer_putnspace.c buffer_puts.c buffer_putsa.c buffer_putsflush.c buffer_putspace.c buffer_putuint64.c buffer_putulong.c buffer_putulonglong.c buffer_putxlong.c buffer_skip_until.c buffer_stubborn.c buffer_stubborn2.c buffer_tosa.c buffer_truncfile.c byte.h byte_chr.c byte_copy.c byte_copyr.c byte_diff.c byte_fill.c byte_zero.c dir.h dir_close.c dir_internal.h dir_open.c dir_read.c dir_time.c dir_type.c fmt.h fmt_minus.c fmt_uint64.c fmt_ulong.c fmt_ulong0.c fmt_ulonglong.c fmt_xlong.c fmt_xlonglong.c mmap.h mmap_map.c mmap_private.c mmap_read.c mmap_read_fd.c mmap_unmap.c open.h open_append.c open_read.c open_rw.c open_trunc.c scan.h scan_fromhex.c scan_ushort.c scan_xlong.c scan_xlonglong.c str.h str_diffn.c str_len.c stralloc.h stralloc_append.c stralloc_cat.c stralloc_catb.c stralloc_catc.c stralloc_catlong0.c stralloc_cats.c stralloc_catulong0.c stralloc_copy.c stralloc_copyb.c stralloc_copys.c stralloc_diffs.c stralloc_free.c stralloc_init.c stralloc_insertb.c stralloc_move.c stralloc_nul.c stralloc_ready.c stralloc_readyplus.c stralloc_remove.c stralloc_trunc.c stralloc_write.c stralloc_zero.c strlist.h strlist_push.c strlist_pushunique.c strlist_at.c strlist_count.c strlist_index_of.c strlist_sort.c

slackpkg: all-release
	@set -x; distdir="_inst"; rm -rf $$distdir; \
		$(MAKE) install-release DESTDIR="$$(realpath $$distdir)"; \
		tar -cJf dirlist-`date +%Y%m%d`-slackware.txz -C $$distdir .; \
		rm -rf $$distdir


inst-slackpkg: slackpkg
	for x in /m*/*/pmagic/pmodules/; do \
		rm -vf "$$x"/dirlist-*.txz; \
		cp -vf dirlist-`date +%Y%m%d`-slackware.txz "$$x"; \
  done


