#include "../cpp_internal.h"

  struct macro*
cpp_get_macro(struct cpp* cpp, const char* name) {
  return MAP_GET(cpp->macros, name, str_len(name) + 1);
}
