/**
 * @defgroup   terminal
 * @brief      Terminal output through function calls.
 * @{
 */
#ifndef TERMINAL_H
#define TERMINAL_H

#include "buffer.h"
#include "windoze.h"

#if !WINDOWS_NATIVE
#include <termios.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* All output goes through these functions, never as escape codes in the
 * text. A POSIX terminal gets VT sequences through the buffer. A native
 * Windows console is driven by console API calls and ignores the buffer. */

/* colors are in ANSI order on every platform, add TERMINAL_BRIGHT for the
 * bright ones, TERMINAL_DEFAULT is the color the terminal started with */
#define TERMINAL_DEFAULT -1
#define TERMINAL_BLACK 0
#define TERMINAL_RED 1
#define TERMINAL_GREEN 2
#define TERMINAL_YELLOW 3
#define TERMINAL_BLUE 4
#define TERMINAL_MAGENTA 5
#define TERMINAL_CYAN 6
#define TERMINAL_WHITE 7
#define TERMINAL_BRIGHT 8

/* flags for terminal_style */
#define TERMINAL_BOLD 1
#define TERMINAL_UNDERLINE 2
#define TERMINAL_REVERSE 4

/* a key is a Unicode code point, control keys as their control byte: tab
 * is 9, enter 10, escape 27, backspace 127, control-a 1. The named keys
 * are above the Unicode range. */
#define TERMINAL_KEY_UP 0x110001
#define TERMINAL_KEY_DOWN 0x110002
#define TERMINAL_KEY_RIGHT 0x110003
#define TERMINAL_KEY_LEFT 0x110004
#define TERMINAL_KEY_HOME 0x110005
#define TERMINAL_KEY_END 0x110006
#define TERMINAL_KEY_PAGEUP 0x110007
#define TERMINAL_KEY_PAGEDOWN 0x110008
#define TERMINAL_KEY_INSERT 0x110009
#define TERMINAL_KEY_DELETE 0x11000a
#define TERMINAL_KEY_RESIZE 0x11000b
#define TERMINAL_KEY_F(n) (0x110100 + (n))

/* modifiers of a key */
#define TERMINAL_SHIFT 1
#define TERMINAL_ALT 2
#define TERMINAL_CTRL 4

struct terminal_key {
  unsigned code, mods;
};

struct terminal {
  buffer* out;
  int infd, outfd;
  unsigned raw : 1, alt : 1, nowrap : 1, hidden : 1;
  int fg, bg;
  unsigned style;
#if WINDOWS_NATIVE
  void *in, *handle, *main;
  unsigned long inmode;
  unsigned short attr0;
  short savex, savey;
  char tail[4];
  unsigned ntail;
  unsigned short hi; /* high surrogate of a key waiting for its pair */
#else
  struct termios saved;
  unsigned char inbuf[64];
  unsigned inpos, inlen;
#endif
};

/* terminal_init prepares t for the descriptors infd and outfd. out is the
 * buffer text and sequences are written to, on Windows it is not used.
 * Returns 0, or -1 if a Windows descriptor is no console. */
int terminal_init(struct terminal* t, int infd, int outfd, buffer* out);

/* terminal_raw turns off line editing and echo of the input, returns 0 or
 * -1. terminal_restore undoes it and leaves the terminal as it was found:
 * normal screen, wrap on, cursor shown, colors reset. */
int terminal_raw(struct terminal* t);
void terminal_restore(struct terminal* t);

/* terminal_getkey stores the next key. Returns 1, 0 if there is none
 * (end of input, or on Windows only events that are no key) and -1 on
 * error. A POSIX terminal blocks in read when nothing is buffered, so
 * wait for the readability of t->infd first, and call again while
 * terminal_pending says more is buffered. An escape sequence is decoded
 * only from what is already buffered, an ESC with nothing behind it is the
 * escape key and never waits. A Windows console reports a size change as
 * TERMINAL_KEY_RESIZE, a POSIX terminal signals it with SIGWINCH. */
int terminal_getkey(struct terminal* t, struct terminal_key* key);

/* terminal_pending is the number of buffered input bytes, on Windows the
 * number of pending console events */
int terminal_pending(struct terminal* t);

/* terminal_flush makes everything written so far visible */
int terminal_flush(struct terminal* t);

/* terminal_put writes n bytes of UTF-8 text, a character may be split
 * between two calls */
void terminal_put(struct terminal* t, const char* s, size_t n);
void terminal_puts(struct terminal* t, const char* s);
void terminal_putc(struct terminal* t, char c);

/* terminal_size stores the visible columns and rows, returns 0 or -1 */
int terminal_size(struct terminal* t, int* cols, int* rows);

/* terminal_goto moves the cursor to column x of row y, both counted from
 * 0 at the top left of the visible window */
void terminal_goto(struct terminal* t, int x, int y);
void terminal_column(struct terminal* t, int x);

/* terminal_move moves the cursor right by dx and down by dy, a negative
 * value moves left or up, it stops at the window edge */
void terminal_move(struct terminal* t, int dx, int dy);

void terminal_cursor_save(struct terminal* t);
void terminal_cursor_restore(struct terminal* t);
void terminal_cursor_show(struct terminal* t, int show);

/* terminal_cursor_position stores the cursor column and row from 0, a
 * POSIX terminal is asked and may not answer. Returns 0 or -1. */
int terminal_cursor_position(struct terminal* t, int* x, int* y);

/* terminal_erase_display clears 0 from the cursor to the end, 1 from the
 * start to the cursor, 2 everything. terminal_erase_line is the same for
 * the line. The cursor does not move. */
void terminal_erase_display(struct terminal* t, int mode);
void terminal_erase_line(struct terminal* t, int mode);

/* terminal_scroll moves the window contents up by n lines, down if n is
 * negative, and leaves blank lines behind */
void terminal_scroll(struct terminal* t, int n);

void terminal_alt_screen(struct terminal* t, int on);
void terminal_linewrap(struct terminal* t, int on);

/* terminal_fg and terminal_bg set a color from 0 to 15 or TERMINAL_DEFAULT.
 * terminal_rgb_fg and terminal_rgb_bg set a 24 bit color, a Windows
 * console shows the nearest of 16. */
void terminal_fg(struct terminal* t, int color);
void terminal_bg(struct terminal* t, int color);
void terminal_rgb_fg(struct terminal* t, unsigned char r, unsigned char g, unsigned char b);
void terminal_rgb_bg(struct terminal* t, unsigned char r, unsigned char g, unsigned char b);

/* terminal_style sets exactly the TERMINAL_* style flags in flags */
void terminal_style(struct terminal* t, unsigned flags);

/* terminal_reset sets default colors and no style */
void terminal_reset(struct terminal* t);

#ifdef __cplusplus
}
#endif

#endif /* defined(TERMINAL_H) */
/** @} */
