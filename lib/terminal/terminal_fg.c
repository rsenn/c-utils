#include "../terminal_internal.h"

void
terminal_fg(struct terminal* t, int color) {
  t->fg = color < 0 ? TERMINAL_DEFAULT : color & 15;
#if WINDOWS_NATIVE
  terminal_wattr(t);
#else
  {
    int p = 39;

    if(color >= 0)
      p = (color & TERMINAL_BRIGHT ? 90 : 30) + (color & 7);
    terminal_seq(t, 0, &p, 1, 'm');
  }
#endif
}
