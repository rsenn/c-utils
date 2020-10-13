#include "../buffer.h"
#include "../str.h"
#include "../fmt.h"

int
buffer_puts_escaped(buffer* b, const char* x, size_t (*escape)(char*, int)) {
  return buffer_put_escaped(b, x, str_len(x), escape ? escape : &fmt_escapecharshell);
}
