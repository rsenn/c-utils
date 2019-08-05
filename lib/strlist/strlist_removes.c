#include "../strlist.h"
#include "../str.h"

int
strlist_removes(strlist* l, const char* s) {
  return strlist_removeb(l, s, str_len(s));
}

