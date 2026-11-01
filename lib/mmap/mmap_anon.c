#include "../windoze.h"
#include "../mmap.h"

#if WINDOWS_NATIVE
#include <windows.h>
#else
#include <sys/mman.h>
#endif

/* size bytes of zeroed, private, read/write memory not backed by a file;
 * NULL on failure. Release it with munmap (VirtualFree on Windows). */
char*
mmap_anon(size_t size) {
#if WINDOWS_NATIVE
  return (char*)VirtualAlloc(NULL, size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
#else
#ifndef MAP_ANONYMOUS
#define MAP_ANONYMOUS MAP_ANON
#endif
  void* p;

  if(size == 0)
    return NULL;
  p = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
  return p == MAP_FAILED ? NULL : (char*)p;
#endif
}
