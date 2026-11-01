#include "../cpp.h"
#include "../cpp_internal.h"
#include "../str.h"

cpp_hideset*
cpp_hideset_new(char* name) {
  cpp_hideset* hs;

  if((hs = alloc_zero(sizeof(cpp_hideset))))
    hs->name = str_dup(name);

  return hs;
}
