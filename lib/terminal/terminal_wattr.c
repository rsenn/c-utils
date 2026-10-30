#include "../terminal_internal.h"

#if WINDOWS_NATIVE
/* ANSI color order to console attribute bits, foreground position */
static WORD
color_bits(int c) {
  static const WORD ansi[8] = {0,
                               FOREGROUND_RED,
                               FOREGROUND_GREEN,
                               FOREGROUND_RED | FOREGROUND_GREEN,
                               FOREGROUND_BLUE,
                               FOREGROUND_RED | FOREGROUND_BLUE,
                               FOREGROUND_GREEN | FOREGROUND_BLUE,
                               FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE};

  return ansi[c & 7] | (c & TERMINAL_BRIGHT ? FOREGROUND_INTENSITY : 0);
}

/* terminal_wattr combines the colors and the style into one attribute, the
 * default colors are the ones the console had at terminal_init. */
void
terminal_wattr(struct terminal* t) {
  WORD fg = t->fg < 0 ? t->attr0 & 0x0f : color_bits(t->fg);
  WORD bg = t->bg < 0 ? (t->attr0 >> 4) & 0x0f : color_bits(t->bg);

  if(t->style & TERMINAL_BOLD)
    fg |= FOREGROUND_INTENSITY;

  if(t->style & TERMINAL_REVERSE) {
    WORD x = fg;

    fg = bg;
    bg = x;
  }
  SetConsoleTextAttribute((HANDLE)t->handle, fg | (bg << 4) | (t->style & TERMINAL_UNDERLINE ? COMMON_LVB_UNDERSCORE : 0));
}
#endif
