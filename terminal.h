#ifndef TERMINAL_H
#define TERMINAL_H

#include "lib/uint8.h"

#define ESC 27

#define terminal_cursor_save() terminal_escape_sequence("s")
#define terminal_cursor_restore() terminal_escape_sequence("u")

void terminal_number_sequence(int n, char c);
void terminal_numbers_sequence(int* numbers, size_t len, char c);
void terminal_escape_sequence(const char* seq);
void terminal_cursor_position(int row, int column);
void terminal_cursor_up(int n);
void terminal_cursor_down(int n);
void terminal_cursor_forward(int n);
void terminal_cursor_backward(int n);
void terminal_cursor_next_line(int n);
void terminal_cursor_previous_line(int n);
void terminal_erase_in_display(int n);
void terminal_erase_in_line(int n);
void terminal_scroll_up(int n);
void terminal_scroll_down(int n);
void terminal_set_alternate_screen(void);
void terminal_set_normal_screen(void);
void terminal_rgb_foreground(uint8 r, uint8 g, uint8 b);
void terminal_rgb_background(uint8 r, uint8 g, uint8 b);

#endif
