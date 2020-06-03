#include "../path_internal.h"

void
path_append(const char* x, size_t len, stralloc* out) {
  if(out->len > 0 && out->s[out->len - 1] != PATHSEP_C)
  stralloc_catc(out, PATHSEP_C);
  stralloc_catb(out, x, len);
}
