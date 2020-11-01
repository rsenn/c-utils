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
debug_long( const char* name, long value) {
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
  buffer_putm_internal(buffer_2, name, ": ", 0);
  buffer_putset(buffer_2, s, sep, str_len(sep));
  buffer_putsflush(buffer_2, debug_nl);
}


void
debug_sl(const char* name, const strlist* l, const char* sep) {
  size_t pos, n;
  const char* x;
  stralloc tmp;
  stralloc_init(&tmp);
  strlist_foreach(l, x, n) {
    if(tmp.len)
      stralloc_cats(&tmp, sep ? sep : "\n");
    if((pos = byte_rchr(x, n, '/')) < n || (pos = byte_rchr(x, n, '\\')) < n)
      stralloc_catb(&tmp, x + pos + 1, n - pos - 1);
    else
      stralloc_catb(&tmp, x, n);
  }
  debug_sa(name, &tmp);
  stralloc_free(&tmp);
}

void
debug_strarray(const char* name, strarray* stra) {
  stralloc sa;
  stralloc_init(&sa);
  strarray_joins(stra, &sa, ",\n  ");
  buffer_puts(buffer_2, name);
  buffer_puts(buffer_2, ":\n  ");
  buffer_putsa(buffer_2, &sa);
  buffer_putnlflush(buffer_2);
  stralloc_free(&sa);
}
