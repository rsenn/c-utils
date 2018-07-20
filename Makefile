DEBUG = 1
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

file-exists = $(shell echo "Checking for $(1) ..." 1>&2; test -e "$(1)" && echo 1)
cmd-exists = $(shell type "$(1)" 2>/dev/null >/dev/null && echo 1)
file-exists = $(shell test -e "$(1)" 2>/dev/null >/dev/null && echo 1)

#$(info ROOTNAME=$(ROOTNAME))
#$(info SYSNAME=$(SYSNAME))

OS ?= $(shell uname -o | tr "[[:upper:]]" "[[:lower:]]")

define cmds-function-exists =
set -e; 
echo "extern int $(1)();
int main() {
  $(1)();
  return 0;
}" >test_$(1).c;
$(CROSS_COMPILE)$(CC) -o test_$(1) test_$(1).c;
endef
cmds-exitcode = ($(1)) 2>&1 >>check.log && echo 1 || echo 0

check-function-exists = $(shell $(call cmds-exitcode,$(cmds-function-exists)))


HAVE_LSEEK64 := $(call check-function-exists,lseek64)
HAVE_LSEEK := $(call check-function-exists,lseek)
HAVE_LLSEEK := $(call check-function-exists,llseek)
HAVE_POSIX_MEMALIGN := $(call check-function-exists,posix_memalign)

$(info HAVE_LSEEK64=$(HAVE_LSEEK64) HAVE_LSEEK=$(HAVE_LSEEK64)  HAVE_LLSEEK=$(HAVE_LLSEEK64))
#$(info llseek: $(call check-function-exists,llseek))
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
PREFIX := $(patsubst %/bin/diet,%,$(DIET))
USE_DIET := 1
endif
endif
endif
ifneq ($(CROSS_COMPILE),$(subst diet,,$(CROSS_COMPILE)))
USE_DIET := 1
endif
ifneq ($(CROSS_COMPILE),$(subst musl,,$(CROSS_COMPILE)))
USE_MUSL := 1
endif

$(info PREFIX: $(PREFIX))
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
ifeq ($(USE_MUSL),1)
    HOST := $(subst gnu,musl,$(BUILD))
else
ifeq ($(USE_DIET),1)
    HOST := $(shell set -x; $(CC) -dumpmachine  | sed 's|[-.0-9]*\\\$$|| ;; s|\\r\$$|| ;; s|^\([^-]*\)-\([^-]*\)-\([^-]*\)-gnu|\1-\2-\3-diet| ;; s|^\([^-]*\)-\([^-]*\)-\([^-]*\)|\1-diet-\3|' )
else
    HOST := $(shell set -x; $(CROSS_COMPILE)$(CC) -dumpmachine  | sed 's|[-.0-9]*\\\$$|| ;; s|\\r\$$||' )
    endif
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

PKG_CONFIG := pkg-config
ifneq ($(CROSS_COMPILE),)
ifeq ($(call cmd-exists,$(CROSS_COMPILE)$(PKG_CONFIG)),1)
PKG_CONFIG := $(CROSS_COMPILE)$(PKG_CONFIG)
else
  P := $(shell set -x; ls -d /usr/$(CROSS_COMPILE:%-=%)/sys*root/*/lib/pkgconfig)
  ifeq ($(call file-exists,$(P)),1)
  PKG_CONFIG_PATH := $(P)
  endif
endif
endif

AR := ar
ifeq ($(call cmd-exists,$(CROSS_COMPILE)$(AR)),1)
AR := $(CROSS_COMPILE)$(AR)
endif

RANLIB := ranlib
ifeq ($(call cmd-exists,$(RANLIB)),1)
RANLIB := $(RANLIB)
endif

STRIP := strip
ifeq ($(call cmd-exists,$(STRIP)),1)
STRIP := $(STRIP)
endif

$(info PKG_CONFIG: $(PKG_CONFIG))
$(info PKG_CONFIG_PATH: $(PKG_CONFIG_PATH))

#PKG_CONFIG_CMD := $(if $(SYSROOT)$(PKG_CONFIG_PATH),env $(if $(SYSROOT),PKG_CONFIG_SYSROOT_DIR=$(SYSROOT) ,)$(if $(PKG_CONFIG_PATH),PKG_CONFIG_PATH=$(PKG_CONFIG_PATH) ,),)$(PKG_CONFIG)
PKG_CONFIG_CMD := $(if $(SYSROOT)$(PKG_CONFIG_PATH),env  ,)$(if $(PKG_CONFIG_PATH),PKG_CONFIG_PATH=$(PKG_CONFIG_PATH) ,)$(PKG_CONFIG)
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

vpath lib lib/array lib/buffer lib/byte lib/dir lib/fmt lib/hmap lib/http lib/io lib/list lib/mmap lib/open lib/pe lib/playlist lib/map lib/scan lib/socket lib/str lib/stralloc lib/tai lib/taia lib/uint16 lib/uint32 lib/uint64 $(BUILDDIR)
VPATH = lib:lib/array:lib/buffer:lib/byte:lib/dir:lib/fmt:lib/hmap:lib/http:lib/io:lib/list:lib/mmap:lib/open:lib/pe:lib/playlist:lib/map:lib/scan:lib/socket:lib/str:lib/stralloc:lib/tai:lib/taia:lib/uint16:lib/uint32:lib/uint64:$(BUILDDIR)
ifeq ($(CXXOPTS),)
##$(info OS: "$(OS)")
ifneq ($(OS),msys)
endif
endif

CPPFLAGS := -I.

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
ifeq ($(USE_DIET),1)
STATIC := 1
endif
ifneq ($(STATIC),1)
STATIC := 0
endif

$(info DIET: $(DIET))
$(info STATIC: $(STATIC))
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
LDFLAGS += -static-libgcc 
DEFS += open=_open read=_read write=_write close=_close
#LDFLAGS += -static-lib{asan,gfortran,lsan,tsan,ubsan}
WIN32 := 1
endif
ifeq ($(CYGWIN),1)
WIN32 := 1
endif
ifeq ($(STATIC),1)
#LDFLAGS += -static
PKG_CONFIG += --static
ifeq ($(MINGW)$(STATIC),10)
LDFLAGS += -static-libgcc -static-libstdc++
else
LDFLAGS += -static
endif
endif
ifeq ($(WIN32),1)
EXTRA_LIBS += -lkernel32
endif
ifeq ($(STATIC_LIBGCC),1)
LDFLAGS += -static-libgcc
endif
ifeq ($(STATIC_LIBSTDCXX),1)
LDFLAGS += -static-libstdc++
endif
ifeq ($(STRIP),1)
LDFLAGS += -s
endif
RM = rm -f
ifneq ($(shell uname -s)-$(shell uname -o),MINGW32_NT-5.1-Msys)
ifeq ($(STATIC),)
ifeq ($(CYGWIN),)
LDFLAGS +=  -static-libgcc -static-libstdc++
endif
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

#$(info ICONV_CFLAGS: $(ICONV_CFLAGS))
#$(info ICONV_LIBS: $(ICONV_LIBS))

LIBXML2_CFLAGS := $(call pkg-conf,--cflags,libxml-2.0 liblzma zlib)
LIBXML2_LIBS := $(call pkg-conf,--libs,libxml-2.0 liblzma zlib)
ifeq ($(USE_DIET),1)
STATIC := 1
endif
ifeq ($(STATIC),1)
LIBXML2_LIBS += $(OTHERLIBS)
LIBXML2_LIBS += -liconv -lpthread -lm
endif

$(info LIBXML2_CFLAGS: $(LIBXML2_CFLAGS))
$(info LIBXML2_LIBS: $(LIBXML2_LIBS))


PROGRAMS = $(patsubst %,$(BUILDDIR)%$(M64_)$(EXESUFFIX)$(EXEEXT),list-r count-depth decode-ls-lR reg2cmd regfilter torrent-progress mediathek-parser mediathek-list xc8-wrapper picc-wrapper picc18-wrapper sdcc-wrapper rdir-test httptest xmltest xmltest2 xmltest3 xmltest4 plsconv compiler-wrapper impgen pathtool ntldd hexedit eagle-init-brd eagle-gen-cmds)


 #opensearch-dump 
LIBSOURCES = $(wildcard lib/*/*.c) 
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
ifneq ($(HAVE_LSEEK64),)
LSEEK = lseek64
else
ifeq ($(HAVE_LSEEK),1)
LSEEK = lseek
else
ifeq ($(HAVE__LLSEEK),1)
LSEEK = _llseek
else
ifeq ($(HAVE_LLSEEK),1)
LSEEK = llseek
else
ifeq ($(HAVE__LSEEK),1)
LSEEK = _lseek
else
ifeq ($(HAVE__LSEEKI64),1)
LSEEK = _lseeki64
else
ifeq ($(HAVE_LSEEK),1)
LSEEK = lseek
endif
endif
endif
endif
endif
endif
endif
endif

