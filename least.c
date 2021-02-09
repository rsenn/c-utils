#include "lib/buffer.h"
#include "lib/alloc.h"
#include "lib/errmsg.h"
#include "lib/strarray.h"
#include "lib/scan.h"
#include "lib/str.h"
#include "lib/io.h"
#include "lib/seek.h"
#include "lib/sig.h"

#include <unistd.h>
#include <errno.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <sys/inotify.h>
#include <sys/stat.h>

#include "terminal.h"

static strarray lines;
static long terminal_rows, terminal_cols, display_rows, first_line,
    command_mode;
static const char* filename;
static buffer terminal;
static struct termios oldterm;
volatile int running = 1, reset = 0;

void
read_terminal_properties(void) {
  struct winsize w;
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
  terminal_rows = w.ws_row;
  terminal_cols = w.ws_col;
  display_rows = terminal_rows - 1;
}

int
read_line(buffer* buf) {
  ssize_t ret;
  stralloc line;
  stralloc_init(&line);
  if((ret = buffer_getline_sa(buf, &line)) > 0) {
    stralloc_trimr(&line, "\r\n", 2);
    stralloc_nul(&line);

    array_catb((array*)&lines, &line.s, sizeof(char*));
    line.s = 0;
    line.a = 0;
    return 1;
  }
  stralloc_free(&line);
  return 0;
}

void
read_content(buffer* b) {
  while(read_line(b))
    ;
}

struct termios*
init_terminal(void) {
  struct termios newt;
  tcgetattr(terminal.fd, &oldterm);
  newt = oldterm;
  newt.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(STDIN_FILENO, TCSANOW, &newt);
  return &oldterm;
}

void
print_status() {
  if(!command_mode) {
    terminal_escape_sequence("7m");
    buffer_puts(buffer_1, filename);
    terminal_escape_sequence("m");
  } else {
    if(first_line + display_rows >= strarray_size(&lines)) {
      terminal_escape_sequence("7m");
      buffer_puts(buffer_1, "(END)");
      terminal_escape_sequence("m");
    } else
      buffer_putc(buffer_1, ':');
  }
  buffer_flush(buffer_1);
}

void
print_added(size_t num_lines) {
  terminal_erase_in_line(1);
  buffer_putc(buffer_1, '\r');
  terminal_escape_sequence("7m");
  buffer_puts(buffer_1, "+");
  buffer_putulong(buffer_1, num_lines);
  buffer_puts(buffer_1, " lines");
  terminal_escape_sequence("m");
  buffer_flush(buffer_1);
}

void
scroll_up(void) {
  if(first_line <= 0)
    return;

  terminal_scroll_down(1);
  terminal_erase_in_line(2);
  terminal_cursor_position(1, 1);
  buffer_puts(buffer_1, strarray_at(&lines, --first_line));
  buffer_putc(buffer_1, '\n');
  terminal_cursor_position(terminal_rows, 1);
  print_status();
}

void
scroll_down(void) {
  if(first_line + display_rows >= strarray_size(&lines))
    return;

  terminal_scroll_up(1);
  terminal_cursor_previous_line(0);
  terminal_erase_in_line(2);
  buffer_puts(buffer_1, strarray_at(&lines, ++first_line + display_rows - 1));
  buffer_putc(buffer_1, '\n');
  print_status();
}

void
scroll_to_line(long line) {
  long i, num_lines;
  first_line = line;
  num_lines = strarray_size(&lines);
  terminal_erase_in_display(2);
  terminal_cursor_position(1, 1);
  for(i = 0; i < display_rows; i++) {
    if(line + i < num_lines)
      buffer_puts(buffer_1, strarray_at(&lines, line + i));
    buffer_putc(buffer_1, '\n');
  }
  print_status();
}

void
scroll_by(int offset) {
  long line = first_line + offset;

  if(line < 0)
    line = 0;
  else if(line >= strarray_size(&lines) - display_rows)
    line = strarray_size(&lines) - display_rows;

  if(line != first_line)
    scroll_to_line(line);
}

void
scroll_page_up(void) {
  scroll_by(-display_rows);
}

void
scroll_page_down(void) {
  scroll_by(display_rows);
}

void
scroll_to_top(void) {
  scroll_to_line(0);
}

void
scroll_to_bottom(void) {
  scroll_to_line(strarray_size(&lines) - display_rows);
}

size_t
last_line(void) {
  return first_line + display_rows;
}

void
handle_escape_sequence(void) {
  char c;

  if(buffer_getc(&terminal, &c) == 1) {
    if(c == 'O' && buffer_getc(&terminal, &c) == 1) {
      if(c == 'A')
        scroll_up();
      else if(c == 'B')
        scroll_down();
      else if(c == 'H')
        scroll_to_top();
      else if(c == 'F')
        scroll_to_bottom();
    } else if(c == '[' && buffer_getc(&terminal, &c) == 1) {
      if(c == 'A')
        scroll_up();
      else if(c == 'B')
        scroll_down();
      else if(c == 'H')
        scroll_to_top();
      else if(c == 'F')
        scroll_to_bottom();
      else if(c == '5' || c == '6') {
        char c2;
        if(buffer_getc(&terminal, &c2) == 1 && c2 == '~') {

          if(c == '5')
            scroll_page_up();
          else if(c == '6')
            scroll_page_down();
        }
      }
    }
  }
}

