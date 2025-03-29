#include "../set.h"

char*
set_at_n(const set_t* set, int64 pos, size_t* len) {
  bucket_t* b;
  int64 i = 0;

  if(pos >= 0) {
    for(b = set->list; b; b = b->list_next) {
      if(i == pos) {
        *len = b->size;
        return b->value;
      }

      i++;
    }
  } else {
    pos = ~pos;

    for(b = set->last; b; b = b->list_prev) {
      if(i == pos) {
        *len = b->size;
        return b->value;
      }

      i++;
    }
  }

  return 0;
}
