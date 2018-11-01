CC = gcc
CXX = g++
CFLAGS = -O2
DEFS = -DHAVE_ERRNO_H=1 -DHAVE_ZLIB=1 -DHAVE_LIBBZ2=1 -DHAVE_LIBLZMA=1
AR = ar
LIBS = -lz -lbz2 -llzma
EXTRA_LIBS = -ladvapi32 -lws2_32 -liphlpapi -lpsapi -lshlwapi

all: all_tests

%.o: %.c
	$(CC) $(CFLAGS) $(CPPFLAGS) $(DEFS) -c $<

all_tests: $(wildcard ../lib/byte/*.c)
all_tests: $(wildcard ../lib/str/*.c)
all_tests: $(wildcard ../lib/case/*.c)
all_tests: $(wildcard ../lib/fmt/*.c)
all_tests: $(wildcard ../lib/scan/*.c)
all_tests: $(wildcard ../lib/open/*.c)
all_tests: $(wildcard ../lib/mmap/*.c)
all_tests: $(wildcard ../lib/uint16/*.c)
all_tests: $(wildcard ../lib/uint32/*.c)
all_tests: $(wildcard ../lib/uint64/*.c)
all_tests: $(wildcard ../lib/tai/*.c)
all_tests: $(wildcard ../lib/taia/*.c)
all_tests: $(wildcard ../lib/array/*.c)
all_tests: $(wildcard ../lib/stralloc/*.c)
all_tests: $(wildcard ../lib/strlist/*.c)
all_tests: $(wildcard ../lib/buffer/*.c)
all_tests: $(wildcard ../lib/unix/*.c)
all_tests: $(wildcard ../lib/path/*.c)

all_tests: unit_test.o unit_test_runner.o unit_test.o test_strlist.o test_path.o 
	$(CC) -o $@ $^

.PHONY: clean
clean:
	$(RM) *.o
	$(RM) *.a

