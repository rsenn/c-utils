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

/*void
terminal_device_reset() {
  put_char(ESC);
  put_char('c');
    buffer_flush(buffer_1);

}
*/
void
terminal_number_sequence(int n, char c) {
  put_escape();
  if(n > 1)
    put_num(n);
  put_char(c);
  buffer_flush(buffer_1);
}

void
terminal_numbers_sequence(int* numbers, size_t len, char c) {
  size_t i;
  put_escape();
  for(i = 0; i < len; i++) {
    if(i > 0)
      put_char(';');
    put_num(numbers[i]);
  }
  put_char(c);
  buffer_flush(buffer_1);
}

void
terminal_escape_char(char c) {
  buffer_putc(buffer_1, ESC);
  buffer_putc(buffer_1, c);
  buffer_flush(buffer_1);
}

void
terminal_escape_sequence(const char* seq) {
  put_escape();
  buffer_putsflush(buffer_1, seq);
}

void
terminal_cursor_up(int n) {
  terminal_number_sequence(n, 'A');
}

void
terminal_cursor_down(int n) {
  terminal_number_sequence(n, 'B');
}

void
terminal_cursor_forward(int n) {
  terminal_number_sequence(n, 'C');
}

void
terminal_cursor_backward(int n) {
  terminal_number_sequence(n, 'D');
}

void
terminal_cursor_next_line(int n) {
  terminal_number_sequence(n, 'E');
}

void
terminal_cursor_previous_line(int n) {
  terminal_number_sequence(n, 'F');
}

void
terminal_cursor_horizontal_absolute(int n) {
  terminal_number_sequence(n, 'G');
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
terminal_erase_in_display(int n) {
  terminal_number_sequence(n, 'J');
}

void
terminal_erase_in_line(int n) {
  terminal_number_sequence(n, 'K');
}

void
terminal_scroll_up(int n) {
  terminal_number_sequence(n, 'S');
}

void
terminal_scroll_down(int n) {
  terminal_number_sequence(n, 'T');
}

void
terminal_set_alternate_screen() {
  put_escape();
  put_char('?');
  put_num(1049);
  put_char('h');
}

void
terminal_set_normal_screen() {
  put_escape();
  put_char('?');
  put_num(1049);
  put_char('l');
}

void
terminal_rgb_foreground(uint8 r, uint8 g, uint8 b) {
  int code[5] = {38, 2, r, g, b};
  terminal_numbers_sequence(code, 5, 'm');
}

void
terminal_rgb_background(uint8 r, uint8 g, uint8 b) {
  int code[5] = {48, 2, r, g, b};
  terminal_numbers_sequence(code, 5, 'm');
}
