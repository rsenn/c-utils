#include "../terminal_internal.h"

void
terminal_cursor_restore(struct terminal* t) {
#if WINDOWS_NATIVE
  terminal_goto(t, t->savex, t->savey);
#else
  buffer_put(t->out, "\0338", 2);
#endif
}
