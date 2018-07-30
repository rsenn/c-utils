#include "../strlist.h"
#include "../str.h"

void
strlist_froms(strlist* sl, const char* s, char delim) {
  strlist_zero(sl);

  while(*s) {
    size_t len = str_chr(s, delim);
    strlist_pushb(sl, s, len);
    s += len;
    if(*s) ++s;
  }
}
