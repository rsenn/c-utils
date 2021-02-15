#ifndef TERMINAL_H
#define TERMINAL_H

#include "lib/uint8.h"

#define ESC 27

#define terminal_devicecode_query() terminal_escape_sequence("c")
#define terminal_devicestatus_query() terminal_number_sequence(5, 'n')
#define terminal_cursor_query() terminal_number_sequence(6, 'n')
#define terminal_device_reset() terminal_escape_char('c')
#define terminal_tab_set() terminal_escape_char('H')
#define terminal_tab_clear() terminal_escape_sequence("g")
#define terminal_tabs_clearall() terminal_number_sequence(3, 'g')

#define terminal_cursor_save() terminal_escape_sequence("s")
#define terminal_cursor_restore() terminal_escape_sequence("u")
#define terminal_linewrap_enable() terminal_number_sequence(7, 'h')
#define terminal_linewrap_disable() terminal_number_sequence(7, 'l')

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
void terminal_cursor_horizontal_absolute(int n);
void terminal_erase_in_display(int n);
void terminal_erase_in_line(int n);
void terminal_scroll_up(int n);
void terminal_scroll_down(int n);
void terminal_set_alternate_screen(void);
void terminal_set_normal_screen(void);
void terminal_rgb_foreground(uint8 r, uint8 g, uint8 b);
void terminal_rgb_background(uint8 r, uint8 g, uint8 b);

#endif
