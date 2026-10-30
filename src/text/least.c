#include "../../lib/uint64.h"
#include "../../lib/byte.h"
#include "../../lib/buffer.h"
#include "../../lib/alloc.h"
#include "../../lib/errmsg.h"
#include "../../lib/strarray.h"
#include "../../lib/scan.h"
#include "../../lib/str.h"
#include "../../lib/io.h"
#include "../../lib/seek.h"
#include "../../lib/sig.h"
#include "../../lib/scan.h"
#include "../../lib/unix.h"
#include "../../lib/fmt.h"
#include "../../lib/windoze.h"

#include <assert.h>
#include <ctype.h>
#include <errno.h>

#if !WINDOWS_NATIVE
#include <sys/stat.h>
#include <unistd.h>
#endif
#if !WINDOWS
#include <sys/inotify.h>
#endif

#include "../../lib/terminal.h"
#include "../../lib/ucs.h"
#include "../../lib/utf8.h"

#define ESC 27

static strarray lines;
static int64 terminal_rows, terminal_cols, display_rows, first_line;

const char* filename;
static struct terminal term;

static stralloc command_buf;
static int end_of_file, tab_size = 8, wrap = 1;
static int64 hscroll;

volatile sig_atomic_t running = 1, reset = 0, resized = 0, command_mode = 0, line_numbers = 0;

static inline int64
line_count(void) {
  return strarray_size(&lines);
}

static int64 line_rows(size_t i);

/* largest useful value of first_line: last page is full, 0 if all fits.
   A wrapped line takes more than one row. */
static int64
max_first(void) {
  int64 n = line_count(), rows = 0, i;

  if(!wrap)
    return n > display_rows ? n - display_rows : 0;

  for(i = n - 1; i >= 0; i--) {
    rows += line_rows(i);
    if(rows > display_rows)
      return i + 1 < n ? i + 1 : n - 1;
  }
  return 0;
}

static const char*
line_at(size_t i) {
  return strarray_AT(&lines, i);
}

/* All output goes through the terminal_* functions, so that ordering is
   preserved and no escape code is written by hand. */
static void
goto_row(int64 row) {
  terminal_goto(&term, 0, (int)row);
}

/* col counts from 1 */
static void
goto_col(int64 col) {
  terminal_column(&term, col < 1 ? 0 : (int)col - 1);
}

static void
put_number(unsigned long long n) {
  char buf[FMT_ULONG];

  terminal_put(&term, buf, fmt_ulonglong(buf, n));
}

void
read_terminal_properties(void) {
  int cols = 0, rows = 0;

  terminal_size(&term, &cols, &rows);
  terminal_cols = cols;
  terminal_rows = rows;

  if(terminal_cols < 1)
    terminal_cols = 80;
  if(terminal_rows < 2)
    terminal_rows = 24;
  display_rows = terminal_rows - 1;
}

int
read_line(buffer* buf) {
  ssize_t ret;
  stralloc line;
  stralloc_init(&line);

  ret = buffer_getline_sa(buf, &line);

  /* the last line of the input may have no newline */
  if(ret == 0 && line.len > 0)
    ret = 1;

  if(ret > 0) {
    char* s;
    ssize_t i;
    int underline = 0;
    stralloc_trimr(&line, "\r\n", 2);
    stralloc_nul(&line);
    s = line.s;
    stralloc_init(&line);

    for(i = 0; s[i]; i++) {
      if(s[i + 1] == 8 && s[i + 2] == s[i]) {
        if(!underline) {
          underline = 1;
          stralloc_cats(&line, "\x1b[4m");
        }
        i += 2;
      } else if(underline) {
        stralloc_cats(&line, "\x1b[0m");
        underline = 0;
      }
      stralloc_CATC(&line, s[i]);
    }

    if(underline)
      stralloc_cats(&line, "\x1b[0m");
    free(s);
    stralloc_nul(&line);
    array_catb((array*)&lines, &line.s, sizeof(char*));
    return 1;
  }
  stralloc_free(&line);
  return ret < 0 ? -1 : 0;
}

