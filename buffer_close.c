#include <buffer.h>
#include <stdlib.h>
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

void buffer_close(buffer* b) {
  if (b->fd != -1) close(b->fd);
  switch (b->todo) {
  case FREE: free(b->x); break;
  case MUNMAP:
#if defined(__MINGW32__) || defined(_WIN32) || defined(__MINGW64__) || defined(_WIN64)
    UnmapViewOfFile(b->x);
#else
    munmap(b->x,b->a); break;
#endif
  default: ;
  }
}
