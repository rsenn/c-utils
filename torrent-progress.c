#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#define _LARGEFILE_SOURCE 1
#define _GNU_SOURCE 1
#define _FILE_OFFSET_BITS 64

#define _LARGEFILE_SOURCE 1
#define _GNU_SOURCE 1

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#include <io.h>
#else
#include <unistd.h>
#include <string.h>
#include <errno.h>
#endif

#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

#if !(defined(_WIN32) || defined(_WIN64))
#include <sys/mman.h>
#endif

#ifndef PRIu64
#define PRIu64 "lu"
#endif
#ifndef PRIx64
#define PRIx64 "lx"
#endif

#include "buffer.h"
#include "open.h"
#include "mmap.h"
#include "uint64.h"

#if defined(__x86_64__) && defined(__linux)
#define lseek lseek64
#endif

#define BLOCK_SIZE 1024 //262144
#if defined(__APPLE__) || defined(__MSYS__)
typedef off_t offset_type;
#elif defined(__CYGWIN__)
typedef loff_t offset_type;
#elif defined(_MSC_VER)
typedef LONG64 offset_type;
#else
typedef off64_t offset_type;
#endif

#if __MINGW64__
#define FMT_OFFS_T "0x%"SCNx64
#define FMT_SIZE_T "%"SCNu64
#elif defined(WIN32)
#ifndef SCNx32
#define SCNx32 "%08x"
#endif
#ifndef SCNu32
#define SCNu32 "%u"
#endif
#define FMT_OFFS_T "0x%"SCNx32
#define FMT_SIZE_T "%"SCNu32
#endif

#ifndef FMT_OFFS_T
#define FMT_OFFS_T "0x%zx"
#endif

#ifndef FMT_SIZE_T
#define FMT_SIZE_T "%zu"
#endif

#ifdef __dietlibc__
#undef lseek
#undef mmap
#endif

int64 filesize(int fd) {
  int64 sz;
#if defined(_WIN32) || defined(_WIN64)
  DWORD fszH;
  sz = GetFileSize((HANDLE)(size_t)fd, &fszH);
  sz |= ((int64)fszH) << 32;
#else
  uint64 pos, end;
  //if(_llseek(fd, 0, 0, &pos, SEEK_CUR) < 0)  return -1;
  if((pos = lseek(fd, 0, SEEK_CUR)) == (off_t) - 1) return -1;
  //if(_llseek(fd, 0, 0, &end, SEEK_END) < 0) return -1;
  if((end = lseek(fd, 0, SEEK_END)) == (off_t) - 1) return -1;

  sz = end;
  lseek(fd, pos, SEEK_SET);
  //_llseek(fd, pos >> 32, pos & 0xffffffff,  &pos, SEEK_SET);
#endif
  return sz;
}

static const char*
last_error_str () {
#if defined(_WIN32) || defined(_WIN64)
  DWORD errCode = GetLastError();
  static char buffer[1024];
  char *err;
  buffer[0] = '\0';
  if(errCode == 0) return buffer;

   SetLastError(0);
  if(!FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
					 NULL,
					 errCode,
					 MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // default language
					 (LPTSTR) &err,
					 0,
					 NULL))
	  return 0;

  snprintf(buffer, sizeof(buffer), "ERROR: %s\n", err);

  //OutputDebugString(buffer); // or otherwise log it
  LocalFree(err);
  return buffer;
#else
	return strerror(errno);
#endif
}

static buffer infile = BUFFER_INIT((void*)read, -1, 0, 0);

int check_block_zero(char* b, size_t n) {
  size_t i;
  for(i = 0; i < n; i++)
  {
    if(b[i] != '\0')
      return 0;
  }
  return 1;
}

ssize_t get_block(char* b)
{
  return buffer_get(&infile, b, BLOCK_SIZE);
}

static int verbose = 0, fraction = 1, space = 1;

