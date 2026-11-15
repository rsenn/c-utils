#include "../pathmap.h"
#include "../str.h"

/* pathmap_addlist adds the rules of list, separated by ':'. Empty items
 * are skipped. Returns how many were added. */
size_t
pathmap_addlist(pathmap* m, const char* list, int root) {
  size_t i, count = 0;

  for(; *list; list += i + (list[i] ? 1 : 0)) {
    i = str_chr(list, ':');

    if(i && pathmap_addb(m, list, i, root))
      count++;

    if(!list[i])
      break;
  }

  return count;
}
