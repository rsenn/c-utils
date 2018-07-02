#include "../strlist.h"
#include "../byte.h"
#include "../str.h"

int
strlist_unshift(strlist* sl, const char* s) {
  size_t i, n = strlist_count(sl);
  strlist newl;
  strlist_init(&newl);
  strlist_push(&newl, s);
  for(i = 0; i < n; ++i) {  
    strlist_push(&newl, strlist_at(sl, i));
  }
  strlist_free(sl);
  *sl = newl;
  return n+1;
}  
