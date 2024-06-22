#include "../cpp_internal.h"
#include "../map.h"
#include "../alloc.h"

int
cpp_macro_add(cpp* pp, const char* name, cpp_macro* macro) {
  cpp_macro* a;

  if((a = alloc_zero(sizeof(cpp_macro)))) {
    byte_copy(a, sizeof(cpp_macro), macro);
    MAP_ADD(pp->macros, (char*)name, a);
    return 1;
  }

  return 0;
}
