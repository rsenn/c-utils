#include "../terminal_internal.h"

void
terminal_column(struct terminal* t, int x) {
#if WINDOWS_NATIVE
  CONSOLE_SCREEN_BUFFER_INFO info;
  COORD c;

  if(!terminal_info(t, &info))
    return;
  c.X = (SHORT)(info.srWindow.Left + x);
  c.Y = info.dwCursorPosition.Y;
  SetConsoleCursorPosition((HANDLE)t->handle, c);
#else
  int p = x + 1;

  terminal_seq(t, 0, &p, 1, 'G');
#endif
}
