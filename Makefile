//DEBUG = 1
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
#$(info PKG_CONFIG_PATH=$(PKG_CONFIG_PATH))

define NL =

endef

#file-exists = $(shell echo "Checking for $(1) ..." 1>&2; test -e "$(1)" && echo 1)

#$(info ROOTNAME=$(ROOTNAME))
#$(info SYSNAME=$(SYSNAME))

OS ?= $(shell uname -o | tr "[[:upper:]]" "[[:lower:]]")

ifeq ($(CC),)
  CC := gcc
endif
ifeq ($(CC),cc)
  CC := gcc
endif
#
#CXX = g++
include Makefile.functions


ifeq ($(SHARED),1)
add-library = $(patsubst %,$(BUILDDIR)lib%.so,$(call clean-lib,$(1)))
else
add-library = $(patsubst %,$(BUILDDIR)%.a,$(call clean-lib,$(1)))
endif


#check-header = $(info $(call cmd-check-header,$(1)))
DEFINES_FILE := Makefile.defines

define NL =
$(EMPTY)
$(EMPTY)
endef

BUILD := $(shell $(CROSS_COMPILE)$(CC) -dumpmachine)
ifneq ($(CC),$(subst m32,,$(CC)))
BUILD := $(subst x86_64,i386,$(BUILD))
endif
#ifneq ($(BUILD),$(subst -pc-,-,$(BUILD)))
#BUILD := $(subst -pc-,-,$(BUILD))
#endif

$(info BUILD: $(BUILD))

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

#$(info PREFIX: $(PREFIX))
#$(info DIET: $(DIET))
#$(info USE_DIET: $(USE_DIET))
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
ifeq ($(word 3,$(subst -, ,$(BUILD))),msys)
MSYS := 1
ifeq ($(shell grep -q msys-1.0.dll /bin/sh.exe && echo 1),1)
BUILD := $(subst msys,msys1,$(BUILD))
endif
else
MSYS := 0
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
HOST := $( shell set -x; $(CROSS_COMPILE)$(CC) -dumpmachine  | sed 's|[-.0-9]*\\\$$|| ;; s|\\r\$$||' )
else
HOST := $( shell set -x; $(CROSS_COMPILE)$(CC) -dumpmachine  | sed 's|[-.0-9]*\\\$$|| ;; s|\\r\$$||' )
endif
endif
endif
ifneq ($(CC),$(subst m32,,$(CC)))
HOST := $(subst x86_64,i386,$(HOST))
endif
ifneq ($(HOST),$(subst linux,,$(HOST)))
#ifneq ($(HOST),$(subst -pc-,-,$(HOST)))
#HOST := $(subst -pc-,-,$(HOST))
#endif
endif
ifeq ($(USE_DIET),1)
HOST := $(subst -diet-,-,$(HOST))
HOST := $(subst gnu,dietlibc,$(HOST))
endif
#$(info USE_DIET: $(USE_DIET))
#$(info MINGW: $(MINGW))
#$(info HOST: $(HOST))
ifeq ($(PREFIX),)
PREFIX := $(shell $(CROSS_COMPILE)$(CC) -print-search-dirs |sed -n 's,.*:\s\+=\?,,; s,/bin.*,,p ; s,/lib.*,,p' |head -n1 )
endif
#$(info PREFIX: $(PREFIX))

#ifeq ($(SYSROOT),)
#ifneq ($(CROSS_COMPILE),$(subst /,-,$(CROSS_COMPILE)))
#SYSROOT := $(subst /bin/,,$(CROSS_COMPILE))
#else
#SYSROOT = $(shell $(CROSS_COMPILE)$(CC) -print-search-dirs | sed -n "/^lib/ { s|.*:\s\+|| ; s|^=|| ; /;/ { s|.*;|;| }; /;/! { s|.*:|| } ; s|^;|| ; s|/lib.*|| ; s|/mingw$$|| ; s|/usr$$|| ; s|/$$||; p }"  )
#
#endif
#$(info SYSROOT: $(SYSROOT))
#endif


prefix ?= $(PREFIX)
bindir = ${prefix}/bin
mandir = ${prefix}/share/man
man3dir = $(mandir)/man3
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
#ifeq ($(HOST),$(BUILD))
#CROSS_COMPILE :=
#endif

