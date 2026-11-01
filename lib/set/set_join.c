#include "../set.h"

/* Appends the values, with sep between them, to out. 1 on success, 0 if the
 * memory ran out (out then holds part of the text). */
int
set_join(const set_t* set, const char* sep, stralloc* out) {
  const bucket_t* b;

  for(b = set->list; b; b = b->list_next) {
    if(b != set->list && !stralloc_cats(out, sep))
      return 0;

    if(!stralloc_catb(out, b->value, b->size))
      return 0;
  }

  return 1;
}
