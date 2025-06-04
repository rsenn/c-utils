#include "lib/windoze.h"
#include "lib/buffer.h"
#include "lib/io_internal.h"
#include "lib/str.h"
#include "lib/stralloc.h"
#include "lib/strarray.h"
#include "lib/strlist.h"
#include "lib/set.h"

#if !WINDOWS_NATIVE
#include <unistd.h>
#endif

const char* debug_nl = "\n";

static char debug_space[BUFFER_INSIZE];
buffer debug_buffer = BUFFER_INIT(write, 2, debug_space, sizeof debug_space);
buffer* debug_buf = &debug_buffer;

void
debug_byte(const char* name, const char* x, size_t n) {
  buffer_puts(debug_buf, name);
  buffer_puts(debug_buf, ": ");
  buffer_put(debug_buf, x, n);
  buffer_putsflush(debug_buf, debug_nl);
}

void
debug_int(const char* name, int i) {
  buffer_puts(debug_buf, name);
  buffer_puts(debug_buf, ": ");
  buffer_putlong(debug_buf, i);
  buffer_putnlflush(debug_buf);
}

void
debug_long(const char* name, long value) {
  buffer_puts(debug_buf, name);
  buffer_puts(debug_buf, ": ");
  buffer_putlong(debug_buf, value);
  buffer_putnlflush(debug_buf);
}

void
debug_str(const char* name, const char* s) {
  buffer_puts(debug_buf, name);
  buffer_puts(debug_buf, s ? ": '" : ": ");
  buffer_puts(debug_buf, s ? s : "NULL");
  buffer_putsflush(debug_buf, s ? "'\n" : "\n");
}

void
debug_sa(const char* name, stralloc* sa) {
  buffer_puts(debug_buf, name);
  buffer_puts(debug_buf, ": ");
  buffer_putsa(debug_buf, sa);
  buffer_putnlflush(debug_buf);
}

void
debug_sl(const char* name, const strlist* l, const char* sep) {
  size_t pos, n;
  const char* x;
  buffer_puts(debug_buf, name);
  buffer_puts(debug_buf, ":\n  ");
  strlist_foreach(l, x, n) {
    if(n)
      buffer_puts(debug_buf, sep);
    buffer_puts(debug_buf, "#");
    buffer_putulong(debug_buf, n++);
    buffer_puts(debug_buf, ": ");
    buffer_put(debug_buf, x, n);
  }
  buffer_putnlflush(debug_buf);
}

void
debug_strarray(const char* name, const strarray* stra) {
  char** p;
  size_t n = 0;
  buffer_puts(debug_buf, name);
  buffer_puts(debug_buf, ":\n  ");
  strarray_foreach(stra, p) {
    if(n)
      buffer_puts(debug_buf, ", ");
    buffer_puts(debug_buf, "#");
    buffer_putulong(debug_buf, n++);
    buffer_puts(debug_buf, ": ");
    buffer_puts(debug_buf, *p ? *p : "(null)");
  }
  buffer_putnlflush(debug_buf);
}
