#include "../terminal_internal.h"

void
terminal_reset(struct terminal* t) {
  t->fg = t->bg = TERMINAL_DEFAULT;
  t->style = 0;
#if WINDOWS_NATIVE
  terminal_wattr(t);
#else
  terminal_seq(t, 0, 0, 0, 'm');
#endif
}
