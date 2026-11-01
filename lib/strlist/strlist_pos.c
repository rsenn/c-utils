#include "../strlist.h"
#include "../byte.h"
#include "../stralloc.h"

/* byte offset of element index in the list, -1 if there is no such element
 * (the scan used to measure every element from the start of the list, so it
 * was only right when all elements had the length of the first one) */
int64
strlist_pos(const strlist* sl, size_t index) {
  const char* x = sl->sa.s;
  size_t pos = 0, n = sl->sa.len, count = 0;

  while(pos < n) {
    size_t i;

    if(index == count)
      return (int64)pos;
    i = byte_chr(x + pos, n - pos, sl->sep);
    pos += i < n - pos ? i + 1 : i;
    ++count;
  }
  return -1;
}
