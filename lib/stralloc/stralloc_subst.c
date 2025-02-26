#include "../byte.h"
#include "../str.h"
#include "../stralloc.h"

int
stralloc_subst(stralloc* out, const char* b, size_t len, const char* from, const char* to) {
  size_t plen = str_len(from);
  size_t i;

  for(i = 0; i < len;) {
    if(i + plen <= len && byte_equal(from, plen, &b[i])) {
      if(!stralloc_cats(out, to))
        return 0;
      i += plen;
    } else {
      if(!stralloc_append(out, &b[i]))
        return 0;
      ++i;
    }
  }
  return 1;
}
