#define _XOPEN_SOURCE_EXTENDED 1
#define _MISC_SOURCE 1
#include "lib/buffer.h"
#include "lib/bool.h"
#include "lib/io_internal.h"
#include "terminal.h"
#if WINDOWS
#include <windows.h>
#include <io.h>
#endif

#if !WINDOWS_NATIVE
#include <unistd.h>
#include <termios.h>
#include <sys/ioctl.h>
#endif

static char terminal_out_buf[32], terminal_in_buf[64];
buffer terminal_out_buffer =
    BUFFER_INIT(write, 1, terminal_out_buf, sizeof(terminal_out_buf));
buffer terminal_in_buffer =
    BUFFER_INIT(read, 0, terminal_in_buf, sizeof(terminal_in_buf));

#if WINDOWS
static WORD COLOR_DEFAULT = FOREGROUND_RED | FOREGROUND_GREEN |
                            FOREGROUND_BLUE | FOREGROUND_INTENSITY;
static HANDLE hConsole;
#else
static struct termios oldterm;
#endif

static inline void
put_escape(buffer* b) {
  buffer_putc(b, ESC);
  buffer_putc(b, '[');
}

static inline void
put_num(int n) {
  buffer_putulong(&terminal_out_buffer, n);
}

static inline void
put_char(int c) {
  buffer_putc(&terminal_out_buffer, c);
}

static inline void
put_string(const char* s) {
  buffer_puts(&terminal_out_buffer, s);
}

/*void
terminal_device_reset() {
  put_char(ESC);
  put_char('c');
    buffer_flush(&terminal_out_buffer);

}
*/
int
terminal_init(void) {
#if WINDOWS
  hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
#elif !NO_TCATTR
  struct termios newt;
  tcgetattr(terminal_out_buffer.fd, &oldterm);
  newt = oldterm;
  newt.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(terminal_out_buffer.fd, TCSANOW, &newt);
#endif
  return 0;
}

void
terminal_restore(void) {
#if !(WINDOWS || NO_TCATTR)
  tcsetattr(terminal_out_buffer.fd, TCSANOW, &oldterm);
#endif
  terminal_set_normal_screen();
  terminal_linewrap_enable();
}

#if WINDOWS
static CONSOLE_SCREEN_BUFFER_INFO
getsize() {
  CONSOLE_SCREEN_BUFFER_INFO csbi;
  GetConsoleScreenBufferInfo(hConsole, &csbi);
  return csbi;
}
#endif

int
terminal_getwidth() {
#if WINDOWS
  CONSOLE_SCREEN_BUFFER_INFO size = getsize();
  return size.srWindow.Right - size.srWindow.Left;
#else
  struct winsize w;
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
  return w.ws_col;
#endif
}

int
terminal_getheight() {
#if WINDOWS
  CONSOLE_SCREEN_BUFFER_INFO size = getsize();
  return size.srWindow.Bottom - size.srWindow.Top;
#else
  struct winsize w;
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
  return w.ws_row;
#endif
}

void
terminal_number_sequence(buffer* b, int n, char c) {
  put_escape(b);
  if(n > 1)
    buffer_putulong(b, n);
  buffer_putc(b, c);
}

void
terminal_numbers_sequence(buffer* b, int numbers[], size_t len, char c) {
  size_t i;
  put_escape(b);
  for(i = 0; i < len; i++) {
    if(i > 0)
      buffer_putc(b, ';');
    buffer_putulong(b, numbers[i]);
  }
  buffer_putc(b, c);
}

void
terminal_escape_number_char(buffer* b, int n, char c) {
  terminal_number_sequence(&terminal_out_buffer, n, c);
  buffer_flush(&terminal_out_buffer);
}
void
terminal_escape_char(buffer* b, char c) {
  buffer_putc(b, ESC);
  buffer_putc(b, c);
}

void
terminal_escape_sequence(buffer* b, const char* seq) {
  put_escape(b);
  buffer_puts(b, seq);
}

void
terminal_command_sequence(buffer* b, const char* seq) {
  terminal_escape_sequence(&terminal_out_buffer, seq);
  buffer_flush(&terminal_out_buffer);
}
void
terminal_command_number_char(int n, char c) {
  terminal_escape_number_char(&terminal_out_buffer, n, c);
  buffer_flush(&terminal_out_buffer);
}
void
terminal_command_char(char c) {
  terminal_escape_char(&terminal_out_buffer, c);
  buffer_flush(&terminal_out_buffer);
}

void
terminal_goto_xy(int x, int y) {
#if WINDOWS
  COORD point;
  point.X = x;
  point.Y = y;
  SetConsoleCursorPosition(hConsole, point);
#else
  int coord[2] = {y + 1, x + 1};
  terminal_numbers_sequence(&terminal_out_buffer, coord, 2, 'H');
  buffer_flush(&terminal_out_buffer);
#endif
}

void
terminal_cursor_up(int n) {
  terminal_command_number_char(n, 'A');
}

void
terminal_cursor_down(int n) {
  terminal_command_number_char(n, 'B');
}

void
terminal_cursor_forward(int n) {
  terminal_command_number_char(n, 'C');
}

