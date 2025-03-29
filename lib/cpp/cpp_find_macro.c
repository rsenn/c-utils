#include "../cpp.h"
#include "../cpp_internal.h"

cpp_macro*
cpp_find_macro(cpp_token* tok) {
  if(tok->kind != TK_IDENT)
    return NULL;

  return hashmap_get2(&cpp_macros, tok->loc, tok->len);
}
