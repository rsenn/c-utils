#include "../buffer.h"
#include "../str.h"
#include "../fmt.h"

int
buffer_puts_escaped(buffer* b, const char* x, size_t (*escape)(char*, unsigned int)) {
  if(escape == NULL)
    escape = &fmt_escapecharshell;
  return buffer_put_escaped(b, x, str_len(x), escape);
}