ifeq ($(LSEEK),)
LSEEK := lseek
endif
ifneq ($(LSEEK),)
DEFS += LSEEK=$(LSEEK)
endif
ifeq ($(HAVE_POSIX_MEMALIGN),1)
DEFS += HAVE_POSIX_MEMALIGN=1
endif


CPPFLAGS += $(DEFS:%=-D%)
CFLAGS += $(DEFS:%=-D%)

$(info DEFS: $(DEFS))


all: \
   $(BUILDDIR) $(PROGRAMS)
all-release:
	$(MAKE) DEBUG=0 all

PROGRAM_OBJECTS = $(patsubst %,$(BUILDDIR)%.o,$(wildcard *.c))
#$(patsubst %,%.o,$(PROGRAMS))

CPPFLAGS := -I.

$(BUILDDIR):
	-mkdir -p $(BUILDDIR) || mkdir $(BUILDDIR)
	-md $(subst /,\,$(BUILDDIR))
	
$(OBJDIR):
	-mkdir -p $(OBJDIR) || mkdir $(OBJDIR)
	-md $(subst /,\,$(OBJDIR))



$(BUILDDIR)array.a: $(BUILDDIR)array_allocate.o $(BUILDDIR)array_bytes.o $(BUILDDIR)array_cat0.o $(BUILDDIR)array_catb.o $(BUILDDIR)array_cat.o $(BUILDDIR)array_cate.o $(BUILDDIR)array_cats0.o $(BUILDDIR)array_cats.o $(BUILDDIR)array_equal.o $(BUILDDIR)array_fail.o $(BUILDDIR)array_get.o $(BUILDDIR)array_length.o $(BUILDDIR)array_reset.o $(BUILDDIR)array_start.o $(BUILDDIR)array_truncate.o $(BUILDDIR)array_trunc.o $(BUILDDIR)/umult64.o
	$(AR) rcs $@ $^

$(BUILDDIR)buffer.a: $(BUILDDIR)buffer_0.o $(BUILDDIR)buffer_0small.o $(BUILDDIR)buffer_1.o $(BUILDDIR)buffer_1small.o $(BUILDDIR)buffer_2.o $(BUILDDIR)buffer_close.o $(BUILDDIR)buffer_default.o $(BUILDDIR)buffer_dummyread.o $(BUILDDIR)buffer_dummyreadmmap.o $(BUILDDIR)buffer_dump.o $(BUILDDIR)buffer_feed.o $(BUILDDIR)buffer_flush.o $(BUILDDIR)buffer_free.o $(BUILDDIR)buffer_fromarray.o $(BUILDDIR)buffer_frombuf.o $(BUILDDIR)buffer_fromsa.o $(BUILDDIR)buffer_fromstr.o $(BUILDDIR)buffer_get.o $(BUILDDIR)buffer_getc.o $(BUILDDIR)buffer_getline.o $(BUILDDIR)buffer_getline_sa.o $(BUILDDIR)buffer_getn.o $(BUILDDIR)buffer_getnewline_sa.o $(BUILDDIR)buffer_get_new_token_sa.o $(BUILDDIR)buffer_get_new_token_sa_pred.o $(BUILDDIR)buffer_get_token.o $(BUILDDIR)buffer_get_token_pred.o $(BUILDDIR)buffer_gettok_sa.o $(BUILDDIR)buffer_get_token_sa.o $(BUILDDIR)buffer_get_token_sa_pred.o $(BUILDDIR)buffer_get_until.o $(BUILDDIR)buffer_init.o $(BUILDDIR)buffer_init_free.o $(BUILDDIR)buffer_mmapprivate.o $(BUILDDIR)buffer_mmapread.o $(BUILDDIR)buffer_mmapread_fd.o $(BUILDDIR)buffer_munmap.o $(BUILDDIR)buffer_peek.o $(BUILDDIR)buffer_prefetch.o $(BUILDDIR)buffer_put8long.o $(BUILDDIR)buffer_putalign.o $(BUILDDIR)buffer_put.o $(BUILDDIR)buffer_putc.o $(BUILDDIR)buffer_puterror2.o $(BUILDDIR)buffer_puterror.o $(BUILDDIR)buffer_putflush.o $(BUILDDIR)buffer_putdouble.o $(BUILDDIR)buffer_putlong.o $(BUILDDIR)buffer_putlonglong.o $(BUILDDIR)buffer_putm_internal.o $(BUILDDIR)buffer_putm_internal_flush.o $(BUILDDIR)buffer_putnlflush.o $(BUILDDIR)buffer_putnspace.o $(BUILDDIR)buffer_putsa.o $(BUILDDIR)buffer_putsaflush.o $(BUILDDIR)buffer_putsalign.o $(BUILDDIR)buffer_puts.o $(BUILDDIR)buffer_putsflush.o $(BUILDDIR)buffer_putspace.o $(BUILDDIR)buffer_putuint64.o $(BUILDDIR)buffer_putulong0.o $(BUILDDIR)buffer_putulong.o $(BUILDDIR)buffer_putulonglong.o $(BUILDDIR)buffer_putxlong.o $(BUILDDIR)buffer_seek.o $(BUILDDIR)buffer_skip_until.o $(BUILDDIR)buffer_stubborn2.o $(BUILDDIR)buffer_stubborn.o $(BUILDDIR)buffer_tosa.o $(BUILDDIR)buffer_truncfile.o $(BUILDDIR)buffer_read_fd.o $(BUILDDIR)buffer_putptr.o $(BUILDDIR)buffer_peekc.o $(BUILDDIR)buffer_skipc.o 
	$(AR) rcs $@ $^

$(BUILDDIR)byte.a: $(BUILDDIR)byte_case_diff.o $(BUILDDIR)byte_case_equal.o $(BUILDDIR)byte_case_start.o $(BUILDDIR)byte_chr.o $(BUILDDIR)byte_copy.o $(BUILDDIR)byte_copyr.o $(BUILDDIR)byte_count.o $(BUILDDIR)byte_diff.o $(BUILDDIR)byte_equal.o $(BUILDDIR)byte_fill.o $(BUILDDIR)byte_lower.o $(BUILDDIR)byte_rchr.o $(BUILDDIR)byte_upper.o $(BUILDDIR)byte_zero.o $(BUILDDIR)byte_fmt.o $(BUILDDIR)byte_scan.o

	$(AR) rcs $@ $^

$(BUILDDIR)cbmap.a: $(builddir)alloc.o $(builddir)cbmap_count.o $(builddir)cbmap_data_node_destroy.o $(builddir)cbmap_delete.o $(builddir)cbmap_destroy.o $(builddir)cbmap_get.o $(builddir)cbmap_insert.o $(builddir)cbmap_internal_node.o $(builddir)cbmap_new.o $(builddir)cbmap_visit.o $(builddir)cbmap_visit_all.o

	$(AR) rcs $@ $^


