#include "../cpp_internal.h"

size_t
cpp_macro_arglist_pos(cpp_macro* macro, const char* iden) {
  size_t i, len = LIST_SIZE(macro->argnames);

  for(i = 0; i < len; i++) {
    char* item = LIST_GET(macro->argnames, i);

    if(!str_diff(item, iden))
      return i;
  }

  return (size_t)-1;
}
