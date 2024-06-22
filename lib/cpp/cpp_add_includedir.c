#include "../cpp_internal.h"

void
cpp_add_includedir(cpp* cpp, const char* includedir) {
  LIST_PUSH_BACK(cpp->includedirs, includedir);
}
