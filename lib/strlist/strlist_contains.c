#include "../byte.h"
#include "../str.h"
#include "../strlist.h"

int
strlist_contains(strlist* sl, const char* s) {
  size_t i, len = str_len(s);

  for(i = 0; i < sl->sa.len;) {
    size_t n = byte_chr(&sl->sa.s[i], sl->sa.len - i, sl->sep);
    if(n == len && byte_equal(&sl->sa.s[i], len, s))
      return 1;
    i += n + 1;
  }
  return 0;
}
