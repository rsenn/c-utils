#include "../buffer.h"
#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#else
#include <sys/mman.h>
#endif

void
buffer_munmap(void* buf) {
  buffer* b = (buffer*)buf;
#if defined(_WIN32) || defined(_WIN64)
  UnmapViewOfFile(b->x);
#else
  munmap(b->x, b->a);
#endif
}
