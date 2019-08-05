#include "../str.h"
#include "../strlist.h"

void
strlist_froms(strlist* sl, const char* s, char delim) {
  size_t i;
  //  strlist_init(sl, delim);

  if(s == NULL)
    return;

  if(sl->sa.len)
    stralloc_catc(&sl->sa, sl->sep);

  stralloc_cats(&sl->sa, s);
  for(i = 0; i < sl->sa.len; ++i) {
    if(sl->sa.s[i] == delim)
      sl->sa.s[i] = sl->sep;
  }
  if(sl->sa.len > 0) {
    if(sl->sa.s[sl->sa.len - 1] != sl->sep)
      stralloc_catc(&sl->sa, sl->sep);
  }
}

