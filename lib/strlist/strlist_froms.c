#include "../str.h"
#include "../strlist.h"

void
strlist_froms(strlist* sl, const char* s, char delim) {
  strlist_init(sl, delim);

  if(s == NULL) return;

  stralloc_copys(&sl->sa, s);
}
