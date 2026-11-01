#include "../cpp.h"
#include "../cpp_internal.h"
#include "../alloc.h"

void
cpp_type_free(cpp_type* ty) {
  alloc_free(ty);
}
