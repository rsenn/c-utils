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

void
cpp_hideset_free(cpp_hideset* hs) {
  alloc_free(hs->name);
  alloc_free(hs);
}

cpp_token*
cpp_hideset_add(cpp_token* tok, cpp_hideset* hs) {
  cpp_token head = {};
  cpp_token* cur = &head;

  for(; tok; tok = tok->next) {
    cpp_token* t = cpp_token_copy(tok);
    t->hideset = cpp_hideset_union(t->hideset, hs);
    cur = cur->next = t;
  }

  return head.next;
}

bool
cpp_hideset_contains(cpp_hideset* hs, char* s, int len) {
  for(; hs; hs = hs->next)
    if(str_len(hs->name) == len && !str_diffn(hs->name, s, len))
      return true;

  return false;
}

cpp_hideset*
cpp_hideset_intersection(cpp_hideset* hs1, cpp_hideset* hs2) {
  cpp_hideset head = {};
  cpp_hideset* cur = &head;

  for(; hs1; hs1 = hs1->next)
    if(cpp_hideset_contains(hs2, hs1->name, str_len(hs1->name)))
      cur = cur->next = cpp_hideset_new(hs1->name);

  return head.next;
}

cpp_hideset*
cpp_hideset_union(cpp_hideset* hs1, cpp_hideset* hs2) {
  cpp_hideset head = {};
  cpp_hideset* cur = &head;

  for(; hs1; hs1 = hs1->next)
    cur = cur->next = cpp_hideset_new(hs1->name);

  cur->next = hs2;
  return head.next;
}
