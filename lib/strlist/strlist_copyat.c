#include "../strlist.h"

int
strlist_copyat(const strlist* sl, size_t i, stralloc* out) {
  const char* x = strlist_at(sl, i);
  size_t pos = (x - sl->sa.s);
  size_t len = byte_chr(x, sl->sa.len - pos, sl->sep);
  if(len > 0 && pos + len <= sl->sa.len) {
    stralloc_copyb(out, x, len);
    return 1;
  }
  return 0;
}
