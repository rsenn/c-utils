#include "../path_internal.h"
int
path_canonical_sa(stralloc* sa, stralloc* out) {
  stralloc_nul(sa);
  return path_collapse(sa->s, out);
  /*  const char *s, *next;
    const char* e = sa->s + sa->len;
    size_t nextlen, len = path_len(sa->s, sa->len);
    stralloc_zero(out);
    for(s = sa->s; s < e; len = nextlen, s = next) {
      next = s + path_skip(s, e - s);
      nextlen = path_len(next, e - next);
      if(len == 1 && *s == '.') continue;
      if((len != 2 || byte_diff(s, 2, ".."))) {
        if(next < e && nextlen == 2 && !byte_diff(next, 2, "..")) {
          next = s + path_skip(next, e - next);
          nextlen = path_len(next, e - next);
          continue;
        }
      }
      if(out->len && out->s[out->len - 1] != PATHSEP_C) stralloc_catc(out, PATHSEP_C);
      stralloc_catb(out, s, len);
    }*/
}
