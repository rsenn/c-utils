#include "../path_internal.h"
void
path_concat(const char* a, size_t alen, const char* b, size_t blen, stralloc* out) {
  stralloc sa;
  const char* x;
  size_t len;
  stralloc_init(&sa);

  path_append(a, alen, &sa);
  path_append(b, blen, &sa);

  x = sa.s;
  len = sa.len;
  if(len > 2 && sa.s[0] == '.' && sa.s[1] == PATHSEP_C) {
    x += 2;
    len -= 2;
  }
  stralloc_copyb(out, x, len);
  stralloc_nul(out);
  stralloc_free(&sa);
}
