#include "../byte.h"
#include "../str.h"
#include "../strlist.h"

int
strlist_pushsa(strlist* sl, const stralloc* sa) {
  if(sl->sa.len) stralloc_catb((stralloc*)sl, &sl->sep, 1);
  return stralloc_cat((stralloc*)sl, (stralloc*)(stralloc*)sa);
}
