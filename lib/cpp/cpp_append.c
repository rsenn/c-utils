#include "../cpp.h"
#include "../cpp_internal.h"

// Append tok2 to the end of tok1.
cpp_token*
cpp_append(cpp_token* tok1, cpp_token* tok2) {
  if(tok1->kind == TK_EOF)
    return tok2;

  cpp_token head = {};
  cpp_token* cur = &head;

  for(; tok1->kind != TK_EOF; tok1 = tok1->next)
    cur = cur->next = cpp_copy_token(tok1);
  cur->next = tok2;
  return head.next;
}
