#include "../stralloc.h"
#include "../set.h"
#include "../byte.h"

void
set_joinq(const set_t* set, stralloc* out, char sep, char quote) {
  bucket_t* b;
  size_t i = 0;
  for(b = set->list; b; b = b->list_next) {
    int q;
    if(i > 0)
      stralloc_catc(out, sep);

    if((q = byte_chr(b->value, b->size, sep) < b->size))
      stralloc_catc(out, quote);

    stralloc_catb(out, b->value, b->size);

    if(q)
      stralloc_catc(out, quote);
    i++;
  }
}