int main(int argc, char* argv[]) {
  int ai;

  for(ai = 1; ai < argc; ++ai) {
    char* av = argv[ai];

    if(av[0] != '-')
      break;

    switch(av[1]) {
    case 'F': fraction = 0; break;
    case 'v': verbose++; break;
    case 'S': space = 0; break;
    case '-': ++ai; goto next;
    default:
      goto next;
    }
  }
next:
  for(; ai < argc; ++ai) {
    uint64 all_blocks = 0, zero_blocks = 0, nonzero_blocks;
    unsigned int percent;
    unsigned int bi;
    int fd;
    uint64 fsize, i;
    uint64 iterations, remain;
    int map_blocks = 1024;
    int map_size = (BLOCK_SIZE * map_blocks);

    fd = open_read(argv[ai]);
      fprintf(stderr, "open_read(%s) = %d\n", argv[ai], fd);
    fsize = filesize(fd);

    iterations = (((fsize + map_size + 1) / map_size) + 7) >> 3;
    remain = fsize;

    if(verbose)
        fprintf(stderr, "memory map size: %lukB (0x%016lx) iterations: %"PRIu64" (end offset: 0x%"PRIx64")\n",
                (long)(map_size / 1024), (long)map_size, iterations, fsize);

    //(uint64)map_size * iterations);
    //mmap_private(argv[ai], &fsize);

    /*struct stat st;
    infile.fd = open_read(argv[1]);
       buffer_puts(buffer_1, "fd #");
       buffer_putulong(buffer_1, infile.fd);;
       buffer_putnlflush(buffer_1);
            fstat(infile.fd, &st);
    fsize = st.st_size;*/

    //buffer_puts(buffer_1, "fsize #"); buffer_putulong(buffer_1, fsize);; buffer_puts(buffer_1, ", blocks #"); buffer_putulong(buffer_1, blocks); buffer_putnlflush(buffer_1);

    for(i = 0; i < iterations; i++) {
      size_t msz =  (remain >= map_size ? map_size : (size_t)remain);
      uint64 mofs =  (uint64)map_size * i;
	  size_t z, blocks;

      char* m = mmap_map(fd, msz, mofs);

	  if(m == NULL) {
		  fprintf(stderr, "mmap_map(%d, "FMT_SIZE_T", "FMT_OFFS_T") failed: %s\n", fd, (size_t)msz, (size_t)mofs, last_error_str());
		  exit(2);
	  }

      z = 0;
	  blocks = msz / BLOCK_SIZE;

      all_blocks += blocks;

      //int remain = msz - (blocks * BLOCK_SIZE);
      for(bi = 0; bi < blocks; bi++)
      {
        //get_block(m);

        z += check_block_zero(&m[bi*BLOCK_SIZE], BLOCK_SIZE);
        //fprintf(stderr, "block #%lu\n", bi); fflush(stderr);
      }

      if(verbose)
        fprintf(stderr, "mmap at "FMT_OFFS_T", size "FMT_SIZE_T"%s\n", (size_t)mofs, (size_t)msz, (z < blocks ? "" : " zero"));

      zero_blocks += z;

      mmap_unmap(m, msz);

      if(remain >= map_size)
        remain -= map_size;
    }
    //   buffer_putulong(buffer_1, blocks);
    //   buffer_putnlflush(buffer_1);
    nonzero_blocks = all_blocks - zero_blocks;
    percent = (unsigned int)((float)nonzero_blocks * 10000 / all_blocks);

    buffer_puts(buffer_1, argv[ai]);
    buffer_puts(buffer_1, (space ? ": " : ":"));

    if(!fraction) percent += 50;

    buffer_putulong(buffer_1, percent / 100);

    if(fraction) {
      buffer_puts(buffer_1, ".");
      buffer_putulong(buffer_1, percent % 100);
    }
    buffer_putnlflush(buffer_1);
  }
  return 0;
}
