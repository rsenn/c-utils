#include "../buffer.h"
#if WINDOWS
#include <windows.h>
#else
#include <sys/mman.h>
#endif

void
buffer_munmap(void* buf) {
  buffer* b = (buffer*)buf;
#if WINDOWS
  UnmapViewOfFile(b->x);
#else
  munmap(b->x, b->a);
#endif
  b->x = NULL;
  b->a = 0;
}
