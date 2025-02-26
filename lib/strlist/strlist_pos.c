#include "../strlist.h"

int64
strlist_pos(const strlist* sl, size_t index) {
  const char* x = sl->sa.s;
  size_t pos, i, n = sl->sa.len;
  size_t count = 0;

  for(pos = 0; n; pos += i, n -= i, count++) {
    if(index == count)
      return pos;
    i = byte_chr(x, n, sl->sep);

    if(i < n)
      i += 1;
  }
  return -1;
}
