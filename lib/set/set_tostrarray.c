#include "../set.h"
#include "../strarray.h"

void
set_tostrarray(const set_t* set, strarray* stra) {
  bucket_t* b;
  for(b = set->list; b; b = b->list_next)
    strarray_pushb(stra, b->value, b->size);
}
