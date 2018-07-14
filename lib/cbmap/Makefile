CC=clang
SOURCES=cbmap.c alloc.c
HEADERS=cbmap.h alloc.h
OBJECTS=$(SOURCES:.c=.o)

main: $(OBJECTS) main.o
	$(CC) -o $@ $(OBJECTS) main.o

run: main
	time ./main

clean:
	rm -rf $(OBJECTS) main

.c.o: $(HEADERS) Makefile
	$(CC) -g -O2 -std=c99 -Wall -c -o $@ $< $(CFLAGS)
