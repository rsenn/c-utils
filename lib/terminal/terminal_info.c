#include "../terminal_internal.h"

#if WINDOWS_NATIVE
int
terminal_info(struct terminal* t, CONSOLE_SCREEN_BUFFER_INFO* info) {
  return GetConsoleScreenBufferInfo((HANDLE)t->handle, info) != 0;
}
#endif
