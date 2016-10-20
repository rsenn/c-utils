#include "buffer.h"
#include <stdlib.h>
#if defined(__MINGW32__) || defined(_WIN32) || defined(__MINGW64__) || defined(_WIN64)
#include <windows.h>
#else
#include <unistd.h>
#include <sys/mman.h>
#endif

void buffer_close(buffer* b) {
  if(b->fd != -1) close(b->fd);
  switch(b->todo) {
  case FREE: free(b->x); break;
  case MUNMAP:
#if defined(__MINGW32__) || defined(_WIN32) || defined(__MINGW64__) || defined(_WIN64)
    UnmapViewOfFile(b->x);
#else
    munmap(b->x, b->a); break;
#endif
  default: ;
  }
}
