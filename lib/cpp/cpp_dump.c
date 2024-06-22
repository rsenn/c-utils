#include "../cpp_internal.h"

void
cpp_dump(cpp* pp) {
  MAP_DUMP(pp->macros);
}
