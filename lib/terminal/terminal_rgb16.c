#include "../terminal_internal.h"

/* terminal_rgb16 maps a channel at 128 or more to its ANSI color bit and
 * makes the color bright if a channel reaches 192. */
int
terminal_rgb16(unsigned char r, unsigned char g, unsigned char b) {
  int c = (r >= 128 ? 1 : 0) | (g >= 128 ? 2 : 0) | (b >= 128 ? 4 : 0);

  if(r >= 192 || g >= 192 || b >= 192)
    c |= TERMINAL_BRIGHT;
  return c;
}
