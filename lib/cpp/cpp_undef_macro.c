#include "../cpp.h"
#include "../cpp_internal.h"

void
cpp_undef_macro(char* name) {
  hashmap_delete(&macros, name);
}
