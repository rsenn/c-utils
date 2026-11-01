#include "../strlist.h"
#include "../byte.h"
#include "../stralloc.h"

/* removes the first element that equals the n bytes at x; 1 if one was
 * removed (removing the last element used to leave an empty one behind) */
int
strlist_removeb(strlist* l, const char* x, size_t n) {
  int64 i = strlist_index_of_b(l, x, n);

  return i != -1 && strlist_erase_element(l, (size_t)i, n);
}
