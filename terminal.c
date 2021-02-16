#include "lib/buffer.h"
#include "lib/io_internal.h"
#include "terminal.h"

static char terminal_buf[32];
buffer terminal_buffer =
    BUFFER_INIT(write, 1, terminal_buf, sizeof(terminal_buf));

static inline void
put_escape(buffer* b) {
  buffer_putc(b, ESC);
  buffer_putc(b, '[');
}

static inline void
put_num(int n) {
  buffer_putulong(&terminal_buffer, n);
}

static inline void
put_char(int c) {
  buffer_putc(&terminal_buffer, c);
}

static inline void
put_string(const char* s) {
  buffer_puts(&terminal_buffer, s);
}

/*void
terminal_device_reset() {
  put_char(ESC);
  put_char('c');
    buffer_flush(&terminal_buffer);

}
*/
void
terminal_number_sequence(buffer* b, int n, char c) {
  put_escape(b);
  if(n > 1)
    buffer_putulong(b, n);
  buffer_putc(b, c);
}

void
terminal_numbers_sequence(buffer* b, int* numbers, size_t len, char c) {
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
  terminal_number_sequence(&terminal_buffer, n, c);
  buffer_flush(&terminal_buffer);
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
  terminal_escape_sequence(&terminal_buffer, seq);
  buffer_flush(&terminal_buffer);
}
void
terminal_command_number_char(int n, char c) {
  terminal_escape_number_char(&terminal_buffer, n, c);
  buffer_flush(&terminal_buffer);
}
void
terminal_command_char(char c) {
  terminal_escape_char(&terminal_buffer, c);
  buffer_flush(&terminal_buffer);
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

void
terminal_cursor_position(int row, int column) {
  int coord[2] = {row, column};
  terminal_numbers_sequence(&terminal_buffer, coord, 2, 'H');
  buffer_flush(&terminal_buffer);
}

void
terminal_erase_in_display(int n) {
  terminal_command_number_char(n, 'J');
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
  put_escape(&terminal_buffer);
  put_char('?');
  put_num(1049);
  put_char('h');
  buffer_flush(&terminal_buffer);
}

void
terminal_set_normal_screen() {
  put_escape(&terminal_buffer);
  put_char('?');
  put_num(1049);
  put_char('l');
  buffer_flush(&terminal_buffer);
}

void
terminal_rgb_foreground(buffer* buf, uint8 r, uint8 g, uint8 b) {
  int code[5] = {38, 2, r, g, b};
  terminal_numbers_sequence(buf, code, 5, 'm');
}

void
terminal_rgb_background(buffer* buf, uint8 r, uint8 g, uint8 b) {
  int code[5] = {48, 2, r, g, b};
  terminal_numbers_sequence(buf, code, 5, 'm');
}
