CC = gcc
CFLAGS = -g3 -ggdb $(if $(NOOPT),,-O0) -Wall
CPPFLAGS = -I$(BUILDDIR) -I.. -I. -I../lib -DYYDEBUG=1 -DYYERROR_VERBOSE=1

BUILD := $(shell $(CC) -dumpmachine)
#BUILDDIR := build/$(BUILD)/
BUILDDIR := 

VPATH = ../lib/alloc:../lib/scan:../lib/seek:../lib/str:../lib/byte:../lib/buffer:../..:../lib/mmap:../lib/open:../lib/fmt:../lib/errmsg:$(BUILDDIR)
vpath ../lib/alloc ../lib/scan ../lib/seek ../lib/str ../lib/byte ../lib/buffer ../lib/mmap ../lib/open ../.. ../lib/fmt ../lib/errmsg $(BUILDDIR)

GENERATED = lex.yy.c lex.yy.h ansi-c.tab.c ansi-c.tab.h

LIBS = -L../build/x86_64-linux-gnu/Debug 

LIB_SOURCES = buffer_0.c buffer_get.c buffer_1.c buffer_2.c buffer_puts.c buffer_putspad.c buffer_putnspace.c buffer_putc.c buffer_put.c buffer_putflush.c buffer_putsflush.c buffer_putnlflush.c buffer_putlong.c buffer_putulong.c buffer_flush.c buffer_feed.c buffer_stubborn.c buffer_stubborn2.c buffer_mmapread.c mmap_read.c buffer_dummyreadmmap.c buffer_munmap.c open_read.c errmsg_warnsys.c errmsg_write.c errmsg_puts.c str_len.c errmsg_iam.c byte_copy.c fmt_ulong.c fmt_long.c byte_fill.c alloc.c str_ansilen.c seek_cur.c seek_end.c scan_ansiskip.c
LIB_OBJECTS = $(LIB_SOURCES:%.c=$(BUILDDIR)%.o)

SOURCES = cparser.c lex.yy.c ansi-c.tab.c 
OBJECTS = $(SOURCES:%.c=$(BUILDDIR)%.o)

all: $(BUILDDIR) $(GENERATED) $(BUILDDIR)cparser
$(BUILDDIR):
	mkdir -p $(BUILDDIR)

clean:
	$(RM) $(GENERATED) $(OBJECTS) cparser

$(BUILDDIR)cparser: $(OBJECTS) $(LIB_OBJECTS)
	$(CC) -o $@ $^ $(LIBS)

$(BUILDDIR)%.o: %.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -c -o $@ $<

$(BUILDDIR)%.e: %.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -E $< | grep -vE '(^#|^\s*$$)' >$@

$(BUILDDIR)%.o: $(BUILDDIR)%.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -c -o $@ $<

$(BUILDDIR)lex.yy.c $(BUILDDIR)lex.yy.h: ansi-c.l
	($(if $(BUILDDIR),cd $(BUILDDIR) && ) flex -T -v --yylineno --bison-locations $(if $(BUILDDIR),../../)$<)

$(BUILDDIR)ansi-c.tab.c $(BUILDDIR)ansi-c.tab.h: ansi-c.y
	($(if $(BUILDDIR),cd $(BUILDDIR) && ) bison   --defines --debug --report=all   $(if $(BUILDDIR),../../)$<)
