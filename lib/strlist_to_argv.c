#include <malloc.h>
#include "strlist.h"

char**
strlist_to_argv(const strlist* sl) {
  size_t i = 0, n = strlist_count(sl);
  char** v = calloc(sizeof(char*), (n + 1));
  if(v == NULL) return NULL;
  
  while(i < n) {
     v[i] = strlist_at(sl, i++);
  }
  //v[i] = NULL;
  return v;
}
  
  