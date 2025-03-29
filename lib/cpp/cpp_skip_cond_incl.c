#include "../cpp.h"
#include "../cpp_internal.h"

/* Skip until next `#else`, `#elif` or `#endif`.
   Nested `#if` and `#endif` are skipped. */
cpp_token*
cpp_skip_cond_incl(cpp_token* tok) {
  while(tok->kind != TK_EOF) {
    if(cpp_is_hash(tok) && (cpp_equal(tok->next, "if") || cpp_equal(tok->next, "ifdef") || cpp_equal(tok->next, "ifndef"))) {
      tok = cpp_skip_cond_incl2(tok->next->next);
      continue;
    }

    if(cpp_is_hash(tok) && (cpp_equal(tok->next, "elif") || cpp_equal(tok->next, "else") || cpp_equal(tok->next, "endif")))
      break;

    tok = tok->next;
  }

  return tok;
}