PKG_CONFIG := pkg-config
ifneq ($(CROSS_COMPILE),)
ifeq ($(call cmd-exists,$(CROSS_COMPILE)$(PKG_CONFIG)),1)
PKG_CONFIG := $(CROSS_COMPILE)$(PKG_CONFIG)
else
  P := $(shell set -x; ls -d /usr/$(CROSS_COMPILE:%-=%)/sys*root/*/lib/pkgconfig)
  ifeq ($(call file-exists,$(P)),1)
  PKG_CONFIG_PATH := $(P)
  ifeq ($(SYSROOT),)
  SYSROOT := $(dir $(subst /lib/pkgconfig,,$(P)))
#$(info SYSROOT=$(SYSROOT))
  endif
  else
  P := $(shell set -x; ls -d /usr/$(CROSS_COMPILE:%-=%)/lib/pkgconfig)
  ifeq ($(call file-exists,$(P)),1)
  PKG_CONFIG_PATH := $(P)
  endif
  endif
endif
endif

ifneq ($(MSYS),1)
C11_COMPILER_DEFS := $(shell $(call get-compiler-defs,-std=c11))
#$(info C11_COMPILER_DEFS: $(C11_COMPILER_DEFS))
ifneq ($(C11_COMPILER_DEFS),)
CC += -std=c11
endif
#NO_AT := 1
endif


AR := ar
ifeq ($(call cmd-exists,$(CROSS_COMPILE)$(AR)),1)
AR := $(CROSS_COMPILE)$(AR)
endif

RANLIB := ranlib
ifeq ($(call cmd-exists,$(CROSS_COMPILE)$(RANLIB)),1)
RANLIB := $(CROSS_COMPILE)$(RANLIB)
endif

STRIP := strip
ifeq ($(call cmd-exists,$(CROSS_COMPILE)$(STRIP)),1)
STRIP := $(CROSS_COMPILE)$(STRIP)
endif

#$(info PKG_CONFIG: $(PKG_CONFIG))
#$(info PKG_CONFIG_PATH: $(PKG_CONFIG_PATH))

#PKG_CONFIG_CMD := $(if $(SYSROOT)$(PKG_CONFIG_PATH),env $(if $(SYSROOT),PKG_CONFIG_SYSROOT_DIR=$(SYSROOT) ,)$(if $(PKG_CONFIG_PATH),PKG_CONFIG_PATH=$(PKG_CONFIG_PATH) ,),)$(PKG_CONFIG)
PKG_CONFIG_CMD := $(if $(SYSROOT)$(PKG_CONFIG_PATH),env  ,)$(if $(PKG_CONFIG_PATH),PKG_CONFIG_PATH=$(PKG_CONFIG_PATH) ,)$(PKG_CONFIG)
#$(info PKG_CONFIG_CMD: $(PKG_CONFIG_CMD))
ifneq ($(TRIPLET),)
ARCH := $(word 1,$(TRIPLET))
OS := $(shell echo $(word 3,$(TRIPLET)) |sed "s|[0-9].*||")
KERN := $(word 2,$(TRIPLET))
SYS := $(word 3,$(TRIPLET))
W := 4
ifeq ($(KERN),pc)
KERN := $(SYS)
endif
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


USE_WINSOCK := $(call check-function-exists,WSAStartup,-lws2_32,winsock2.h)
#$(info USE_WINSOCK=$(USE_WINSOCK))
ifeq ($(USE_WINSOCK),1)
WINSOCK_LIB = -lws2_32 -lmswsock
IPHLPAPI_LIB = -liphlpapi
endif
ifeq ($(MSYS),1)
#WINSOCK_LIB = -lws2_32
KERNEL32_LIB = -lkernel32
ADVAPI32_LIB = -ladvapi32
IPHLPAPI_LIB = -liphlpapi
endif
SHLWAPI_LIB = -lshlwapi

#$(call def-function-exists,ZLIB,deflate,-lz)
$(foreach inc,sys/types.h inttypes.h vcruntime.h stdint.h stddef.h errno.h,$(call def-include-exists,$(inc)))

#$(info HAVE_SYS_TYPES_H=$(HAVE_SYS_TYPES_H))
#$(info HAVE_INTTYPES_H=$(HAVE_INTTYPES_H))
#$(info HAVE_VCRUNTIME_H=$(HAVE_VCRUNTIME_H))
#$(info HAVE_STDINT_H=$(HAVE_STDINT_H))
#$(info HAVE_STDDEF_H=$(HAVE_STDDEF_H))
#$(info HAVE_ERRNO_H=$(HAVE_ERRNO_H))

#$(info HAVE_LSEEK64=$(HAVE_LSEEK64) HAVE_LSEEK=$(HAVE_LSEEK64)  HAVE_LLSEEK=$(HAVE_LLSEEK64))
#$(info llseek: $(call check-function-exists,llseek))
ifeq ($SUBLIME_FILENAME),None)
PATH = /c/git-sdk-64/usr/bin
MAKE = c:/git-sdk-64/usr/bin/make
endif



ifneq ($(OS),linux)
ifeq ($(ARCH),x86_64)
M64 = 64
endif
else
  READDIR := 1
endif
ifeq ($(OS),msys)
EXEEXT = .exe
STATIC_LIBGCC := 1
endif
ifneq ($(HOST),$(subst msys,,$(HOST)))
EXEEXT = .exe
STATIC_LIBGCC := 1
endif
ifeq ($(KERN),cygwin)
EXEEXT = .exe
endif
ifeq ($(OS),mingw)
EXEEXT = .exe
STATIC_LIBGCC := 1
ifeq ($(ARCH),x86_64)
#M64_ := -x64
endif
endif
ifeq ($(OS),cygwin)
EXEEXT = .exe
endif
ifeq ($(OS),mingw32)
EXEEXT = .exe
endif
BOOST_LIBS = boost_random

#$(info SYS: $(SYS))
#$(info KERN: $(KERN))
#$(info ARCH: $(ARCH))
#$(info OS: $(OS))
#$(info Host: $(HOST))
#$(info Build: $(BUILD))

HOST1 := $(word 1,$(subst -, ,$(HOST)))
HOST2 := $(word 2,$(subst -, ,$(HOST)))
HOST3 := $(subst $(HOST1)-$(HOST2)-,,$(HOST))



#$(info HOST1=$(HOST1), HOST2=$(HOST2), HOST3=$(HOST3))

#ifneq ($(SYSNAME),)
#BUILDDIR = $(subst w64,$(SYSNAME),$(BUILDDIR))
#endif
ifeq ($(HOST2),pc)
HOST2 := $(SYSNAME)
endif
ifeq ($(HOST2),w64)
HOST2 := $(SYSNAME)
endif
ifeq ($(HOST2),)
ifneq ($(KERN),)
HOST2 := $(KERN)
else
HOST2 := pc
endif
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
  BUILDDIR = build/$(HOST)/$(BUILD_TYPE)/
else
#	ifeq ($(CROSS_COMPILE),)
	BUILDDIR = build/$(HOST)/$(BUILD_TYPE)/
#	else
#	BUILDDIR = build/$(patsubst %-,%,$(CROSS_COMPILE))/$(BUILD_TYPE)/
#	endif
  endif
endif

vpath lib lib/array lib/binfmt lib/buffer lib/byte lib/case lib/cb lib/cbmap lib/charbuf lib/dir lib/dns lib/elf lib/env lib/errmsg lib/expand lib/fmt lib/gpio lib/hmap lib/http lib/iarray lib/io lib/json lib/list lib/map lib/mmap lib/ndelaylib/open lib/path lib/pe lib/playlist lib/range lib/rdir lib/scan lib/sig lib/slist lib/socket lib/str lib/stralloc lib/strarray lib/strlist lib/tai lib/taia lib/textbuf lib/uint16 lib/uint32 lib/uint64 lib/var lib/vartab lib/wait lib/xml $(BUILDDIR) tests
VPATH = lib:lib/array:lib/binfmt:lib/buffer:lib/byte:lib/case:lib/cb:lib/cbmap:lib/charbuf:lib/dir:lib/dns:lib/elf:lib/env:lib/errmsg:lib/expand:lib/fmt:lib/gpio:lib/hmap:lib/http:lib/iarray:lib/io:lib/json:lib/list:lib/map:lib/mmap:lib/ndelay:lib/open:lib/path:lib/pe:lib/playlist:/ib/rdir:lib/scan:lib/sig:lib/slist:lib/socket:lib/str:lib/stralloc:lib/strarray:lib/strlist:lib/tai:lib/taia:lib/textbuf:lib/uint16:lib/uint32:lib/uint64:lib/var:lib/vartab:lib/wait:lib/xml:$(BUILDDIR):tests

ifeq ($(CXXOPTS),)
##$(info OS: "$(OS)")
ifneq ($(OS),msys)
endif
endif

CPPFLAGS := -I.

DEFINES += INLINE=inline
#DEFINES += PATH_MAX=4096
ifeq ($(READDIR),)
ifeq ($(SYS),mingw32)
#DEFINES += USE_READDIR=0
READDIR :=0
else
ifeq ($(SYS),msys)
  READDIR := 1
#DEFINES += USE_READDIR=0
endif
endif
endif

#$(info READDIR: $(READDIR))

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
OTHERLIBS := -lws2_32 -lmswsock
endif
ifeq ($(WIN32),1)
WIDECHAR := 1
endif
ifeq ($(SYS),msys)
WIDECHAR := 0
endif
ifeq ($(WIDECHAR),)
WIDECHAR := 0
endif

ifneq ($(WIDECHAR),)
DEFINES += USE_WIDECHAR=$(WIDECHAR)
endif
ifneq ($(READDIR),)
DEFINES += USE_READDIR=$(READDIR)
endif
ifeq ($(LARGEFILE),1)
DEFINES += _FILE_OFFSET_BITS=64
DEFINES += _LARGEFILE_SOURCE=1
endif
DEFINES += _GNU_SOURCE=1

WARNINGS += no-strict-aliasing

STRIP ?= strip

ifneq ($(NOPIPE),1)
CFLAGS += -pipe
endif

CFLAGS_Prof = -pg -Os
CFLAGS_Debug = -g -ggdb -O0
CFLAGS_MinSizeRel = -g -fomit-frame-pointer -Os
CFLAGS_RelWithDebInfo = -g -ggdb -Os
CFLAGS_Release = -g -fomit-frame-pointer -Os

CXXFLAGS += -pipe
CXXFLAGS += -std=c++11

CXXFLAGS_Prof = -pg -Os
CXXFLAGS_Debug = -g -ggdb -O0
CXXFLAGS_MinSizeRel = -g -fomit-frame-pointer -Os
CXXFLAGS_RelWithDebInfo = -g -ggdb -Os
CXXFLAGS_Release = -g -fomit-frame-pointer -Os
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

#$(info BUILDDIR: \
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
CFLAGS += $(CFLAGS_Debug)
endif
ifeq ($(BUILD_TYPE),RelWithDebInfo)
DEBUG := 1
RELEASE := 1
MINSIZE := 0
CFLAGS += $(CFLAGS_RelWithDebInfo)
endif
ifeq ($(BUILD_TYPE),MinSizeRel)
DEBUG := 0
RELEASE := 1
MINSIZE := 1
CFLAGS += $(CFLAGS_MinSizeRel)
endif
ifeq ($(BUILD_TYPE),Release)
DEBUG := 0
RELEASE := 1
MINSIZE := 0
CFLAGS += $(CFLAGS_Release)
endif

ifeq ($(DEBUG),1)
DEFINES += DEBUG=1
CFLAGS += -g3 -ggdb -O0
else
DEFINES += NDEBUG=1
CFLAGS += -g -Os -fomit-frame-pointer
endif

INCLUDES += -I.

ifneq ($(CFLAGS),)
FLAGS += $(CFLAGS)
endif
#ifneq ($(CFLAGS_$(BUILD_TYPE)),)
#FLAGS += $(CFLAGS_$(BUILD_TYPE))
#endif
CXXFLAGS += $(CXXFLAGS_$(BUILD_TYPE))
ifeq ($(USE_DIET),1)
STATIC := 1
endif
ifneq ($(STATIC),1)
STATIC := 0
endif

#$(info DIET: $(DIET))
#$(info STATIC: $(STATIC))
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
DEFINES += open=_open read=_read write=_write close=_close
#LDFLAGS += -static-lib{asan,gfortran,lsan,tsan,ubsan}
WIN32 := 1
endif
ifeq ($(CYGWIN),1)
WIN32 := 1
NO_AT := 0
endif
ifeq ($(STATIC),1)
#LDFLAGS += -static
PKG_CONFIG += --static
ifeq ($(MINGW)$(STATIC),10)
LDFLAGS += -static-libgcc -static-libstdc++
else
ifeq ($(STATIC),1)
LDFLAGS += -static
endif
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


#CFLAGS = $(patsubst %,-W%,$(WARNINGS))

LIB_SRC = $(wildcard *_*.c umult*.c)
LIB_OBJ = $(patsubst %.o,$(BUILDDIR)%.o,$(patsubst %.c,%.o,$(LIB_SRC)))

pkg-conf = $(foreach L,$(2),$(shell $(PKG_CONFIG_CMD) $(1) $(L) |sed "s,\([[:upper:]]:/\),\
-I\1,g ; /^-I$$/d"))

FLAGS_FILE := $(patsubst %/,%,$(dir $(patsubst %/,%,$(BUILDDIR))))/$(notdir $(patsubst %/,%,$(BUILDDIR))).flags

ifneq (0,1)
#$(call file-exists,$(FLAGS_FILE)),1)

#$(info ICONV_CFLAGS: $(ICONV_CFLAGS))
#$(info ICONV_LIBS: $(ICONV_LIBS))

#ifeq ($(USE_DIET),1)
#STATIC := 1
#endif
#ifeq ($(STATIC),1)
#endif
#
define CHECK_SCOPE_ID =
#include <sys/types.h>\n
#include <sys/socket.h>\n
#include <netinet/in.h>\n
\n
int main() {\n
  struct sockaddr_in6 sa;\n
  sa.sin6_family = PF_INET6;\n
  sa.sin6_scope_id = 23;\n
  (void)sa;\n
  return 0;\n
}\n
endef

HAVE_SCOPE := $(call check-try-compile,$(CHECK_SCOPE_ID))
ifeq ($(HAVE_SCOPE),1)
DEFINES += LIBC_HAS_SCOPE_ID=1
endif
#$(info HAVE_SCOPE=$(HAVE_SCOPE))

ifeq ($(call check-include-exists,errno.h),1)
DEFINES += HAVE_ERRNO_H=1
endif

HAVE_N2I := $(call check-include-exists,net/if.h)
ifeq ($(HAVE_N2I),1)
DEFINES += HAVE_N2I=1
endif
#$(info HAVE_N2I=$(HAVE_N2I))

LIBC_HAS_IP6 := $(call check-function-exists,inet_pton)
ifeq ($(LIBC_HAS_IP6),1)
DEFINES += LIBC_HAS_IP6=1
endif
#$(info LIBC_HAS_IP6=$(LIBC_HAS_IP6))

HAVE_PIPE2 := $(call check-function-exists,pipe2)
ifeq ($(HAVE_PIPE2),1)
DEFINES += HAVE_PIPE2=1
endif
#$(info HAVE_PIPE2=$(HAVE_PIPE2))

HAVE_WORDEXP := $(call check-function-exists,wordexp)
ifeq ($(HAVE_WORDEXP),1)
DEFINES += HAVE_WORDEXP=1
endif
#$(info HAVE_WORDEXP=$(HAVE_WORDEXP))

HAVE_FNMATCH := $(call check-function-exists,fnmatch)
ifeq ($(HAVE_FNMATCH),1)
DEFINES += HAVE_FNMATCH=1
endif
#$(info HAVE_FNMATCH=$(HAVE_FNMATCH))

HAVE_LSTAT := $(call check-function-exists,lstat)
ifeq ($(HAVE_LSTAT),1)
DEFINES += HAVE_LSTAT=1
endif
#$(info HAVE_LSTAT=$(HAVE_LSTAT))

HAVE_GETDELIM := $(call check-function-exists,getdelim)
ifeq ($(HAVE_GETDELIM),1)
DEFINES += HAVE_GETDELIM=1
endif
#$(info HAVE_GETDELIM=$(HAVE_GETDELIM))

HAVE_ROUND := $(call check-function-exists,round)
ifeq ($(HAVE_ROUND),1)
DEFINES += HAVE_ROUND=1
endif
#$(info HAVE_ROUND=$(HAVE_ROUND))

HAVE_ALLOCA := $(call check-function-exists,alloca,,alloca.h)
ifeq ($(HAVE_ALLOCA),1)
DEFINES += HAVE_ALLOCA=1
endif
#$(info HAVE_ALLOCA=$(HAVE_ALLOCA))

HAVE_SENDFILE := $(call check-function-exists,sendfile)
ifeq ($(HAVE_SENDFILE),1)
DEFINES += HAVE_SENDFILE=1
endif
#$(info HAVE_SENDFILE=$(HAVE_SENDFILE))

HAVE_CYGWIN_CONV_PATH := $(call check-function-exists,cygwin_conv_path)
ifeq ($(HAVE_CYGWIN_CONV_PATH),1)
DEFINES += HAVE_CYGWIN_CONV_PATH=1
endif
#$(info HAVE_CYGWIN_CONV_PATH=$(HAVE_CYGWIN_CONV_PATH))

HAVE_POPEN := $(call check-function-exists,popen)
ifeq ($(HAVE_POPEN),1)
DEFINES += HAVE_POPEN=1
endif
#$(info HAVE_POPEN=$(HAVE_POPEN))

HAVE_GETOPT := $(call check-function-exists,getopt)
ifeq ($(HAVE_GETOPT),1)
DEFINES += HAVE_GETOPT=1
endif
#$(info HAVE_GETOPT=$(HAVE_GETOPT))

HAVE_GETOPT_LONG := $(call check-function-exists,getopt_long)
ifeq ($(HAVE_GETOPT_LONG),1)
DEFINES += HAVE_GETOPT_LONG=1
endif
#$(info HAVE_GETOPT_LONG=$(HAVE_GETOPT_LONG))

HAVE_PSAPI := $(call check-function-exists,GetMappedFileNameA,-lpsapi,windows.h psapi.h)
ifeq ($(HAVE_PSAPI),1)
DEFINES += HAVE_PSAPI=1
LIBPSAPI = -lpsapi
endif
$(info HAVE_PSAPI=$(HAVE_PSAPI))


#$(call def-include-exists,errno.h,HAVE_ERRNO_H)
$(call def-include-exists,sys/devpoll.h,HAVE_DEVPOLL)
#$(info HAVE_DEVPOLL=$(HAVE_DEVPOLL))

HAVE_ZLIB := $(call check-function-exists,deflate,-lz)
#$(info HAVE_ZLIB=$(HAVE_ZLIB))

HAVE_LIBLZMA := $(call check-function-exists,lzma_code,-llzma)
#$(info HAVE_LIBLZMA=$(HAVE_LIBLZMA))

HAVE_LIBBZ2 := $(call check-function-exists,BZ2_bzCompress,-lbz2)
$(info HAVE_LIBBZ2=$(HAVE_LIBBZ2))

#$(call def-function-exists,EPOLL,epoll_wait,)
#$(info HAVE_EPOLL=$(HAVE_EPOLL))

$(call def-function-exists,KQUEUE,kevent,)
#$(info HAVE_KQUEUE=$(HAVE_KQUEUE))

$(call def-function-exists,POLL,poll,)
#$(info HAVE_POLL=$(HAVE_POLL))

#$(call def-function-exists,SIGWAITINFO,sigwaitinfo,)
#$(info HAVE_SIGWAITINFO=$(HAVE_SIGWAITINFO))

$(call def-function-exists,SIGIO,sigtimedwait,)
#$(info HAVE_SIGIO=$(HAVE_SIGIO))


HAVE_LSEEK64 := $(call check-function-exists,lseek64)
HAVE_LSEEK := $(call check-function-exists,lseek)
HAVE_LLSEEK := $(call check-function-exists,llseek)
HAVE_POSIX_MEMALIGN := $(call check-function-exists,posix_memalign)
#$(info HAVE_POSIX_MEMALIGN=$(HAVE_POSIX_MEMALIGN))


ifeq ($(OS),darwin)
  READDIR := 1
#DEFINES += USE_READDIR=1
#CFLAGS += -DUSE_READDIR=1
#CPPFLAGS += -DUSE_READDIR=1
ifneq ($(HAVE_LSEEK64),)
io_seek = lseek64
else
ifeq ($(HAVE_LSEEK),1)
io_seek = lseek
else
ifeq ($(HAVE__LLSEEK),1)
io_seek = _llseek
else
ifeq ($(HAVE_LLSEEK),1)
io_seek = llseek
else
ifeq ($(HAVE__LSEEK),1)
io_seek = _lseek
else
ifeq ($(HAVE__LSEEKI64),1)
io_seek = _lseeki64
else
ifeq ($(HAVE_LSEEK),1)
io_seek = lseek
endif
endif
endif
endif
endif
endif
endif
endif

ifeq ($(HAVE_ZLIB),1)
DEFINES += HAVE_ZLIB=1
CPPFLAGS += -DHAVE_ZLIB=1
LIBS = -lz
endif
ifeq ($(HAVE_LIBLZMA),1)
DEFINES += HAVE_LIBLZMA=1
CPPFLAGS += -DHAVE_LIBLZMA=1
LIBLZMA = -llzma
endif
ifeq ($(HAVE_LIBBZ2),1)
DEFINES += HAVE_LIBBZ2=1
CPPFLAGS += -DHAVE_LIBBZ2=1
LIBBZ2 = -lbz2
endif
ifeq ($(HAVE_ERRNO_H),1)
DEFINES += HAVE_ERRNO_H=1
endif

ifeq ($(io_seek),)
io_seek := lseek
endif
ifneq ($(io_seek),)
DEFINES += io_seek=$(io_seek)
endif
ifeq ($(HAVE_POSIX_MEMALIGN),1)
DEFINES += HAVE_POSIX_MEMALIGN=1
endif


#CPPFLAGS += $(DEFS:%=-D%)
#CFLAGS += $(DEFS:%=-D%)
#
#$(info DEFS: $(DEFS))
DEFS += $(patsubst %,-D%,$(DEFINES))
DEFS := $(patsubst -D-D%,-D%,$(patsubst %,-D%,$(DEFS)))
DEFS := $(sort $(DEFS))

CPPFLAGS += $(DEFS)

#CFLAGS := $(subst -Os,-Os,$(CFLAGS))

FLAGS += $(patsubst %,-W%,$(WARNINGS)) 
FLAGS += $(CPPFLAGS)
FLAGS := $(sort $(FLAGS))

SPACE := $(DUMMY) $(DUMMY)
define NL


endef

ifneq ($(HOST),$(subst msys1,,$(HOST)))
  NO_AT ?= 1
endif


ifneq ($(SYSROOT),)
ifneq ($(call file-exists,$(SYSROOT)),1)
SYSROOT :=
endif
endif


ifeq ($(SYSROOT),)

ifeq ($(call file-exists,/opt/$(HOST)/sys-root),1)
SYSROOT := /opt/$(HOST)/sys-root
else
ifeq ($(call file-exists,/usr/$(HOST)/sys-root),1)
SYSROOT := /usr/$(HOST)/sys-root
else
ifeq ($(call file-exists,/usr/$(HOST)/sysroot),1)
SYSROOT := /usr/$(HOST)/sysroot
else
ifeq ($(call file-exists,/opt/$(HOST)/sysroot),1)
SYSROOT := /opt/$(HOST)/sys-root
else
  ifneq ($(HOST),)
SYSROOT := /usr/$(HOST)
  endif
endif
endif
endif
endif

endif

ifeq ($(SYSROOT),/usr/)
  SYSROOT :=
endif

ifneq ($(SYS),msys)
ifneq ($(SYSROOT),)
ifeq ($(call file-exists,$(SYSROOT)/include),1)
FLAGS += --sysroot=$(SYSROOT)
endif
endif
endif

else

ifneq ($(NO_AT),1)
  COMPILe := $(CROSS_COMPILE)$(CC) @$(FLAGS_FILE) -c
CPPFLAGS := 
INCLUDES := 

$(file >$@,$(subst $(SPACE),\
,$(FLAGS)))
endif

# no flags file
endif

CC += @$(FLAGS_FILE)

$(info COMPILE=$(COMPILE))
$(info FLAGS=$(FLAGS))
$(info CFLAGS=$(CFLAGS))
$(info @FLAGS_FILE: $(shell cat $(FLAGS_FILE)))

PROGRAMS = $(patsubst %,$(BUILDDIR)%$(M64_)$(EXEEXT),list-r count-depth decode-ls-lR reg2cmd regfilter torrent-progress mediathek-parser mediathek-list xc8-wrapper picc-wrapper picc18-wrapper sdcc-wrapper rdir-test httptest xmlpp xmltest xmltest2 xmltest3 xmltest4 plsconv compiler-wrapper impgen pathtool ntldd hexedit eagle-init-brd eagle-gen-cmds eagle-to-circuit buffertest jsontest elfwrsec ccat ziptest pkgcfg dnstest dnsip dnsname sln bsdiffcat binfmttest elf64list macho32list pelist elflist genmakefile strarraytest)
MAN3 = $(wildcard lib/*/*.3)

 #opensearch-dump
