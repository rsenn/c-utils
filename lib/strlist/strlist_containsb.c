#include "../byte.h"
#include "../strlist.h"

int
strlist_containsb(const strlist* sl, const char* x, size_t len) {
  size_t i;

  for(i = 0; i < sl->sa.len;) {
    size_t n = byte_chr(&sl->sa.s[i], sl->sa.len - i, sl->sep);
    if(n == len && byte_equal(&sl->sa.s[i], len, x))
      return 1;
    i += n + 1;
  }
  return 0;
}
