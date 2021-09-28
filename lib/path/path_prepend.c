#include "../path_internal.h"

void
path_prepend(const char* x, size_t len, stralloc* out) {
  const char c = PATHSEP_C;
  if(out->len > 0 && out->s[0] != c)
    stralloc_insertb(out, &c, 0, 1);

  stralloc_insertb(out, x, len, 0);
}

void
path_prepends(const char* s, stralloc* out) {
  const char c = PATHSEP_C;
  if(out->len > 0 && out->s[0] != c)
    stralloc_insertb(out, &c, 0, 1);

  stralloc_inserts(out, s, 0);
}
