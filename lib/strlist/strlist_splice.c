#include "../strlist.h"

int
strlist_splice(strlist* l, size_t pos, size_t ndelete, char* buf, size_t len) {
  int64 start, p;
  size_t i, end = l->sa.len;
  char* x = l->sa.s;
  start = strlist_pos(l, pos);
  for(p = start; p < end;) {
    if(ndelete-- == 0)
      break;
    i = byte_chr(&x[p], end - p, l->sep);
    if(p + i < end)
      i++;
    p += i;
  }
  return stralloc_replace(&l->sa, start, p - start, buf, len);
}