int
read_content(buffer* b, size_t max_lines) {
  size_t i;

  if(!end_of_file)
    for(i = 0; max_lines == 0 || i < max_lines; i++)
      if(read_line(b) <= 0) {
        end_of_file = 1;
        break;
      }
  return end_of_file;
}

static int
have_line(buffer* b) {
  return byte_chr(buffer_PEEK(b), buffer_LEN(b), '\n') < buffer_LEN(b);
}

static size_t
number_width(size_t n) {
  char buf[FMT_ULONG];
  return fmt_ulonglong(buf, n);
}

static size_t
print_linenumber(size_t number, size_t pad) {
  char buf[FMT_ULONG];
  size_t len, n = fmt_ulonglong(buf, number);
  len = n;

  while(pad-- > n) {
    terminal_putc(&term, ' ');
    len++;
  }
  terminal_rgb_fg(&term, 143, 255, 245);
  terminal_put(&term, buf, n);
  terminal_reset(&term);
  return len;
}

/* xterm 256 color index to a terminal color */
static void
set_color256(int bg, int n) {
  unsigned char r, g, b;

  if(n < 16) {
    if(bg)
      terminal_bg(&term, n);
    else
      terminal_fg(&term, n);
    return;
  }
  if(n < 232) {
    n -= 16;
    r = n / 36 ? (n / 36) * 40 + 55 : 0;
    g = (n / 6) % 6 ? ((n / 6) % 6) * 40 + 55 : 0;
    b = n % 6 ? (n % 6) * 40 + 55 : 0;
  } else
    r = g = b = 8 + 10 * (n - 232);
  if(bg)
    terminal_rgb_bg(&term, r, g, b);
  else
    terminal_rgb_fg(&term, r, g, b);
}

/* The text of a line may carry ANSI colors, such as the output of a
   colored diff. They are not copied to the terminal but carried out with
   the terminal_* functions, every other sequence is dropped. */
static void
apply_ansi(const char* s, size_t n) {
  int p[16], np = 0, i;
  size_t k;

  if(n < 3 || s[0] != 27 || s[1] != '[' || s[n - 1] != 'm')
    return;

  p[0] = 0;
  for(k = 2; k < n - 1; k++) {
    if(s[k] >= '0' && s[k] <= '9')
      p[np] = p[np] * 10 + (s[k] - '0');
    else if(s[k] == ';' && np < 15)
      p[++np] = 0;
  }
  np++;

  for(i = 0; i < np; i++) {
    int c = p[i];

    if(c == 0)
      terminal_reset(&term);
    else if(c == 1)
      terminal_style(&term, term.style | TERMINAL_BOLD);
    else if(c == 4)
      terminal_style(&term, term.style | TERMINAL_UNDERLINE);
    else if(c == 7)
      terminal_style(&term, term.style | TERMINAL_REVERSE);
    else if(c == 22)
      terminal_style(&term, term.style & ~TERMINAL_BOLD);
    else if(c == 24)
      terminal_style(&term, term.style & ~TERMINAL_UNDERLINE);
    else if(c == 27)
      terminal_style(&term, term.style & ~TERMINAL_REVERSE);
    else if(c >= 30 && c <= 37)
      terminal_fg(&term, c - 30);
    else if(c >= 90 && c <= 97)
      terminal_fg(&term, TERMINAL_BRIGHT | (c - 90));
    else if(c >= 40 && c <= 47)
      terminal_bg(&term, c - 40);
    else if(c >= 100 && c <= 107)
      terminal_bg(&term, TERMINAL_BRIGHT | (c - 100));
    else if(c == 39)
      terminal_fg(&term, TERMINAL_DEFAULT);
    else if(c == 49)
      terminal_bg(&term, TERMINAL_DEFAULT);
    else if((c == 38 || c == 48) && i + 2 < np && p[i + 1] == 5) {
      set_color256(c == 48, p[i + 2]);
      i += 2;
    } else if((c == 38 || c == 48) && i + 4 < np && p[i + 1] == 2) {
      if(c == 48)
        terminal_rgb_bg(&term, p[i + 2], p[i + 3], p[i + 4]);
      else
        terminal_rgb_fg(&term, p[i + 2], p[i + 3], p[i + 4]);
      i += 4;
    }
  }
}