$(BUILDDIR)dir.a: $(BUILDDIR)dir_close.o $(BUILDDIR)dir_name.o $(BUILDDIR)dir_open.o $(BUILDDIR)dir_read.o $(BUILDDIR)dir_time.o $(BUILDDIR)dir_type.o  $(BUILDDIR)utf8.o
	$(AR) rcs $@ $^

$(BUILDDIR)fmt.a: $(BUILDDIR)fmt_8long.o $(BUILDDIR)fmt_8longlong.o $(BUILDDIR)fmt_asn1derlength.o $(BUILDDIR)fmt_asn1dertag.o $(BUILDDIR)fmt_double.o $(BUILDDIR)fmt_escapecharc.o $(BUILDDIR)fmt_escapecharhtml.o $(BUILDDIR)fmt_escapecharquotedprintable.o $(BUILDDIR)fmt_escapecharquotedprintableutf8.o $(BUILDDIR)fmt_escapecharxml.o $(BUILDDIR)fmt_fill.o $(BUILDDIR)fmt_httpdate.o $(BUILDDIR)fmt_human.o $(BUILDDIR)fmt_humank.o $(BUILDDIR)fmt_iso8601.o $(BUILDDIR)fmt_long.o $(BUILDDIR)fmt_longlong.o $(BUILDDIR)fmt_minus.o $(BUILDDIR)fmt_pad.o $(BUILDDIR)fmt_plusminus.o $(BUILDDIR)fmt_str.o $(BUILDDIR)fmt_strm_internal.o $(BUILDDIR)fmt_strn.o $(BUILDDIR)fmt_tohex.o $(BUILDDIR)fmt_uint64.o $(BUILDDIR)fmt_ulong0.o $(BUILDDIR)fmt_ulong.o $(BUILDDIR)fmt_ulonglong.o $(BUILDDIR)fmt_utf8.o $(BUILDDIR)fmt_xlong.o $(BUILDDIR)fmt_xlonglong.o $(BUILDDIR)fmt_xmlescape.o 
	$(AR) rcs $@ $^

$(BUILDDIR)hmap.a: $(BUILDDIR)hmap_add.o $(BUILDDIR)hmap_set_stralloc.o $(BUILDDIR)hmap_set_chars.o $(BUILDDIR)hmap_set.o $(BUILDDIR)hmap_add_tuple_with_data.o $(BUILDDIR)hmap_delete.o $(BUILDDIR)hmap_destroy.o $(BUILDDIR)hmap_free_data.o $(BUILDDIR)hmap_init.o $(BUILDDIR)hmap_internal.o $(BUILDDIR)hmap_is_locate.o $(BUILDDIR)hmap_print_list.o $(BUILDDIR)hmap_print_table.o $(BUILDDIR)hmap_print_tree.o $(BUILDDIR)hmap_search.o $(BUILDDIR)hmap_set.o $(BUILDDIR)hmap_truncate.o $(BUILDDIR)hmap_dump.o 
	$(AR) rcs $@ $^

$(BUILDDIR)iarray.a: $(BUILDDIR)iarray_allocate.o $(BUILDDIR)iarray_get.o $(BUILDDIR)iarray_init.o 
	$(AR) rcs $@ $^

$(BUILDDIR)io.a: $(BUILDDIR)io_appendfile.o $(BUILDDIR)io_block.o $(BUILDDIR)io_canread.o $(BUILDDIR)io_canwrite.o $(BUILDDIR)io_check.o $(BUILDDIR)io_close.o $(BUILDDIR)io_closeonexec.o $(BUILDDIR)io_createfile.o $(BUILDDIR)io_debugstring.o $(BUILDDIR)io_dontwantread.o $(BUILDDIR)io_dontwantwrite.o $(BUILDDIR)io_eagain.o $(BUILDDIR)io_eagain_read.o $(BUILDDIR)io_eagain_write.o $(BUILDDIR)io_fd.o $(BUILDDIR)io_finishandshutdown.o $(BUILDDIR)io_getcookie.o $(BUILDDIR)io_mmapwritefile.o $(BUILDDIR)io_nonblock.o $(BUILDDIR)io_pipe.o $(BUILDDIR)io_readfile.o $(BUILDDIR)io_readwritefile.o $(BUILDDIR)io_sendfile.o $(BUILDDIR)io_setcookie.o $(BUILDDIR)io_sigpipe.o $(BUILDDIR)io_socketpair.o $(BUILDDIR)io_timedout.o $(BUILDDIR)io_timeout.o $(BUILDDIR)io_timeouted.o $(BUILDDIR)io_tryread.o $(BUILDDIR)io_tryreadtimeout.o $(BUILDDIR)io_trywrite.o $(BUILDDIR)io_trywritetimeout.o $(BUILDDIR)io_wait.o $(BUILDDIR)io_waitread.o $(BUILDDIR)io_waituntil2.o $(BUILDDIR)io_waituntil.o $(BUILDDIR)io_waitwrite.o $(BUILDDIR)io_wantread.o $(BUILDDIR)io_wantwrite.o  $(BUILDDIR)iopause.o 
	$(AR) rcs $@ $^

$(BUILDDIR)mmap.a: $(BUILDDIR)mmap_map.o $(BUILDDIR)mmap_private.o $(BUILDDIR)mmap_read.o $(BUILDDIR)mmap_read_fd.o $(BUILDDIR)mmap_unmap.o 
	$(AR) rcs $@ $^

$(BUILDDIR)ndelay.a: $(BUILDDIR)ndelay_on.o 
	$(AR) rcs $@ $^

$(BUILDDIR)open.a: $(BUILDDIR)open_append.o $(BUILDDIR)open_read.o $(BUILDDIR)open_rw.o $(BUILDDIR)open_trunc.o 
	$(AR) rcs $@ $^

$(BUILDDIR)rdir.a: $(BUILDDIR)rdir_close.o $(BUILDDIR)rdir_open.o $(BUILDDIR)rdir_read.o 
	$(AR) rcs $@ $^

$(BUILDDIR)case.a: $(BUILDDIR)case_diffb.o $(BUILDDIR)case_diffs.o $(BUILDDIR)case_lowerb.o $(BUILDDIR)case_lowers.o $(BUILDDIR)case_starts.o
	$(AR) rcs $@ $^

$(BUILDDIR)scan.a: $(BUILDDIR)scan_8int.o $(BUILDDIR)scan_8long.o $(BUILDDIR)scan_8longlong.o $(BUILDDIR)scan_8longn.o $(BUILDDIR)scan_8short.o $(BUILDDIR)scan_asn1derlength.o $(BUILDDIR)scan_asn1dertag.o $(BUILDDIR)scan_charsetnskip.o $(BUILDDIR)scan_double.o $(BUILDDIR)scan_fromhex.o $(BUILDDIR)scan_httpdate.o $(BUILDDIR)scan_int.o $(BUILDDIR)scan_iso8601.o $(BUILDDIR)scan_long.o $(BUILDDIR)scan_longlong.o $(BUILDDIR)scan_longn.o $(BUILDDIR)scan_netstring.o $(BUILDDIR)scan_noncharsetnskip.o $(BUILDDIR)scan_nonwhitenskip.o $(BUILDDIR)scan_pb_tag.o $(BUILDDIR)scan_pb_type0_sint.o $(BUILDDIR)scan_pb_type1_fixed64.o $(BUILDDIR)scan_pb_type5_fixed32.o $(BUILDDIR)scan_plusminus.o $(BUILDDIR)scan_short.o $(BUILDDIR)scan_uint.o $(BUILDDIR)scan_ulong.o $(BUILDDIR)scan_ulonglong.o $(BUILDDIR)scan_ulongn.o $(BUILDDIR)scan_ushort.o $(BUILDDIR)scan_utf8.o $(BUILDDIR)scan_varint.o $(BUILDDIR)scan_whitenskip.o $(BUILDDIR)scan_xint.o $(BUILDDIR)scan_xlong.o $(BUILDDIR)scan_xlonglong.o $(BUILDDIR)scan_xlongn.o $(BUILDDIR)scan_xshort.o $(BUILDDIR)scan_xmlescape.o
	$(AR) rcs $@ $^

