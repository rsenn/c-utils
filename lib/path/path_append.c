#include "../path_internal.h"

void
path_append(const char* x, size_t len, stralloc* out) {
  if(out->len > 0 && out->s[out->len - 1] != PATHSEP_C)
    stralloc_catc(out, PATHSEP_C);

  /*  if(len > 2 && x[0] == '.' && x[1] == PATHSEP_C) {
      x += 2;
      len -= 2;
    }*/
  stralloc_catb(out, x, len);
}

void
path_appends(const char* s, stralloc* out) {
  if(out->len > 0 && out->s[out->len - 1] != PATHSEP_C)
    stralloc_catc(out, PATHSEP_C);

  /* while(str_start(s, "./"))
     s += 2;*/

  stralloc_cats(out, s);
}