/* A line is cut into units: an ANSI sequence has no width, a tab goes to
   the next tab stop, a control character is shown as ^X and takes 2, a
   UTF-8 character takes what wc_charwidth says (0 for a combining one). */
enum { U_TEXT, U_ANSI, U_TAB, U_CTRL };

struct unit {
  size_t n;
  int width, kind;
};

static void
next_unit(const char* s, size_t len, size_t col, struct unit* u) {
  unsigned char c = s[0];
  size_t n;
  uint32 code;

  u->n = 1;
  u->width = 1;
  u->kind = U_TEXT;

  if((n = scan_ansiskip(s, len)) > 0) {
    u->n = n > len ? len : n;
    u->width = 0;
    u->kind = U_ANSI;
  } else if(c == '\t') {
    u->width = tab_size - col % tab_size;
    u->kind = U_TAB;
  } else if(c < 0x20 || c == 0x7f) {
    u->width = 2;
    u->kind = U_CTRL;
  } else if(c >= 0x80 && (n = scan_utf8_ucs4(s, len, &code)) > 0) {
    u->n = n;
    u->width = wc_charwidth((wchar_t)code);
    if(u->width < 0)
      u->width = 0;
  }
}

/* the ANSI sequences applied so far in the line being drawn, they are
   applied again after the indent of a wrapped row */
static char sgr_hist[1024];
static size_t sgr_len;

struct pline {
  int64 row, avail, used, rowcol, tw, pw, lo, hi, col;
  int draw, shown;
};

/* spaces for the line number column, in the default colors */
static void
indent(struct pline* p) {
  size_t i = 0, n;

  terminal_reset(&term);
  for(n = 0; n < (size_t)p->pw; n++)
    terminal_putc(&term, ' ');
  while(i < sgr_len && (n = scan_ansiskip(&sgr_hist[i], sgr_len - i)) > 0) {
    apply_ansi(&sgr_hist[i], n);
    i += n;
  }
}

/* start the next row of a wrapped line, 0 if the page is full */
static int
newrow(struct pline* p) {
  if(p->used >= p->avail)
    return 0;
  p->used++;
  p->rowcol = 0;
  if(p->draw) {
    goto_row(p->row + p->used - 1);
    indent(p);
  }
  return 1;
}

/* Put the b[n] that is w cells wide. A wrapped line moves to the next row
   if it does not fit, and a wide character is not split. Without wrapping
   only the cells from lo to hi are shown, a wide character cut by the edge
   becomes a space. Returns 0 when there is nothing more to show. */
static int
put_cells(struct pline* p, const char* b, size_t n, int w) {
  int64 from, to, vis;

  if(wrap) {
    if(p->rowcol > 0 && p->rowcol + w > p->tw) {
      if(p->draw)
        for(; p->rowcol < p->tw; p->rowcol++)
          terminal_putc(&term, ' ');
      if(!newrow(p))
        return 0;
    }
    if(p->draw)
      terminal_put(&term, b, n);
    p->rowcol += w;
    p->col += w;
    p->shown = 1;
    return 1;
  }

  from = p->col > p->lo ? p->col : p->lo;
  to = p->col + w < p->hi ? p->col + w : p->hi;
  vis = to > from ? to - from : 0;

  if(p->draw) {
    if(vis == w)
      terminal_put(&term, b, n);
    else
      for(from = 0; from < vis; from++)
        terminal_putc(&term, ' ');
  }
  p->shown = vis == w;
  p->rowcol += vis;
  p->col += w;
  return p->col < p->hi;
}

/* Print line `number` from screen row `row`, at most `avail` rows. Without
   wrapping that is one row, the part from column hscroll. Returns the rows
   used. With draw 0 nothing is written, it only counts. */
