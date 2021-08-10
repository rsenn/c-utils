#include "../set.h"
#include "../stralloc.h"

char*
set_at_sa(const set_t* set, const size_t pos, stralloc* out) {
  bucket_t* b;
  size_t i = 0;
  for(b = set->list; b; b = b->list_next) {
    if(i == pos) {
      stralloc_copyb(out, b->value, b->size);
      stralloc_nul(out);
      return out->s;
    }
    i++;
  }
  return 0;
}
