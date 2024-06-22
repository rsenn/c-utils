#include "../cpp_internal.h"

int
cpp_parse_error(tokenizer* t, int is_error) {
  int ws_count, ret;
  token tmp;

  if(!(ret = tokenizer_skip_chars(t, " \t", &ws_count)))
    return ret;

  tmp.column = t->column;
  tmp.line = t->line;

  ret = tokenizer_read_until(t, "\n", 1);

  if(is_error) {
    error(t->buf, t, &tmp);
    return 0;
  }

  warning(t->buf, t, &tmp);
  return 1;
}