static int64
print_line(size_t number, int64 row, int64 avail, int draw) {
  const char* s = line_at(number);
  size_t len = str_len(s), i = 0;
  struct pline p;

  p.row = row;
  p.avail = avail;
  p.used = 1;
  p.rowcol = p.col = p.shown = 0;
  p.draw = draw;
  p.pw = line_numbers ? (int64)number_width(line_count()) + 1 : 0;
  p.tw = terminal_cols > p.pw ? terminal_cols - p.pw : 1;
  p.lo = wrap ? 0 : hscroll;
  p.hi = wrap ? ((int64)1 << 60) : hscroll + p.tw;

  if(draw) {
    goto_row(row);
    sgr_len = 0;
    if(line_numbers) {
      print_linenumber(number + 1, number_width(line_count()));
      terminal_putc(&term, ' ');
    }
  }

  while(i < len) {
    struct unit u;
    int k;

    next_unit(&s[i], len - i, p.col, &u);

    switch(u.kind) {
      case U_ANSI:
        if(draw) {
          apply_ansi(&s[i], u.n);
          if(sgr_len + u.n <= sizeof(sgr_hist)) {
            byte_copy(sgr_hist + sgr_len, u.n, &s[i]);
            sgr_len += u.n;
          }
        }
        break;
      case U_TAB:
        for(k = 0; k < u.width; k++)
          if(!put_cells(&p, " ", 1, 1))
            goto done;
        break;
      case U_CTRL:
        if(!put_cells(&p, "^", 1, 1))
          goto done;
        if(!put_cells(&p, s[i] == 0x7f ? "?" : (char[]){s[i] + 64}, 1, 1))
          goto done;
        break;
      default:
        if(u.width == 0) {
          if(draw && p.shown)
            terminal_put(&term, &s[i], u.n);
        } else if(!put_cells(&p, &s[i], u.n, u.width))
          goto done;
    }
    i += u.n;
  }

done:
  if(draw) {
    terminal_reset(&term);
    if(p.rowcol < p.tw)
      terminal_erase_line(&term, 0);
  }
  return p.used;
}

static int64
line_rows(size_t i) {
  return wrap ? print_line(i, 0, (int64)1 << 40, 0) : 1;
}

/* the width of line i in cells */
static int64
line_width(size_t i) {
  const char* s = line_at(i);
  size_t len = str_len(s), pos = 0;
  int64 col = 0;

  while(pos < len) {
    struct unit u;

    next_unit(&s[pos], len - pos, col, &u);
    col += u.width;
    pos += u.n;
  }
  return col;
}

/* how far the page can be moved to the right: the longest line on it ends
   at the right edge */
static int64
max_hscroll(void) {
  int64 i, w, widest = 0, tw = terminal_cols - (line_numbers ? (int64)number_width(line_count()) + 1 : 0);

  for(i = first_line; i < first_line + display_rows && i < line_count(); i++)
    if((w = line_width(i)) > widest)
      widest = w;
  return widest > tw ? widest - tw : 0;
}

/* the lines that fill one page from first_line on, or back from it */
static int64
page_lines(int back) {
  int64 i, rows = 0, count = 0, r;

  if(!wrap)
    return display_rows;

  if(back) {
    for(i = first_line - 1; i >= 0; i--) {
      if(rows + (r = line_rows(i)) > display_rows)
        break;
      rows += r;
      count++;
    }
  } else {
    for(i = first_line; i < line_count(); i++) {
      if(rows + (r = line_rows(i)) > display_rows)
        break;
      rows += r;
      count++;
    }
  }
  return count ? count : 1;
}

