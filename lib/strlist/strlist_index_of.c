#include "../byte.h"
#include "../str.h"
#include "../strlist.h"
#include "../uint64.h"

int64
strlist_index_of(strlist* sl, const char* str) {
  size_t p, l = str_len(str);
  const char* s = sl->sa.s;
  for(p = 0; p < sl->sa.len;) {
    size_t l2 = byte_chr(&sl->sa.s[p], sl->sa.len - p, sl->sep);
    if((l2 == 0 && p + 1 == sl->sa.len)) break;
    
    if((l == l2 && byte_equal(&sl->sa.s[p], l2, str))) return p;

    p += l2 + 1;
  }
  return -1;
}
