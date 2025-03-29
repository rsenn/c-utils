#include "../cpp.h"
#include "../cpp_internal.h"

bool cpp_has_varargs(cpp_macro_arg *args) {
  for (cpp_macro_arg *ap = args; ap; ap = ap->next)
    if (!strcmp(ap->name, "__VA_ARGS__"))
      return ap->tok->kind != TK_EOF;
  return false;
}