static void
print_status(void) {
  int64 range = max_first();

  goto_row(display_rows);
  terminal_reset(&term);
  terminal_erase_line(&term, 2);
  terminal_style(&term, TERMINAL_REVERSE);
  terminal_puts(&term, filename);
  terminal_putc(&term, ' ');

  if(first_line >= range) {
    terminal_puts(&term, "(END)");
  } else {
    terminal_puts(&term, "(line ");
    put_number(first_line + 1);
    terminal_putc(&term, '/');
    put_number(line_count());
    terminal_putc(&term, ')');
  }
  terminal_putc(&term, ' ');
  put_number(range > 0 ? first_line * 100 / range : 100);
  terminal_putc(&term, '%');

  if(!wrap && hscroll > 0) {
    terminal_puts(&term, " col ");
    put_number(hscroll + 1);
  }

  if(command_buf.len) {
    terminal_puts(&term, " [");
    terminal_put(&term, command_buf.s, command_buf.len);
    terminal_putc(&term, ']');
  }
  terminal_reset(&term);
  terminal_flush(&term);
}

static void
print_added(size_t num_lines) {
  goto_row(display_rows);
  terminal_reset(&term);
  terminal_erase_line(&term, 2);
  terminal_style(&term, TERMINAL_REVERSE);
  terminal_putc(&term, '+');
  put_number(num_lines);
  terminal_puts(&term, " lines");
  terminal_reset(&term);
  terminal_flush(&term);
}

/* Redraw the whole screen without clearing it first (no flicker). */
static void
redraw(void) {
  int64 row = 0, i = first_line, range;

  terminal_linewrap(&term, 0);

  if(wrap)
    hscroll = 0;
  else if(hscroll > (range = max_hscroll()))
    hscroll = range;

  while(row < display_rows) {
    if(i < line_count())
      row += print_line(i++, row, display_rows - row, 1);
    else {
      goto_row(row++);
      terminal_reset(&term);
      terminal_erase_line(&term, 2);
    }
  }
  print_status();
}

void
scroll_to(int64 line) {
  int64 range = max_first();

  if(line > range)
    line = range;
  if(line < 0)
    line = 0;
  first_line = line;
  redraw();
}

void
scroll_by(int64 offset) {
  int64 to = first_line + offset, range = max_first();

  if(to > range)
    to = range;
  if(to < 0)
    to = 0;
  if(to != first_line)
    scroll_to(to);
}

/* move the page to the right, or to the left if offset is negative */
void
scroll_h(int64 offset) {
  int64 to = hscroll + offset, range = wrap ? 0 : max_hscroll();

  if(to > range)
    to = range;
  if(to < 0)
    to = 0;
  if(to != hscroll) {
    hscroll = to;
    redraw();
  }
}

#define scroll_up() scroll_by(-1)
#define scroll_down() scroll_by(1)
#define scroll_to_top() scroll_to(0)
#define scroll_to_bottom() scroll_to(max_first())

/* The arrow, page and home keys scroll. Shift scrolls 10 times as much,
   control 100 times, alt a tenth of a page. Left and right move the page
   sideways in 8 columns when lines are not wrapped. */
static void
handle_named_key(const struct terminal_key* k) {
  int64 mult = 1, page;

  if(k->mods & TERMINAL_CTRL)
    mult = 100;
  else if(k->mods & TERMINAL_SHIFT)
    mult = 10;

  switch(k->code) {
    case TERMINAL_KEY_UP: scroll_by(-mult); break;
    case TERMINAL_KEY_DOWN: scroll_by(mult); break;
    case TERMINAL_KEY_LEFT: scroll_h(-8 * mult); break;
    case TERMINAL_KEY_RIGHT: scroll_h(8 * mult); break;
    case TERMINAL_KEY_HOME: scroll_to_top(); break;
    case TERMINAL_KEY_END: scroll_to_bottom(); break;
    case TERMINAL_KEY_PAGEUP:
    case TERMINAL_KEY_PAGEDOWN:
      page = page_lines(k->code == TERMINAL_KEY_PAGEUP);
      if(k->mods & TERMINAL_ALT)
        page = page / 10 ? page / 10 : 1;
      else
        page *= mult;
      scroll_by(k->code == TERMINAL_KEY_PAGEUP ? -page : page);
      break;
  }
}

/* 1-based line number; negative counts from the end (-1 = last line) */
static void
jump_command(int64 to) {
  if(to < 0)
    to += line_count() + 1;
  scroll_to(to - 1);
}

