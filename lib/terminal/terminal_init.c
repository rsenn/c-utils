#include "../terminal_internal.h"
#include "../byte.h"

#if WINDOWS_NATIVE
#include <io.h>
#endif

int
terminal_init(struct terminal* t, int infd, int outfd, buffer* out) {
  byte_zero(t, sizeof(*t));
  t->out = out;
  t->infd = infd;
  t->outfd = outfd;
  t->fg = t->bg = TERMINAL_DEFAULT;

#if WINDOWS_NATIVE
  {
    CONSOLE_SCREEN_BUFFER_INFO info;

    t->in = (HANDLE)_get_osfhandle(infd);
    t->handle = t->main = (HANDLE)_get_osfhandle(outfd);

    if(!terminal_info(t, &info))
      return -1;
    t->attr0 = info.wAttributes;
  }
#endif
  return 0;
}
