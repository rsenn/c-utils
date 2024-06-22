#include "../cpp_internal.h"

void
cpp_macro_add(cpp_t* cpp, const char* name, cpp_macro* m) {
  MAP_INSERT(cpp->macros, name, str_len(name) + 1, m, sizeof(cpp_macro));
}