static void
read_goto(const char* cmd) {
  double to;
  size_t pos;

  if((pos = scan_double(cmd, &to))) {
    if(cmd[pos] == '%')
      scroll_to((int64)(max_first() * to / 100));
    else
      jump_command((int64)to);
  }
}

int
match_pattern(const char* pattern, const char* string) {
  return str_contains(string, pattern) || unix_fnmatch(pattern, string, FNM_CASEFOLD) == 0;
}

int
nomatch_pattern(const char* pattern, const char* string) {
  return !match_pattern(pattern, string);
}

/* first matching line after first_line (wrapping around), or -1 */
static int64
find_line(const char* cmd, int (*predicate)(const char*, const char*)) {
  int64 i, n = line_count();

  for(i = 1; i <= n; i++) {
    int64 l = (first_line + i) % n;

    if(predicate(cmd, line_at(l)))
      return l;
  }
  return -1;
}

static size_t
count_matches(const char* cmd, int (*predicate)(const char*, const char*)) {
  size_t count = 0, i, n = line_count();

  for(i = 0; i < n; i++)
    if(predicate(cmd, line_at(i)))
      count++;
  return count;
}

static void
search_update(int (*predicate)(const char*, const char*)) {
  int64 first;
  size_t matches = 0;
  stralloc_nul(&command_buf);

  if((first = find_line(command_buf.s, predicate)) >= 0)
    matches = count_matches(command_buf.s, predicate);
  terminal_cursor_save(&term);
  goto_col(terminal_cols - 30 - (int64)command_buf.len);
  terminal_erase_line(&term, 0);
  terminal_style(&term, TERMINAL_BOLD);
  terminal_rgb_bg(&term, 25, 73, 216);
  terminal_rgb_fg(&term, 208, 240, 248);
  terminal_puts(&term, "↓ ");
  terminal_rgb_fg(&term, 230, 198, 5);
  terminal_put(&term, command_buf.s, command_buf.len);

  if(matches) {
    terminal_rgb_fg(&term, 208, 240, 248);
    terminal_puts(&term, " ✖");
    terminal_rgb_fg(&term, 230, 198, 5);
    terminal_putc(&term, ' ');
    put_number(matches);
    terminal_puts(&term, " matches");
    terminal_rgb_fg(&term, 208, 240, 248);
    terminal_puts(&term, " ⇲ ");
    terminal_rgb_fg(&term, 230, 198, 5);
    put_number(first + 1);
  } else {
    terminal_rgb_fg(&term, 208, 240, 248);
    terminal_puts(&term, " ∞ ");
    terminal_rgb_fg(&term, 230, 198, 5);
    terminal_puts(&term, " not found");
  }
  terminal_putc(&term, ' ');
  terminal_reset(&term);
  terminal_cursor_restore(&term);
  terminal_flush(&term);
}

static const char* command_prefixes[4] = {":", " :", "/", "\\"};

static void
draw_prompt(void) {
  goto_row(display_rows);
  terminal_reset(&term);
  terminal_erase_line(&term, 2);
  terminal_puts(&term, command_prefixes[command_mode & 0x03]);
  terminal_put(&term, command_buf.s, command_buf.len);
  terminal_flush(&term);
}

static void
read_command(void) {
  int mode = command_mode;
  struct terminal_key k;
  stralloc_zero(&command_buf);
  draw_prompt();

  while(command_mode == mode && terminal_getkey(&term, &k) == 1) {
    if(k.code == '\n' || k.code == '\r')
      break;

    if(k.code == ESC) {
      command_mode = 0;
      break;
    }

    if(k.code == 0x7f || k.code == 8) {
      /* remove a whole UTF-8 character */
      while(command_buf.len && (command_buf.s[command_buf.len - 1] & 0xc0) == 0x80)
        command_buf.len--;
      if(command_buf.len)
        command_buf.len--;
    } else if(k.code == 21) {
      command_buf.len = 0;
    } else if(k.code >= 0x20 && k.code < 0x110000) {
      char u[4];

      stralloc_catb(&command_buf, u, fmt_ucs4_utf8(u, k.code));
    } else {
      continue;
    }

    draw_prompt();

    if(mode >= 2)
      search_update(mode == 2 ? &match_pattern : &nomatch_pattern);
  }

  if(command_mode == mode) {
    stralloc_nul(&command_buf);

    switch(mode) {
      case 1: read_goto(command_buf.s); break;
      case 2:
      case 3: {
        int64 found = find_line(command_buf.s, mode == 2 ? &match_pattern : &nomatch_pattern);

        if(found >= 0)
          scroll_to(found);
        else
          print_status();
        break;
      }
    }
  }
  command_mode = 0;
  stralloc_zero(&command_buf);
  print_status();
}

