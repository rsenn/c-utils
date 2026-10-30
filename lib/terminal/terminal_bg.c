#include "../terminal_internal.h"

void
terminal_bg(struct terminal* t, int color) {
  t->bg = color < 0 ? TERMINAL_DEFAULT : color & 15;
#if WINDOWS_NATIVE
  terminal_wattr(t);
#else
  {
    int p = 49;

    if(color >= 0)
      p = (color & TERMINAL_BRIGHT ? 100 : 40) + (color & 7);
    terminal_seq(t, 0, &p, 1, 'm');
  }
#endif
}
