#include "../cpp.h"
#include "../cpp_internal.h"
#include "../str.h"

cpp_hideset*
cpp_hideset_intersection(cpp_hideset* hs1, cpp_hideset* hs2) {
  cpp_hideset head = {};
  cpp_hideset* cur = &head;

  for(; hs1; hs1 = hs1->next)
    if(cpp_hideset_contains(hs2, hs1->name, str_len(hs1->name)))
      cur = cur->next = cpp_new_hideset(hs1->name);
  return head.next;
}
