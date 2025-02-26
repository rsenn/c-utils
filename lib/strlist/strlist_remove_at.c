#include "../strlist.h"

int
strlist_remove_at(strlist* l, size_t n) {
  const char* x = strlist_at(l, n);

  if(x) {
    size_t i = x - l->sa.s;

    if(i + n < l->sa.len) {
      if(l->sa.s[i + n] == l->sep)
        n++;
    } else if(i > 0) {
      if(l->sa.s[i - 1] == l->sep) {
        i--;
        n++;
      }
    }
    stralloc_remove(&l->sa, i, n);
    return 1;
  }
  return 0;
}
