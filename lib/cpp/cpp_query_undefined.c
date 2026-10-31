#include "../cpp.h"
#include "../cpp_internal.h"
#include "../alloc.h"
#include "../str.h"

/* Notes the name of tok in undef_tested (by_value is 0) or in undef_valued
   (1; 2 when the expression uses it as more than a truth value, which puts
   it in undef_cmp as well) when query tracking is on, tok is an identifier
   and no macro of that name exists. */
void
cpp_query_undefined(cpp_ctx* pp, cpp_token* tok, int by_value) {
  char* name;

  if(!pp->track_queries || tok->kind != TK_IDENT || cpp_macro_find(pp, tok))
    return;

  if((name = str_ndup(tok->loc, tok->len))) {
    strarray_push_unique(by_value ? &pp->undef_valued : &pp->undef_tested, name);

    if(by_value == 2)
      strarray_push_unique(&pp->undef_cmp, name);
    alloc_free(name);
  }
}
