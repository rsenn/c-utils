#include "../buffer.h"
#include "../stralloc.h"
#include "../byte.h"
#include "../fmt.h"
#include <ctype.h>

static int
escaped_pred(int c) {
  if(isalpha(c) || isdigit(c) || ispunct(c) || c == ' ') return 0;
  return 1;
}

int
buffer_puts_escaped(buffer* b, const char* x) {
  int ret;
  stralloc e;
  stralloc_init(&e);
  byte_fmt_pred(x, str_len(x), &e, &fmt_escapecharc, &escaped_pred);
  ret = buffer_putsa(b, &e);
  stralloc_free(&e);
  return ret;
}
