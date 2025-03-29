#include "../cpp.h"
#include "../cpp_internal.h"
#include "../alloc.h"

cpp_hideset *cpp_new_hideset(char *name) {
  cpp_hideset *hs = alloc_zero( sizeof(cpp_hideset));
  hs->name = name;
  return hs;
}
