#include "../buffer.h"
#include "../stralloc.h"
#include "../byte.h"
#include "../fmt.h"
#include <ctype.h>

static int
escaped_pred(int c) {
  if(c == '\'' || c == '"' || c == '$' || c < 0x20) return 1;
  if(isalpha(c) || isdigit(c) || ispunct(c)  || c == ' ') return 0;
  return 1;
}

int
buffer_put_escaped(buffer* b, const char* x, size_t len) {
  int ret;
  stralloc e;
  stralloc_init(&e);
  stralloc_fmt_pred(x, len, &e, &fmt_escapecharshell, &escaped_pred);
  ret = buffer_putsa(b, &e);
  stralloc_free(&e);
  return ret;
}
