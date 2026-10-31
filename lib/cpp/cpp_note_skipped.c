#include "../cpp.h"
#include "../cpp_internal.h"
#include "../alloc.h"
#include "../str.h"

/* The tokens from..to were skipped as a branch that is not taken. With
   query tracking on, notes the #defines in it, and the names its conditionals
   ask about, as if they were read. */
void
cpp_note_skipped(cpp_ctx* pp, cpp_token* from, cpp_token* to) {
  cpp_token *t, *d, *prev;
  char* name;

  if(!pp->track_queries)
    return;

  for(t = from; t && t != to && t->kind != TK_EOF; t = t->next) {
    if(!cpp_is_hash(t) || t->next->kind == TK_EOF)
      continue;

    d = t->next;

    if(cpp_equal(d, "define") && d->next->kind == TK_IDENT) {
      if((name = str_ndup(d->next->loc, d->next->len))) {
        strarray_push_unique(&pp->defined_names, name);
        alloc_free(name);
      }
    } else if(cpp_equal(d, "ifdef") || cpp_equal(d, "ifndef")) {
      cpp_query_undefined(pp, d->next, 0);
    } else if(cpp_equal(d, "if") || cpp_equal(d, "elif")) {
      /* the names of the line */
      for(prev = 0, d = d->next; d->kind != TK_EOF && !d->at_bol; prev = d, d = d->next) {
        if(cpp_equal(d, "defined")) {
          cpp_token* e = d->next;

          if(cpp_equal(e, "("))
            e = e->next;

          cpp_query_undefined(pp, e, 0);
          d = e;
        } else if(d->kind == TK_IDENT) {
          cpp_query_undefined(pp, d, cpp_is_truth_operand(prev, d) ? 1 : 2);
        }
      }
    }
  }
}
