
INSTALL = install
CC = gcc
CXX = g++
#CPPFLAGS = -I/usr/include/libowfat
#CPPFLAGS = -I. -D__USE_BSD=1
CPPFLAGS = -I.  -DPATHLEN=4096 -DUSE_READDIR=1 -DINLINE=__inline__ -DUSE_READDIR=1
CFLAGS = -g -O2 -Wall
CXXFLAGS = $(CFLAGS)
#LIBS = -lowfat
EXEEXT =
HOST = $(shell $(CC) -dumpmachine |sed 's,.*-,,')
RM = rm -f

prefix := `$(CC)  -print-search-dirs|sed -n "s|^[^:]*: =\?\(/\?[^/]\+\)/.*|\1|p" | head -n1`
bindir = ${prefix}/bin

#EXTRA_DEP = $(EXTRA_MODULES:%=%.o)
EXTRA_MODULES =
SOURCES = strlist_append.c strlist_append_unique.c strlist_sort.c
LIB_SOURCES = strlist_append.c strlist_append_unique.c strlist_sort.c

#LIB_DEP = $(LIB_MODULES:%=%.o)
LIB_MODULES = $(LIB_SOURCES:%.c=%)

LIB_DEP = libowfat.a

PROGRAMS = list-r$(EXESUFFIX)$(EXEEXT) count-depth$(EXESUFFIX)$(EXEEXT) decode-ls-lR$(EXESUFFIX)$(EXEEXT) torrent-progress$(EXESUFFIX)$(EXEEXT)
all: $(LIB_DEP) $(PROGRAMS)

libowfat.a: $(LIB_MODULES:%=%.o)
	ar rcs $@ $^

decode-ls-lR.o: decode-ls-lR.c
decode-ls-lR$(EXESUFFIX)$(EXEEXT): decode-ls-lR.o $(EXTRA_DEP) $(LIB_DEP)
	$(CC) $(LDFLAGS) $(CFLAGS) -o $@ $^ $(LIBS) $(EXTRA_LIBS)

count-depth.o: count-depth.c
count-depth$(EXESUFFIX)$(EXEEXT): count-depth.o $(EXTRA_DEP) $(LIB_DEP)
	$(CC) $(LDFLAGS) $(CFLAGS) -o $@ $^ $(LIBS) $(EXTRA_LIBS)

list-r.o: list-r.c libowfat.a
list-r$(EXESUFFIX)$(EXEEXT): list-r.o $(EXTRA_DEP) $(LIB_DEP)
	$(CXX) $(LDFLAGS) $(CFLAGS) -o $@ $^ $(LIBS) $(EXTRA_LIBS)

#list-r$(EXESUFFIX)$(EXEEXT): LIB_MODULES += file directory_iterator

torrent-progress.o: torrent-progress.c
torrent-progress$(EXESUFFIX)$(EXEEXT): torrent-progress.o $(EXTRA_DEP) $(LIB_DEP)
	$(CC) $(LDFLAGS) $(CFLAGS) -o $@ $^ $(LIBS) $(EXTRA_LIBS)

.c.o:
	$(CC) $(DEFS:%=-D%) $(CPPFLAGS) $(CFLAGS) -c $<

.cpp.o:
	$(CXX) $(DEFS:%=-D%) $(CPPFLAGS) $(CXXFLAGS) -c $<

clean:
	$(RM) -f $(EXTRA_MODULES:%=%.o) $(LIB_MODULES:%=%.o) list-r.o list-r$(EXESUFFIX)$(EXEEXT)

install: $(PROGRAMS)
	$(INSTALL) -d $(DESTDIR)$(bindir)
	$(INSTALL) -m 755 $(PROGRAMS) $(DESTDIR)$(bindir)
	$(INSTALL) -d $(DESTDIR)$(bindir)

