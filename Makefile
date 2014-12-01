prefix = /usr/local
bindir = ${prefix}/bin

DEBUG = 1

INSTALL = install

CC ?= cc

BUILD = $(shell $(CC) -dumpmachine)
HOST ?= $(BUILD)

ifneq ($(HOST),)
TRIPLET = $(subst -, ,$(HOST))
MACHINE := $(word 1,$(TRIPLET))
OS := $(word 3,$(TRIPLET))
ifneq ($(HOST),$(BUILD))
CROSS = $(HOST)-
BUILDDIR = build/$(HOST)/
endif

endif

ifeq ($(MACHINE),x86_64)
EXESUFFIX = 64
endif


$(info Machine: $(MACHINE))
$(info OS: $(OS))

ifeq ($(OS),mingw32)
EXEEXT = .exe
LDFLAGS = -s -static
endif

ifeq ($(OS),cygwin)
EXEEXT = .exe
LDFLAGS = -s -static
endif


CC = $(CROSS)gcc
CXX = $(CROSS)g++


#CPPFLAGS = -I/usr/include/libowfat 
#CPPFLAGS = -I. -D__USE_BSD=1
CPPFLAGS = -I.  -DPATH_MAX=4096 -D_LARGEFILE_SOURCE=1 -D_GNU_SOURCE=1 -D_FILE_OFFSET_BITS=64
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


LIB_SRCS = $(wildcard *_*.c)
LIB_OBJ = $(patsubst %.o,$(BUILDDIR)%.o,$(LIB_SRCS:%.c=%.o))

LIBS += -lstdc++

PROGRAMS = $(BUILDDIR)list-r$(EXESUFFIX)$(EXEEXT) $(BUILDDIR)count-depth$(EXESUFFIX)$(EXEEXT) $(BUILDDIR)decode-ls-lR$(EXESUFFIX)$(EXEEXT) $(BUILDDIR)reg2cmd$(EXESUFFIX)$(EXEEXT) $(BUILDDIR)torrent-progress$(EXESUFFIX)$(EXEEXT)
OBJECTS = $(PROGRAMS:%=%.o) $(LIB_OBJ)

vpath $(BUILDDIR)

VPATH = $(BUILDDIR):.

all: $(BUILDDIR) $(PROGRAMS) 

$(BUILDDIR):
	mkdir -p $(BUILDDIR)

$(BUILDDIR)decode-ls-lR.o: decode-ls-lR.c
$(BUILDDIR)decode-ls-lR$(EXESUFFIX)$(EXEEXT): $(BUILDDIR)decode-ls-lR.o $(LIB_OBJ)
	$(CC) $(LDFLAGS) $(CFLAGS) -o $@ $(BUILDDIR)decode-ls-lR.o $(LIB_OBJ) $(LIBS)

$(BUILDDIR)reg2cmd.o: reg2cmd.c
$(BUILDDIR)reg2cmd$(EXESUFFIX)$(EXEEXT): $(BUILDDIR)reg2cmd.o $(LIB_OBJ)
	$(CC) $(LDFLAGS) $(CFLAGS) -o $@ $(BUILDDIR)reg2cmd.o $(LIB_OBJ) $(LIBS)

$(BUILDDIR)count-depth.o: count-depth.c
$(BUILDDIR)count-depth$(EXESUFFIX)$(EXEEXT): $(BUILDDIR)count-depth.o $(LIB_OBJ)
	$(CC) $(LDFLAGS) $(CFLAGS) -o $@ $(BUILDDIR)count-depth.o $(LIB_OBJ) $(LIBS)

$(BUILDDIR)list-r.o: list-r.c
$(BUILDDIR)list-r$(EXESUFFIX)$(EXEEXT): $(BUILDDIR)list-r.o $(LIB_OBJ)
	$(CXX) $(LDFLAGS) $(CFLAGS) -o $@ $(BUILDDIR)list-r.o $(LIB_OBJ) $(LIBS)

$(BUILDDIR)torrent-progress.o: torrent-progress.c
$(BUILDDIR)torrent-progress$(EXESUFFIX)$(EXEEXT): $(BUILDDIR)torrent-progress.o $(LIB_OBJ)
	$(CC) $(LDFLAGS) $(CFLAGS) -o $@ $(BUILDDIR)torrent-progress.o $(LIB_OBJ) $(LIBS)

.c.o:
	$(CC) $(CPPFLAGS) $(CFLAGS) -c -o $@ $<

%.o: %.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -c -o $@ $<

$(BUILDDIR)%.o: %.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -c -o $@ $<

.cpp.o:
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c -o $(BUILDIR)$(patsubst %.cpp,%.o,$<) $<

%.o: %.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c -o $(BUILDIR)$(patsubst %.cpp,%.o,$<) $<

$(BUILDDIR)%.o: %.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c -o $(BUILDIR)$(patsubst %.cpp,%.o,$<) $<

clean:
	$(RM) -f $(OBJECTS) list-r.o list-r$(EXESUFFIX)$(EXEEXT)

install: $(PROGRAMS)
	$(INSTALL) -d $(DESTDIR)$(bindir)
	$(INSTALL) -m 755 $(PROGRAMS) $(DESTDIR)$(bindir)
	$(INSTALL) -d $(DESTDIR)$(bindir)

uninstall: 
	@for PROGRAM in $(PROGRAMS); do \
		echo $(RM) $(DESTDIR)$(bindir)/$$PROGRAM; \
		$(RM) $(DESTDIR)$(bindir)/$$PROGRAM; \
  done

