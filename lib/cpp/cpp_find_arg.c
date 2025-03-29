#include "../cpp.h"
#include "../cpp_internal.h"
#include "../str.h"

cpp_macro_arg*
cpp_find_arg(cpp_macro_arg* args, cpp_token* tok) {
  for(cpp_macro_arg* ap = args; ap; ap = ap->next)
    if(tok->len == str_len(ap->name) && !str_diffn(tok->loc, ap->name, tok->len))
      return ap;

  return NULL;
}
