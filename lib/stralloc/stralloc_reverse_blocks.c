/* ISC license. */

#include <string.h>
#include "../stralloc.h"

void stralloc_reverse_blocks(stralloc *sa, size_t size) {
  size_t n = sa->len / (size << 1) ;
  size_t i = 0 ;
  char* tmp = alloca(size);
  for(; i < n ; i++) {
    byte_copy(tmp, size, sa->s + i * size) ;
    byte_copy(sa->s + i * size, size, sa->s + (2 * n - 1 - i) * size) ;
    byte_copy(sa->s + (2 * n - 1 - i) * size, size, tmp) ;
  }
}
