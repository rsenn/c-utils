#include "../strlist.h"
#include "../byte.h"
#include "../stralloc.h"

/* removes from l one occurrence of every element of o; returns how many
 * elements were removed */
int
strlist_sub(strlist* l, const strlist* o) {
  int r = 0;
  char* x;
  size_t n;

  strlist_foreach(o, x, n) {
    if(strlist_removeb(l, x, n))
      r++;
  }
  return r;
}