$(BUILDDIR)socket.a: $(BUILDDIR)socket_connect4.o $(BUILDDIR)socket_connect6.o $(BUILDDIR)socket_connected.o $(BUILDDIR)socket_ip4loopback.o $(BUILDDIR)socket_noipv6.o $(BUILDDIR)socket_tcp4b.o $(BUILDDIR)socket_tcp4.o $(BUILDDIR)socket_tcp6b.o $(BUILDDIR)socket_tcp6.o $(BUILDDIR)socket_v4mappedprefix.o $(BUILDDIR)socket_v6loopback.o  $(BUILDDIR)winsock2errno.o $(BUILDDIR)winsock_init.o
	$(AR) rcs $@ $^

$(BUILDDIR)stralloc.a: $(BUILDDIR)stralloc_0.o $(BUILDDIR)stralloc_append.o $(BUILDDIR)stralloc_append_sa.o $(BUILDDIR)stralloc_buffer.o $(BUILDDIR)stralloc_case_diff.o $(BUILDDIR)stralloc_case_diffs.o $(BUILDDIR)stralloc_case_end.o $(BUILDDIR)stralloc_case_equal.o $(BUILDDIR)stralloc_case_equals.o $(BUILDDIR)stralloc_case_start.o $(BUILDDIR)stralloc_case_starts.o $(BUILDDIR)stralloc_cat.o $(BUILDDIR)stralloc_catb.o $(BUILDDIR)stralloc_catc.o $(BUILDDIR)stralloc_cathexb.o $(BUILDDIR)stralloc_catint.o $(BUILDDIR)stralloc_catint0.o $(BUILDDIR)stralloc_catlong.o $(BUILDDIR)stralloc_catlong0.o $(BUILDDIR)stralloc_catm_internal.o $(BUILDDIR)stralloc_cats.o $(BUILDDIR)stralloc_catuint.o $(BUILDDIR)stralloc_catuint0.o $(BUILDDIR)stralloc_catulong.o $(BUILDDIR)stralloc_catulong0.o $(BUILDDIR)stralloc_catxlong.o $(BUILDDIR)stralloc_chomp.o $(BUILDDIR)stralloc_chop.o $(BUILDDIR)stralloc_chr.o $(BUILDDIR)stralloc_copy.o $(BUILDDIR)stralloc_copyb.o $(BUILDDIR)stralloc_copys.o $(BUILDDIR)stralloc_count.o $(BUILDDIR)stralloc_decamelize.o $(BUILDDIR)stralloc_diff.o $(BUILDDIR)stralloc_diffb.o $(BUILDDIR)stralloc_diffs.o $(BUILDDIR)stralloc_ends.o $(BUILDDIR)stralloc_endb.o $(BUILDDIR)stralloc_ends.o $(BUILDDIR)stralloc_equal.o $(BUILDDIR)stralloc_equalb.o $(BUILDDIR)stralloc_equals.o $(BUILDDIR)stralloc_erase.o $(BUILDDIR)stralloc_find.o $(BUILDDIR)stralloc_findb.o $(BUILDDIR)stralloc_finds.o $(BUILDDIR)stralloc_free.o $(BUILDDIR)stralloc_init.o $(BUILDDIR)stralloc_insertb.o $(BUILDDIR)stralloc_lower.o $(BUILDDIR)stralloc_move.o $(BUILDDIR)stralloc_nul.o $(BUILDDIR)stralloc_rchr.o $(BUILDDIR)stralloc_ready.o $(BUILDDIR)stralloc_readyplus.o $(BUILDDIR)stralloc_remove.o $(BUILDDIR)stralloc_replace.o $(BUILDDIR)stralloc_replace_non_printable.o $(BUILDDIR)stralloc_start.o $(BUILDDIR)stralloc_startb.o $(BUILDDIR)stralloc_starts.o $(BUILDDIR)stralloc_trim.o $(BUILDDIR)stralloc_trunc.o $(BUILDDIR)stralloc_write.o $(BUILDDIR)stralloc_zero.o $(BUILDDIR)stralloc_fmt.o $(BUILDDIR)stralloc_scan.o
	$(AR) rcs $@ $^

$(BUILDDIR)strarray.a: $(BUILDDIR)strarray_push.o $(BUILDDIR)strarray_pushd.o 
	$(AR) rcs $@ $^

$(BUILDDIR)str.a: $(BUILDDIR)str_basename.o $(BUILDDIR)str_case_diff.o $(BUILDDIR)str_case_diffn.o $(BUILDDIR)str_case_equal.o $(BUILDDIR)str_case_start.o $(BUILDDIR)str_cat.o $(BUILDDIR)str_chr.o $(BUILDDIR)str_copy.o $(BUILDDIR)str_copyb.o $(BUILDDIR)str_decamelize.o $(BUILDDIR)str_diff.o $(BUILDDIR)str_diffn.o $(BUILDDIR)str_dup.o $(BUILDDIR)str_ndup.o $(BUILDDIR)str_equal.o $(BUILDDIR)str_find.o $(BUILDDIR)str_findb.o $(BUILDDIR)str_istr.o $(BUILDDIR)str_len.o $(BUILDDIR)str_lower.o $(BUILDDIR)str_ptime.o $(BUILDDIR)str_rchr.o $(BUILDDIR)str_start.o $(BUILDDIR)str_tok.o
	$(AR) rcs $@ $^

$(BUILDDIR)strlist.a: $(BUILDDIR)strlist_at.o $(BUILDDIR)strlist_cat.o $(BUILDDIR)strlist_count.o $(BUILDDIR)strlist_dump.o $(BUILDDIR)strlist_index_of.o $(BUILDDIR)strlist_join.o $(BUILDDIR)strlist_pushb.o $(BUILDDIR)strlist_push.o $(BUILDDIR)strlist_pushm_internal.o $(BUILDDIR)strlist_push_sa.o $(BUILDDIR)strlist_pushsa.o $(BUILDDIR)strlist_push_tokens.o $(BUILDDIR)strlist_push_unique.o $(BUILDDIR)strlist_shift.o $(BUILDDIR)strlist_sort.o $(BUILDDIR)strlist_to_argv.o  $(BUILDDIR)strlist_unshift.o $(BUILDDIR)strlist_range.o 
	$(AR) rcs $@ $^

$(BUILDDIR)time.a: $(BUILDDIR)time_table_spd.o 
	$(AR) rcs $@ $^

$(BUILDDIR)uint16.a: $(BUILDDIR)uint16_pack_big.o 
	$(AR) rcs $@ $^

$(BUILDDIR)uint32.a: $(BUILDDIR)uint32_pack_big.o $(BUILDDIR)uint32_pack.o $(BUILDDIR)uint32_unpack_big.o $(BUILDDIR)uint32_unpack.o 
	$(AR) rcs $@ $^
$(BUILDDIR)tai.a: $(BUILDDIR)tai_add.o $(BUILDDIR)tai_now.o $(BUILDDIR)tai_pack.o $(BUILDDIR)tai_sub.o $(BUILDDIR)tai_uint.o $(BUILDDIR)tai_unpack.o 
	$(AR) rcs $@ $^

$(BUILDDIR)taia.a: $(BUILDDIR)taia_add.o $(BUILDDIR)taia_addsec.o $(BUILDDIR)taia_approx.o $(BUILDDIR)taia_frac.o $(BUILDDIR)taia_half.o $(BUILDDIR)taia_less.o $(BUILDDIR)taia_now.o $(BUILDDIR)taia_pack.o $(BUILDDIR)taia_sub.o $(BUILDDIR)taia_tai.o $(BUILDDIR)taia_uint.o $(BUILDDIR)taia_unpack.o 
	$(AR) rcs $@ $^

