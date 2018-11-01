#include "../byte.h"
#include "../str.h"
#include "../strlist.h"

int
strlist_push(strlist* sl, const char* s) {
  if(sl->sa.len) {
    if(sl->sa.s[sl->sa.len - 1] != sl->sep) stralloc_catc(&sl->sa, sl->sep);
  }
  stralloc_cats(&sl->sa, s);
  stralloc_catc(&sl->sa, sl->sep);
//  --sl->sa.len;
}