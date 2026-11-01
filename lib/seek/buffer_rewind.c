#include "../buffer.h"
#include "../seek.h"

ssize_t buffer_dummyreadbuf(fd_type, void*, size_t, buffer*);
ssize_t buffer_dummyreadmmap(fd_type, void*, size_t, buffer*);

/* back to the start of the input; a buffer that holds its whole input in memory (buffer_frombuf,
 * the mmap readers) keeps it, there is nothing to read again */
void
buffer_rewind(buffer* b) {
  if(b->op == (buffer_op_proto*)(void*)&buffer_dummyreadbuf || b->op == (buffer_op_proto*)(void*)&buffer_dummyreadmmap) {
    b->p = 0;
    return;
  }
  if(b->fd >= 0) {
    seek_set(b->fd, 0);
  }
  b->p = b->n = 0;
}
