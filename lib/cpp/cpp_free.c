#include "../cpp_internal.h"

void
cpp_free(cpp_t* cpp) {
  cpp_free_macros(cpp);
  LIST_DESTROY(cpp->includedirs);
}