$(BUILDDIR)http.a: $(BUILDDIR)http_get.o $(BUILDDIR)http_init.o $(BUILDDIR)http_readable.o $(BUILDDIR)http_sendreq.o $(BUILDDIR)http_writeable.o 
	$(AR) rcs $@ $^

$(BUILDDIR)pe.a: $(BUILDDIR)pe_offsets.o 
	$(AR) rcs $@ $^


$(BUILDDIR)list.a: $(patsubst %.c,$(BUILDDIR)%.o,$(notdir $(wildcard lib/list_*.c)))
	$(AR) rcs $@ $^

$(BUILDDIR)playlist.a: $(addprefix $(BUILDDIR),playlist_m3u.o playlist_init.o playlist_pls.o playlist_xspf.o playlist_read.o playlist_write_start.o playlist_write_entry.o playlist_write_finish.o )
	$(AR) rcs $@ $^

$(BUILDDIR)map.a: $(addprefix $(BUILDDIR),map_deinit.o map_get.o map_iter.o map_next.o map_remove.o map_set.o)
	$(AR) rcs $@ $^

$(BUILDDIR)xml.a: $(addprefix $(BUILDDIR),xml_attrnode.o xml_dump.o xml_newnode.o xml_read_callback.o xml_read_tree.o xml_textnode.o xml_path.o xml_free.o)
	$(AR) rcs $@ $^

$(BUILDDIR)decode-ls-lR.o: decode-ls-lR.c
$(BUILDDIR)decode-ls-lR$(M64_)$(EXESUFFIX)$(EXEEXT): $(BUILDDIR)decode-ls-lR.o $(BUILDDIR)buffer.a $(BUILDDIR)str.a $(BUILDDIR)stralloc.a $(BUILDDIR)byte.a
	$(CROSS_COMPILE)$(CC) $(LDFLAGS) $(CFLAGS) $(EXTRA_CPPFLAGS) -o $@ $^ $(LIBS) $(EXTRA_LIBS)

$(BUILDDIR)count-depth.o: count-depth.c
$(BUILDDIR)count-depth$(M64_)$(EXESUFFIX)$(EXEEXT): $(BUILDDIR)count-depth.o $(BUILDDIR)buffer.a $(BUILDDIR)byte.a $(BUILDDIR)fmt.a
	$(CROSS_COMPILE)$(CC) $(LDFLAGS) $(CFLAGS) $(EXTRA_CPPFLAGS) -o $@ $^ $(LIBS) $(EXTRA_LIBS)
ifeq ($(DO_STRIP),1)
	$(STRIP) $@
endif

$(BUILDDIR)fnmatch.o: fnmatch.c


$(BUILDDIR)list-r.o: list-r.c
$(BUILDDIR)list-r$(M64_)$(EXESUFFIX)$(EXEEXT): $(BUILDDIR)list-r.o $(BUILDDIR)fnmatch.o $(BUILDDIR)open.a $(BUILDDIR)array.a $(BUILDDIR)buffer.a  $(BUILDDIR)stralloc.a $(BUILDDIR)byte.a $(BUILDDIR)rdir.a $(BUILDDIR)dir.a $(BUILDDIR)fmt.a $(BUILDDIR)str.a
	$(CROSS_COMPILE)$(CC) $(LDFLAGS) $(CFLAGS) $(EXTRA_CPPFLAGS) -o $@ $^ $(LIBS) $(EXTRA_LIBS)
ifeq ($(DO_STRIP),1)
	$(STRIP) $@
endif

$(BUILDDIR)rdir-test.o: rdir-test.c
$(BUILDDIR)rdir-test$(M64_)$(EXESUFFIX)$(EXEEXT): $(BUILDDIR)rdir-test.o $(BUILDDIR)fnmatch.o $(BUILDDIR)rdir.a $(BUILDDIR)dir.a $(BUILDDIR)array.a $(BUILDDIR)buffer.a  $(BUILDDIR)stralloc.a $(BUILDDIR)byte.a $(BUILDDIR)fmt.a $(BUILDDIR)str.a
	$(CROSS_COMPILE)$(CC) $(LDFLAGS) $(CFLAGS) $(EXTRA_CPPFLAGS) -o $@ $^ $(LIBS) $(EXTRA_LIBS)
ifeq ($(DO_STRIP),1)
	$(STRIP) $@
endif

$(BUILDDIR)reg2cmd.o: reg2cmd.c
$(BUILDDIR)reg2cmd$(M64_)$(EXESUFFIX)$(EXEEXT): $(BUILDDIR)reg2cmd.o $(BUILDDIR)buffer.a $(BUILDDIR)fmt.a $(BUILDDIR)scan.a $(BUILDDIR)str.a $(BUILDDIR)stralloc.a $(BUILDDIR)byte.a
	$(CROSS_COMPILE)$(CC) $(LDFLAGS) $(CFLAGS) $(EXTRA_CPPFLAGS) -o $@ $^ $(LIBS) $(EXTRA_LIBS)
ifeq ($(DO_STRIP),1)
	$(STRIP) $@
endif

$(BUILDDIR)regfilter.o: regfilter.c
$(BUILDDIR)regfilter$(M64_)$(EXESUFFIX)$(EXEEXT): $(BUILDDIR)regfilter.o $(BUILDDIR)buffer.a $(BUILDDIR)fmt.a $(BUILDDIR)scan.a $(BUILDDIR)str.a $(BUILDDIR)stralloc.a $(BUILDDIR)byte.a
	$(CROSS_COMPILE)$(CC) $(LDFLAGS) $(CFLAGS) $(EXTRA_CPPFLAGS) -o $@ $^ $(LIBS) $(EXTRA_LIBS)
ifeq ($(DO_STRIP),1)
	$(STRIP) $@
endif

$(BUILDDIR)torrent-progress.o: torrent-progress.c
$(BUILDDIR)torrent-progress$(M64_)$(EXESUFFIX)$(EXEEXT): $(BUILDDIR)torrent-progress.o $(BUILDDIR)buffer.a $(BUILDDIR)fmt.a $(BUILDDIR)mmap.a $(BUILDDIR)open.a $(BUILDDIR)str.a $(BUILDDIR)byte.a
	$(CROSS_COMPILE)$(CC) $(LDFLAGS) $(CFLAGS) $(EXTRA_CPPFLAGS) -o $@ $^ $(LIBS) $(EXTRA_LIBS)
ifeq ($(DO_STRIP),1)
	$(STRIP) $@
endif

$(BUILDDIR)mediathek-parser$(M64_)$(EXESUFFIX)$(EXEEXT): $(BUILDDIR)mediathek-parser.o $(BUILDDIR)array.a $(BUILDDIR)buffer.a $(BUILDDIR)fmt.a $(BUILDDIR)mmap.a $(BUILDDIR)open.a  $(BUILDDIR)str.a $(BUILDDIR)strlist.a $(BUILDDIR)stralloc.a $(BUILDDIR)byte.a $(BUILDDIR)isleap.o $(BUILDDIR)time_table_spd.o
	$(CROSS_COMPILE)$(CC) $(LDFLAGS) $(CFLAGS) $(EXTRA_CPPFLAGS) -o $@ $^ $(LIBS)   $(EXTRA_LIBS)
ifeq ($(DO_STRIP),1)
	$(STRIP) $@
endif

