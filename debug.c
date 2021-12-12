#include "lib/str.h"
#include "lib/stralloc.h"
#include "lib/strarray.h"
#include "lib/strlist.h"
#include "lib/set.h"

const char* debug_nl = "\n";

void
debug_byte(const char* name, const char* x, size_t n) {
  buffer_puts(buffer_2, name);
  buffer_puts(buffer_2, ": ");
  buffer_put(buffer_2, x, n);
  buffer_putsflush(buffer_2, debug_nl);
}

void
debug_int(const char* name, int i) {
  buffer_puts(buffer_2, name);
  buffer_puts(buffer_2, ": ");
  buffer_putlong(buffer_2, i);
  buffer_putnlflush(buffer_2);
}

void
debug_long(const char* name, long value) {
  buffer_puts(buffer_2, name);
  buffer_puts(buffer_2, ": ");
  buffer_putlong(buffer_2, value);
  buffer_putnlflush(buffer_2);
}

void
debug_str(const char* name, const char* s) {
  buffer_puts(buffer_2, name);
  buffer_puts(buffer_2, s ? ": '" : ": ");
  buffer_puts(buffer_2, s ? s : "NULL");
  buffer_putsflush(buffer_2, s ? "'\n" : "\n");
}

void
debug_sa(const char* name, stralloc* sa) {
  buffer_puts(buffer_2, name);
  buffer_puts(buffer_2, ": ");
  buffer_putsa(buffer_2, sa);
  buffer_putnlflush(buffer_2);
}

void
debug_set(const char* name, const set_t* s, const char* sep) {
  buffer_putm_internal(buffer_2, name, ": ", NULL);
  buffer_putset(buffer_2, s, sep, str_len(sep));
  buffer_putsflush(buffer_2, debug_nl);
}

void
debug_sl(const char* name, const strlist* l, const char* sep) {
  size_t pos, n;
  const char* x;
  buffer_puts(buffer_2, name);
  buffer_puts(buffer_2, ":\n  ");
  strlist_foreach(l, x, n) {
    if(n)
      buffer_puts(buffer_2, sep);
    buffer_puts(buffer_2, "#");
    buffer_putulong(buffer_2, n++);
    buffer_puts(buffer_2, ": ");
    buffer_put(buffer_2, x, n);
  }
  buffer_putnlflush(buffer_2);
}

void
debug_strarray(const char* name, const strarray* stra) {
  char** p;
  size_t n = 0;
  buffer_puts(buffer_2, name);
  buffer_puts(buffer_2, ":\n  ");
  strarray_foreach(stra, p) {
    if(n)
      buffer_puts(buffer_2, ", ");
    buffer_puts(buffer_2, "#");
    buffer_putulong(buffer_2, n++);
    buffer_puts(buffer_2, ": ");
    buffer_puts(buffer_2, *p ? *p : "(null)");
  }
  buffer_putnlflush(buffer_2);
}