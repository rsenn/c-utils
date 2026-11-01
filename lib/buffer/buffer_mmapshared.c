#include "../buffer.h"
#include "../open.h"
#include "../windoze.h"

#if WINDOWS_NATIVE
#include <io.h>
#else
#include <unistd.h>
#endif

/* maps filename read/write and shared into the buffer; 0 on success, -1 on
 * failure with an empty buffer */
int
buffer_mmapshared(buffer* b, const char* filename) {
  int fd = open_rw(filename);
  int ret;

  if(fd == -1) {
    b->x = NULL;
    b->p = b->n = b->a = 0;
    b->fd = -1;
    b->deinit = 0;
    b->cookie = 0;
    return -1;
  }
  ret = buffer_mmapshared_fd(b, fd);
  close(fd); /* buffer_mmapshared_fd keeps its own duplicate */
  return ret;
}
