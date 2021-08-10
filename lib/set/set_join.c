#include "../stralloc.h"
#include "../set.h"

void
set_join(const set_t* set, const char* sep, stralloc* out) {
  bucket_t* b;
  size_t i = 0;
  for(b = set->list; b; b = b->list_next) {
    if(i > 0)
      stralloc_cats(out, sep);
    stralloc_catb(out, b->value, b->size);
    i++;
  }
}
