#include "../str.h"
#include "../strlist.h"

int
strlist_prepends(strlist* sl, const char* s) {
  return strlist_prependb(sl, s, str_len(s));
}

