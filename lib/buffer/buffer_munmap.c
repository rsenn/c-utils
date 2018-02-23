#include "buffer.h"
#ifdef _WIN32
#include <windows.h>
#else
#include <sys/mman.h>
#endif

void 
buffer_munmap(void* buf) {
  buffer* b=(buffer*)buf;
#ifdef _WIN32
  UnmapViewOfFile(b->x);
#else
  munmap(b->x,b->a);
#endif
}
