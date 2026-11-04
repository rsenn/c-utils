#define _GNU_SOURCE 1
#include "../lib/stralloc.h"
#include "unit_test.h"
#include "../lib/terminal.h"
#include "../lib/buffer.h"
#include "../lib/byte.h"
#include "../lib/terminal_internal.h"
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <string.h>

/* a terminal whose output is collected in a stralloc and whose input is a pipe */
struct fixture {
  stralloc sa;
  buffer b;
  struct terminal t;
  int p[2];
  size_t last;
};

static void
fx_open(struct fixture* f) {
  stralloc_init(&f->sa);
  pipe(f->p);
  buffer_tosa(&f->b, &f->sa);
  terminal_init(&f->t, f->p[0], f->p[1], &f->b);
  f->last = f->sa.len;
}

static void
fx_close(struct fixture* f) {
  close(f->p[0]);
  close(f->p[1]);
  stralloc_free(&f->sa);
}

/* what was written since the last call, as a NUL terminated string */
static const char*
fx_out(struct fixture* f) {
  static char out[256];
  size_t n;

  terminal_flush(&f->t);
  n = f->sa.len - f->last;
  if(n > sizeof(out) - 1)
    n = sizeof(out) - 1;
  byte_copy(out, n, f->sa.s + f->last);
  out[n] = '\0';
  f->last = f->sa.len;
  return out;
}

#define ASSERT_OUT(f, expected) ASSERT_STR_EQUAL(expected, fx_out(f))

/*
 * terminal_init, terminal_put, terminal_puts, terminal_putc, terminal_flush
 */
TEST(test_terminal_text) {
  struct fixture f;

  fx_open(&f);
  ASSERT_OUT(&f, "");
  terminal_puts(&f.t, "hi");
  ASSERT_OUT(&f, "hi");
  terminal_putc(&f.t, 'x');
  ASSERT_OUT(&f, "x");
  terminal_put(&f.t, "abc", 2);
  ASSERT_OUT(&f, "ab");
  terminal_put(&f.t, "\xc3", 1); /* a character may be split between calls */
  terminal_put(&f.t, "\xa9", 1);
  ASSERT_OUT(&f, "\xc3\xa9");
  fx_close(&f);
}

/*
 * terminal_goto, terminal_column, terminal_move, terminal_cursor_save,
 * terminal_cursor_restore, terminal_cursor_show
 */
TEST(test_terminal_cursor) {
  struct fixture f;

  fx_open(&f);
  terminal_goto(&f.t, 3, 4);
  ASSERT_OUT(&f, "\x1b[5;4H"); /* counted from 0, the sequence from 1 */
  terminal_column(&f.t, 7);
  ASSERT_OUT(&f, "\x1b[8G");
  terminal_move(&f.t, 2, 0);
  ASSERT_OUT(&f, "\x1b[2C");
  terminal_move(&f.t, -2, 0);
  ASSERT_OUT(&f, "\x1b[2D");
  terminal_move(&f.t, 0, 3);
  ASSERT_OUT(&f, "\x1b[3B");
  terminal_move(&f.t, 0, -3);
  ASSERT_OUT(&f, "\x1b[3A");
  terminal_move(&f.t, 0, 0);
  ASSERT_OUT(&f, "");
  terminal_cursor_save(&f.t);
  ASSERT_OUT(&f,
             "\x1b"
             "7");
  terminal_cursor_restore(&f.t);
  ASSERT_OUT(&f,
             "\x1b"
             "8");
  terminal_cursor_show(&f.t, 0);
  ASSERT_OUT(&f, "\x1b[?25l");
  terminal_cursor_show(&f.t, 1);
  ASSERT_OUT(&f, "\x1b[?25h");
  fx_close(&f);
}

/*
 * terminal_erase_display, terminal_erase_line, terminal_scroll,
 * terminal_alt_screen, terminal_linewrap
 */
