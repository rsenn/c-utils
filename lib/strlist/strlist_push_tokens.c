#include <stdlib.h>
#include "../str.h"
#include "../strlist.h"

void
strlist_push_tokens(strlist* sl, const char* s, const char* delim) {
  char *p = NULL, *tok;

  if((s = str_dup(s)) == NULL)
    return;

  if((tok = str_tok((char*)s, delim, &p)) != NULL) {
    do {
      strlist_push(sl, tok);
    } while((tok = str_tok(NULL, delim, &p)));
  }
  free((char*)s);
}
