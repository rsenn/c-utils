#include "../cpp_internal.h"

  void
cpp_add_macro(struct cpp* cpp, const char* name, struct macro* m) {
  MAP_INSERT(cpp->macros, name, str_len(name) + 1, m, sizeof(struct macro));
}
