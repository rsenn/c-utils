#include "../cpp_internal.h"

cpp_macro*
cpp_macro_get(cpp* pp, const char* name) {
  return MAP_GET(pp->macros, name, str_len(name));
}
