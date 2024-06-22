#include "../cpp_internal.h"

void
cpp_error_or_warning(const char* err, const char* type, tokenizer* t, token* curr) {
  int i;
  unsigned column = curr ? curr->column : t->column;
  unsigned line = curr ? curr->line : t->line;

  buffer_putm_internal(buffer_2, "<", t->filename, "> ", NULL);
  buffer_putulong(buffer_2, line);
  buffer_putc(buffer_2, ':');
  buffer_putulong(buffer_2, column);
  buffer_putm_internal(buffer_2, " ", type, ": '", err, "'\n", t->buf, "\n", NULL);

  /*  dprintf(2, "<%s> %u:%u %s: '%s'\n", t->filename, line, column, type, err);
    dprintf(2, "%s\n", t->buf);*/
  for(i = 0; i < str_len(t->buf); i++)
    buffer_puts(buffer_2, "^");

  buffer_putnlflush(buffer_2);
}
