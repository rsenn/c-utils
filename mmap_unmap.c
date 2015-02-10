#include <sys/types.h>
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif // defined HAVE_UNISTD_H
#if defined(__MINGW32__) || defined(_WIN32) || defined(__MINGW64__) || defined(_WIN64)
#include <windows.h>
#else
#ifdef HAVE_SYS_MMAN_H
#include <sys/mman.h>
#endif // defined HAVE_SYS_MMAN_H
#endif
#include "open.h"
#include "mmap.h"

int mmap_unmap(char* mapped,size_t maplen) {
#if defined(__MINGW32__) || defined(_WIN32) || defined(__MINGW64__) || defined(_WIN64)
  (void)maplen;
  return UnmapViewOfFile(mapped)?0:-1;
#else
  return munmap(mapped,maplen);
#endif
}