TEST(test_terminal_screen) {
  struct fixture f;

  fx_open(&f);
  terminal_erase_display(&f.t, 0);
  ASSERT_OUT(&f, "\x1b[J");
  terminal_erase_display(&f.t, 2);
  ASSERT_OUT(&f, "\x1b[2J");
  terminal_erase_line(&f.t, 1);
  ASSERT_OUT(&f, "\x1b[1K");
  terminal_scroll(&f.t, 2);
  ASSERT_OUT(&f, "\x1b[2S");
  terminal_scroll(&f.t, -2);
  ASSERT_OUT(&f, "\x1b[2T");
  terminal_alt_screen(&f.t, 1);
  ASSERT_OUT(&f, "\x1b[?1049h");
  terminal_alt_screen(&f.t, 0);
  ASSERT_OUT(&f, "\x1b[?1049l");
  terminal_linewrap(&f.t, 0);
  ASSERT_OUT(&f, "\x1b[?7l");
  terminal_linewrap(&f.t, 1);
  ASSERT_OUT(&f, "\x1b[?7h");
  fx_close(&f);
}

/*
 * terminal_fg, terminal_bg, terminal_rgb_fg, terminal_rgb_bg, terminal_style, terminal_reset
 */
TEST(test_terminal_colors) {
  struct fixture f;

  fx_open(&f);
  terminal_fg(&f.t, TERMINAL_RED);
  ASSERT_OUT(&f, "\x1b[31m");
  terminal_fg(&f.t, TERMINAL_RED + TERMINAL_BRIGHT);
  ASSERT_OUT(&f, "\x1b[91m");
  terminal_fg(&f.t, TERMINAL_DEFAULT);
  ASSERT_OUT(&f, "\x1b[39m");
  terminal_bg(&f.t, TERMINAL_BLUE);
  ASSERT_OUT(&f, "\x1b[44m");
  terminal_bg(&f.t, TERMINAL_BLUE + TERMINAL_BRIGHT);
  ASSERT_OUT(&f, "\x1b[104m");
  terminal_bg(&f.t, TERMINAL_DEFAULT);
  ASSERT_OUT(&f, "\x1b[49m");
  terminal_rgb_fg(&f.t, 1, 2, 3);
  ASSERT_OUT(&f, "\x1b[38;2;1;2;3m");
  terminal_rgb_bg(&f.t, 4, 5, 6);
  ASSERT_OUT(&f, "\x1b[48;2;4;5;6m");
  terminal_style(&f.t, TERMINAL_BOLD | TERMINAL_UNDERLINE);
  ASSERT_OUT(&f, "\x1b[1;4m");
  terminal_style(&f.t, TERMINAL_REVERSE); /* exactly the flags given: the others go off */
  ASSERT_OUT(&f, "\x1b[22;24;7m");
  terminal_style(&f.t, 0);
  ASSERT_OUT(&f, "\x1b[27m");
  terminal_reset(&f.t);
  ASSERT_OUT(&f, "\x1b[m");
  fx_close(&f);
}

/*
 * terminal_getkey, terminal_pending
 */
TEST(test_terminal_getkey) {
  struct fixture f;
  struct terminal_key k;

  fx_open(&f);
  ASSERT_EQ(0, terminal_pending(&f.t));
  {
    static const char keys[] = "a\x1b[A\x1b[3~\x1bOP\x1b[1;5C\xc3\xa9\t\n\x1b";
    write(f.p[1], keys, sizeof(keys) - 1);
  }

  ASSERT_EQ(1, terminal_getkey(&f.t, &k));
  ASSERT_EQ('a', k.code);
  ASSERT_EQ(1, terminal_pending(&f.t) > 0);
  ASSERT_EQ(1, terminal_getkey(&f.t, &k));
  ASSERT_EQ(TERMINAL_KEY_UP, k.code);
  ASSERT_EQ(1, terminal_getkey(&f.t, &k));
  ASSERT_EQ(TERMINAL_KEY_DELETE, k.code);
  ASSERT_EQ(1, terminal_getkey(&f.t, &k));
  ASSERT_EQ(TERMINAL_KEY_F(1), k.code);
  ASSERT_EQ(1, terminal_getkey(&f.t, &k));
  ASSERT_EQ(TERMINAL_KEY_RIGHT, k.code);
  ASSERT_EQ(TERMINAL_CTRL, k.mods);
  ASSERT_EQ(1, terminal_getkey(&f.t, &k));
  ASSERT_EQ(0xe9, k.code); /* UTF-8 decoded to a code point */
  ASSERT_EQ(1, terminal_getkey(&f.t, &k));
  ASSERT_EQ(9, k.code);
  ASSERT_EQ(1, terminal_getkey(&f.t, &k));
  ASSERT_EQ(10, k.code);
  ASSERT_EQ(1, terminal_getkey(&f.t, &k));
  ASSERT_EQ(27, k.code); /* an escape with nothing behind it is the escape key */
  ASSERT_EQ(0, terminal_pending(&f.t));

  close(f.p[1]); /* end of input */
  ASSERT_EQ(0, terminal_getkey(&f.t, &k));
  f.p[1] = -1;
  close(f.p[0]);
  stralloc_free(&f.sa);
}

