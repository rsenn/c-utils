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

#define scroll_page_up() scroll_by(-display_rows)
#define scroll_page_down() scroll_by(display_rows)
#define scroll_to_top() scroll_to(0)
#define scroll_to_bottom() scroll_to(line_range() - 1)
#define last_line() (first_line + display_rows)

static strarray lines;
static int64 terminal_rows, terminal_cols, display_rows, first_line, match_index;

const char* filename;
buffer terminal;

static stralloc command_buf;
static int end_of_file, tab_size = 8;

volatile sig_atomic_t running = 1, reset = 0, resized = 0, command_mode = 0, line_numbers = 0;

static inline size_t
line_count(void) {
  return strarray_size(&lines);
}

static inline int64
line_range(void) {
  return line_count() - display_rows;
}

static const char*
line_at(size_t i) {
  return strarray_at(&lines, i);
}

size_t
line_numbytes(const char* s, size_t maxwidth) {
  size_t len = str_len(s);
  size_t i = 0, width = 0;
  while(i < len) {
    size_t n;
    if((n = scan_ansiskip(&s[i], len - i)) > 0) {
      i += n;
      continue;
    }
    if(s[i] == '\t')
      width += tab_size;
    else
      width++;

    if(width > maxwidth)
      break;
    i++;
  }
  return i;
}

void
read_terminal_properties(void) {
  terminal_cols = terminal_getwidth();
  terminal_rows = terminal_getheight();
  display_rows = terminal_rows - 1;
}

int
read_line(buffer* buf) {
  size_t bytes = buffer_LEN(buf);
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
      if(s[i] == '\t') {
        int j;
        //  for(j = 0; j < tab_size; j++)
        stralloc_CATC(&line, '\t');
        continue;
      }
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
    stralloc_init(&line);
    return 1;
  } else {
    ret = buffer_LEN(buf) - bytes;
  }
  stralloc_free(&line);
  return ret;
}

int
read_content(buffer* b, size_t max_lines) {
  size_t i;
  if(!end_of_file)
    for(i = 0; max_lines == 0 || i < max_lines; i++)
      if(!read_line(b)) {
        end_of_file = 1;
        break;
      }
  return end_of_file;
}

void
print_progress() {
  float progress = (double)first_line * 100 / (line_range() - 1);
  buffer_putdouble(buffer_1, progress, 3);
  buffer_putc(buffer_1, '%');
}

void
print_position() {
  buffer_puts(buffer_1, "(line ");
  buffer_putulonglong(buffer_1, first_line + 1);
  buffer_putc(buffer_1, '/');
  buffer_putulonglong(buffer_1, line_count());
  buffer_putc(buffer_1, ')');
}

size_t
number_width(size_t n) {
  char buf[FMT_ULONG];
  return fmt_ulonglong(buf, n);
}

size_t
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

void
print_line(size_t number) {
  const char* s;
  size_t len, pos = 0;
  if(line_numbers) {
    pos = print_linenumber(number + 1, number_width(last_line()));
    buffer_putspace(buffer_1);
  }
  s = line_at(number);
  len = line_numbytes(s, terminal_cols - pos - 1);
  buffer_put(buffer_1, s, len);
}

void
print_status() {
  buffer_putc(buffer_1, '\r');
  terminal_erase_in_line(0);
  if(command_mode) {
    buffer_puts(buffer_1, command_mode == 1 ? ":" : "/");
    buffer_putsa(buffer_1, &command_buf);
  } else {
    terminal_number_sequence(buffer_1, 7, 'm');
    buffer_puts(buffer_1, filename);
    buffer_putspace(buffer_1);
    if(first_line >= line_range() - 1)
      buffer_puts(buffer_1, "(END)");
    else
      print_position();
    buffer_putspace(buffer_1);
    print_progress();
    terminal_number_sequence(buffer_1, 0, 'm');
  }
  buffer_flush(buffer_1);
}

