#include "../terminal_internal.h"

void
terminal_alt_screen(struct terminal* t, int on) {
#if WINDOWS_NATIVE
  if(on && !t->alt) {
    HANDLE h = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, 0, CONSOLE_TEXTMODE_BUFFER, 0);

    if(h == INVALID_HANDLE_VALUE)
      return;
    SetConsoleActiveScreenBuffer(h);
    t->handle = h;
  } else if(!on && t->alt) {
    SetConsoleActiveScreenBuffer((HANDLE)t->main);
    CloseHandle((HANDLE)t->handle);
    t->handle = t->main;
  } else
    return;
#else
  int p = 1049;

  terminal_seq(t, '?', &p, 1, on ? 'h' : 'l');
#endif
  t->alt = on != 0;
}