/*
 * terminal_size, terminal_cursor_position, terminal_info (not a terminal here)
 */
TEST(test_terminal_size_position) {
  struct fixture f;
  int cols = 0, rows = 0, x = -1, y = -1, r;

  fx_open(&f);
  ASSERT_EQ(-1, terminal_size(&f.t, &cols, &rows)); /* a pipe has no size */

  /* the answer to the query is in the input already */
  write(f.p[1], "\x1b[12;34R", 8);
  r = terminal_cursor_position(&f.t, &x, &y);
  ASSERT_EQ(0, r);
  ASSERT_EQ(33, x);
  ASSERT_EQ(11, y);
  ASSERT_OUT(&f, "\x1b[6n");
  fx_close(&f);
}

/*
 * terminal_seq (internal): ESC [ prefix p0 ; p1 ... final
 */
TEST(test_terminal_seq) {
  struct fixture f;
  int p[3] = {1, 22, 333};

  fx_open(&f);
  terminal_seq(&f.t, 0, p, 3, 'm');
  ASSERT_OUT(&f, "\x1b[1;22;333m");
  terminal_seq(&f.t, '?', p, 1, 'h');
  ASSERT_OUT(&f, "\x1b[?1h");
  terminal_seq(&f.t, 0, NULL, 0, 'H');
  ASSERT_OUT(&f, "\x1b[H");
  fx_close(&f);
}

/*
 * terminal_raw, terminal_restore, terminal_size on a real (pseudo) terminal
 */
TEST(test_terminal_raw_pty) {
  struct fixture f;
  struct termios tio;
  struct winsize ws;
  int master, slave, cols = 0, rows = 0;
  char* name;

  if((master = posix_openpt(O_RDWR | O_NOCTTY)) < 0 || grantpt(master) < 0 || unlockpt(master) < 0 ||
     (name = ptsname(master)) == NULL)
    return; /* no pseudo terminals in this environment */
  slave = open(name, O_RDWR | O_NOCTTY);
  ASSERT_EQ(1, slave >= 0);

  stralloc_init(&f.sa);
  buffer_tosa(&f.b, &f.sa);
  terminal_init(&f.t, slave, slave, &f.b);
  f.last = 0;

  ws.ws_col = 100;
  ws.ws_row = 30;
  ws.ws_xpixel = ws.ws_ypixel = 0;
  ioctl(slave, TIOCSWINSZ, &ws);
  ASSERT_EQ(0, terminal_size(&f.t, &cols, &rows));
  ASSERT_EQ(100, cols);
  ASSERT_EQ(30, rows);

  tcgetattr(slave, &tio);
  ASSERT_EQ(1, (tio.c_lflag & ICANON) != 0);
  ASSERT_EQ(0, terminal_raw(&f.t));
  tcgetattr(slave, &tio);
  ASSERT_EQ(0, tio.c_lflag & (ICANON | ECHO));
  terminal_restore(&f.t);
  tcgetattr(slave, &tio);
  ASSERT_EQ(1, (tio.c_lflag & ICANON) != 0);
  ASSERT_EQ(1, (tio.c_lflag & ECHO) != 0);

  close(slave);
  close(master);
  stralloc_free(&f.sa);
}

#define RUN_TERMINAL_TESTS() \
  RUN(test_terminal_text); \
  RUN(test_terminal_cursor); \
  RUN(test_terminal_screen); \
  RUN(test_terminal_colors); \
  RUN(test_terminal_getkey); \
  RUN(test_terminal_size_position); \
  RUN(test_terminal_seq); \
  RUN(test_terminal_raw_pty)

TESTS(terminal) { RUN_TERMINAL_TESTS(); }
