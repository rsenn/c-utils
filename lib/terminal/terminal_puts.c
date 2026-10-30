#include "../terminal_internal.h"
#include "../str.h"

void
terminal_puts(struct terminal* t, const char* s) {
  terminal_put(t, s, str_len(s));
}
