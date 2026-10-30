#include "../terminal_internal.h"

void
terminal_erase_line(struct terminal* t, int mode) {
#if WINDOWS_NATIVE
  CONSOLE_SCREEN_BUFFER_INFO info;
  COORD from;
  DWORD n, done;
  SHORT left = 0;

  if(!terminal_info(t, &info))
    return;
  left = info.srWindow.Left;
  from.Y = info.dwCursorPosition.Y;
  from.X = left;
  n = (DWORD)(info.srWindow.Right - left + 1);

  if(mode == 0) {
    from.X = info.dwCursorPosition.X;
    n = (DWORD)(info.srWindow.Right - from.X + 1);
  } else if(mode == 1)
    n = (DWORD)(info.dwCursorPosition.X - left + 1);

  FillConsoleOutputCharacterW((HANDLE)t->handle, L' ', n, from, &done);
  FillConsoleOutputAttribute((HANDLE)t->handle, info.wAttributes, n, from, &done);
#else
  terminal_seq(t, 0, &mode, mode ? 1 : 0, 'K');
#endif
}
