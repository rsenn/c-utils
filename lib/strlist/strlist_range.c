#include "../strlist.h"
#include "../stralloc.h"

strlist
strlist_range(const strlist* sl, size_t from, size_t to) {
  strlist ret;
  size_t s, e;
  s = strlist_at(sl, from) - sl->sa.s;
  e = strlist_at(sl, to) - sl->sa.s;

  ret.sa.s = &sl->sa.s[s];
  ret.sa.len = e - s;
  ret.sa.a = 0;
  return ret;
}
  
