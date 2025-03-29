#include "../cpp.h"
#include "../cpp_internal.h"
#include "../str.h"

bool
cpp_hideset_contains(cpp_hideset* hs, char* s, int len) {
  for(; hs; hs = hs->next)
    if(str_len(hs->name) == len && !str_diffn(hs->name, s, len))
      return true;

  return false;
}
