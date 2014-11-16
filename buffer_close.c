#include <buffer.h>
#include <shell.h>
#ifndef WIN32
#include <unistd.h>
#endif
#if !(defined(WIN32) || defined(__MINGW32__) ||defined(__MSYS__))
#include <sys/mman.h>
#else
#include <windows.h>
#endif

void buffer_close(buffer* b) {
  if (b->fd != -1) close(b->fd);
  switch (b->todo) {
  case FREE: shell_free(b->x); break;
  case MUNMAP:
#if defined(WIN32) || defined(__MINGW32__) || defined(__MSYS__)
    UnmapViewOfFile(b->x);
#else
    munmap(b->x,b->a); break;
#endif
  default: ;
  }
}
