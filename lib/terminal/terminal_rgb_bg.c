#include "../terminal_internal.h"

void
terminal_rgb_bg(struct terminal* t, unsigned char r, unsigned char g, unsigned char b) {
#if WINDOWS_NATIVE
  terminal_bg(t, terminal_rgb16(r, g, b));
#else
  int p[5] = {48, 2, r, g, b};

  terminal_seq(t, 0, p, 5, 'm');
#endif
}
