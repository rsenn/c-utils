#include "../strlist.h"
#include "../byte.h"
#include "../stralloc.h"

/* removes the last element; 1 on success, 0 on an empty list (a list with one
 * element could not be popped, and an empty one was read before its start) */
int
strlist_pop(strlist* sl) {
  size_t i, len = sl->sa.len;

  if(len == 0)
    return 0;
  for(i = len; i > 0; --i)
    if(sl->sa.s[i - 1] == sl->sep)
      break;
  sl->sa.len = i > 0 ? i - 1 : 0;
  return 1;
}