void
print_added(size_t num_lines) {
  terminal_erase_in_line(1);
  buffer_putc(buffer_1, '\r');
  terminal_number_sequence(buffer_1, 7, 'm');
  buffer_puts(buffer_1, "+");
  buffer_putulong(buffer_1, num_lines);
  buffer_puts(buffer_1, " lines");
  terminal_number_sequence(buffer_1, 0, 'm');
  buffer_flush(buffer_1);
}

void
scroll_up(void) {
  if(first_line <= 0)
    return;
  terminal_scroll_down(1);
  terminal_goto_xy(0, 0);
  //  terminal_erase_in_line(2);
  print_line(--first_line);
  buffer_flush(buffer_1);
  terminal_goto_xy(terminal_rows - 1, 0);
  print_status();
}

void
scroll_down(void) {
  if(first_line >= line_range() - 1)
    return;
  terminal_scroll_up(1);
  terminal_cursor_previous_line(0);
  terminal_erase_in_line(2);
  print_line(++first_line + display_rows - 1);
  buffer_putc(buffer_1, '\n');
  print_status();
}

void
scroll_to(int64 line) {
  int64 i;
  size_t num_lines = line_count();
  i = num_lines - display_rows;

  if(i >= 1)
    first_line = line % i;
  else
    first_line = line;
  if(first_line < 0)
    first_line += i;
  terminal_erase_in_display(2);
  terminal_goto_xy(0, 0);
  terminal_linewrap_disable();
  for(i = 0; i < display_rows; i++) {
    if(line + i < num_lines)
      print_line(line + i);
    buffer_puts(buffer_1, "\x1b[m\n");
  }
  print_status();
}

void
scroll_by(int offset) {
  int64 to, range;
  to = first_line + offset;
  range = line_range();
  if(to < 0)
    to = 0;
  else if(to >= range)
    to = range - 1;
  if(to != first_line)
    scroll_to(to);
}

size_t
handle_key_modifier(buffer* b) {
  unsigned long key = 0, modifier = 0;
  size_t i = 0;
  float multiplier = 1;

  if(buffer_LEN(b) && (i = scan_ulong(buffer_PEEK(b), &key))) {
    buffer_SEEK(b, i);
    if(buffer_LEN(b) > 1 && *buffer_PEEK(b) == ';') {
      buffer_SEEK(b, 1);
      i = scan_ulong(buffer_PEEK(b), &modifier);
      buffer_SEEK(b, i);
    }
    if(buffer_LEN(b) && *buffer_PEEK(b) == '~')
      buffer_SEEK(b, 1);
    switch(modifier) {
      case 2 /*shift*/: multiplier = 10; break;
      case 5 /*control*/: multiplier = 100; break;
      case 3 /*alt*/: multiplier = 0.1; break;
    }
    switch(key) {
      case 1: scroll_to_top(); break;
      case 4: scroll_to_bottom(); break;
      case 5: scroll_by((float)-display_rows * multiplier); break;
      case 6: scroll_by((float)display_rows * multiplier); break;
    }
  }
  return i;
}

ssize_t
handle_escape_sequence(void) {
  ssize_t r;
  char c1, c2, c3;
  if((r = buffer_getc(&terminal, &c1)) == 1) {
    if(c1 == 'O' && (r = buffer_getc(&terminal, &c2)) == 1) {
      switch(c2) {
        case 'A': scroll_up(); break;
        case 'B': scroll_down(); break;
        case 'H': scroll_to_top(); break;
        case 'F': scroll_to_bottom(); break;
      }
    } else if(c1 == '[' && (r = buffer_peekc(&terminal, &c2)) == 1) {
      if(isdigit(c2)) {
        r = handle_key_modifier(&terminal);
      } else {
        r = buffer_skipc(&terminal);
        switch(c2) {
          case 'A': scroll_up(); break;
          case 'B': scroll_down(); break;
          case 'H': scroll_to_top(); break;
          case 'F': scroll_to_bottom(); break;
        }
      }
    }
  }
  if(r < 0)
    errmsg_warnsys("reading input", 0);
  return r;
}

