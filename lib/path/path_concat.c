#include "../path_internal.h"

void
path_concat(const char* a, const char* b, stralloc* out) {
  stralloc sa;
  const char* x;
  size_t len;

  stralloc_init(&sa);

  path_appends(a, &sa);
  path_appends(b, &sa);

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
