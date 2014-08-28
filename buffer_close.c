#include <buffer.h>
#include <shell.h>
#include <unistd.h>
#if !(defined(__MINGW32__) ||defined(__MSYS__))
#include <sys/mman.h>
#else
#include <windows.h>
#endif

void buffer_close(buffer* b) {
  if (b->fd != -1) close(b->fd);
  switch (b->todo) {
  case FREE: shell_free(b->x); break;
  case MUNMAP:
#if defined(__MINGW32__) || defined(__MSYS__)
    UnmapViewOfFile(b->x);
#else
    munmap(b->x,b->a); break;
#endif
  default: ;
  }
}
