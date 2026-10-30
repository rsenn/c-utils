#include "../terminal_internal.h"

void
terminal_move(struct terminal* t, int dx, int dy) {
#if WINDOWS_NATIVE
  CONSOLE_SCREEN_BUFFER_INFO info;
  COORD c;

  if(!terminal_info(t, &info))
    return;
  c.X = (SHORT)(info.dwCursorPosition.X + dx);
  c.Y = (SHORT)(info.dwCursorPosition.Y + dy);
  if(c.X < info.srWindow.Left)
    c.X = info.srWindow.Left;
  if(c.X > info.srWindow.Right)
    c.X = info.srWindow.Right;
  if(c.Y < info.srWindow.Top)
    c.Y = info.srWindow.Top;
  if(c.Y > info.srWindow.Bottom)
    c.Y = info.srWindow.Bottom;
  SetConsoleCursorPosition((HANDLE)t->handle, c);
#else
  int p;

  if(dy) {
    p = dy < 0 ? -dy : dy;
    terminal_seq(t, 0, &p, 1, dy < 0 ? 'A' : 'B');
  }
  if(dx) {
    p = dx < 0 ? -dx : dx;
    terminal_seq(t, 0, &p, 1, dx < 0 ? 'D' : 'C');
  }
#endif
}
