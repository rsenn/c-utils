#include "../strlist.h"

size_t
strlist_count_pred(strlist* list, int (*predicate)(const char*, size_t)) {
  const char* x;
  size_t n, ret = 0;

  strlist_foreach(list, x, n) if(predicate(x, n))++ ret;

  return ret;
}
