#include "../cpp.h"
#include "../cpp_internal.h"
#include "../str.h"

void
cpp_hideset_free(cpp_hideset* hs) {
  alloc_free(hs->name);
  alloc_free(hs);
}
