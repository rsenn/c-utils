#include "../cpp.h"
#include "../cpp_internal.h"
#include "../alloc.h"

cpp_cond_incl* cond_incl = 0;

cpp_cond_incl*
cpp_push_cond_incl(cpp_token* tok, bool included) {
  cpp_cond_incl* ci;

  if((ci = alloc_zero(sizeof(cpp_cond_incl)))) {
    ci->next = cond_incl;
    ci->ctx = IN_THEN;
    ci->tok = tok;
    ci->included = included;
  }

  cond_incl = ci;

  return ci;
}