$(BUILDDIR)mediathek-list$(M64_)$(EXESUFFIX)$(EXEEXT): $(BUILDDIR)mediathek-list.o $(BUILDDIR)array.a $(BUILDDIR)strlist.a $(BUILDDIR)buffer.a $(BUILDDIR)fmt.a $(BUILDDIR)mmap.a $(BUILDDIR)open.a  $(BUILDDIR)scan.a $(BUILDDIR)str.a $(BUILDDIR)stralloc.a $(BUILDDIR)byte.a $(BUILDDIR)isleap.o $(BUILDDIR)time_table_spd.o
	$(CROSS_COMPILE)$(CC) $(LDFLAGS) $(CFLAGS) $(EXTRA_CPPFLAGS) -o $@ $^ $(LIBS)   $(EXTRA_LIBS)
ifeq ($(DO_STRIP),1)
	$(STRIP) $@
endif

$(BUILDDIR)httptest$(M64_)$(EXESUFFIX)$(EXEEXT): LIBS += $(OTHERLIBS)
$(BUILDDIR)httptest$(M64_)$(EXESUFFIX)$(EXEEXT): $(BUILDDIR)httptest.o $(BUILDDIR)http.a $(BUILDDIR)socket.a $(BUILDDIR)io.a $(BUILDDIR)iarray.a $(BUILDDIR)array.a $(BUILDDIR)ndelay.a $(BUILDDIR)uint16.a $(BUILDDIR)buffer.a $(BUILDDIR)fmt.a $(BUILDDIR)mmap.a $(BUILDDIR)open.a  $(BUILDDIR)scan.a $(BUILDDIR)stralloc.a $(BUILDDIR)str.a $(BUILDDIR)byte.a   $(BUILDDIR)taia.a  $(BUILDDIR)uint32.a  
	$(CROSS_COMPILE)$(CC) $(LDFLAGS) $(CFLAGS) $(EXTRA_CPPFLAGS) -o $@ $^ $(LIBS)  $(EXTRA_LIBS)
ifeq ($(DO_STRIP),1)
	$(STRIP) $@
endif

$(BUILDDIR)xmltest$(M64_)$(EXESUFFIX)$(EXEEXT): LIBS += $(LIBXML2_LIBS) -lm
$(BUILDDIR)xmltest$(M64_)$(EXESUFFIX)$(EXEEXT): CFLAGS += $(LIBXML2_CFLAGS)
$(BUILDDIR)xmltest$(M64_)$(EXESUFFIX)$(EXEEXT): $(BUILDDIR)xmltest.o 
	$(CROSS_COMPILE)$(CC) $(LDFLAGS) $(CFLAGS) $(EXTRA_CPPFLAGS) -o $@ $^ $(LIBS)  $(EXTRA_LIBS)
ifeq ($(DO_STRIP),1)
	$(STRIP) $@
endif

$(BUILDDIR)xmltest2$(M64_)$(EXESUFFIX)$(EXEEXT): LIBS += $(LIBXML2_LIBS) -lm
$(BUILDDIR)xmltest2$(M64_)$(EXESUFFIX)$(EXEEXT): CFLAGS += $(LIBXML2_CFLAGS)
$(BUILDDIR)xmltest2$(M64_)$(EXESUFFIX)$(EXEEXT): $(BUILDDIR)xmltest2.o 
	$(CROSS_COMPILE)$(CC) $(LDFLAGS) $(CFLAGS) $(EXTRA_CPPFLAGS) -o $@ $^ $(LIBS)  $(EXTRA_LIBS)
ifeq ($(DO_STRIP),1)
	$(STRIP) $@
endif

$(BUILDDIR)xmltest3$(M64_)$(EXESUFFIX)$(EXEEXT): LIBS += $(LIBXML2_LIBS) -lm
$(BUILDDIR)xmltest3$(M64_)$(EXESUFFIX)$(EXEEXT): CFLAGS += $(LIBXML2_CFLAGS)
$(BUILDDIR)xmltest3$(M64_)$(EXESUFFIX)$(EXEEXT): $(BUILDDIR)xmltest3.o $(BUILDDIR)buffer.a $(BUILDDIR)str.a
	$(CROSS_COMPILE)$(CC) $(LDFLAGS) $(CFLAGS) $(EXTRA_CPPFLAGS) -o $@ $^ $(LIBS)  $(EXTRA_LIBS)
ifeq ($(DO_STRIP),1)
	$(STRIP) $@
endif

$(BUILDDIR)xmltest4$(M64_)$(EXESUFFIX)$(EXEEXT): $(BUILDDIR)xmltest4.o $(BUILDDIR)xml.a $(BUILDDIR)hmap.a $(BUILDDIR)buffer.a $(BUILDDIR)mmap.a $(BUILDDIR)open.a $(BUILDDIR)stralloc.a $(BUILDDIR)byte.a $(BUILDDIR)scan.a $(BUILDDIR)fmt.a $(BUILDDIR)fmt.a $(BUILDDIR)str.a
	$(CROSS_COMPILE)$(CC) $(LDFLAGS) $(CFLAGS) $(EXTRA_CPPFLAGS) -o $@ $^ $(LIBS)  $(EXTRA_LIBS)
ifeq ($(DO_STRIP),1)
	$(STRIP) $@
endif

$(BUILDDIR)plsconv$(M64_)$(EXESUFFIX)$(EXEEXT): LIBS += $(LIBXML2_LIBS) -lm
$(BUILDDIR)plsconv$(M64_)$(EXESUFFIX)$(EXEEXT): CFLAGS += $(LIBXML2_CFLAGS)
$(BUILDDIR)plsconv$(M64_)$(EXESUFFIX)$(EXEEXT): $(BUILDDIR)plsconv.o  $(BUILDDIR)playlist.a $(BUILDDIR)stralloc.a  $(BUILDDIR)buffer.a $(BUILDDIR)mmap.a $(BUILDDIR)open.a $(BUILDDIR)str.a $(BUILDDIR)fmt.a $(BUILDDIR)scan.a  $(BUILDDIR)byte.a 
	$(CROSS_COMPILE)$(CC) $(LDFLAGS) $(CFLAGS) $(EXTRA_CPPFLAGS) -o $@ $^ $(LIBS)  $(EXTRA_LIBS)
ifeq ($(DO_STRIP),1)
	$(STRIP) $@
endif

$(BUILDDIR)pathtool$(M64_)$(EXESUFFIX)$(EXEEXT): $(BUILDDIR)pathtool.o $(BUILDDIR)strlist.a $(BUILDDIR)stralloc.a $(BUILDDIR)buffer.a $(BUILDDIR)mmap.a $(BUILDDIR)open.a $(BUILDDIR)str.a $(BUILDDIR)fmt.a $(BUILDDIR)scan.a $(BUILDDIR)byte.a 
	$(CROSS_COMPILE)$(CC) $(LDFLAGS) $(CFLAGS) $(EXTRA_CPPFLAGS) -o $@ $^ $(LIBS)  $(EXTRA_LIBS)
ifeq ($(DO_STRIP),1)
	$(STRIP) $@
endif

$(BUILDDIR)access.o: access.c
$(BUILDDIR)access$(M64_)$(EXESUFFIX)$(EXEEXT): $(BUILDDIR)access.o $(BUILDDIR)open.a $(BUILDDIR)array.a $(BUILDDIR)buffer.a  $(BUILDDIR)stralloc.a $(BUILDDIR)byte.a $(BUILDDIR)rdir.a $(BUILDDIR)dir.a $(BUILDDIR)fmt.a $(BUILDDIR)str.a
	$(CROSS_COMPILE)$(CC) $(LDFLAGS) $(CFLAGS) $(EXTRA_CPPFLAGS) -o $@ $^ $(LIBS) $(EXTRA_LIBS)
ifeq ($(DO_STRIP),1)
	$(STRIP) $@
endif

