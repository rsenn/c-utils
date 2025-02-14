#include "../cpp_internal.h"

static void
cpp_free_macros(cpp* pp) {
  MAP_PAIR_T i;
  MAP_FOREACH(pp->macros, i) {
    cpp_macro_undef(pp, MAP_KEY(/*pp->macros, */ i));
  }
  MAP_DESTROY(pp->macros);
}

void
cpp_free(cpp* pp) {
  cpp_free_macros(pp);
  LIST_DESTROY(pp->includedirs);
}
