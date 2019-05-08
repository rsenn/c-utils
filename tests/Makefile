CC = gcc
CXX = g++
CFLAGS = -g3 -ggdb -O0
DEFS = -DHAVE_ERRNO_H=1 -DHAVE_LIBZ=1 -DHAVE_LIBBZ2=1 -DHAVE_LIBLZMA=1
AR = ar
LIBS = -lz -lbz2 -llzma
EXTRA_LIBS = -ladvapi32 -lws2_32 -liphlpapi -lpsapi -lshlwapi
DEPS_RSP = deps.rsp

all: all_tests

%.o: %.c
	$(CC) $(CFLAGS) $(CPPFLAGS) $(DEFS) -c $<

DEPS := $(wildcard ../lib/byte/*.c)
DEPS += $(wildcard ../lib/str/*.c)
DEPS += $(wildcard ../lib/case/*.c)
DEPS += $(wildcard ../lib/dir/*.c)
DEPS += $(wildcard ../lib/errmsg/*.c)
DEPS += $(wildcard ../lib/fmt/*.c)
DEPS += $(wildcard ../lib/scan/*.c)
DEPS += $(wildcard ../lib/open/*.c)
DEPS += $(wildcard ../lib/mmap/*.c)
DEPS += $(wildcard ../lib/uint16/*.c)
DEPS += $(wildcard ../lib/uint32/*.c)
DEPS += $(wildcard ../lib/uint64/*.c)
DEPS += $(wildcard ../lib/tai/*.c)
DEPS += $(wildcard ../lib/taia/*.c)
DEPS += $(wildcard ../lib/array/*.c)
DEPS += $(wildcard ../lib/stralloc/*.c)
DEPS += $(wildcard ../lib/strlist/*.c)
DEPS += $(wildcard ../lib/buffer/*.c)
DEPS += $(wildcard ../lib/unix/*.c)
DEPS += $(wildcard ../lib/path/*.c)

$(DEPS_RSP):
	$(file >$(DEPS_RSP),$(DEPS))

all_tests: all_tests.o unit_test.o unit_test_runner.o unit_test.o test_buffer.o test_stralloc.o test_strlist.o test_path.o test_tuple.o | $(DEPS_RSP)
	$(CC) $(CFLAGS) -o $@ $^ @$(DEPS_RSP)

.PHONY: clean
clean:
	$(RM) *.o
	$(RM) *.a
	$(RM) $(DEPS_RSP)