$(BUILDDIR)opensearch-dump$(M64_)$(EXESUFFIX)$(EXEEXT): INCLUDES += $(LIBXML2_CFLAGS) $(ICONV_CFLAGS)
$(BUILDDIR)opensearch-dump$(M64_)$(EXESUFFIX)$(EXEEXT): LIBS += $(LIBXML2_LIBS) $(ICONV_LIBS) $(OTHERLIBS)
$(BUILDDIR)opensearch-dump$(M64_)$(EXESUFFIX)$(EXEEXT): $(BUILDDIR)opensearch-dump.o $(BUILDDIR)buffer.a $(BUILDDIR)str.a $(BUILDDIR)stralloc.a $(BUILDDIR)byte.a
	$(CROSS_COMPILE)$(CC) $(LDFLAGS) $(CFLAGS) $(EXTRA_CPPFLAGS) -o $@ $^ $(LIBS)   $(EXTRA_LIBS)
ifeq ($(DO_STRIP),1)
	$(STRIP) $@
endif

$(BUILDDIR)eagle-init-brd$(M64_)$(EXESUFFIX)$(EXEEXT): CFLAGS += $(LIBXML2_CFLAGS) $(ICONV_CFLAGS)
$(BUILDDIR)eagle-init-brd$(M64_)$(EXESUFFIX)$(EXEEXT): LIBS += $(LIBXML2_LIBS) $(ICONV_LIBS) $(OTHERLIBS) -lm
$(BUILDDIR)eagle-init-brd$(M64_)$(EXESUFFIX)$(EXEEXT): $(BUILDDIR)eagle-init-brd.o  $(BUILDDIR)hmap.a $(BUILDDIR)buffer.a $(BUILDDIR)str.a $(BUILDDIR)stralloc.a $(BUILDDIR)byte.a $(BUILDDIR)scan.a
	$(CROSS_COMPILE)$(CC) $(LDFLAGS) $(CFLAGS) $(EXTRA_CPPFLAGS) -o $@ $^ $(LIBS)   $(EXTRA_LIBS)
ifeq ($(DO_STRIP),1)
	#$(STRIP) $@
endif

$(BUILDDIR)eagle-gen-cmds$(M64_)$(EXESUFFIX)$(EXEEXT): CFLAGS += $(LIBXML2_CFLAGS) $(ICONV_CFLAGS)
$(BUILDDIR)eagle-gen-cmds$(M64_)$(EXESUFFIX)$(EXEEXT): LIBS += $(LIBXML2_LIBS) $(ICONV_LIBS) $(OTHERLIBS) -lm
$(BUILDDIR)eagle-gen-cmds$(M64_)$(EXESUFFIX)$(EXEEXT): $(BUILDDIR)eagle-gen-cmds.o $(BUILDDIR)array.a $(BUILDDIR)cbmap.a $(BUILDDIR)strlist.a $(BUILDDIR)map.a $(BUILDDIR)hmap.a $(BUILDDIR)stralloc.a $(BUILDDIR)buffer.a $(BUILDDIR)mmap.a $(BUILDDIR)open.a  $(BUILDDIR)fmt.a $(BUILDDIR)str.a $(BUILDDIR)byte.a $(BUILDDIR)scan.a
	$(CROSS_COMPILE)$(CC) $(LDFLAGS) $(CFLAGS) $(EXTRA_CPPFLAGS) -o $@ $^ $(LIBS)   $(EXTRA_LIBS)
ifeq ($(DO_STRIP),1)
	#$(STRIP) $@
endif
$(BUILDDIR)impgen$(M64_)$(EXESUFFIX)$(EXEEXT): $(BUILDDIR)impgen.o $(BUILDDIR)pe.a $(BUILDDIR)mmap.a $(BUILDDIR)buffer.a $(BUILDDIR)byte.a $(BUILDDIR)fmt.a $(BUILDDIR)str.a $(BUILDDIR)open.a $(BUILDDIR)uint32.a 
	$(CROSS_COMPILE)$(CC) $(LDFLAGS) $(CFLAGS) $(EXTRA_CPPFLAGS) -o $@ $^ $(LIBS)   $(EXTRA_LIBS)
ifeq ($(DO_STRIP),1)
	#$(STRIP) $@
endif

$(BUILDDIR)ntldd$(M64_)$(EXESUFFIX)$(EXEEXT): LIBS += $(EXTRA_LIBS)
$(BUILDDIR)ntldd$(M64_)$(EXESUFFIX)$(EXEEXT): CPPFLAGS += -DNTLDD_VERSION_MAJOR=0 -DNTLDD_VERSION_MINOR=2
$(BUILDDIR)ntldd$(M64_)$(EXESUFFIX)$(EXEEXT): $(BUILDDIR)ntldd.o $(BUILDDIR)libntldd.o $(BUILDDIR)pe.a $(BUILDDIR)mmap.a $(BUILDDIR)buffer.a $(BUILDDIR)byte.a $(BUILDDIR)fmt.a $(BUILDDIR)str.a $(BUILDDIR)open.a $(BUILDDIR)uint32.a 
	$(CROSS_COMPILE)$(CC) $(LDFLAGS) $(CFLAGS) $(EXTRA_CPPFLAGS) -o $@ $^ $(LIBS)   $(EXTRA_LIBS)
ifeq ($(DO_STRIP),1)
	#$(STRIP) $@
endif

$(BUILDDIR)hexedit$(M64_)$(EXESUFFIX)$(EXEEXT): LIBS += $(EXTRA_LIBS)
$(BUILDDIR)hexedit$(M64_)$(EXESUFFIX)$(EXEEXT): CPPFLAGS += -Dhexedit_VERSION_MAJOR=0 -Dhexedit_VERSION_MINOR=2
$(BUILDDIR)hexedit$(M64_)$(EXESUFFIX)$(EXEEXT): $(BUILDDIR)hexedit.o $(BUILDDIR)mmap.a $(BUILDDIR)buffer.a $(BUILDDIR)byte.a $(BUILDDIR)fmt.a $(BUILDDIR)str.a $(BUILDDIR)open.a $(BUILDDIR)uint32.a 
	$(CROSS_COMPILE)$(CC) $(LDFLAGS) $(CFLAGS) $(EXTRA_CPPFLAGS) -o $@ $^ $(LIBS)   $(EXTRA_LIBS)
ifeq ($(DO_STRIP),1)
	#$(STRIP) $@
endif

#
#$(BUILDDIR)xc8-wrapper/:
#	mkdir -p $@
#
$(BUILDDIR)compiler-wrapper$(M64_)$(EXESUFFIX)$(EXEEXT): $(BUILDDIR)compiler-wrapper.o $(BUILDDIR)dir.a $(BUILDDIR)strlist.a $(BUILDDIR)stralloc.a $(BUILDDIR)buffer.a $(BUILDDIR)fmt.a $(BUILDDIR)str.a  $(BUILDDIR)byte.a 
	$(CROSS_COMPILE)$(CC) $(LDFLAGS) $(CFLAGS) $(EXTRA_CPPFLAGS) -o $@ $^ $(LIBS)   $(EXTRA_LIBS)
ifeq ($(DO_STRIP),1)
	$(STRIP) $@
endif

$(BUILDDIR)xc8-wrapper$(M64_)$(EXESUFFIX)$(EXEEXT): DEFS += XC8_WRAPPER=1
$(BUILDDIR)xc8-wrapper$(M64_)$(EXESUFFIX)$(EXEEXT): $(BUILDDIR)compiler-wrapper.o $(BUILDDIR)strlist.a $(BUILDDIR)stralloc.a $(BUILDDIR)buffer.a $(BUILDDIR)str.a $(BUILDDIR)byte.a $(BUILDDIR)fmt.a $(BUILDDIR)dir.a 
	$(CROSS_COMPILE)$(CC) $(LDFLAGS) $(CFLAGS) $(EXTRA_CPPFLAGS) -o $@ $^ $(LIBS)   $(EXTRA_LIBS)
ifeq ($(DO_STRIP),1)
	$(STRIP) $@
endif

