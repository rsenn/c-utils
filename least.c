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

#include "terminal.h"


#define CSI "\x1b["

static strarray lines;
static int64 terminal_rows, terminal_cols, display_rows, first_line;

const char* filename;
buffer terminal;

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

/* All escape sequences go through buffer_1 so that ordering is preserved
   (terminal_*() without a buffer argument use a separate buffer). */
static void
goto_row(int64 row) {
  buffer_puts(buffer_1, CSI);
  buffer_putulonglong(buffer_1, row + 1);
  buffer_puts(buffer_1, ";1H");
}

static void
goto_col(int64 col) {
  buffer_puts(buffer_1, CSI);
  buffer_putulonglong(buffer_1, col < 1 ? 1 : col);
  buffer_putc(buffer_1, 'G');
}

void
read_terminal_properties(void) {
  terminal_cols = terminal_getwidth();
  terminal_rows = terminal_getheight();

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
    buffer_putspace(buffer_1);
    len++;
  }
  terminal_rgb_foreground(buffer_1, 143, 255, 245);
  buffer_put(buffer_1, buf, n);
  terminal_number_sequence(buffer_1, 0, 'm');
  return len;
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
    buffer_putspace(buffer_1);
    col++;
  }

  while(i < len) {
    unsigned char c = s[i];
    size_t n;

    if((n = scan_ansiskip(&s[i], len - i)) > 0) {
      buffer_put(buffer_1, &s[i], n);
      i += n;
    } else if(c >= 0x80 && c < 0xc0) {
      buffer_put(buffer_1, &s[i++], 1);
    } else if(c == '\t') {
      n = tab_size - col % tab_size;
      if(col + n > (size_t)terminal_cols)
        break;
      col += n;
      while(n--)
        buffer_putspace(buffer_1);
      i++;
    } else if(c < 0x20 || c == 0x7f) {
      if(col + 2 > (size_t)terminal_cols)
        break;
      buffer_putc(buffer_1, '^');
      buffer_putc(buffer_1, c == 0x7f ? '?' : c + 64);
      col += 2;
      i++;
    } else {
      if(col + 1 > (size_t)terminal_cols)
        break;
      buffer_put(buffer_1, &s[i++], 1);
      col++;
    }
  }
}

static void
print_status(void) {
  int64 range = max_first();

  goto_row(display_rows);
  buffer_puts(buffer_1, CSI "m" CSI "2K");
  buffer_puts(buffer_1, CSI "7m");
  buffer_puts(buffer_1, filename);
  buffer_putspace(buffer_1);

  if(first_line >= range) {
    buffer_puts(buffer_1, "(END)");
  } else {
    buffer_puts(buffer_1, "(line ");
    buffer_putulonglong(buffer_1, first_line + 1);
    buffer_putc(buffer_1, '/');
    buffer_putulonglong(buffer_1, line_count());
    buffer_putc(buffer_1, ')');
  }
  buffer_putspace(buffer_1);
  buffer_putulonglong(buffer_1, range > 0 ? first_line * 100 / range : 100);
  buffer_putc(buffer_1, '%');

  if(command_buf.len) {
    buffer_puts(buffer_1, " [");
    buffer_put(buffer_1, command_buf.s, command_buf.len);
    buffer_putc(buffer_1, ']');
  }
  buffer_puts(buffer_1, CSI "m");
  buffer_flush(buffer_1);
}

static void
print_added(size_t num_lines) {
  goto_row(display_rows);
  buffer_puts(buffer_1, CSI "m" CSI "2K" CSI "7m+");
  buffer_putulong(buffer_1, num_lines);
  buffer_puts(buffer_1, " lines" CSI "m");
  buffer_flush(buffer_1);
}

/* Redraw the whole screen without clearing it first (no flicker). */
static void
redraw(void) {
  int64 i;

  buffer_puts(buffer_1, CSI "?7l");

  for(i = 0; i < display_rows; i++) {
    goto_row(i);
    if(first_line + i < line_count())
      print_line(first_line + i);
    buffer_puts(buffer_1, CSI "m" CSI "K");
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

  if(!buffer_LEN(&terminal) || buffer_getc(&terminal, &c) != 1 || (c != '[' && c != 'O'))
    return;

  for(;;) {
    if(!buffer_LEN(&terminal) || buffer_getc(&terminal, &c) != 1)
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
  buffer_puts(buffer_1, CSI "s");
  goto_col(terminal_cols - 30 - (int64)command_buf.len);
  buffer_puts(buffer_1, CSI "0K" CSI "1m");
  terminal_rgb_background(buffer_1, 25, 73, 216);
  terminal_rgb_foreground(buffer_1, 208, 240, 248);
  buffer_puts(buffer_1, "↓ ");
  terminal_rgb_foreground(buffer_1, 230, 198, 5);
  buffer_putsa(buffer_1, &command_buf);

  if(matches) {
    terminal_rgb_foreground(buffer_1, 208, 240, 248);
    buffer_puts(buffer_1, " ✖");
    terminal_rgb_foreground(buffer_1, 230, 198, 5);
    buffer_putspace(buffer_1);
    buffer_putulong(buffer_1, matches);
    buffer_puts(buffer_1, " matches");
    terminal_rgb_foreground(buffer_1, 208, 240, 248);
    buffer_puts(buffer_1, " ⇲ ");
    terminal_rgb_foreground(buffer_1, 230, 198, 5);
    buffer_putulonglong(buffer_1, first + 1);
  } else {
    terminal_rgb_foreground(buffer_1, 208, 240, 248);
    buffer_puts(buffer_1, " ∞ ");
    terminal_rgb_foreground(buffer_1, 230, 198, 5);
    buffer_puts(buffer_1, " not found");
  }
  buffer_putspace(buffer_1);
  buffer_puts(buffer_1, CSI "m" CSI "u");
  buffer_flush(buffer_1);
}

static const char* command_prefixes[4] = {":", " :", "/", "\\"};

static void
draw_prompt(void) {
  goto_row(display_rows);
  buffer_puts(buffer_1, CSI "m" CSI "2K");
  buffer_puts(buffer_1, command_prefixes[command_mode & 0x03]);
  buffer_putsa(buffer_1, &command_buf);
  buffer_flush(buffer_1);
}

static void
read_command(void) {
  int mode = command_mode;
  unsigned char c;
  stralloc_zero(&command_buf);
  draw_prompt();

  while(command_mode == mode && buffer_getc(&terminal, &c) == 1) {
    if(c == '\n' || c == '\r')
      break;

    if(c == ESC) {
      /* cancel; swallow the rest of a key sequence, if any */
      buffer_SKIP(&terminal, buffer_LEN(&terminal));
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

  if(buffer_getc(&terminal, &c) < 1)
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
  buffer_putnlflush(buffer_1);

  terminal_restore();
  buffer_flush(buffer_1);

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

  buffer_read_fd(&terminal, STDOUT_FILENO);

  read_terminal_properties();
  read_content(&input, display_rows);

#if !WINDOWS_NATIVE
  {
    struct stat st;

    if(fstat(input.fd, &st) != -1)
      is_pipe = S_ISFIFO(st.st_mode);
  }
#endif

  terminal_init();
  terminal_set_alternate_screen();
  scroll_to(0);

  if(input.fd != STDIN_FILENO) {
#if !WINDOWS
    watchfd = inotify_init();
    wd = inotify_add_watch(watchfd, filename, IN_MODIFY);
#endif
  } else if(is_pipe) {
    watchfd = input.fd;
  }

  io_fd(terminal.fd);
  io_wantread(terminal.fd);

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
      if(fd == terminal.fd)
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
