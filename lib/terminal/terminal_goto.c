#include "../terminal_internal.h"

void
terminal_goto(struct terminal* t, int x, int y) {
#if WINDOWS_NATIVE
  CONSOLE_SCREEN_BUFFER_INFO info;
  COORD c;

  if(!terminal_info(t, &info))
    return;
  c.X = (SHORT)(info.srWindow.Left + x);
  c.Y = (SHORT)(info.srWindow.Top + y);
  SetConsoleCursorPosition((HANDLE)t->handle, c);
#else
  int p[2];

  p[0] = y + 1;
  p[1] = x + 1;
  terminal_seq(t, 0, p, 2, 'H');
#endif
}
