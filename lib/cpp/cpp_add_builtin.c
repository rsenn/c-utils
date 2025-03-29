#include "../cpp.h"
#include "../cpp_internal.h"

cpp_macro*
cpp_add_builtin(char* name, macro_handler_fn* fn) {
  cpp_macro* m = cpp_add_macro(name, true, NULL);
  m->handler = fn;
  return m;
}