void
jump_command(int64 to) {
  size_t range = line_range();
  if(to < 0 && ((to = range + to + 1) < 0))
    to = 0;
  if(to > range)
    to = range - 1;
  scroll_to(to);
  command_mode = 0;
  buffer_putc(buffer_1, '\r');
  print_status();
}

void
read_goto(const char* cmd) {
  double to;
  size_t pos;
  if((pos = scan_double(cmd, &to))) {
    if(cmd[pos] == '%') {
      to = (line_range() - 1) * to / 100;
      to = ((int)to * display_rows) / display_rows;
    }
    jump_command((int)to);
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

int64
find_line(const char* cmd, int (*predicate)(const char*, const char*)) {
  size_t i, n = line_count();
  for(i = 0; i < n; i++) {
    const char* s = line_at((first_line + i) % n);
    if(predicate(cmd, s))
      return first_line + i;
  }
  return -1;
}

size_t
count_matches(const char* cmd, int (*predicate)(const char*, const char*)) {
  size_t count = 0, i, n = line_count();
  for(i = 0; i < n; i++) {
    const char* s = line_at((first_line + i) % n);
    if(predicate(cmd, s))
      count++;
  }
  return count;
}

void
search_update(int (*predicate)(const char*, const char*)) {
  int64 first;
  size_t matches = 0;
  stralloc_nul(&command_buf);
  if((first = find_line(command_buf.s, predicate)) >= 0)
    matches = count_matches(command_buf.s, predicate);
  terminal_cursor_horizontal_absolute(terminal_cols - 30 - command_buf.len);
  terminal_erase_in_line(0);

  terminal_escape_sequence(buffer_1, "1m");
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
    buffer_putlonglong(buffer_1, first);
  } else {
    terminal_rgb_foreground(buffer_1, 208, 240, 248);
    buffer_puts(buffer_1, " ∞ " /*" ∅ "*/);
    terminal_rgb_foreground(buffer_1, 230, 198, 5);
    buffer_puts(buffer_1, " not found");
  }
  buffer_putspace(buffer_1);
  terminal_escape_sequence(buffer_1, "m");
  buffer_flush(buffer_1);
}

int64
search_command(const char* cmd, int (*predicate)(const char*, const char*)) {
  int64 found;
  command_mode = 0;
  if((found = find_line(command_buf.s, predicate)) >= 0) {
    scroll_to(found);
  } else {
    buffer_putc(buffer_1, '\r');
    print_status();
  }
}

static const char* command_prefixes[4] = {":", " :", "/", "\\"};

int
read_command(void) {
  unsigned char c;
  stralloc_zero(&command_buf);

  /*terminal_erase_in_line(2);
       terminal_cursor_horizontal_absolute(1);*/
  terminal_goto_xy(display_rows, 0);
  terminal_erase_in_line(2);
  buffer_puts(buffer_1, command_prefixes[command_mode & 0x03]);
  buffer_flush(buffer_1);
  //      terminal_cursor_horizontal_absolute(2);

  while(buffer_getc(&terminal, &c) == 1) {
    if(c == '\n' || c == '\r')
      break;
    terminal_cursor_horizontal_absolute(command_buf.len + 2);

    if(c == 0x7f || c == 8) {
      if(command_buf.len) {
        terminal_cursor_backward(1);

        command_buf.len -= 1;
      }

      //  terminal_cursor_horizontal_absolute(command_buf.len + 2);
      //      terminal_erase_in_line(0);
    } else if(c >= 0x20) {

      stralloc_catc(&command_buf, c);
      buffer_put(buffer_1, &c, 1);
      buffer_flush(buffer_1);
    } else {
      if(c < 0x20) {
        buffer_puts(buffer_2, "Character: ");
        buffer_putulong(buffer_2, c);
        buffer_putnlflush(buffer_2);
      }
      assert(c >= 0x20);
    }

    terminal_cursor_horizontal_absolute(command_buf.len + 2);
    // terminal_erase_in_line(0);

    // terminal_goto_xy(display_rows, command_buf.len);

    /*terminal_erase_in_line(2);
    terminal_cursor_horizontal_absolute(1);
    buffer_puts(buffer_1,  command_prefixes[command_mode & 0x03]);
        buffer_putsa(buffer_1, &command_buf);*/
    // char buf[1024];

    /*buffer_put(buffer_1, buf, fmt_hexbs(buf, command_buf.s,
      command_buf.len)); buffer_flush(buffer_1);*/

    if(command_mode >= 2) {
      terminal_cursor_save();
      /*  terminal_erase_in_line(0);*/

      search_update(command_mode == 2 ? &match_pattern : &nomatch_pattern);
      terminal_cursor_restore();
    }
  }
  stralloc_nul(&command_buf);
  switch(command_mode) {
    case 1: read_goto(command_buf.s); break;
    case 2: search_command(command_buf.s, &match_pattern); break;
    case 3: search_command(command_buf.s, &nomatch_pattern); break;
  }
  command_mode = 0;
  stralloc_zero(&command_buf);
}

void
update_number() {
  buffer_putc(buffer_1, '\r');
  print_status();
}

int64
get_number() {
  int64 x = 1;
  if(command_buf.len) {
    stralloc_nul(&command_buf);
    scan_longlong(command_buf.s, &x);
    stralloc_zero(&command_buf);
    update_number();
  }
  return x;
}

int
handle_input(void) {
  char c;
  int64 n;
  if(buffer_getc(&terminal, &c) < 1)
    return 0;

  if(isdigit(c)) {
    stralloc_catc(&command_buf, c);
    update_number();
    return 1;
  }

  if(command_buf.len && c == 0x7f) {
    command_buf.len--;
    update_number();
  }

  n = get_number();

  switch(c) {
    case 'q':
    case 'Q': running = 0; break;

    case 'e':
    case 5:
    case 'j':
    case '\n': scroll_by(n); break;

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
    case 21: scroll_by(-display_rows / 2 * n); break;

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

    default: return 0;
  }

  stralloc_zero(&command_buf);
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
  int watchfd, wd, is_pipe = 0;
  ssize_t inputsize;

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
    if(fstat(input.fd, &st) != -1) {
      inputsize = st.st_size;
      is_pipe = S_ISFIFO(st.st_mode);
    }
  }
#endif

  terminal_init();
  terminal_linewrap_disable();
  terminal_set_alternate_screen();

  buffer_flush(buffer_1);

  scroll_to(0);

  if(input.fd != STDIN_FILENO /*&& !is_pipe*/) {
#if !WINDOWS
    watchfd = inotify_init();

    wd = inotify_add_watch(watchfd, filename, IN_MODIFY);
#endif
  } else {
    watchfd = input.fd;
  }

  io_fd(terminal.fd);
  io_fd(watchfd);

  io_wantread(terminal.fd);
  io_wantread(watchfd);

  while(running) {
    fd_type fd;
    int on_terminal = 0, on_input = 0;

    read_content(&input, 0);

    while(io_wait() == -1) {
      if(errno == EINTR) {
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
      size_t newsize, prev_lines;
      ssize_t ret;
      int do_scroll;

      if(!is_pipe)
        if(!get_filesize(input.fd, &newsize))
          newsize = 0;

      prev_lines = strarray_size(&lines);
      do_scroll = last_line() >= prev_lines - 1;

      while(is_pipe || get_position(&input) < newsize) {
        if((ret = read_line(&input)) <= 0)
          break;
        if(do_scroll)
          scroll_down();
      }
      if(!do_scroll) {
        size_t added = strarray_size(&lines) - prev_lines;

        if(added)
          print_added(added);
      }

      if(ret == 0 && is_pipe)
        io_dontwantread(watchfd);
    }

    /*if(!on_terminal && !on_input) {
      io_dontwantread(watchfd);
    }*/
  }
  terminate(0);

  strarray_free(&lines);

  buffer_close(&input);

  return 0;
}
