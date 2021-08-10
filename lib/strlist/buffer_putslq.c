#include "../buffer.h"
#include "../strlist.h"

void
buffer_putslq(buffer* out, const strlist* sl, char delim, char quote) {
  const char* x;
  size_t i = 0, n;
  strlist_foreach(sl, x, n) {
    int q = byte_chr(x, n, delim) < n;
    if(i)
      buffer_put(out, &delim, 1);

    if(q)
      buffer_put(out, &quote, 1);
    buffer_put(out, x, n);
    if(q)
      buffer_put(out, &quote, 1);
    ++i;
  }
}
