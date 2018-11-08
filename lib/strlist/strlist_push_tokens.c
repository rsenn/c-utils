#include "../str.h"
#include "../strlist.h"
#include <stdlib.h>

void
strlist_push_tokens(strlist* sl, const char* s, const char* delim) {
  size_t p, n, len = str_len(delim);

  for(p = 0; s[p]; p += n + len) {
  	n = str_find(&s[p], delim);
    strlist_pushb(sl, &s[p], n);
  }
}
