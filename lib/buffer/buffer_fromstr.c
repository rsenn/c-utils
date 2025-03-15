#include "../buffer.h"
#include "../str.h"

ssize_t
buffer_dummyread_fromstr() {
  return 0;
}

void
buffer_fromstr(buffer* b, char* s, size_t n) {
  b->x = s;
  b->p = 0;
  b->n = n;
  b->a = b->n + 1;
  b->fd = -1;
  b->op = (buffer_op_proto*)&buffer_dummyread_fromstr;
}
