CC = gcc
CXX = g++
CFLAGS = -O2
DEFS = -DHAVE_ERRNO_H=1 -DHAVE_ZLIB=1 -DHAVE_LIBBZ2=1 -DHAVE_LIBLZMA=1
AR = ar
LIBS = -lz -lbz2 -llzma
EXTRA_LIBS = -ladvapi32 -lws2_32 -liphlpapi -lpsapi -lshlwapi

all: build\gcc\Debug
build\gcc\Debug:
	IF NOT EXIST "build\gcc\Debug" MKDIR "build\gcc\Debug"

build\gcc\Debug\test_path.o: ..\C:\Users\roman\Desktop\dirlist\tests\test_path.c
	$(CC) $(CFLAGS) $(CPPFLAGS) $(DEFS) -c -o build\gcc\Debug\test_path.o ..\C:\Users\roman\Desktop\dirlist\tests\test_path.c

build\gcc\Debug\test_strlist.o: ..\C:\Users\roman\Desktop\dirlist\tests\test_strlist.c
	$(CC) $(CFLAGS) $(CPPFLAGS) $(DEFS) -c -o build\gcc\Debug\test_strlist.o ..\C:\Users\roman\Desktop\dirlist\tests\test_strlist.c

build\gcc\Debug\unit_test.o: ..\C:\Users\roman\Desktop\dirlist\tests\unit_test.c ..\C:\Users\roman\Desktop\dirlist\tests\unit_test.c
	$(CC) $(CFLAGS) $(CPPFLAGS) $(DEFS) -c -o build\gcc\Debug\unit_test.o ..\C:\Users\roman\Desktop\dirlist\tests\unit_test.c

build\gcc\Debug\unit_test_runner.o: ..\C:\Users\roman\Desktop\dirlist\tests\unit_test_runner.c
	$(CC) $(CFLAGS) $(CPPFLAGS) $(DEFS) -c -o build\gcc\Debug\unit_test_runner.o ..\C:\Users\roman\Desktop\dirlist\tests\unit_test_runner.c

build\gcc\Debug\tests.a: build\gcc\Debug\unit_test.o build\gcc\Debug\unit_test_runner.o build\gcc\Debug\unit_test.o build\gcc\Debug\test_strlist.o build\gcc\Debug\test_path.o
	$(AR) rcs build\gcc\Debug\tests.a build\gcc\Debug\unit_test.o build\gcc\Debug\unit_test_runner.o build\gcc\Debug\unit_test.o build\gcc\Debug\test_strlist.o build\gcc\Debug\test_path.o

.PHONY: clean
clean:
	DEL /F /Q build\gcc\Debug\*.o
	DEL /F /Q build\gcc\Debug\*.a

