#include "../cpp.h"
#include "../cpp_internal.h"
#include "../str.h"

cpp_token*
cpp_hideset_add(cpp_token* tok, cpp_hideset* hs) {
  cpp_token head = {0};
  cpp_token* cur = &head;

  for(; tok; tok = tok->next) {
    cpp_token* t = cpp_token_copy(tok);
    t->hideset = cpp_hideset_union(t->hideset, hs);
    cur = cur->next = t;
  }

  return head.next;
}
