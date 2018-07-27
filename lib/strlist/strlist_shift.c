#include "../strlist.h"
#include "../str.h"

int
strlist_shift(strlist* sl, const char** strp) {
  strlist newl;
  size_t i, n = strlist_count(sl);
  if(n <= 0) return -1;

  *strp = (const char*)str_dup(strlist_at(sl, 0));
  strlist_init(&newl);

  for(i = 1; i < n; ++i) {
    strlist_push(&newl, strlist_at(sl, i));
  }
  strlist_free(sl);
  *sl = newl;
  return n-1;
}
