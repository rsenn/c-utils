#include "../cpp.h"
#include "../cpp_internal.h"
#include "../str.h"

bool
cpp_has_varargs(cpp_macro_arg* args) {
  for(cpp_macro_arg* ap = args; ap; ap = ap->next)
    if(str_equal(ap->name, "__VA_ARGS__"))
      return ap->tok->kind != TK_EOF;

  return false;
}
