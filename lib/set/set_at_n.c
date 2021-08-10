#include "../set.h"

char*
set_at_n(const set_t* set, const size_t pos, size_t* len) {
  bucket_t* b;
  size_t i = 0;
  for(b = set->list; b; b = b->list_next) {
    if(i == pos) {
      *len = b->size;
      return b->value;
    }
    i++;
  }
  return 0;
}
