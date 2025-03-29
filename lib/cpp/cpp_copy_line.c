#include "../cpp.h"
#include "../cpp_internal.h"

/* Copy all tokens until the next newline, terminate them with
   an EOF token and then returns them. This function is used to
   create a new list of tokens for `#if` arguments. */
cpp_token*
cpp_copy_line(cpp_token** rest, cpp_token* tok) {
  cpp_token head = {};
  cpp_token* cur = &head;

  for(; !tok->at_bol; tok = tok->next)
    cur = cur->next = cpp_token_copy(tok);

  cur->next = cpp_new_eof(tok);
  *rest = tok;
  return head.next;
}
