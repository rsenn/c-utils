#include "../terminal_internal.h"

int
terminal_flush(struct terminal* t) {
#if WINDOWS_NATIVE
  (void)t;
  return 0;
#else
  return buffer_flush(t->out);
#endif
}
