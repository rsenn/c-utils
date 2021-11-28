#include "../stralloc.h"
#include "../path_internal.h"

void
path_concat_sa(const stralloc* a, const stralloc* b, stralloc* out) {
  path_concatb(a->s, a->len, b->s, b->len, out);
}
