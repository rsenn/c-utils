#include "lib/buffer.h"
#include "terminal.h"

static inline void
put_escape() {
  buffer_putc(buffer_1, ESC);
  buffer_putc(buffer_1, '[');
}

static inline void
put_num(int n) {
  buffer_putulong(buffer_1, n);
}

static inline void
put_char(int c) {
  buffer_putc(buffer_1, c);
}

static inline void
put_string(const char* s) {
  buffer_puts(buffer_1, s);
}

static void
put_n_cmd(int n, char c) {
  put_escape();
  if(n > 1)
    put_num(n);
  put_char(c);
}

void
terminal_escape_sequence(const char* seq) {
  put_escape();
  buffer_putsflush(buffer_1, seq);
}

void
terminal_cursor_position(int row, int column) {
  put_escape();
  put_num(row);
  put_char(';');
  put_num(column);
  put_char('H');
  buffer_flush(buffer_1);
}

void
terminal_cursor_up(int n) {
  put_n_cmd(n, 'A');
}

void
terminal_cursor_down(int n) {
  put_n_cmd(n, 'B');
}

void
terminal_cursor_forward(int n) {
  put_n_cmd(n, 'C');
}

void
terminal_cursor_backward(int n) {
  put_n_cmd(n, 'D');
}

void
terminal_cursor_next_line(int n) {
  put_n_cmd(n, 'E');
}

void
terminal_cursor_previous_line(int n) {
  put_n_cmd(n, 'F');
}

void
terminal_erase_in_display(int n) {
  put_n_cmd(n, 'J');
}

void
terminal_erase_in_line(int n) {
  put_n_cmd(n, 'K');
}

void
terminal_scroll_up(int n) {
  put_n_cmd(n, 'S');
}

void
terminal_scroll_down(int n) {
  put_n_cmd(n, 'T');
}

void
terminal_set_alternate_screen() {
  put_escape();
  put_char('?');
  put_num(47);
  put_char('h');
}

void
terminal_set_normal_screen() {
  put_escape();
  put_char('?');
  put_num(47);
  put_char('l');
}
