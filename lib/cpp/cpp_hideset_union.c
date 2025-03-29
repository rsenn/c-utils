#include "../cpp.h"
#include "../cpp_internal.h"

cpp_hideset*
cpp_hideset_union(cpp_hideset* hs1, cpp_hideset* hs2) {
  cpp_hideset head = {};
  cpp_hideset* cur = &head;

  for(; hs1; hs1 = hs1->next)
    cur = cur->next = cpp_new_hideset(hs1->name);
  cur->next = hs2;
  return head.next;
}
