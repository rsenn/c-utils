#include "../terminal_internal.h"

/* terminal_style switches off what is not in flags first, so that the
 * colors stay. */
void
terminal_style(struct terminal* t, unsigned flags) {
#if WINDOWS_NATIVE
  t->style = flags;
  terminal_wattr(t);
#else
  int p[4], n = 0;

  if((t->style & ~flags & TERMINAL_BOLD))
    p[n++] = 22;
  if((t->style & ~flags & TERMINAL_UNDERLINE))
    p[n++] = 24;
  if((t->style & ~flags & TERMINAL_REVERSE))
    p[n++] = 27;
  if((flags & ~t->style & TERMINAL_BOLD))
    p[n++] = 1;
  if((flags & ~t->style & TERMINAL_UNDERLINE))
    p[n++] = 4;
  if((flags & ~t->style & TERMINAL_REVERSE))
    p[n++] = 7;
  if(n)
    terminal_seq(t, 0, p, n, 'm');
  t->style = flags;
#endif
}
