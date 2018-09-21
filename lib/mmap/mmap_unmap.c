#include "../windoze.h"

#include "../io.h"
#include "../mmap.h"
#include "../open.h"
#if WINDOWS_NATIVE
#include <windows.h>
#else
#include <sys/mman.h>
#endif

int
mmap_unmap(void* mapped, size_t maplen) {
#if WINDOWS_NATIVE
  (void)maplen;
  return UnmapViewOfFile(mapped) ? 0 : -1;
#else
  return munmap(mapped, maplen);
#endif
}
