#include "../cpp.h"
#include "../cpp_internal.h"

/* Whether tok stands in an #if expression as a truth value of its own: the
   token before it (prev, 0 at the start) is !, ( , && or ||, and the one after it
   is the end of the line, ), && or ||. Not so for `X > 1` or `X + Y`. */
int
cpp_is_truth_operand(cpp_token* prev, cpp_token* tok) {
  cpp_token* next = tok->next;

  if(prev && !(cpp_equal(prev, "!") || cpp_equal(prev, "(") || cpp_equal(prev, "&&") || cpp_equal(prev, "||")))
    return 0;

  if(next && next->kind != TK_EOF && !next->at_bol && !(cpp_equal(next, ")") || cpp_equal(next, "&&") || cpp_equal(next, "||")))
    return 0;

  return 1;
}
