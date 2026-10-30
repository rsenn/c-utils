#include "../terminal_internal.h"

void
terminal_cursor_save(struct terminal* t) {
#if WINDOWS_NATIVE
  CONSOLE_SCREEN_BUFFER_INFO info;

  if(!terminal_info(t, &info))
    return;
  t->savex = info.dwCursorPosition.X - info.srWindow.Left;
  t->savey = info.dwCursorPosition.Y - info.srWindow.Top;
#else
  buffer_put(t->out, "\0337", 2);
#endif
}
