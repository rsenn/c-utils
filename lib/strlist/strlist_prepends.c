#include "../strlist.h"
#include "../str.h"

int
strlist_prepends(strlist* sl, const char* s) {
  return strlist_prependb(sl, s, str_len(s));
}
