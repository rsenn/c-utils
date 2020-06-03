#include "../path_internal.h"
void
path_concat(const char* a, size_t alen, const char* b, size_t blen, stralloc* out) {
  stralloc sa;
  stralloc_init(&sa);

  stralloc_copyb(&sa, a, alen);
  path_append( b, blen, &sa);
  stralloc_copy(out, &sa);
  stralloc_nul(out);
  stralloc_free(&sa);
}