void
terminal_cursor_backward(int n) {
  terminal_command_number_char(n, 'D');
}

void
terminal_cursor_next_line(int n) {
  terminal_command_number_char(n, 'E');
}

void
terminal_cursor_previous_line(int n) {
  terminal_command_number_char(n, 'F');
}

void
terminal_cursor_horizontal_absolute(int n) {
  terminal_command_number_char(n, 'G');
}

/*void
terminal_cursor_position(int row, int column) {
  int coord[2] = {row, column};
  terminal_numbers_sequence(&terminal_out_buffer, coord, 2, 'H');
  buffer_flush(&terminal_out_buffer);
}
*/
void
terminal_cursor_position(int* x, int* y) {
#if WINDOWS
  CONSOLE_SCREEN_BUFFER_INFO csbi;
  if(!GetConsoleScreenBufferInfo(hConsole, &csbi))
    return;

  if(x)
    *x = csbi.dwCursorPosition.X;
  if(y)
    *y = csbi.dwCursorPosition.Y;
#else
  terminal_command_number_char(6, 'n');

  buffer_feed(&terminal_in_buffer);

#endif
}

void
terminal_erase_in_display(int n) {
#if WINDOWS
  COORD coordScreen = {0, 0};
  DWORD cCharsWritten;
  CONSOLE_SCREEN_BUFFER_INFO csbi;
  DWORD dwConSize;

  if(n == 2) {
    GetConsoleScreenBufferInfo(hConsole, &csbi);
    dwConSize = csbi.dwSize.X * csbi.dwSize.Y;
    FillConsoleOutputCharacter(
        hConsole, TEXT(' '), dwConSize, coordScreen, &cCharsWritten);
    GetConsoleScreenBufferInfo(hConsole, &csbi);
    FillConsoleOutputAttribute(hConsole,
                               csbi.wAttributes,
                               dwConSize,
                               coordScreen,
                               &cCharsWritten);
    SetConsoleCursorPosition(hConsole, coordScreen);
  }
#else
  terminal_command_number_char(n, 'J');
#endif
}

void
terminal_erase_in_line(int n) {
  terminal_command_number_char(n, 'K');
}

void
terminal_scroll_up(int n) {
  terminal_command_number_char(n, 'S');
}

void
terminal_scroll_down(int n) {
  terminal_command_number_char(n, 'T');
}

void
terminal_set_alternate_screen() {
  put_escape(&terminal_out_buffer);
  put_char('?');
  put_num(1049);
  put_char('h');
  buffer_flush(&terminal_out_buffer);
}

void
terminal_set_normal_screen() {
  put_escape(&terminal_out_buffer);
  put_char('?');
  put_num(1049);
  put_char('l');
  buffer_flush(&terminal_out_buffer);
}

void
terminal_set_bg(int color) {
  int c = color & 7;
  bool bright = (color & COLOR_BRIGHT) == COLOR_BRIGHT;

#if WINDOWS
  CONSOLE_SCREEN_BUFFER_INFO csbi;
  GetConsoleScreenBufferInfo(hConsole, &csbi);

  if(color == COLOR_RESET)
    c = (COLOR_DEFAULT & 240) | (csbi.wAttributes & 15);
  else {
    if(bright)
      c += 8;

    c = c << 4;
    c |= (csbi.wAttributes & 15);
  }

  SetConsoleTextAttribute(hConsole, c);
#else
  if(color == COLOR_RESET)
    terminal_command_number_char(49, 'm');
  else
    terminal_numbers_sequence(&terminal_out_buffer,
                              (int[]){bright ? 0 : 1, 40 + c},
                              2,
                              'm');
//    printf("\033[%d;%dm", bright ? 0 : 1, 40 + c);
#endif
}

void
terminal_set_fg(int color) {
  int c = color & 7;
  bool bright = (color & COLOR_BRIGHT) == COLOR_BRIGHT;

#if WINDOWS
  CONSOLE_SCREEN_BUFFER_INFO csbi;
  GetConsoleScreenBufferInfo(hConsole, &csbi);

  if(color == COLOR_RESET)
    c = (COLOR_DEFAULT & 15) | (csbi.wAttributes & 240);
  else {
    if(bright)
      c += 8;

    c |= (csbi.wAttributes & 240);
  }

  SetConsoleTextAttribute(hConsole, c);
#else
  if(color == COLOR_RESET)
    terminal_command_number_char(39, 'm');
  else
    terminal_numbers_sequence(&terminal_out_buffer,
                              (int[]){bright ? 0 : 2, 30 + c},
                              2,
                              'm');
    // printf("\033[%d;%dm", bright ? 0 : 2, 30 + c);
#endif
}
void
terminal_rgb_foreground(buffer* buf, uint8 r, uint8 g, uint8 b) {
  int code[5] = {38, 2};
  code[2] = r;
  code[3] = g;
  code[4] = b;
  terminal_numbers_sequence(buf, code, 5, 'm');
}

void
terminal_rgb_background(buffer* buf, uint8 r, uint8 g, uint8 b) {
  int code[5] = {48, 2};
  code[2] = r;
  code[3] = g;
  code[4] = b;
  terminal_numbers_sequence(buf, code, 5, 'm');
}
