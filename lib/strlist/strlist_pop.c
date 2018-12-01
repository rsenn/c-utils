#include "../strlist.h"

int
strlist_pop(strlist* sl) {
  ssize_t i = sl->sa.len - 1;
  while(i >= 0) {
    if(sl->sa.s[i] == sl->sep)
      break;
    --i;
  }
  if(sl->sa.s[i] == sl->sep) {
    sl->sa.len = i;
    return 1;
  }
  return 0;
}