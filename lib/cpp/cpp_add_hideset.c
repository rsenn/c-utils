#include "../cpp.h"
#include "../cpp_internal.h"

cpp_token*
cpp_add_hideset(cpp_token* tok, cpp_hideset* hs) {
  cpp_token head = {};
  cpp_token* cur = &head;

  for(; tok; tok = tok->next) {
    cpp_token* t = cpp_copy_token(tok);
    t->hideset = cpp_hideset_union(t->hideset, hs);
    cur = cur->next = t;
  }

  return head.next;
}
