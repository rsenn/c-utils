#include <sys/types.h>

#if defined(_WIN32) || defined(_WIN32) || defined(__MINGW64__) || defined(_WIN64)
#include <windows.h>
#else
#include <sys/mman.h>
#include <unistd.h>
#endif
#include "../mmap.h"
#include "../open.h"

int
mmap_unmap(void* mapped, size_t maplen) {
#if defined(_WIN32) || defined(_WIN32) || defined(__MINGW64__) || defined(_WIN64)
  (void)maplen;
  return UnmapViewOfFile(mapped) ? 0 : -1;
#else
  return munmap(mapped, maplen);
#endif
}
