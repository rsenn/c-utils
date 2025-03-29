#include "../cpp.h"
#include "../cpp_internal.h"
#include "../alloc.h"

cpp_token*
cpp_copy_token(cpp_token* tok) {
  cpp_token* t = alloc_zero(sizeof(cpp_token));
  *t = *tok;
  t->next = NULL;
  return t;
}
