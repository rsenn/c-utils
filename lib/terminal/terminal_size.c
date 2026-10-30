#include "../terminal_internal.h"

#if !WINDOWS_NATIVE
#include <sys/ioctl.h>
#endif

int
terminal_size(struct terminal* t, int* cols, int* rows) {
#if WINDOWS_NATIVE
  CONSOLE_SCREEN_BUFFER_INFO info;

  if(!terminal_info(t, &info))
    return -1;
  *cols = info.srWindow.Right - info.srWindow.Left + 1;
  *rows = info.srWindow.Bottom - info.srWindow.Top + 1;
#else
  struct winsize w;

  if(ioctl(t->outfd, TIOCGWINSZ, &w) == -1 && ioctl(t->infd, TIOCGWINSZ, &w) == -1)
    return -1;
  *cols = w.ws_col;
  *rows = w.ws_row;
#endif
  return 0;
}
