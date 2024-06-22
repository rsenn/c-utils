#include "../cpp_internal.h"

void
cpp_location(tokenizer* t, buffer* out) {
  unsigned column = t->column;
  unsigned line = t->line;

  buffer_puts(out, t->filename);
  buffer_putc(out, ':');
  buffer_putulong(out, line);
  buffer_putc(out, ':');
  buffer_putulong(out, column);
}
