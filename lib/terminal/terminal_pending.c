#include "../terminal_internal.h"

int
terminal_pending(struct terminal* t) {
#if WINDOWS_NATIVE
  DWORD n;

  return GetNumberOfConsoleInputEvents((HANDLE)t->in, &n) ? (int)n : 0;
#else
  return (int)(t->inlen - t->inpos);
#endif
}
