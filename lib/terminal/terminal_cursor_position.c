#include "../terminal_internal.h"

#if !WINDOWS_NATIVE
#include <poll.h>
#include <unistd.h>
#endif

int
terminal_cursor_position(struct terminal* t, int* x, int* y) {
#if WINDOWS_NATIVE
  CONSOLE_SCREEN_BUFFER_INFO info;

  if(!terminal_info(t, &info))
    return -1;
  *x = info.dwCursorPosition.X - info.srWindow.Left;
  *y = info.dwCursorPosition.Y - info.srWindow.Top;
  return 0;
#else
  /* ask with ESC [ 6 n and read the answer ESC [ row ; col R */
  struct pollfd pfd;
  char c;
  int p = 6, n[2] = {0, 0}, i = 0, started = 0;

  terminal_seq(t, 0, &p, 1, 'n');
  terminal_flush(t);
  pfd.fd = t->infd;
  pfd.events = POLLIN;

  for(;;) {
    if(poll(&pfd, 1, 300) <= 0 || read(t->infd, &c, 1) != 1)
      return -1;
    if(c == 27)
      started = 1;
    else if(!started || c == '[')
      continue;
    else if(c >= '0' && c <= '9')
      n[i] = n[i] * 10 + (c - '0');
    else if(c == ';' && i == 0)
      i = 1;
    else if(c == 'R' && i == 1)
      break;
    else
      return -1;
  }
  *y = n[0] - 1;
  *x = n[1] - 1;
  return 0;
#endif
}
