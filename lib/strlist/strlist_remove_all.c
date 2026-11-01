#include "../strlist.h"
#include "../byte.h"
#include "../stralloc.h"

/* removes from sl every occurrence of every element of remove; returns how many
 * elements were removed */
size_t
strlist_remove_all(strlist* sl, const strlist* remove) {
  const char* x;
  size_t count = 0, n;

  strlist_foreach(remove, x, n) {
    while(strlist_removeb(sl, x, n))
      ++count;
  }
  return count;
}