LIBSOURCES = $(wildcard lib/*/*.c)
ifeq ($(DO_CXX),1)
PROGRAMS += \
  piccfghex$(M64_)$(EXEEXT)
#  $(BUILDDIR)mediathek-parser-cpp$(M64_)$(EXEEXT)
endif
OBJECTS = $(PROGRAMS:%=%.o) $(LIB_OBJ)

vpath $(BUILDDIR) lib src

VPATH = $(BUILDDIR):.:lib:src

$(info BUILD_TYPE: $(BUILD_TYPE))
$(info FLAGS: $(FLAGS))
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
$(info FLAGS: $(FLAGS))
$(info CFLAGS: $(CFLAGS))
$(info CXXFLAGS: $(CXXFLAGS))
$(info LDFLAGS: $(LDFLAGS))
$(info EXTRA_CPPFLAGS: $(EXTRA_CPPFLAGS))
$(info CC: $(CC))
$(info COMPILE: $(COMPILE))
$(info CROSS_COMPILE: $(CROSS_COMPILE))

MODULES += $(patsubst %,$(BUILDDIR)%.a,array binfmt buffer byte case cb cbmap charbuf dir dns elf env errmsg expand fmt gpio hmap http iarray io json list map mmap ndelay open path pe playlist range rdir scan sig slist socket str stralloc strarray strlist tai taia textbuf uint16 uint32 uint64 var vartab xml)

$(info BUILDDIR: $(BUILDDIR))
$(info NO_AT: $(NO_AT))

COMPILE := $(CROSS_COMPILE)$(CC) $(CFLAGS) $(CPPFLAGS) $(DEFS) -c

all: builddir $(BUILDDIR) $(FLAGS_FILE) $(MODULES) $(PROGRAMS)










$(FLAGS_FILE): $(BUILDDIR)
	$(file >$@,$(subst $(SPACE),\
	,$(FLAGS)))

all-release:
	$(MAKE) DEBUG=0 all

PROGRAM_OBJECTS = $(patsubst %.c,$(BUILDDIR)%.o,$(wildcard *.c))


CPPFLAGS := -I.

.PHONY: builddir
builddir: $(BUILDDIR) $(OBJDIR)
	-mkdir -p $(BUILDDIR) || mkdir $(BUILDDIR)
	-md $(subst /,\,$(BUILDDIR))

$(BUILDDIR):
	-mkdir -p $(BUILDDIR) || mkdir $(BUILDDIR)
	-md $(subst /,\,$(BUILDDIR))

$(OBJDIR):
	-mkdir -p $(OBJDIR) || mkdir $(OBJDIR)
	-md $(subst /,\,$(OBJDIR))


$(call lib-target,array,lib/umult64.c)
$(call lib-target,binfmt)
$(call lib-target,buffer)
$(call lib-target,byte)
$(call lib-target,case)
$(call lib-target,cb)
$(call lib-target,cbmap,lib/memalign.c)
$(call lib-target,charbuf)
$(call lib-target,dir)
$(call lib-target,elf)
$(call lib-target,env,lib/setenv.c lib/getenv.c)
$(call lib-target,expand,lib/glob.c)
$(call lib-target,fmt)
$(call lib-target,gpio)
$(call lib-target,hmap)
$(call lib-target,http)
$(call lib-target,iarray)
$(call lib-target,io,lib/iopause.c)
$(call lib-target,json)
$(call lib-target,list)
$(call lib-target,map)
$(call lib-target,mmap)
$(call lib-target,ndelay)
$(call lib-target,path,lib/readlink.c lib/symlink.c lib/fork.c lib/wordexp.c lib/pipe2.c lib/getdelim.c)
$(call lib-target,open)
$(call lib-target,pe)
$(call lib-target,playlist)
$(call lib-target,range)
$(call lib-target,rdir)
$(call lib-target,scan)
$(call lib-target,sig)
$(call lib-target,slist)
$(call lib-target,str,lib/isleap.c lib/time_table_spd.c)
$(call lib-target,stralloc,lib/utf8.c)
$(call lib-target,strarray)
$(call lib-target,strlist)
$(call lib-target,tai)
$(call lib-target,taia)
$(call lib-target,textbuf)
$(call lib-target,time)
$(call lib-target,uint16)
$(call lib-target,uint32)
$(call lib-target,uint64)
$(call lib-target,unix)
$(call lib-target,var)
$(call lib-target,vartab)

$(call lib-target,xml)
$(call lib-target,socket,lib/socket/winsock2errno.c lib/socket/winsock_init.c)
$(call lib-target,errmsg)
$(call lib-target,dns)

$(BUILDDIR)decode-ls-lR.o: decode-ls-lR.c
$(BUILDDIR)decode-ls-lR$(M64_)$(EXEEXT): $(BUILDDIR)decode-ls-lR.o $(call add-library, buffer stralloc str byte)
	$(CROSS_COMPILE)$(CC) $(LDFLAGS) $(EXTRA_LDFLAGS) $(CFLAGS) $(EXTRA_CPPFLAGS) -Wl,-rpath=$(BUILDDIR:%/=%) -o $@ $^ $(LIBS) $(EXTRA_LIBS)

$(BUILDDIR)count-depth.o: count-depth.c
$(BUILDDIR)count-depth$(M64_)$(EXEEXT): $(BUILDDIR)count-depth.o $(call add-library, buffer byte fmt)
	
	$(CROSS_COMPILE)$(CC) $(LDFLAGS) $(EXTRA_LDFLAGS) $(CFLAGS) $(EXTRA_CPPFLAGS) -Wl,-rpath=$(BUILDDIR:%/=%) -o $@ $^ $(LIBS) $(EXTRA_LIBS)
ifeq ($(DO_STRIP),1)
	$(STRIP) $@
endif

$(BUILDDIR)list-r.o: list-r.c
$(BUILDDIR)list-r$(M64_)$(EXEEXT): $(BUILDDIR)list-r.o $(BUILDDIR)getopt.o $(BUILDDIR)fnmatch.o $(call add-library,strarray open array rdir dir buffer byte stralloc fmt str)
	$(CROSS_COMPILE)$(CC) $(LDFLAGS) $(EXTRA_LDFLAGS) $(CFLAGS) $(EXTRA_CPPFLAGS) -Wl,-rpath=$(BUILDDIR:%/=%) -o $@ $^ $(LIBS) $(EXTRA_LIBS)
ifeq ($(DO_STRIP),1)
	$(STRIP) $@
endif

$(BUILDDIR)rdir-test.o: rdir-test.c
$(BUILDDIR)rdir-test$(M64_)$(EXEEXT): $(BUILDDIR)rdir-test.o $(BUILDDIR)fnmatch.o $(call add-library, rdir dir array buffer  byte stralloc fmt str)
	$(CROSS_COMPILE)$(CC) $(LDFLAGS) $(EXTRA_LDFLAGS) $(CFLAGS) $(EXTRA_CPPFLAGS) -Wl,-rpath=$(BUILDDIR:%/=%) -o $@ $^ $(LIBS) $(EXTRA_LIBS)
ifeq ($(DO_STRIP),1)
	$(STRIP) $@
endif

$(BUILDDIR)reg2cmd.o: reg2cmd.c
$(BUILDDIR)reg2cmd$(M64_)$(EXEEXT): $(BUILDDIR)reg2cmd.o $(call add-library, buffer open fmt scan stralloc str byte)
	$(CROSS_COMPILE)$(CC) $(LDFLAGS) $(EXTRA_LDFLAGS) $(CFLAGS) $(EXTRA_CPPFLAGS) -Wl,-rpath=$(BUILDDIR:%/=%) -o $@ $^ $(LIBS) $(EXTRA_LIBS)
ifeq ($(DO_STRIP),1)
	$(STRIP) $@
endif

$(BUILDDIR)regfilter.o: regfilter.c
$(BUILDDIR)regfilter$(M64_)$(EXEEXT): $(BUILDDIR)regfilter.o $(call add-library, buffer open fmt scan stralloc str byte)
	$(CROSS_COMPILE)$(CC) $(LDFLAGS) $(EXTRA_LDFLAGS) $(CFLAGS) $(EXTRA_CPPFLAGS) -Wl,-rpath=$(BUILDDIR:%/=%) -o $@ $^ $(LIBS) $(EXTRA_LIBS)
ifeq ($(DO_STRIP),1)
	$(STRIP) $@
endif

$(BUILDDIR)torrent-progress.o: torrent-progress.c
$(BUILDDIR)torrent-progress$(M64_)$(EXEEXT): $(BUILDDIR)torrent-progress.o $(call add-library, buffer stralloc fmt mmap open str byte)
	$(CROSS_COMPILE)$(CC) $(LDFLAGS) $(EXTRA_LDFLAGS) $(CFLAGS) $(EXTRA_CPPFLAGS) -Wl,-rpath=$(BUILDDIR:%/=%) -o $@ $^ $(LIBS) $(EXTRA_LIBS)
ifeq ($(DO_STRIP),1)
	$(STRIP) $@
endif

$(BUILDDIR)mediathek-parser$(M64_)$(EXEEXT): LIBS += $(KERNEL32_LIB)
$(BUILDDIR)mediathek-parser$(M64_)$(EXEEXT): $(BUILDDIR)mediathek-parser.o $(BUILDDIR)getopt.o $(call add-library, array buffer fmt mmap open  strlist stralloc str unix byte)
	$(CROSS_COMPILE)$(CC) $(LDFLAGS) $(EXTRA_LDFLAGS) $(CFLAGS) $(EXTRA_CPPFLAGS) -Wl,-rpath=$(BUILDDIR:%/=%) -o $@ $^ $(LIBS)   $(EXTRA_LIBS)
ifeq ($(DO_STRIP),1)
	$(STRIP) $@
endif

$(BUILDDIR)mediathek-parser$(M64_)$(EXEEXT): LIBS += $(KERNEL32_LIB)
$(BUILDDIR)mediathek-list$(M64_)$(EXEEXT): $(BUILDDIR)mediathek-list.o $(BUILDDIR)getopt.o $(BUILDDIR)popen.o $(call add-library, array strlist buffer fmt mmap open  scan stralloc str unix byte)
	$(CROSS_COMPILE)$(CC) $(LDFLAGS) $(EXTRA_LDFLAGS) $(CFLAGS) $(EXTRA_CPPFLAGS) -Wl,-rpath=$(BUILDDIR:%/=%) -o $@ $^ $(LIBS)   $(EXTRA_LIBS)
ifeq ($(DO_STRIP),1)
	$(STRIP) $@
endif

$(BUILDDIR)httptest$(M64_)$(EXEEXT): LIBS += $(WINSOCK_LIB) $(IPHLPAPI_LIB)
$(BUILDDIR)httptest$(M64_)$(EXEEXT): LIBS += $(OTHERLIBS)
$(BUILDDIR)httptest$(M64_)$(EXEEXT): $(BUILDDIR)httptest.o $(call add-library,http dns errmsg socket io case taia tai iarray array ndelay uint16 buffer fmt mmap open  scan stralloc str byte   taia  uint32 )
	$(CROSS_COMPILE)$(CC) $(LDFLAGS) $(EXTRA_LDFLAGS) $(CFLAGS) $(EXTRA_CPPFLAGS) -Wl,-rpath=$(BUILDDIR:%/=%) -o $@ $^ $(LIBS)  $(EXTRA_LIBS)
ifeq ($(DO_STRIP),1)
	$(STRIP) $@
endif

$(BUILDDIR)xmlpp$(M64_)$(EXEEXT): $(BUILDDIR)xmlpp.o $(call add-library, xml array charbuf textbuf hmap buffer mmap open stralloc byte scan fmt fmt str)
	$(CROSS_COMPILE)$(CC) $(LDFLAGS) $(CFLAGS) $(EXTRA_CPPFLAGS) -Wl,-rpath=$(BUILDDIR:%/=%) -o $@ $^ $(LIBS) $(EXTRA_LIBS)
ifeq ($(DO_STRIP),1)
	$(STRIP) $@
endif

$(BUILDDIR)xmltest$(M64_)$(EXEEXT): $(BUILDDIR)xmltest.o $(call add-library, xml array charbuf textbuf hmap buffer mmap open stralloc byte scan fmt fmt str)
	$(CROSS_COMPILE)$(CC) $(LDFLAGS) $(CFLAGS) $(EXTRA_CPPFLAGS) -Wl,-rpath=$(BUILDDIR:%/=%) -o $@ $^ $(LIBS) $(EXTRA_LIBS)
ifeq ($(DO_STRIP),1)
	$(STRIP) $@
endif

$(BUILDDIR)xmltest2$(M64_)$(EXEEXT): $(BUILDDIR)xmltest2.o $(call add-library, xml strlist array charbuf textbuf hmap buffer mmap open stralloc byte scan fmt fmt str)
	$(CROSS_COMPILE)$(CC) $(LDFLAGS) $(CFLAGS) $(EXTRA_CPPFLAGS) -Wl,-rpath=$(BUILDDIR:%/=%) -o $@ $^ $(LIBS) $(EXTRA_LIBS)
ifeq ($(DO_STRIP),1)
	$(STRIP) $@
endif

$(BUILDDIR)xmltest3$(M64_)$(EXEEXT): $(BUILDDIR)xmltest3.o $(call add-library, xml strlist array charbuf textbuf hmap buffer mmap open stralloc byte scan fmt fmt str)
	$(CROSS_COMPILE)$(CC) $(LDFLAGS) $(CFLAGS) $(EXTRA_CPPFLAGS) -Wl,-rpath=$(BUILDDIR:%/=%) -o $@ $^ $(LIBS) $(EXTRA_LIBS)
ifeq ($(DO_STRIP),1)
	$(STRIP) $@
endif

$(BUILDDIR)xmltest4$(M64_)$(EXEEXT): $(BUILDDIR)xmltest4.o $(call add-library, cbmap xml array strlist charbuf textbuf hmap buffer mmap open stralloc byte scan fmt fmt str)
	$(CROSS_COMPILE)$(CC) $(LDFLAGS) $(EXTRA_LDFLAGS) $(CFLAGS) $(EXTRA_CPPFLAGS) -Wl,-rpath=$(BUILDDIR:%/=%) -o $@ $^ $(LIBS)  $(EXTRA_LIBS)
ifeq ($(DO_STRIP),1)
	$(STRIP) $@
endif

$(BUILDDIR)jsontest$(M64_)$(EXEEXT): $(BUILDDIR)jsontest.o $(call add-library, json array charbuf textbuf hmap buffer mmap open stralloc str byte scan fmt)
	$(CROSS_COMPILE)$(CC) $(LDFLAGS) $(EXTRA_LDFLAGS) $(CFLAGS) $(EXTRA_CPPFLAGS) -Wl,-rpath=$(BUILDDIR:%/=%) -o $@ $^ $(LIBS)  $(EXTRA_LIBS)
ifeq ($(DO_STRIP),1)
	$(STRIP) $@
endif


$(BUILDDIR)elfwrsec$(M64_)$(EXEEXT): $(BUILDDIR)elfwrsec.o $(call add-library, elf buffer mmap open fmt scan str byte)
	$(CROSS_COMPILE)$(CC) $(LDFLAGS) $(EXTRA_LDFLAGS) $(CFLAGS) $(EXTRA_CPPFLAGS) -Wl,-rpath=$(BUILDDIR:%/=%) -o $@ $^ $(LIBS)  $(EXTRA_LIBS)
ifeq ($(DO_STRIP),1)
	$(STRIP) $@
endif

ifeq ($(HAVE_ZLIB),1)
$(BUILDDIR)buffertest$(M64_)$(EXEEXT): LIBS += -lz
endif
ifeq ($(HAVE_LIBLZMA),1)
$(BUILDDIR)buffertest$(M64_)$(EXEEXT): LIBS += -llzma
endif
ifeq ($(HAVE_LIBBZ2),1)
$(BUILDDIR)buffertest$(M64_)$(EXEEXT): LIBS += -lbz2
endif
$(BUILDDIR)buffertest$(M64_)$(EXEEXT): LIBS += $(LIBPSAPI)
$(BUILDDIR)buffertest$(M64_)$(EXEEXT): $(BUILDDIR)buffertest.o $(call add-library, array charbuf textbuf hmap stralloc buffer mmap open byte scan fmt fmt str)
	$(CROSS_COMPILE)$(CC) $(LDFLAGS) $(EXTRA_LDFLAGS) $(CFLAGS) $(EXTRA_CPPFLAGS) -Wl,-rpath=$(BUILDDIR:%/=%) -o $@ $^ $(LIBS)  $(EXTRA_LIBS)
ifeq ($(DO_STRIP),1)
	$(STRIP) $@
endif

$(BUILDDIR)ccat$(M64_)$(EXEEXT): LIBS += $(LIBZ) $(LIBLZMA) $(LIBBZ2)
$(BUILDDIR)ccat$(M64_)$(EXEEXT): $(BUILDDIR)ccat.o $(BUILDDIR)getopt.o $(call add-library, array charbuf textbuf hmap buffer mmap open stralloc byte scan fmt fmt str)
	$(CROSS_COMPILE)$(CC) $(LDFLAGS) $(EXTRA_LDFLAGS) $(CFLAGS) $(EXTRA_CPPFLAGS) -Wl,-rpath=$(BUILDDIR:%/=%) -o $@ $^ $(LIBS)  $(EXTRA_LIBS)
ifeq ($(DO_STRIP),1)
	$(STRIP) $@
endif

$(BUILDDIR)ziptest$(M64_)$(EXEEXT): LIBS += $(LIBZ) $(LIBLZMA) $(LIBBZ2)
$(BUILDDIR)ziptest$(M64_)$(EXEEXT): $(BUILDDIR)ziptest.o $(BUILDDIR)getopt.o $(call add-library, array charbuf textbuf hmap buffer mmap open stralloc byte scan fmt fmt str)
	$(CROSS_COMPILE)$(CC) $(LDFLAGS) $(EXTRA_LDFLAGS) $(CFLAGS) $(EXTRA_CPPFLAGS) -Wl,-rpath=$(BUILDDIR:%/=%) -o $@ $^ $(LIBS)  $(EXTRA_LIBS)
ifeq ($(DO_STRIP),1)
	$(STRIP) $@
endif

$(BUILDDIR)plsconv$(M64_)$(EXEEXT): LIBS += -lm
#$(BUILDDIR)plsconv$(M64_)$(EXEEXT): CFLAGS +=
$(BUILDDIR)plsconv$(M64_)$(EXEEXT): $(BUILDDIR)plsconv.o $(BUILDDIR)getopt.o $(call add-library,  playlist xml hmap stralloc buffer mmap open str fmt scan  byte)
	$(CROSS_COMPILE)$(CC) $(LDFLAGS) $(CFLAGS) $(EXTRA_CPPFLAGS) -Wl,-rpath=$(BUILDDIR:%/=%) -o $@ $^ $(LIBS) $(EXTRA_LIBS)
ifeq ($(DO_STRIP),1)
	$(STRIP) $@
endif

$(BUILDDIR)pathtool$(M64_)$(EXEEXT): $(BUILDDIR)pathtool.o $(BUILDDIR)getopt.o $(call add-library,errmsg path unix strlist stralloc buffer mmap open str fmt scan byte)
	$(CROSS_COMPILE)$(CC) $(LDFLAGS) $(EXTRA_LDFLAGS) $(CFLAGS) $(EXTRA_CPPFLAGS) -Wl,-rpath=$(BUILDDIR:%/=%) -o $@ $^ $(LIBS)  $(EXTRA_LIBS)
ifeq ($(DO_STRIP),1)
	$(STRIP) $@
endif

$(BUILDDIR)access.o: access.c
$(BUILDDIR)access$(M64_)$(EXEEXT): $(BUILDDIR)access.o $(call add-library, open array  rdir dir buffer byte stralloc fmt str)
	$(CROSS_COMPILE)$(CC) $(LDFLAGS) $(EXTRA_LDFLAGS) $(CFLAGS) $(EXTRA_CPPFLAGS) -Wl,-rpath=$(BUILDDIR:%/=%) -o $@ $^ $(LIBS) $(EXTRA_LIBS)
ifeq ($(DO_STRIP),1)
	$(STRIP) $@
endif

#$(BUILDDIR)opensearch-dump$(M64_)$(EXEEXT): INCLUDES += $(ICONV_CFLAGS)
$(BUILDDIR)opensearch-dump$(M64_)$(EXEEXT): LIBS += $(ICONV_LIBS) $(OTHERLIBS)
$(BUILDDIR)opensearch-dump$(M64_)$(EXEEXT): $(BUILDDIR)opensearch-dump.o $(call add-library, xml hmap buffer mmap open scan fmt stralloc str byte)
	$(CROSS_COMPILE)$(CC) $(LDFLAGS) $(CFLAGS) $(EXTRA_CPPFLAGS) -Wl,-rpath=$(BUILDDIR:%/=%) -o $@ $^ $(LIBS) $(EXTRA_LIBS)
ifeq ($(DO_STRIP),1)
	$(STRIP) $@
endif

$(BUILDDIR)eagle-init-brd$(M64_)$(EXEEXT): LIBS += $(OTHERLIBS) -lm
$(BUILDDIR)eagle-init-brd$(M64_)$(EXEEXT): $(BUILDDIR)eagle-init-brd.o $(call add-library, errmsg path strarray strlist xml hmap buffer mmap open scan stralloc fmt str byte)
	$(CROSS_COMPILE)$(CC) $(LDFLAGS) $(CFLAGS) $(EXTRA_CPPFLAGS) -Wl,-rpath=$(BUILDDIR:%/=%) -o $@ $^ $(LIBS) $(EXTRA_LIBS)
ifeq ($(DO_STRIP),1)
	#$(STRIP) $@
endif

#$(BUILDDIR)eagle-gen-cmds$(M64_)$(EXEEXT): CFLAGS += $(ICONV_CFLAGS)
$(BUILDDIR)eagle-gen-cmds$(M64_)$(EXEEXT): LIBS += $(ICONV_LIBS) $(OTHERLIBS) -lm
$(BUILDDIR)eagle-gen-cmds$(M64_)$(EXEEXT): $(BUILDDIR)eagle-gen-cmds.o $(BUILDDIR)getopt.o $(call add-library, errmsg path strarray cbmap xml hmap array buffer strlist stralloc mmap open scan fmt str byte)
	$(CROSS_COMPILE)$(CC) $(LDFLAGS) $(CFLAGS) $(EXTRA_CPPFLAGS) -Wl,-rpath=$(BUILDDIR:%/=%) -o $@ $^ $(LIBS) $(EXTRA_LIBS)
ifeq ($(DO_STRIP),1)
	#$(STRIP) $@
endif

$(BUILDDIR)eagle-to-circuit$(M64_)$(EXEEXT): LIBS += $(OTHERLIBS) -lm
$(BUILDDIR)eagle-to-circuit$(M64_)$(EXEEXT): $(BUILDDIR)eagle-to-circuit.o $(call add-library, errmsg path cbmap xml array charbuf textbuf hmap strlist buffer mmap open stralloc byte scan fmt fmt str)
	$(CROSS_COMPILE)$(CC) $(LDFLAGS) $(CFLAGS) $(EXTRA_CPPFLAGS) -Wl,-rpath=$(BUILDDIR:%/=%) -o $@ $^ $(LIBS) $(EXTRA_LIBS)
ifeq ($(DO_STRIP),1)
	#$(STRIP) $@
endif
$(BUILDDIR)impgen$(M64_)$(EXEEXT): $(BUILDDIR)impgen.o $(call add-library, pe mmap buffer byte fmt str open uint32)
	$(CROSS_COMPILE)$(CC) $(LDFLAGS) $(EXTRA_LDFLAGS) $(CFLAGS) $(EXTRA_CPPFLAGS) -Wl,-rpath=$(BUILDDIR:%/=%) -o $@ $^ $(LIBS)   $(EXTRA_LIBS)
ifeq ($(DO_STRIP),1)
	#$(STRIP) $@
endif

$(BUILDDIR)ntldd$(M64_)$(EXEEXT): LIBS += $(EXTRA_LIBS) $(ADVAPI32_LIB)
$(BUILDDIR)ntldd$(M64_)$(EXEEXT): CPPFLAGS += -DNTLDD_VERSION_MAJOR=0 -DNTLDD_VERSION_MINOR=2
$(BUILDDIR)ntldd$(M64_)$(EXEEXT): $(BUILDDIR)ntldd.o $(BUILDDIR)libntldd.o $(BUILDDIR)getopt.o $(call add-library, path pe mmap strlist buffer stralloc fmt str byte open uint32)
	$(CROSS_COMPILE)$(CC) $(LDFLAGS) $(EXTRA_LDFLAGS) $(CFLAGS) $(EXTRA_CPPFLAGS) -Wl,-rpath=$(BUILDDIR:%/=%) -o $@ $^ $(LIBS)   $(EXTRA_LIBS)
ifeq ($(DO_STRIP),1)
	#$(STRIP) $@
endif

$(BUILDDIR)hexedit$(M64_)$(EXEEXT): LIBS += $(EXTRA_LIBS) $(WINSOCK_LIB)
$(BUILDDIR)hexedit$(M64_)$(EXEEXT): $(BUILDDIR)hexedit.o $(call add-library, path errmsg io iarray array buffer mmap open byte str unix stralloc scan fmt open uint32)
	$(CROSS_COMPILE)$(CC) $(LDFLAGS) $(EXTRA_LDFLAGS) $(CFLAGS) $(EXTRA_CPPFLAGS) -Wl,-rpath=$(BUILDDIR:%/=%) -o $@ $^ $(LIBS)   $(EXTRA_LIBS)
ifeq ($(DO_STRIP),1)
	#$(STRIP) $@
endif

$(BUILDDIR)dnstest$(M64_)$(EXEEXT): LIBS += $(EXTRA_LIBS) $(WINSOCK_LIB) $(IPHLPAPI_LIB)
$(BUILDDIR)dnstest$(M64_)$(EXEEXT): $(BUILDDIR)dnstest.o $(call add-library, dns io socket ndelay errmsg taia tai open buffer case fmt scan stralloc byte str uint32 uint16)
	$(CROSS_COMPILE)$(CC) $(LDFLAGS) $(EXTRA_LDFLAGS) $(CFLAGS) $(EXTRA_CPPFLAGS) -Wl,-rpath=$(BUILDDIR:%/=%) -o $@ $^ $(LIBS)   $(EXTRA_LIBS)
ifeq ($(DO_STRIP),1)
	#$(STRIP) $@
endif

$(BUILDDIR)dnsip$(M64_)$(EXEEXT): LIBS += $(EXTRA_LIBS) $(WINSOCK_LIB) $(IPHLPAPI_LIB)
$(BUILDDIR)dnsip$(M64_)$(EXEEXT): $(BUILDDIR)dnsip.o $(call add-library, dns io socket ndelay errmsg taia tai open buffer case fmt scan stralloc byte str uint32 uint16)
	$(CROSS_COMPILE)$(CC) $(LDFLAGS) $(EXTRA_LDFLAGS) $(CFLAGS) $(EXTRA_CPPFLAGS) -Wl,-rpath=$(BUILDDIR:%/=%) -o $@ $^ $(LIBS)   $(EXTRA_LIBS)
ifeq ($(DO_STRIP),1)
	#$(STRIP) $@
endif

$(BUILDDIR)dnsname$(M64_)$(EXEEXT): LIBS += $(EXTRA_LIBS) $(WINSOCK_LIB) $(IPHLPAPI_LIB)
$(BUILDDIR)dnsname$(M64_)$(EXEEXT): $(BUILDDIR)dnsname.o $(call add-library, dns io socket ndelay errmsg taia tai open buffer case fmt scan stralloc byte str uint32 uint16)
	$(CROSS_COMPILE)$(CC) $(LDFLAGS) $(EXTRA_LDFLAGS) $(CFLAGS) $(EXTRA_CPPFLAGS) -Wl,-rpath=$(BUILDDIR:%/=%) -o $@ $^ $(LIBS)   $(EXTRA_LIBS)
ifeq ($(DO_STRIP),1)
	#$(STRIP) $@
endif

ifeq ($(USE_DIET),1)
$(BUILDDIR)pkgcfg$(M64_)$(EXEEXT): LIBS += -lcompat -lpthread
endif
$(BUILDDIR)pkgcfg$(M64_)$(EXEEXT): LIBS += $(EXTRA_LIBS)
$(BUILDDIR)pkgcfg$(M64_)$(EXEEXT): $(BUILDDIR)pkgcfg.o $(BUILDDIR)getopt.o $(call add-library,env slist cbmap path dir buffer errmsg strarray strlist stralloc array mmap byte scan fmt str unix open uint32)
	$(CROSS_COMPILE)$(CC) $(LDFLAGS) $(EXTRA_LDFLAGS) $(CFLAGS) $(EXTRA_CPPFLAGS) -Wl,-rpath=$(BUILDDIR:%/=%) -o $@ $^ $(LIBS)   $(EXTRA_LIBS)
ifeq ($(DO_STRIP),1)
	#$(STRIP) $@
endif

ifneq ($(DIET),)
$(BUILDDIR)sln$(M64_)$(EXEEXT): LIBS += -lcompat
endif
$(BUILDDIR)sln$(M64_)$(EXEEXT): LIBS += $(EXTRA_LIBS)
$(BUILDDIR)sln$(M64_)$(EXEEXT): $(BUILDDIR)sln.o $(call add-library, slist cbmap path dir buffer errmsg strarray strlist stralloc array mmap byte scan fmt str open uint32)
	$(CROSS_COMPILE)$(CC) $(LDFLAGS) $(EXTRA_LDFLAGS) $(CFLAGS) $(EXTRA_CPPFLAGS) -Wl,-rpath=$(BUILDDIR:%/=%) -o $@ $^ $(LIBS)   $(EXTRA_LIBS)
ifeq ($(DO_STRIP),1)
	#$(STRIP) $@
endif

$(BUILDDIR)compiler-wrapper$(M64_)$(EXEEXT): $(BUILDDIR)compiler-wrapper.o $(call add-library, dir strlist stralloc buffer fmt str  byte)
	$(CROSS_COMPILE)$(CC) $(LDFLAGS) $(EXTRA_LDFLAGS) $(CFLAGS) $(EXTRA_CPPFLAGS) -Wl,-rpath=$(BUILDDIR:%/=%) -o $@ $^ $(LIBS)   $(EXTRA_LIBS)
ifeq ($(DO_STRIP),1)
	$(STRIP) $@
endif

$(BUILDDIR)xc8-wrapper$(M64_)$(EXEEXT): DEFINES += XC8_WRAPPER=1
$(BUILDDIR)xc8-wrapper$(M64_)$(EXEEXT): $(BUILDDIR)compiler-wrapper.o $(call add-library, strlist dir stralloc buffer str byte fmt)
	$(CROSS_COMPILE)$(CC) $(LDFLAGS) $(EXTRA_LDFLAGS) $(CFLAGS) $(EXTRA_CPPFLAGS) -Wl,-rpath=$(BUILDDIR:%/=%) -o $@ $^ $(LIBS)   $(EXTRA_LIBS)
ifeq ($(DO_STRIP),1)
	$(STRIP) $@
endif

$(BUILDDIR)picc-wrapper$(M64_)$(EXEEXT): DEFINES += PICC_WRAPPER=1
$(BUILDDIR)picc-wrapper$(M64_)$(EXEEXT): $(BUILDDIR)compiler-wrapper.o $(call add-library, strlist dir stralloc buffer str byte fmt)
	$(CROSS_COMPILE)$(CC) $(LDFLAGS) $(EXTRA_LDFLAGS) $(CFLAGS) $(EXTRA_CPPFLAGS) -Wl,-rpath=$(BUILDDIR:%/=%) -o $@ $^ $(LIBS)   $(EXTRA_LIBS)
ifeq ($(DO_STRIP),1)
	$(STRIP) $@
endif

$(BUILDDIR)picc18-wrapper$(M64_)$(EXEEXT): DEFINES += PICC18_WRAPPER=1
$(BUILDDIR)picc18-wrapper$(M64_)$(EXEEXT): $(BUILDDIR)compiler-wrapper.o $(call add-library, strlist dir stralloc buffer str byte fmt)
	$(CROSS_COMPILE)$(CC) $(LDFLAGS) $(EXTRA_LDFLAGS) $(CFLAGS) $(EXTRA_CPPFLAGS) -Wl,-rpath=$(BUILDDIR:%/=%) -o $@ $^ $(LIBS)   $(EXTRA_LIBS)
ifeq ($(DO_STRIP),1)
	$(STRIP) $@
endif

$(BUILDDIR)sdcc-wrapper$(M64_)$(EXEEXT): DEFINES += SDCC_WRAPPER=1
$(BUILDDIR)sdcc-wrapper$(M64_)$(EXEEXT): $(BUILDDIR)compiler-wrapper.o $(call add-library, strlist dir stralloc buffer str byte fmt)
	$(CROSS_COMPILE)$(CC) $(LDFLAGS) $(EXTRA_LDFLAGS) $(CFLAGS) $(EXTRA_CPPFLAGS) -Wl,-rpath=$(BUILDDIR:%/=%) -o $@ $^ $(LIBS)   $(EXTRA_LIBS)
ifeq ($(DO_STRIP),1)
	$(STRIP) $@
endif
ifeq ($(DO_CXX),1)
$(BUILDDIR)piccfghex.o: piccfghex.cpp
$(BUILDDIR)piccfghex$(M64_)$(EXEEXT): $(BUILDDIR)piccfghex.o $(BUILDDIR)intelhex.o
	$(CROSS_COMPILE)$(CXX) $(LDFLAGS) $(EXTRA_LDFLAGS) $(CXXFLAGS) $(EXTRA_CPPFLAGS) -Wl,-rpath=$(BUILDDIR:%/=%) -o $@ $^ $(LIBS) $(EXTRA_LIBS)
ifeq ($(DO_STRIP),1)
	$(STRIP) $@
endif

$(BUILDDIR)mediathek-parser-cpp.o: mediathek-parser.cpp
	$(CROSS_COMPILE)$(CXX) $(CXXFLAGS) $(INCLUDES) -c $(EXTRA_CPPFLAGS) -Wl,-rpath=$(BUILDDIR:%/=%) -o $@ $<

$(BUILDDIR)mediathek-parser-cpp$(M64_)$(EXEEXT): $(BUILDDIR)mediathek-parser-cpp.o
	$(CROSS_COMPILE)$(CXX) $(LDFLAGS) $(EXTRA_LDFLAGS) $(CXXFLAGS) $(EXTRA_CPPFLAGS) -Wl,-rpath=$(BUILDDIR:%/=%) -o $@ $^ $(LIBS) $(EXTRA_LIBS)
ifeq ($(DO_STRIP),1)
	$(STRIP) $@
endif

	$(CROSS_COMPILE)$(CC) $(LDFLAGS) $(EXTRA_LDFLAGS) $(CFLAGS) $(EXTRA_CPPFLAGS) -Wl,-rpath=$(BUILDDIR:%/=%) -o $@ $^ $(LIBS) $(EXTRA_LIBS)
$(BUILDDIR)kbd-adjacency.o: kbd-adjacency.cpp
	$(CROSS_COMPILE)$(CXX) $(CXXFLAGS) $(INCLUDES) -c $(EXTRA_CPPFLAGS) -o  $(BUILDDIR)$(patsubst %.cpp,%.o,$(notdir $<))  $<

$(BUILDDIR)kbd-adjacency$(M64_)$(EXEEXT): $(BUILDDIR)kbd-adjacency.o $(LIB_OBJ)
	$(CROSS_COMPILE)$(CXX) $(LDFLAGS) $(EXTRA_LDFLAGS) $(CXXFLAGS) $(EXTRA_CPPFLAGS) -Wl,-rpath=$(BUILDDIR:%/=%) -o $@ $^ $(LIBS) $(EXTRA_LIBS)
ifeq ($(DO_STRIP),1)
	$(STRIP) $@
endif
endif

$(BUILDDIR)bsdiffcat$(M64_)$(EXEEXT): LIBS += $(LIBBZ2)
$(BUILDDIR)bsdiffcat$(M64_)$(EXEEXT): $(BUILDDIR)bsdiffcat.o $(call add-library, array strlist dir stralloc errmsg buffer mmap open str byte fmt uint64 uint32)
	$(CROSS_COMPILE)$(CC) $(LDFLAGS) $(EXTRA_LDFLAGS) $(CFLAGS) $(EXTRA_CPPFLAGS) -Wl,-rpath=$(BUILDDIR:%/=%) -o $@ $^ $(LIBS) $(EXTRA_LIBS)
ifeq ($(DO_STRIP),1)
	$(STRIP) $@
endif

$(BUILDDIR)binfmttest$(M64_)$(EXEEXT): LIBS += $(LIBBZ2)
$(BUILDDIR)binfmttest$(M64_)$(EXEEXT): $(BUILDDIR)binfmttest.o $(call add-library, binfmt pe array strlist dir stralloc errmsg buffer mmap open str byte fmt uint64 uint32)
	$(CROSS_COMPILE)$(CC) $(LDFLAGS) $(EXTRA_LDFLAGS) $(CFLAGS) $(EXTRA_CPPFLAGS) -Wl,-rpath=$(BUILDDIR:%/=%) -o $@ $^ $(LIBS) $(EXTRA_LIBS)
ifeq ($(DO_STRIP),1)
	$(STRIP) $@
endif

$(BUILDDIR)elf64list$(M64_)$(EXEEXT): LIBS += $(LIBBZ2)
$(BUILDDIR)elf64list$(M64_)$(EXEEXT): $(BUILDDIR)elf64list.o $(call add-library, binfmt pe array strlist dir stralloc errmsg buffer mmap open str byte fmt uint64 uint32)
	$(CROSS_COMPILE)$(CC) $(LDFLAGS) $(EXTRA_LDFLAGS) $(CFLAGS) $(EXTRA_CPPFLAGS) -Wl,-rpath=$(BUILDDIR:%/=%) -o $@ $^ $(LIBS) $(EXTRA_LIBS)
ifeq ($(DO_STRIP),1)
	$(STRIP) $@
endif

$(BUILDDIR)macho32list$(M64_)$(EXEEXT): LIBS += $(LIBBZ2)
$(BUILDDIR)macho32list$(M64_)$(EXEEXT): $(BUILDDIR)macho32list.o $(call add-library, binfmt pe array strlist dir stralloc errmsg buffer mmap open str byte fmt uint64 uint32)
	$(CROSS_COMPILE)$(CC) $(LDFLAGS) $(EXTRA_LDFLAGS) $(CFLAGS) $(EXTRA_CPPFLAGS) -Wl,-rpath=$(BUILDDIR:%/=%) -o $@ $^ $(LIBS) $(EXTRA_LIBS)
ifeq ($(DO_STRIP),1)
	$(STRIP) $@
endif

$(BUILDDIR)pelist$(M64_)$(EXEEXT): LIBS += $(LIBBZ2)
$(BUILDDIR)pelist$(M64_)$(EXEEXT): $(BUILDDIR)pelist.o $(BUILDDIR)getopt.o $(call add-library, binfmt pe array strlist dir stralloc errmsg buffer mmap open str byte fmt uint64 uint32)
	$(CROSS_COMPILE)$(CC) $(LDFLAGS) $(EXTRA_LDFLAGS) $(CFLAGS) $(EXTRA_CPPFLAGS) -Wl,-rpath=$(BUILDDIR:%/=%) -o $@ $^ $(LIBS) $(EXTRA_LIBS)
ifeq ($(DO_STRIP),1)
	$(STRIP) $@
endif


$(BUILDDIR)elflist$(M64_)$(EXEEXT): LIBS += $(LIBBZ2)
$(BUILDDIR)elflist$(M64_)$(EXEEXT): $(BUILDDIR)elflist.o $(call add-library, binfmt elf range array strlist dir stralloc errmsg buffer mmap open str byte fmt uint64 uint32)
	$(CROSS_COMPILE)$(CC) $(LDFLAGS) $(EXTRA_LDFLAGS) $(CFLAGS) $(EXTRA_CPPFLAGS) -Wl,-rpath=$(BUILDDIR:%/=%) -o $@ $^ $(LIBS) $(EXTRA_LIBS)
ifeq ($(DO_STRIP),1)
	$(STRIP) $@
endif

$(BUILDDIR)strarraytest$(M64_)$(EXEEXT): LIBS += $(LIBBZ2)
$(BUILDDIR)strarraytest$(M64_)$(EXEEXT): $(BUILDDIR)strarraytest.o $(call add-library, strarray array strlist dir stralloc errmsg buffer mmap open str byte fmt uint64 uint32)
	$(CROSS_COMPILE)$(CC) $(LDFLAGS) $(EXTRA_LDFLAGS) $(CFLAGS) $(EXTRA_CPPFLAGS) -Wl,-rpath=$(BUILDDIR:%/=%) -o $@ $^ $(LIBS) $(EXTRA_LIBS)
ifeq ($(DO_STRIP),1)
	$(STRIP) $@
endif


$(BUILDDIR)genmakefile$(M64_)$(EXEEXT): LIBS += $(LIBBZ2) $(SHLWAPI_LIB)
$(BUILDDIR)genmakefile$(M64_)$(EXEEXT): $(BUILDDIR)genmakefile.o $(call add-library,strarray slist rdir dir path strlist hmap buffer mmap open unix stralloc scan fmt str byte array)
	$(CROSS_COMPILE)$(CC) $(LDFLAGS) $(EXTRA_LDFLAGS) $(CFLAGS) $(EXTRA_CPPFLAGS) -Wl,-rpath=$(BUILDDIR:%/=%) -o $@ $^ $(LIBS) $(EXTRA_LIBS)
ifeq ($(DO_STRIP),1)
	$(STRIP) $@
endif


ifeq ($(BUILDDIR),)
.c.o:
	$(COMPILE) $<

%.o: %.c
	$(COMPILE) $<

.cpp.o:
	$(CROSS_COMPILE)$(CXX) $(CXXOPTS) $(CXXFLAGS) $(EXTRA_CPPFLAGS) -c $<

%.o: %.cpp
	$(CROSS_COMPILE)$(CXX) $(CXXOPTS) $(CXXFLAGS) $(EXTRA_CPPFLAGS) -c $<
else
.c.o:
	$(COMPILE) $(EXTRA_CPPFLAGS) -o $(BUILDDIR)$@ $<

$(BUILDDIR)%.o: lib/%.c
	$(COMPILE) $(EXTRA_CPPFLAGS) -o $(BUILDDIR)$(patsubst lib/%.c,%.o,$<) $<

$(BUILDDIR)%.o: %.c
	$(COMPILE) $(EXTRA_CPPFLAGS) -o $(BUILDDIR)$(patsubst %.c,%.o,$<) $<

.cpp.o:
	$(CROSS_COMPILE)$(CXX) $(CXXOPTS) $(CXXFLAGS) $(EXTRA_CPPFLAGS) -c $<


	$(CROSS_COMPILE)$(CXX) $(CXXOPTS) $(CXXFLAGS) $(INCLUDES) -c $(EXTRA_CPPFLAGS) -o $(BUILDDIR)$(patsubst %.cpp,%.o,$<) $<
endif

clean:
	$(RM) -f $(OBJECTS) list-r.o list-r$(M64_)$(EXEEXT)

install: all
	$(INSTALL) -d $(DESTDIR)$(bindir)
	$(INSTALL) -m 755 $(PROGRAMS) $(DESTDIR)$(bindir)
	$(INSTALL) -d $(DESTDIR)$(man3dir)
	$(INSTALL) -m 644 $(MAN3) $(DESTDIR)$(man3dir)
	@echo "gzip -3 -f $(DESTDIR)$(man3dir)/*.3" 1>&2; cd $(DESTDIR)$(man3dir) && \
		gzip -3 -f $(notdir $(MAN3))
#	@echo 'DESTDIR="$(DESTDIR)"' 1>&2
#	@echo 'bindir="$(bindir)"' 1>&2
#	@echo 'man3dir="$(man3dir)"' 1>&2
#	@echo 'MAN3="$(MAN3)"' 1>&2
#	@echo 'prefix="$(prefix)"' 1>&2

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
#-include Makefile.a

$(BUILDDIR)/%.pic.o: CFLAGS += -fPIC


#$(PROGRAM_OBJECTS): CFLAGS += -Ilib
#$(PROGRAM_OBJECTS): CPPFLAGS += -Ilib
$(PROGRAMS):  #CPPFLAGS += -I.
#$(PROGRAMS): CPPFLAGS += -Ilib

#$(info PROGRAMS=$(subst $(SPACE),$(NL)    ,$(shell ls -td -- $(PROGRAMS))))



-include $(BUILDDIR)defines.make
-include Makefile.tests

