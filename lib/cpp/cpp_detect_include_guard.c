#include "../cpp.h"
#include "../cpp_internal.h"
#include "../str.h"

//   #endif
char*
cpp_detect_include_guard(cpp_token* tok) {
  // Detect the first two lines.
  if(!cpp_is_hash(tok) || !cpp_equal(tok->next, "ifndef"))
    return NULL;
  tok = tok->next->next;

  if(tok->kind != TK_IDENT)
    return NULL;

  char* macro = str_ndup(tok->loc, tok->len);
  tok = tok->next;

  if(!cpp_is_hash(tok) || !cpp_equal(tok->next, "define") || !cpp_equal(tok->next->next, macro))
    return NULL;

  // Read until the end of the file.
  while(tok->kind != TK_EOF) {
    if(!cpp_is_hash(tok)) {
      tok = tok->next;
      continue;
    }

    if(cpp_equal(tok->next, "endif") && tok->next->next->kind == TK_EOF)
      return macro;

    if(cpp_equal(tok, "if") || cpp_equal(tok, "ifdef") || cpp_equal(tok, "ifndef"))
      tok = cpp_skip_cond_incl(tok->next);
    else
      tok = tok->next;
  }
  return NULL;
}
