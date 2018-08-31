#include "../strlist.h"
#include "../str.h"

int
strlist_contains(strlist* sl, const char* s) {
  size_t i;

  for(i = 0; i < sl->sa.len; i += str_len(&sl->sa.s[i]) + 1) {
    if(str_equal(&sl->sa.s[i], s))
     return 1;
  }
  return 0;
}
