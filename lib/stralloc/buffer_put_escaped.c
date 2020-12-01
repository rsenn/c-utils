#include "../buffer.h"
#include "../byte.h"
#include "../fmt.h"
#include "../stralloc.h"
#include <ctype.h>

/*static int
escaped_pred(int c) {
  if(c == '\'' || c == '"' || c == '$' || c < 0x20)
    return 1;
  if(isalpha(c) || isdigit(c) || ispunct(c) || c == ' ')
    return 0;
  return 1;
}*/

int
buffer_put_escaped(buffer* b, const char* x, size_t len, size_t (*escape)(char*, int)) {
  int ret;
  stralloc e;
  if(len == 0)
    return 0;

  stralloc_init(&e);
  stralloc_fmt(&e, x, len, (stralloc_fmt_fn*)(void*)&escape);
  ret = buffer_putsa(b, &e);
  stralloc_free(&e);
  return ret;
}
