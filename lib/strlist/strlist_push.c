#include "../byte.h"
#include "../str.h"
#include "../strlist.h"

int
strlist_push(strlist* sl, const char* s) {
  if(sl->sa.len)
    stralloc_catc(&sl->sa, sl->sep);
  return stralloc_cats(&sl->sa, s);
}
