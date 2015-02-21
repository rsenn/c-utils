#include <string.h>
#include "strlist.h"
#include "uint64.h"

int64
strlist_index_of(strlist* sl, const char* str) {
  size_t p, l = 0;
  const char *s = sl->sa.s;
  for(p = 0; p < sl->sa.len; ++p) {
    if(strcmp(s, str) == 0)
      return p;

    if(sl->sa.s[p] == '\0')
      ++l;

    s = &sl->sa.s[p + 1];
  }
  return -1;
}
