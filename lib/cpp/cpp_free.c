#include "../cpp_internal.h"

  void
cpp_free_macros(struct cpp* cpp) {
  MAP_ITER_T i;
  MAP_FOREACH(cpp->macros, i) { /* while(hbmap_iter_index_valid(cpp->macros, i)) */
    cpp_undef_macro(cpp, MAP_KEY(cpp->macros, i));
  }
  MAP_DESTROY(cpp->macros);
}
