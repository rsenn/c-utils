#include "../cpp.h"
#include "../cpp_internal.h"
#include "../str.h"

cpp_hideset*
cpp_hideset_union(cpp_hideset* hs1, cpp_hideset* hs2) {
  cpp_hideset head = {0};
  cpp_hideset* cur = &head;

  for(; hs1; hs1 = hs1->next)
    cur = cur->next = cpp_hideset_new(hs1->name);

  cur->next = hs2;
  return head.next;
}
