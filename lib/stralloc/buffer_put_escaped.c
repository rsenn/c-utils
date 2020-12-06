#include "../buffer.h"
#include "../byte.h"
#include "../fmt.h"
#include "../stralloc.h"
#include <ctype.h>

int
buffer_put_escaped(buffer* b, const char* x, size_t len, size_t (*escape)(char*, int)) {
  int ret;
  stralloc e;
  if(len == 0)
    return 0;

  stralloc_init(&e);
  stralloc_fmt(&e, x, len, (stralloc_fmt_fn*)(void*)escape);
  ret = buffer_putsa(b, &e);
  stralloc_free(&e);
  return ret;
}
