#include "../array.h"

void*
array_find(array* a, uint64 membersize, void* data) {
  char *s = a->p;
  char *e = s + a->initialized;
  while(s < e) {
    if(byte_equal(s, membersize, data)) return s;
    s += membersize;
  }
  return 0;
}
