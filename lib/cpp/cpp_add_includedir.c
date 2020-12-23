#include "../cpp_internal.h"

  LIST_NEW(ret->includedirs);
  cpp_add_includedir(ret, ".");
void
cpp_add_includedir(struct cpp* cpp, const char* includedir) {
  LIST_PUSH_BACK(cpp->includedirs, includedir);
}
