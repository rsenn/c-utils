#include "lib/buffer.h"
#include "lib/fmt.h"

void
dump_sep() {
  buffer_putspace(buffer_2);
  buffer_flush(buffer_2);
}
void
dump_newline() {
  buffer_putnlflush(buffer_2);
}
void
dump_str(const char* str) {
  if(str)
    buffer_puts(buffer_2, str);
}
void
dump_key(const char* str) {
  if(str) {
    dump_str(str);
    dump_str("=");
  }
}
void
dump_long(long num) {
  dump_str("0");
  buffer_put8long(buffer_2, num);
}
void
dump_long_n(long num, int n) {
  buffer_putlong0(buffer_2, num, n);
}
void
dump_quote() {
  dump_str("\"");
}

void
dump_char(char c) {
  if(c == '\n')
    dump_str("\\n");
  else if(c == '\r')
    dump_str("\\r");
  else if(c == '\t')
    dump_str("\\t");
  else if(c == '\0')
    dump_str("\\0");
  else if(c == ' ')
    dump_str("\\x20");
  else
    buffer_putc(buffer_2, c);
}
void
dump_bytes(const char* x, size_t n) {
  size_t i;
  char buf[FMT_ULONG];
  for(i = 0; i < n; i++) {
    if(isspace(x[i]) || isprint(x[i])) {
      dump_char(x[i]);
    } else {
      buffer_puts(buffer_2, "\\");
      buffer_put(buffer_2, buf, fmt_ulong0(buf, x[i], 3));
    }
  }
}

void
dump_ints(const int* x, size_t n) {
  size_t i;
  buffer_puts(buffer_2, "[");
  for(i = 0; i < n; i++) {
    if(x[i] == -1)
      break;
    if(i > 0)
      dump_str(",");
    buffer_putlong(buffer_2, x[i]);
  }
  buffer_puts(buffer_2, "]");
  dump_sep();
}
