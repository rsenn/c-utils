#include "../cpp_internal.h"

static void
cpp_free_macros(cpp* cpp) {
  MAP_PAIR_T i;
  MAP_FOREACH(cpp->macros, i) { cpp_macro_undef(cpp, MAP_KEY(/*cpp->macros, */ i)); }
  MAP_DESTROY(cpp->macros);
}

void
cpp_free(cpp* cpp) {
  cpp_free_macros(cpp);
  LIST_DESTROY(cpp->includedirs);
}
