#include "../terminal_internal.h"

void
terminal_scroll(struct terminal* t, int n) {
#if WINDOWS_NATIVE
  CONSOLE_SCREEN_BUFFER_INFO info;
  SMALL_RECT r;
  CHAR_INFO fill;
  COORD to;

  if(!n || !terminal_info(t, &info))
    return;
  r = info.srWindow;
  to.X = r.Left;
  to.Y = (SHORT)(r.Top - n);
  fill.Char.UnicodeChar = L' ';
  fill.Attributes = info.wAttributes;
  ScrollConsoleScreenBufferW((HANDLE)t->handle, &r, &r, to, &fill);
#else
  int p = n < 0 ? -n : n;

  if(n)
    terminal_seq(t, 0, &p, 1, n < 0 ? 'T' : 'S');
#endif
}
