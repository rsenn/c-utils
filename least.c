#include "lib/uint64.h"
#include "lib/buffer.h"
#include "lib/alloc.h"
#include "lib/errmsg.h"
#include "lib/strarray.h"
#include "lib/scan.h"
#include "lib/str.h"
#include "lib/io.h"
#include "lib/seek.h"
#include "lib/sig.h"
#include "lib/scan.h"
#include "lib/unix.h"
#include "lib/fmt.h"
#include "lib/windoze.h"

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

#include "lib/terminal.h"

#define ESC 27


static strarray lines;
static int64 terminal_rows, terminal_cols, display_rows, first_line;

const char* filename;
buffer tty_in;
static struct terminal term;

static stralloc command_buf;
static int end_of_file, tab_size = 8;

volatile sig_atomic_t running = 1, reset = 0, resized = 0, command_mode = 0, line_numbers = 0;

static inline int64
line_count(void) {
  return strarray_size(&lines);
}

/* largest useful value of first_line: last page is full, 0 if all fits */
static inline int64
max_first(void) {
  int64 n = line_count();
  return n > display_rows ? n - display_rows : 0;
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

  if((ret = buffer_getline_sa(buf, &line)) > 0) {
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

/* Print line `number` clipped to the terminal width: tabs become spaces
   (aligned to tab stops), ANSI sequences cost no width, UTF-8 continuation
   bytes cost no width, other control characters are shown as ^X. */
static void
print_line(size_t number) {
  const char* s = line_at(number);
  size_t len = str_len(s), i = 0, col = 0;

  if(line_numbers) {
    col = print_linenumber(number + 1, number_width(line_count()));
    terminal_putc(&term, ' ');
    col++;
  }

  while(i < len) {
    unsigned char c = s[i];
    size_t n;

    if((n = scan_ansiskip(&s[i], len - i)) > 0) {
      apply_ansi(&s[i], n);
      i += n;
    } else if(c >= 0x80 && c < 0xc0) {
      terminal_put(&term, &s[i++], 1);
    } else if(c == '\t') {
      n = tab_size - col % tab_size;
      if(col + n > (size_t)terminal_cols)
        break;
      col += n;
      while(n--)
        terminal_putc(&term, ' ');
      i++;
    } else if(c < 0x20 || c == 0x7f) {
      if(col + 2 > (size_t)terminal_cols)
        break;
      terminal_putc(&term, '^');
      terminal_putc(&term, c == 0x7f ? '?' : c + 64);
      col += 2;
      i++;
    } else {
      if(col + 1 > (size_t)terminal_cols)
        break;
      terminal_put(&term, &s[i++], 1);
      col++;
    }
  }
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
  int64 i;

  terminal_linewrap(&term, 0);

  for(i = 0; i < display_rows; i++) {
    goto_row(i);
    if(first_line + i < line_count())
      print_line(first_line + i);
    terminal_reset(&term);
    terminal_erase_line(&term, 0);
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

#define scroll_up() scroll_by(-1)
#define scroll_down() scroll_by(1)
#define scroll_to_top() scroll_to(0)
#define scroll_to_bottom() scroll_to(max_first())

/* ESC [ A, ESC O A, ESC [ 5 ~, ESC [ 1 ; 5 A ... Everything that is not
   already buffered is a lone ESC and ignored (never block on it). */
static void
handle_escape_sequence(void) {
  unsigned long key = 0, mod = 0, *p = &key;
  char c;
  int64 page = display_rows, step = 1;

  if(!buffer_LEN(&tty_in) || buffer_getc(&tty_in, &c) != 1 || (c != '[' && c != 'O'))
    return;

  for(;;) {
    if(!buffer_LEN(&tty_in) || buffer_getc(&tty_in, &c) != 1)
      return;
    if(c >= '0' && c <= '9')
      *p = *p * 10 + (c - '0');
    else if(c == ';')
      p = &mod;
    else
      break;
  }

  switch(mod) {
    case 2 /*shift*/: page *= 10, step = 10; break;
    case 5 /*control*/: page *= 100, step = 100; break;
    case 3 /*alt*/: page = page / 10 ? page / 10 : 1; break;
  }

  if(c == '~') {
    switch(key) {
      case 1:
      case 7: c = 'H'; break;
      case 4:
      case 8: c = 'F'; break;
      case 5: c = 'P'; break;
      case 6: c = 'N'; break;
      default: return;
    }
  }

  switch(c) {
    case 'A': scroll_by(-step); break;
    case 'B': scroll_by(step); break;
    case 'H': scroll_to_top(); break;
    case 'F': scroll_to_bottom(); break;
    case 'P': scroll_by(-page); break;
    case 'N': scroll_by(page); break;
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
  unsigned char c;
  stralloc_zero(&command_buf);
  draw_prompt();

  while(command_mode == mode && buffer_getc(&tty_in, &c) == 1) {
    if(c == '\n' || c == '\r')
      break;

    if(c == ESC) {
      /* cancel; swallow the rest of a key sequence, if any */
      buffer_SKIP(&tty_in, buffer_LEN(&tty_in));
      command_mode = 0;
      break;
    }

    if(c == 0x7f || c == 8) {
      if(command_buf.len)
        command_buf.len--;
    } else if(c == 21) {
      command_buf.len = 0;
    } else if(c >= 0x20) {
      stralloc_catc(&command_buf, c);
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
  unsigned char c;
  int64 n;

  if(buffer_getc(&tty_in, &c) < 1)
    return 0;

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
    case ' ': scroll_by(display_rows * n); break;

    case 'b':
    case 'B':
    case 2: scroll_by(-display_rows * n); break;

    case 'd':
    case 'D':
    case 4: scroll_by(display_rows / 2 * n); break;

    case 'u':
    case 'U':
    case 21: scroll_by(-(display_rows / 2) * n); break;

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
    case ESC: handle_escape_sequence(); break;

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
  int watchfd = -1, wd, is_pipe = 0;

  sig_catch(SIGTERM, terminate);
  sig_catch(SIGINT, interrupt);
#ifdef SIGWINCH
  sig_catch(SIGWINCH, winsizechange);
#endif

  if(argc < 2) {
    buffer_read_fd(&input, STDIN_FILENO);
    filename = "(stdin)";
  } else {
    filename = argv[1];

    if(buffer_readfile(&input, filename))
      diesys(127, "Unable to open file ", filename);
  }

  buffer_read_fd(&tty_in, STDOUT_FILENO);
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

  io_fd(tty_in.fd);
  io_wantread(tty_in.fd);

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
      if(fd == tty_in.fd)
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
      handle_input();

    if(on_input) {
      uint64 newsize = 0;
      size_t prev_lines = line_count();
      ssize_t ret = 1;
      int do_scroll = first_line >= max_first();

      if(is_pipe) {
        /* never block on a partial line */
        if((ret = read_line(&input)) > 0)
          while(have_line(&input) && (ret = read_line(&input)) > 0) {}
      } else {
        if(!get_filesize(input.fd, &newsize))
          newsize = 0;
        while(get_position(&input) < newsize && (ret = read_line(&input)) > 0) {}
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
