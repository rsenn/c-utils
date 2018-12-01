#include "../path_internal.h"
#include "../str.h"
#include "../strlist.h"
#include <ctype.h>
int
path_split(const char* p, strlist* sl) {
  size_t len = str_len(p);
  char sep = ':';
#if WINDOWS
  if(len > 2 && isalnum(p[0]) && p[1] == ':' && p[2] == '\\' || p[2] == '/')
    sep = ';';
#endif
  strlist_froms(sl, p, sep);
  return strlist_count(sl) > 1;
}
