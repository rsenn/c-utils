#include <stdlib.h>
#include "../strlist.h"

int
strlist_unshiftb(strlist* sl, const char* x, size_t len) {
  char* tmp = malloc(len + 1);
  byte_copy(tmp, len, x);
  tmp[len] = sl->sep;
  stralloc_insertb(&sl->sa, tmp, 0, len + 1);
  free(tmp);
  return 1;
}
