#include "../cpp_internal.h"

void
cpp_free_macros(cpp_t* cpp) {
  MAP_PAIR_T i;
  MAP_FOREACH(cpp->macros, i) { cpp_undef_macro(cpp, MAP_KEY(/*cpp->macros, */ i)); }
  MAP_DESTROY(cpp->macros);
}

void
cpp_free(cpp_t* cpp) {
  cpp_free_macros(cpp);
  LIST_DESTROY(cpp->includedirs);
}
