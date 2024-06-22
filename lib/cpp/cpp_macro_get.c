#include "../cpp_internal.h"

cpp_macro*
cpp_macro_get(cpp_t* cpp, const char* name) {
  return MAP_GET(cpp->macros, name, str_len(name) + 1);
}
