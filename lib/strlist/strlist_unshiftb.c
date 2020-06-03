#include "../strlist.h"

int
strlist_unshiftb(strlist* sl, const char* x, size_t len) {
  stralloc_insertb(&sl->sa, x, 0, len + 1);
  sl->sa.s[len] = sl->sep;
  return 1;
}
