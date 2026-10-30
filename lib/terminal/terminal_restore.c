#include "../terminal_internal.h"

void
terminal_restore(struct terminal* t) {
  if(t->alt)
    terminal_alt_screen(t, 0);
  if(t->nowrap)
    terminal_linewrap(t, 1);
  if(t->hidden)
    terminal_cursor_show(t, 1);
  terminal_reset(t);
  terminal_flush(t);

  if(t->raw) {
#if WINDOWS_NATIVE
    SetConsoleMode((HANDLE)t->in, t->inmode);
#else
    tcsetattr(t->infd, TCSANOW, &t->saved);
#endif
    t->raw = 0;
  }
}
