#include "../cpp_internal.h"

void cpp_error_tok(cpp_token *tok, char *fmt, ...) {
  va_list ap;
  
  va_start(ap, fmt);
  verror_at(tok->file->name, tok->file->contents, tok->line_no, tok->loc, fmt, ap);
  exit(1);
}
