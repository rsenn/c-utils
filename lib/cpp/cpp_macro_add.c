#include "../cpp_internal.h"
#include "../map.h"
#include "../alloc.h"

int
cpp_macro_add(cpp* pp, const char* name, cpp_macro* macro) {
  MAP_ITER_T it;

  it = MAP_INSERT(
      pp->macros, name, str_len(name), macro, sizeof(cpp_macro));

  return it != 0;
}