/* digits typed so far -> count, 1 if none */
static int64
get_number(void) {
  int64 x = 1;

  if(command_buf.len) {
    stralloc_nul(&command_buf);
    scan_longlong(command_buf.s, &x);
    stralloc_zero(&command_buf);
  }
  return x;
}

int
handle_input(void) {
  struct terminal_key k;
  unsigned c;
  int64 n;

  if(terminal_getkey(&term, &k) < 1)
    return 0;

  if(k.code == TERMINAL_KEY_RESIZE) {
    resized = 1;
    return 1;
  }

  if(k.code >= 0x110000) {
    handle_named_key(&k);
    print_status();
    return 1;
  }
  c = k.code;

  if(c >= '0' && c <= '9') {
    stralloc_catc(&command_buf, c);
    print_status();
    return 1;
  }

  if(command_buf.len && (c == 0x7f || c == 8)) {
    command_buf.len--;
    print_status();
    return 1;
  }

  n = get_number();

  switch(c) {
    case 'q':
    case 'Q': running = 0; break;

    case 'e':
    case 5:
    case 'j':
    case '\n':
    case '\r': scroll_by(n); break;

    case 'y':
    case 25:
    case 'k':
    case '\v': scroll_by(-n); break;

    case 'f':
    case 6:
    case 22:
    case ' ': scroll_by(page_lines(0) * n); break;

    case 'b':
    case 'B':
    case 2: scroll_by(-page_lines(1) * n); break;

    case 'd':
    case 'D':
    case 4: scroll_by((page_lines(0) / 2 ? page_lines(0) / 2 : 1) * n); break;

    case 'u':
    case 'U':
    case 21: scroll_by(-(page_lines(1) / 2 ? page_lines(1) / 2 : 1) * n); break;

    case 'h': scroll_h(-8 * n); break;
    case 'l': scroll_h(8 * n); break;

    case 'w':
      wrap = !wrap;
      hscroll = 0;
      scroll_to(first_line);
      break;

    case 'g': scroll_to_top(); break;
    case 'G': scroll_to_bottom(); break;

    case 'r':
    case 'R':
    case 12: redraw(); break;

    case 'n':
    case 'N':
    case 14:
      line_numbers = !line_numbers;
      scroll_to(first_line);
      break;

    case ':':
      command_mode = 1;
      read_command();
      break;
    case '/':
      command_mode = 2;
      read_command();
      break;
    case '\\':
    case '<':
      command_mode = 3;
      read_command();
      break;
    case ESC: break;

    default: print_status(); return 0;
  }

  if(running)
    print_status();
  return 1;
}

int
get_filesize(int fd, uint64* size) {
  seek_pos save, end;
  int ret = 0;
#if !WINDOWS_NATIVE
  struct stat st;

  if(fstat(fd, &st) != -1 && (*size = st.st_size))
    return 1;
#endif
  save = seek_cur(fd);

  if((*size = seek_end(fd)))
    ret = 1;
  seek_set(fd, save);
  return ret;
}

size_t
get_position(buffer* b) {
  size_t pos = seek_cur(b->fd);
  pos -= buffer_LEN(b);
  return pos;
}

void
interrupt(int sig) {
  command_mode = 0;
  reset = 1;
}

