#include "../cpp_internal.h"

void
cpp_add_includedir(cpp* pp, const char* includedir) {
  LIST_PUSH_BACK(pp->includedirs, includedir);
}
