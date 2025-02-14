#include "../set.h"
#include "../buffer.h"

void
buffer_putset(buffer* b,
              const set_t* set,
              const char* sep,
              size_t seplen) {
  char* x;
  size_t i = 0, n;
  set_iterator_t it;

  set_foreach(set, it, x, n) {
    while(n > 0 && x[n - 1] == '\0')
      n--;
    if(i++ > 0)
      buffer_put(b, sep, seplen);
    buffer_put(b, x, n);
  }
}
