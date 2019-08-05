#include "../buffer.h"
#include "../stralloc.h"

int
buffer_getnewline_sa(buffer* b, stralloc* sa) {
  stralloc_zero(sa);
  return buffer_getline_sa(b, sa);
}

