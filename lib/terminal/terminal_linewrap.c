#include "../terminal_internal.h"

void
terminal_linewrap(struct terminal* t, int on) {
#if WINDOWS_NATIVE
  DWORD mode;

  if(!GetConsoleMode((HANDLE)t->handle, &mode))
    return;
  SetConsoleMode((HANDLE)t->handle, on ? mode | ENABLE_WRAP_AT_EOL_OUTPUT : mode & ~(DWORD)ENABLE_WRAP_AT_EOL_OUTPUT);
#else
  int p = 7;

  terminal_seq(t, '?', &p, 1, on ? 'h' : 'l');
#endif
  t->nowrap = !on;
}
