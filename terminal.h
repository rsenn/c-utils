#ifndef TERMINAL_H
#define TERMINAL_H

#include "lib/uint8.h"
#include "lib/buffer.h"

#define ESC 27

extern buffer terminal_buffer;

#define terminal_devicecode_query() terminal_escape_sequence("c")
#define terminal_devicestatus_query() terminal_command_number_char(5, 'n')
#define terminal_cursor_query() terminal_command_number_char(6, 'n')
#define terminal_device_reset() terminal_command_char('c')
#define terminal_tab_set() terminal_command_char('H')
#define terminal_tab_clear() terminal_command_char('g')
#define terminal_tabs_clearall() terminal_command_number_char(3, 'g')

#define terminal_cursor_save() terminal_command_char('s')
#define terminal_cursor_restore() terminal_command_char('u')
#define terminal_linewrap_enable() terminal_command_number_char(7, 'h')
#define terminal_linewrap_disable() terminal_command_number_char(7, 'l')

void terminal_command_char(char c);
void terminal_command_number_char(int n, char c);
void terminal_command_sequence(buffer* b, const char* seq);
void terminal_cursor_backward(int n);
void terminal_cursor_down(int n);
void terminal_cursor_forward(int n);
void terminal_cursor_horizontal_absolute(int n);
void terminal_cursor_next_line(int n);
void terminal_cursor_position(int* x, int* y);
void terminal_cursor_previous_line(int n);
void terminal_cursor_up(int n);
void terminal_erase_in_display(int n);
void terminal_erase_in_line(int n);
void terminal_escape_char(buffer* b, char c);
void terminal_escape_number_char(buffer* b, int n, char c);
void terminal_escape_sequence(buffer* b, const char* seq);
int terminal_getheight(void);
int terminal_getwidth(void);
void terminal_goto_xy(int x, int y);
int terminal_init(void);
void terminal_numbers_sequence(buffer* b, int* numbers, size_t len, char c);
void terminal_number_sequence(buffer* b, int n, char c);
void terminal_restore(void);
void terminal_rgb_background(buffer* buf, uint8 r, uint8 g, uint8 b);
void terminal_rgb_foreground(buffer* buf, uint8 r, uint8 g, uint8 b);
void terminal_scroll_down(int n);
void terminal_scroll_up(int n);
void terminal_set_alternate_screen(void);
void terminal_set_normal_screen(void);

#endif
