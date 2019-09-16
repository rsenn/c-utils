#include "../str.h"
#include "../strlist.h"

int
strlist_unshift(strlist* sl, const char* s) {
  size_t len = str_len(s);
  stralloc_insertb(&sl->sa, s, 0, len + 1);
  sl->sa.s[len] = sl->sep;
  return 1;
}
