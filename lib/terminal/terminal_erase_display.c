#include "../terminal_internal.h"

void
terminal_erase_display(struct terminal* t, int mode) {
#if WINDOWS_NATIVE
  CONSOLE_SCREEN_BUFFER_INFO info;
  COORD from;
  DWORD n, cur, done;

  if(!terminal_info(t, &info))
    return;
  /* cells are counted from the window top over rows of the buffer width */
  cur = (DWORD)(info.dwCursorPosition.Y - info.srWindow.Top) * info.dwSize.X + info.dwCursorPosition.X;
  n = (DWORD)(info.srWindow.Bottom - info.srWindow.Top + 1) * info.dwSize.X;
  from.X = 0;
  from.Y = info.srWindow.Top;

  if(mode == 0) {
    from = info.dwCursorPosition;
    n -= cur;
  } else if(mode == 1)
    n = cur + 1;

  FillConsoleOutputCharacterW((HANDLE)t->handle, L' ', n, from, &done);
  FillConsoleOutputAttribute((HANDLE)t->handle, info.wAttributes, n, from, &done);
#else
  terminal_seq(t, 0, &mode, mode ? 1 : 0, 'J');
#endif
}