void
terminate(int sig) {
  terminal_putc(&term, '\n');
  terminal_restore(&term);

  exit(0);
}

void
winsizechange(int sig) {
  resized = 1;
}

int
main(int argc, char* argv[]) {
  buffer input;
  int watchfd = -1, wd, is_pipe = 0, argi = 1;

  sig_catch(SIGTERM, terminate);
  sig_catch(SIGINT, interrupt);
#ifdef SIGWINCH
  sig_catch(SIGWINCH, winsizechange);
#endif

  /* -S does not wrap long lines, -N shows line numbers */
  while(argi < argc && argv[argi][0] == '-' && argv[argi][1]) {
    const char* f;

    for(f = argv[argi] + 1; *f; f++) {
      if(*f == 'S')
        wrap = 0;
      else if(*f == 'N')
        line_numbers = 1;
      else
        die(2, "usage: least [-SN] [file]", 0);
    }
    argi++;
  }

  if(argi >= argc) {
    buffer_read_fd(&input, STDIN_FILENO);
    filename = "(stdin)";
  } else {
    filename = argv[argi];

    if(buffer_readfile(&input, filename))
      diesys(127, "Unable to open file ", filename);
  }

  terminal_init(&term, STDOUT_FILENO, STDOUT_FILENO, buffer_1);

  read_terminal_properties();
  read_content(&input, display_rows);

#if !WINDOWS_NATIVE
  {
    struct stat st;

    if(fstat(input.fd, &st) != -1)
      is_pipe = S_ISFIFO(st.st_mode);
  }
#endif

  terminal_raw(&term);
  terminal_alt_screen(&term, 1);
  scroll_to(0);

  if(input.fd != STDIN_FILENO) {
#if !WINDOWS
    watchfd = inotify_init();
    wd = inotify_add_watch(watchfd, filename, IN_MODIFY);
#endif
  } else if(is_pipe) {
    watchfd = input.fd;
  }

  io_fd(term.infd);
  io_wantread(term.infd);

  if(watchfd != -1) {
    io_fd(watchfd);
    io_wantread(watchfd);
  }

  while(running) {
    fd_type fd;
    int on_terminal = 0, on_input = 0;

    if(!is_pipe)
      read_content(&input, 0);

    while(io_wait() == -1) {
      if(errno == EINTR) {
        if(resized) {
          resized = 0;
          read_terminal_properties();
          reset = 1;
        }
        if(reset) {
          scroll_to(first_line);
          reset = 0;
        }
        continue;
      }

      diesys(2, "io_wait");
    }

    while((fd = io_canread()) != -1) {
      if(fd == term.infd)
        on_terminal = 1;

      if(fd == watchfd) {
#if !WINDOWS
        struct inotify_event event;

        if(watchfd != input.fd) {
          if(read(watchfd, &event, sizeof(event)) <= 0)
            continue;
        }
#endif
        on_input = 1;
      }
    }

    if(on_terminal)
      do
        handle_input();
      while(terminal_pending(&term) > 0);
    if(resized) {
      resized = 0;
      read_terminal_properties();
      scroll_to(first_line);
    }

    if(on_input) {
      uint64 newsize = 0;
      size_t prev_lines = line_count();
      ssize_t ret = 1;
      int do_scroll = first_line >= max_first();

      if(is_pipe) {
        /* never block on a partial line */
        if((ret = read_line(&input)) > 0)
          while(have_line(&input) && (ret = read_line(&input)) > 0) {
          }
      } else {
        if(!get_filesize(input.fd, &newsize))
          newsize = 0;
        while(get_position(&input) < newsize && (ret = read_line(&input)) > 0) {
        }
      }

      if(ret <= 0 && is_pipe) {
        end_of_file = 1;
        io_dontwantread(watchfd);
      }

      if(line_count() > prev_lines) {
        if(do_scroll)
          scroll_to(max_first());
        else
          print_added(line_count() - prev_lines);
      }
    }
  }
  terminate(0);

  strarray_free(&lines);
  buffer_close(&input);
  return 0;
}