void
handle_command(const char* cmd) {
  long line;
  size_t n;
  if((n = scan_long(cmd, &line)) > 0 && line) {
    if(line < 0) {
      line = strarray_size(&lines) + line - display_rows + 1;
      if(line < 0)
        line = 0;
    } else {
      line -= 1;
      if(line >= strarray_size(&lines) - display_rows)
        line = strarray_size(&lines) - display_rows;
    }
    scroll_to_line(line);
  }
}

void
handle_input(void) {
  char c;
  if(buffer_getc(&terminal, &c) == 1) {
    command_mode = 1;
    if(c == 'q' || c == 'Q')
      running = 0;
    else if(c == 'e' || c == 5 || c == 'j' || c == '\n')
      scroll_down();
    else if(c == 'y' || c == 25 || c == 'k' || c == '\v')
      scroll_up();
    else if(c == 'f' || c == 6 || c == 22 || c == ' ')
      scroll_page_down();
    else if(c == 'b' || c == 2)
      scroll_page_up();
    else if(c == 'd' || c == 4)
      scroll_by(display_rows / 2);
    else if(c == 'u' || c == 21)
      scroll_by(-display_rows / 2);
    else if(c == ':') {
      stralloc cmd;
      stralloc_init(&cmd);

      terminal_erase_in_line(2);
      buffer_putsflush(buffer_1, "\r :");

      while(buffer_getc(&terminal, &c) == 1) {
        if(c == '\n' || c == '\r')
          break;

        if(c == 0x7f) {
          buffer_puts(buffer_1, "\x08");
          terminal_erase_in_line(0);

          if(cmd.len)
            stralloc_trunc(&cmd, cmd.len - 1);
        } else {
          buffer_putc(buffer_1, c);
          stralloc_catc(&cmd, c);
        }
        buffer_flush(buffer_1);
      }
      stralloc_nul(&cmd);
      handle_command(cmd.s);
      stralloc_free(&cmd);
    } else if(c == ESC)
      handle_escape_sequence();
  }
}

ssize_t
get_filesize(int fd) {
  struct stat st;
  if(fstat(fd, &st) != -1)
    return st.st_size;
  return 0;
}

size_t
get_position(buffer* b) {
  size_t pos = seek_cur(b->fd);
  pos -= b->n - b->p;
  return pos;
}

int
interrupt(int sig) {
  reset = 1;
}

int
terminate(int sig) {
  tcsetattr(STDOUT_FILENO, TCSANOW, &oldterm);

  terminal_set_normal_screen();
  buffer_flush(buffer_1);

  exit(0);
}

int
main(int argc, char* argv[]) {
  buffer input;
  int watchfd, wd, is_pipe = 0;
  ssize_t inputsize;
  struct stat st;

  sig_catch(SIGTERM, terminate);
  sig_catch(SIGINT, interrupt);
  // sig_ignore(SIGINT);

  if(argc < 2) {
    buffer_read_fd(&input, STDIN_FILENO);
    filename = "(stdin)";
  } else {
    filename = argv[1];
    if(buffer_readfile(&input, filename))
      diesys(127, "Unable to open file ", filename);
  }

  buffer_read_fd(&terminal, STDERR_FILENO);

  read_terminal_properties();
  read_content(&input);

  if(fstat(input.fd, &st) != -1) {
    inputsize = st.st_size;
    is_pipe = S_ISFIFO(st.st_mode);
  }

  init_terminal();
  terminal_set_alternate_screen();
  buffer_flush(buffer_1);

  scroll_to_line(0);

  if(input.fd != STDIN_FILENO /*&& !is_pipe*/) {
    watchfd = inotify_init();

    wd = inotify_add_watch(watchfd, filename, IN_MODIFY);
  } else {
    watchfd = input.fd;
  }

  io_fd(terminal.fd);
  io_fd(watchfd);

  while(running) {
    fd_t fd;

    io_wantread(terminal.fd);
    io_wantread(watchfd);

    while(io_wait() == -1) {
      if(errno == EINTR) {
        if(reset) {
          scroll_to_line(first_line);
          reset = 0;
        }
        continue;
      }

      diesys(2, "io_wait");
    }

    while((fd = io_canread()) != -1) {
      if(fd == terminal.fd)
        handle_input();

      if(fd == watchfd) {
        struct inotify_event event;
        size_t newsize, prev_lines;
        int do_scroll;

        if(watchfd != input.fd) {
          if(read(watchfd, &event, sizeof(event)) <= 0)
            continue;
        }

        if(!is_pipe)
          newsize = get_filesize(input.fd);

        prev_lines = strarray_size(&lines);
        do_scroll = last_line() >= prev_lines - 1;
        while(is_pipe || get_position(&input) < newsize) {
          if(!read_line(&input))
            break;
          if(do_scroll)
            scroll_down();
        }
        if(!do_scroll)
          print_added(strarray_size(&lines) - prev_lines);
      }
    }
  }

  strarray_free(&lines);

  buffer_close(&input);

  terminate(0);
  return 0;
}