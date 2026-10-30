#include "../terminal_internal.h"

/* terminal_raw keeps the signal keys, only line editing and echo go. A
 * console also gets the size change events. */
int
terminal_raw(struct terminal* t) {
#if WINDOWS_NATIVE
  DWORD mode;

  if(!GetConsoleMode((HANDLE)t->in, &mode))
    return -1;
  t->inmode = mode;
  if(!SetConsoleMode((HANDLE)t->in, (mode & ~(DWORD)(ENABLE_LINE_INPUT | ENABLE_ECHO_INPUT)) | ENABLE_WINDOW_INPUT))
    return -1;
#else
  struct termios raw;

  if(tcgetattr(t->infd, &t->saved) == -1)
    return -1;
  raw = t->saved;
  raw.c_lflag &= ~(ICANON | ECHO);
  raw.c_cc[VMIN] = 1;
  raw.c_cc[VTIME] = 0;
  if(tcsetattr(t->infd, TCSANOW, &raw) == -1)
    return -1;
#endif
  t->raw = 1;
  return 0;
}
