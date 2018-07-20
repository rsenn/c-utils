#include "../buffer.h"

int
buffer_skipc(buffer* b) {
  if(b->p < b->n) {
    ++b->p;
    return 1;
  }
  return 0;
}
