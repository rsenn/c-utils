#include "../terminal_internal.h"

void
terminal_putc(struct terminal* t, char c) {
  terminal_put(t, &c, 1);
}
