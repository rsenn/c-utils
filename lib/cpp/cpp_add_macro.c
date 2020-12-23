#include "../cpp_internal.h"

void
cpp_add_macro(cpp_t* cpp, const char* name, struct macro_s* m) {
  MAP_INSERT(cpp->macros, name, str_len(name) + 1, m, sizeof(struct macro_s));
}
