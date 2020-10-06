#include "../set.h"
#include "../buffer.h"

void
set_dump(buffer* b, const set_t* set) {
  char* x;
  size_t i = 0, n;
  buffer_puts(b, "{ ");
  set_foreach(set, x, n) {

    if(i++ > 0)
      buffer_puts(b, ", ");

    buffer_put(b, x, n);
  }
  buffer_putsflush(b, " }\n");
}
