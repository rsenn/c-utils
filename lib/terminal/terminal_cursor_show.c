#include "../terminal_internal.h"

void
terminal_cursor_show(struct terminal* t, int show) {
#if WINDOWS_NATIVE
  CONSOLE_CURSOR_INFO info;

  if(!GetConsoleCursorInfo((HANDLE)t->handle, &info))
    return;
  info.bVisible = show != 0;
  SetConsoleCursorInfo((HANDLE)t->handle, &info);
#else
  int p = 25;

  terminal_seq(t, '?', &p, 1, show ? 'h' : 'l');
#endif
  t->hidden = !show;
}
