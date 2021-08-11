#include "../set.h"
#include "../stralloc.h"

char*
set_at_sa(const set_t* set, int64 pos, stralloc* out) {
  bucket_t* b;
  char* x;
  size_t len;

  if((x = set_at_n(set, pos, &len))) {
    stralloc_copyb(out, x, len);
    stralloc_nul(out);
    return out->s;
  }
  return 0;
}
