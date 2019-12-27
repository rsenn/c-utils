GENERATED = lex.yy.c lex.yy.h y.tab.c y.tab.h

LIBS = -L../build/x86_64-linux-gnu/Debug 

VPATH = ../lib/str:../lib/byte:../lib/buffer
vpath ../lib/str ../lib/byte ../lib/buffer

LIB_SOURCES = buffer_0.c buffer_getc.c buffer_1.c buffer_flush.c buffer_feed.c buffer_stubborn.c buffer_stubborn2.c
LIB_OBJECTS = $(LIB_SOURCES:%.c=%.o)

all: $(GENERATED) cparser

clean:
	$(RM) $(GENERATED)

cparser: cparser.o lex.yy.o y.tab.o $(LIB_OBJECTS)
	$(CC) -o $@ $^ $(LIBS)

lex.yy.c lex.yy.h: ansi-c.l
	flex -l --header-file=lex.yy.h $<
y.tab.c y.tab.h: ansi-c.y
	bison -y --defines $<
