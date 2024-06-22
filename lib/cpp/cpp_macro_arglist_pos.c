#include "../cpp_internal.h"

size_t
cpp_macro_arglist_pos(cpp_macro* m, const char* iden) {
  size_t i, len = LIST_SIZE(m->argnames);

  for(i = 0; i < len; i++) {
    char* item = LIST_GET(m->argnames, i);

    if(!str_diff(item, iden))
      return i;
  }

  return (size_t)-1;
}
