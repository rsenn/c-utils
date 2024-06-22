#include "../cpp_internal.h"

int
cpp_stringify(cpp_t* ccp, tokenizer* t, buffer* output) {
  int ret = 1;
  token tok;
  buffer_puts(output, "\"");
  while(1) {
    ret = tokenizer_next(t, &tok);
    if(!ret)
      return ret;
    if(tok.type == TT_EOF)
      break;
    if(is_char(&tok, '\n'))
      continue;
    if(is_char(&tok, '\\') && tokenizer_peek(t) == '\n')
      continue;
    if(tok.type == TT_DQSTRING_LIT) {
      char* s = t->buf;
      char buf[2] = {0};
      while(*s) {
        if(*s == '\"') {
          buffer_puts(output, "\\\"");
        } else if(*s == '\\') {
          buffer_puts(output, "\\\\");
        } else {
          buf[0] = *s;
          buffer_puts(output, buf);
        }
        ++s;
      }
    } else
      emit_token(output, &tok, t->buf);
  }
  buffer_puts(output, "\"");
  return ret;
}
