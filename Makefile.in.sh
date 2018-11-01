makefile_in() {

	PROGRAMS=$( echo $(grep -l 'main(' *.c | sed 's|\.c$||; /test_/d; s,$,$(EXEEXT),' ))
	LIB_SOURCES=$(find lib -name "*.c")
	LIB_OBJECTS=$(echo "$LIB_SOURCES" | sed 's|\.c|.o|; s|.*/||')
	cat <<EOF
srcdir = @srcdir@
builddir = @builddir@
top_srcdir = @top_srcdir@
top_builddir = @top_builddir@

bindir = @bindir@
datadir = @datadir@
datarootdir = @datarootdir@
docdir = @docdir@
dvidir = @dvidir@
htmldir = @htmldir@
includedir = @includedir@
infodir = @infodir@
libdir = @libdir@
libexecdir = @libexecdir@
localedir = @localedir@
localstatedir = @localstatedir@
mandir = @mandir@
oldincludedir = @oldincludedir@
pdfdir = @pdfdir@
psdir = @psdir@
sbindir = @sbindir@
sharedstatedir = @sharedstatedir@
sysconfdir = @sysconfdir@

CC = @CC@
CPP = @CPP@
AR = @AR@
RANLIB = @RANLIB@

CFLAGS = @CFLAGS@
CPPFLAGS = @CPPFLAGS@
DEFS = @DEFS@

ECHO_C = @ECHO_C@
ECHO_N = @ECHO_N@
ECHO_T = @ECHO_T@

EXEEXT = @EXEEXT@

INSTALL = @INSTALL@
INSTALL_DATA = @INSTALL_DATA@
INSTALL_PROGRAM = @INSTALL_PROGRAM@
INSTALL_SCRIPT = @INSTALL_SCRIPT@
LDFLAGS = @LDFLAGS@
LIBOBJS = @LIBOBJS@
LIBS = @LIBS@
OBJEXT = @OBJEXT@

PACKAGE_NAME = @PACKAGE_NAME@
PACKAGE_URL = @PACKAGE_URL@
PACKAGE_VERSION = @PACKAGE_VERSION@

ZLIB_CFLAGS = @ZLIB_CFLAGS@
ZLIB_LIBS = @ZLIB_LIBS@
ZLIB_LIB_DIR = @ZLIB_LIB_DIR@
BZLIB_CFLAGS = @BZLIB_CFLAGS@
BZLIB_LIBS = @BZLIB_LIBS@
LIBLZMA_CFLAGS = @LIBLZMA_CFLAGS@
LIBLZMA_LIBS = @LIBLZMA_LIBS@

build = @build@
exec_prefix = @exec_prefix@
host = @host@
prefix = @prefix@
program_transform_name = @program_transform_name@
target_alias = @target_alias@

vpath @srcdir@ @srcdir@/lib @srcdir@/lib/array @srcdir@/lib/buffer @srcdir@/lib/byte @srcdir@/lib/case @srcdir@/lib/cb @srcdir@/lib/cbmap @srcdir@/lib/charbuf @srcdir@/lib/dir @srcdir@/lib/dns @srcdir@/lib/env @srcdir@/lib/errmsg @srcdir@/lib/expand @srcdir@/lib/fmt @srcdir@/lib/gpio @srcdir@/lib/hmap @srcdir@/lib/http @srcdir@/lib/iarray @srcdir@/lib/io @srcdir@/lib/json @srcdir@/lib/list @srcdir@/lib/map @srcdir@/lib/mmap @srcdir@/lib/ndelay @srcdir@/lib/open @srcdir@/lib/path @srcdir@/lib/pe @srcdir@/lib/playlist @srcdir@/lib/rdir @srcdir@/lib/scan @srcdir@/lib/sig @srcdir@/lib/slist @srcdir@/lib/socket @srcdir@/lib/str @srcdir@/lib/stralloc @srcdir@/lib/strarray @srcdir@/lib/strlist @srcdir@/lib/tai @srcdir@/lib/taia @srcdir@/lib/textbuf @srcdir@/lib/uint16 @srcdir@/lib/uint32 @srcdir@/lib/uint64 @srcdir@/lib/var @srcdir@/lib/vartab @srcdir@/lib/xml
VPATH = @srcdir@:@srcdir@/lib:@srcdir@/lib/array:@srcdir@/lib/buffer:@srcdir@/lib/byte:@srcdir@/lib/case:@srcdir@/lib/cb:@srcdir@/lib/cbmap:@srcdir@/lib/charbuf:@srcdir@/lib/dir:@srcdir@/lib/dns:@srcdir@/lib/env:@srcdir@/lib/errmsg:@srcdir@/lib/expand:@srcdir@/lib/fmt:@srcdir@/lib/gpio:@srcdir@/lib/hmap:@srcdir@/lib/http:@srcdir@/lib/iarray:@srcdir@/lib/io:@srcdir@/lib/json:@srcdir@/lib/list:@srcdir@/lib/map:@srcdir@/lib/mmap:@srcdir@/lib/ndelay:@srcdir@/lib/open:@srcdir@/lib/path:@srcdir@/lib/pe:@srcdir@/lib/playlist:@srcdir@/lib/rdir:@srcdir@/lib/scan:@srcdir@/lib/sig:@srcdir@/lib/slist:@srcdir@/lib/socket:@srcdir@/lib/str:@srcdir@/lib/stralloc:@srcdir@/lib/strarray:@srcdir@/lib/strlist:@srcdir@/lib/tai:@srcdir@/lib/taia:@srcdir@/lib/textbuf:@srcdir@/lib/uint16:@srcdir@/lib/uint32:@srcdir@/lib/uint64:@srcdir@/lib/var:@srcdir@/lib/vartab:@srcdir@/lib/xml

PROGRAMS = $PROGRAMS
PROGRAM_OBJECTS = \$(patsubst %\$(EXEEXT),%.o,\$(PROGRAMS))

LIB_SOURCES = $(echo $LIB_SOURCES)
LIB_OBJECTS = $(echo $LIB_OBJECTS)

INCLUDES = -I\$(top_srcdir) -I\$(top_builddir)

all: libowcarb.a \$(PROGRAMS)

%.o: %.c
	\$(CC) \$(CFLAGS) \$(CPPFLAGS) -c \$<

libowcarb.a: \$(LIB_OBJECTS)
	\$(AR) rc \$@ \$^
	\$(RANLIB) \$@

\$(PROGRAMS): %\$(EXEEXT): %.o
	\$(CC) \$(LDFLAGS) \$(CFLAGS) -o \$@ \$^ \$(LIBS) \$(EXTRA_LIBS)
	

\$(PROGRAMS): LIBS += -L\$(builddir) -lowcarb


\$(PROGRAMS): CFLAGS += \$(ZLIB_CFLAGS) \$(BZLIB_CFLAGS) \$(LIBLZMA_CFLAGS)
\$(PROGRAMS): LIBS += \$(ZLIB_LIBS) \$(BZLIB_LIBS) \$(LIBLZMA_LIBS)


ntldd\$(EXEEXT): libntldd.o

.PHONY: clean
clean: clean-library clean-programs

.PHONY: clean-library
clean-library:
	\$(RM) \$(LIB_OBJECTS) libowcarb.a

.PHONY: clean-programs
clean-programs:
	\$(RM) \$(PROGRAM_OBJECTS) \$(PROGRAMS) 

.PHONY: install
install: install-programs

.PHONY: install-programs
install-programs:
	\$(INSTALL_PROGRAM) -d \$(DESTDIR)\$(bindir)
	\$(INSTALL_PROGRAM) -m 755 \$(PROGRAMS) \$(DESTDIR)\$(bindir)

EOF
}

makefile_in
