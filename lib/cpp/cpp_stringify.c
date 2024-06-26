#include "../cpp_internal.h"

int
cpp_stringify(cpp* pp, tokenizer* t, buffer* out) {
  token tok;

  buffer_puts(out, "\"");

  for(;;) {
    if(!tokenizer_next(t, &tok))
      return 0;

    if(tok.type == TT_EOF)
      break;

    if(token_is_char(&tok, '\n'))
      continue;

    if(token_is_char(&tok, '\\') && tokenizer_peek(t) == '\n')
      continue;

    if(tok.type == TT_DQSTRING_LIT) {
      char *s = t->buf, buf[2] = {0};

      while(*s) {
        if(*s == '\"') {
          buffer_puts(out, "\\\"");
        } else if(*s == '\\') {
          buffer_puts(out, "\\\\");
        } else {
          buf[0] = *s;
          buffer_puts(out, buf);
        }

        ++s;
      }
    } else {
      emit_token(out, &tok, t->buf);
    }
  }

  buffer_puts(out, "\"");
  return 1;
}
