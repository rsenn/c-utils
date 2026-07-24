#include "../cpp.h"
#include "../cpp_internal.h"

cpp_macro*
cpp_add_builtin(cpp_ctx* pp, char* name, macro_handler_fn* fn) {
  cpp_macro* m = cpp_macro_add(pp, name, true, NULL);
  m->handler = fn;
  return m;
}
