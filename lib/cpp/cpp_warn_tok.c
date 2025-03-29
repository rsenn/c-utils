#include "../cpp_internal.h"

void cpp_warn_tok(cpp_token *tok, char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  cpp_verror_at(tok->file->name, tok->file->contents, tok->line_no, tok->loc, fmt, ap);
  va_end(ap);
}