$(BUILDDIR)picc-wrapper$(M64_)$(EXESUFFIX)$(EXEEXT): DEFS += PICC_WRAPPER=1
$(BUILDDIR)picc-wrapper$(M64_)$(EXESUFFIX)$(EXEEXT): $(BUILDDIR)compiler-wrapper.o $(BUILDDIR)strlist.a $(BUILDDIR)stralloc.a $(BUILDDIR)buffer.a $(BUILDDIR)str.a $(BUILDDIR)byte.a $(BUILDDIR)fmt.a $(BUILDDIR)dir.a 
	$(CROSS_COMPILE)$(CC) $(LDFLAGS) $(CFLAGS) $(EXTRA_CPPFLAGS) -o $@ $^ $(LIBS)   $(EXTRA_LIBS)
ifeq ($(DO_STRIP),1)
	$(STRIP) $@
endif

$(BUILDDIR)picc18-wrapper$(M64_)$(EXESUFFIX)$(EXEEXT): DEFS += PICC18_WRAPPER=1
$(BUILDDIR)picc18-wrapper$(M64_)$(EXESUFFIX)$(EXEEXT): $(BUILDDIR)compiler-wrapper.o $(BUILDDIR)strlist.a $(BUILDDIR)stralloc.a $(BUILDDIR)buffer.a $(BUILDDIR)str.a $(BUILDDIR)byte.a $(BUILDDIR)fmt.a $(BUILDDIR)dir.a 
	$(CROSS_COMPILE)$(CC) $(LDFLAGS) $(CFLAGS) $(EXTRA_CPPFLAGS) -o $@ $^ $(LIBS)   $(EXTRA_LIBS)
ifeq ($(DO_STRIP),1)
	$(STRIP) $@
endif

$(BUILDDIR)sdcc-wrapper$(M64_)$(EXESUFFIX)$(EXEEXT): DEFS += SDCC_WRAPPER=1
$(BUILDDIR)sdcc-wrapper$(M64_)$(EXESUFFIX)$(EXEEXT): $(BUILDDIR)compiler-wrapper.o $(BUILDDIR)strlist.a $(BUILDDIR)stralloc.a $(BUILDDIR)buffer.a $(BUILDDIR)str.a $(BUILDDIR)byte.a $(BUILDDIR)fmt.a $(BUILDDIR)dir.a 
	$(CROSS_COMPILE)$(CC) $(LDFLAGS) $(CFLAGS) $(EXTRA_CPPFLAGS) -o $@ $^ $(LIBS)   $(EXTRA_LIBS)
ifeq ($(DO_STRIP),1)
	$(STRIP) $@
endif
ifeq ($(DO_CXX),1)
$(BUILDDIR)piccfghex.o: piccfghex.cpp
$(BUILDDIR)piccfghex$(M64_)$(EXESUFFIX)$(EXEEXT): $(BUILDDIR)piccfghex.o $(BUILDDIR)intelhex.o
	$(CROSS_COMPILE)$(CXX) $(LDFLAGS) $(CXXFLAGS) $(EXTRA_CPPFLAGS) -o $@ $^ $(LIBS) $(EXTRA_LIBS)
ifeq ($(DO_STRIP),1)
	$(STRIP) $@
endif

$(BUILDDIR)mediathek-parser-cpp.o: mediathek-parser.cpp
	$(CROSS_COMPILE)$(CXX) $(DEFS:%=-D%) $(CPPFLAGS) $(CXXFLAGS) $(INCLUDES) -c $(EXTRA_CPPFLAGS) -o $@ $<

$(BUILDDIR)mediathek-parser-cpp$(M64_)$(EXESUFFIX)$(EXEEXT): $(BUILDDIR)mediathek-parser-cpp.o
	$(CROSS_COMPILE)$(CXX) $(LDFLAGS) $(CXXFLAGS) $(EXTRA_CPPFLAGS) -o $@ $^ $(LIBS) $(EXTRA_LIBS)
ifeq ($(DO_STRIP),1)
	$(STRIP) $@
endif

	$(CROSS_COMPILE)$(CC) $(LDFLAGS) $(CFLAGS) $(EXTRA_CPPFLAGS) -o $@ $^ $(LIBS) $(EXTRA_LIBS)
$(BUILDDIR)kbd-adjacency.o: kbd-adjacency.cpp
	$(CROSS_COMPILE)$(CXX) $(DEFS:%=-D%) $(CPPFLAGS) $(CXXFLAGS) $(INCLUDES) -c $(EXTRA_CPPFLAGS) -o  $(BUILDDIR)$(patsubst %.cpp,%.o,$(notdir $<))  $<

$(BUILDDIR)kbd-adjacency$(M64_)$(EXESUFFIX)$(EXEEXT): $(BUILDDIR)kbd-adjacency.o $(LIB_OBJ)
	$(CROSS_COMPILE)$(CXX) $(LDFLAGS) $(CXXFLAGS) $(EXTRA_CPPFLAGS) -o $@ $^ $(LIBS) $(EXTRA_LIBS)
ifeq ($(DO_STRIP),1)
	$(STRIP) $@
endif
endif
ifeq ($(BUILDDIR),)
.c.o:
	$(CROSS_COMPILE)$(CC) $(DEFS:%=-D%) $(CPPFLAGS) $(CFLAGS) $(EXTRA_CPPFLAGS) -c $<

%.o: %.c
	$(CROSS_COMPILE)$(CC) $(DEFS:%=-D%) $(CPPFLAGS) $(CFLAGS) $(EXTRA_CPPFLAGS) -c $<

.cpp.o:
	$(CROSS_COMPILE)$(CXX) $(CXXOPTS) $(DEFS:%=-D%) $(CPPFLAGS) $(CXXFLAGS) $(EXTRA_CPPFLAGS) -c $<

%.o: %.cpp
	$(CROSS_COMPILE)$(CXX) $(CXXOPTS) $(DEFS:%=-D%) $(CPPFLAGS) $(CXXFLAGS) $(EXTRA_CPPFLAGS) -c $<
else
.c.o:
	$(CROSS_COMPILE)$(CC) $(DEFS:%=-D%) $(CPPFLAGS) $(CFLAGS) -c $(EXTRA_CPPFLAGS) -o $(BUILDDIR)$@ $<

$(BUILDDIR)%.o: lib/%.c
	$(CROSS_COMPILE)$(CC) $(DEFS:%=-D%) $(CPPFLAGS) $(CFLAGS) $(INCLUDES) -c $(EXTRA_CPPFLAGS) -o $(BUILDDIR)$(patsubst lib/%.c,%.o,$<) $<

$(BUILDDIR)%.o: %.c
	$(CROSS_COMPILE)$(CC) $(DEFS:%=-D%) $(CPPFLAGS) $(CFLAGS) $(INCLUDES) -c $(EXTRA_CPPFLAGS) -o $(BUILDDIR)$(patsubst %.c,%.o,$<) $<

.cpp.o:
	$(CROSS_COMPILE)$(CXX) $(CXXOPTS) $(DEFS:%=-D%) $(CPPFLAGS) $(CXXFLAGS) $(EXTRA_CPPFLAGS) -c $<

$(BUILDDIR)%.o: %.cpp
	$(CROSS_COMPILE)$(CXX) $(CXXOPTS) $(DEFS:%=-D%) $(CPPFLAGS) $(CXXFLAGS) $(INCLUDES) -c $(EXTRA_CPPFLAGS) -o $(BUILDDIR)$(patsubst %.cpp,%.o,$<) $<
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

-include Makefile.deps

#$(PROGRAM_OBJECTS): CFLAGS += -Ilib
#$(PROGRAM_OBJECTS): CPPFLAGS += -Ilib
$(PROGRAMS): CPPFLAGS += -I.
#$(PROGRAMS): CPPFLAGS += -Ilib

$(info PROGRAM_OBJECTS=$(PROGRAM_OBJECTS))

