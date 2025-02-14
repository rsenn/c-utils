#ifndef TERMINAL_H
#define TERMINAL_H

#include "lib/uint8.h"
#include "lib/buffer.h"
#include "lib/windoze.h"

#define COLOR_RESET -1
#define COLOR_BLACK 0
#define COLOR_GREEN 2
#define COLOR_MAGENTA 5
#define COLOR_FUSCHIA 5
#define COLOR_PURPLE 5
#define COLOR_WHITE 7
#define COLOR_BRIGHT 8
#if WINDOWS
#define COLOR_AQUA 3
#define COLOR_BLUE 1
#define COLOR_CYAN 3
#define COLOR_RED 4
#define COLOR_YELLOW 6
#else
#define COLOR_AQUA 6
#define COLOR_BLUE 4
#define COLOR_CYAN 6
#define COLOR_RED 1
#define COLOR_YELLOW 3
#endif

// BRIGHT
#define COLOR_BRIGHTAQUA (COLOR_BRIGHT | COLOR_AQUA)
#define COLOR_BRIGHTBLACK (COLOR_BRIGHT | COLOR_BLACK)
#define COLOR_BRIGHTBLUE (COLOR_BRIGHT | COLOR_BLUE)
#define COLOR_BRIGHTCYAN (COLOR_BRIGHT | COLOR_CYAN)
#define COLOR_BRIGHTFUSCHIA (COLOR_BRIGHT | COLOR_FUSCHIA)
#define COLOR_BRIGHTGREEN (COLOR_BRIGHT | COLOR_GREEN)
#define COLOR_BRIGHTMAGENTA (COLOR_BRIGHT | COLOR_MAGENTA)
#define COLOR_BRIGHTPURPLE (COLOR_BRIGHT | COLOR_PURPLE)
#define COLOR_BRIGHTRED (COLOR_BRIGHT | COLOR_RED)
#define COLOR_BRIGHTWHITE (COLOR_BRIGHT | COLOR_WHITE)
#define COLOR_BRIGHTYELLOW (COLOR_BRIGHT | COLOR_YELLOW)
#define COLOR_GRAY (COLOR_BRIGHT | COLOR_BLACK)

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
void
terminal_numbers_sequence(buffer* b, int numbers[], size_t len, char c);
void terminal_number_sequence(buffer* b, int n, char c);
void terminal_restore(void);
void terminal_rgb_background(buffer* buf, uint8 r, uint8 g, uint8 b);
void terminal_rgb_foreground(buffer* buf, uint8 r, uint8 g, uint8 b);
void terminal_scroll_down(int n);
void terminal_scroll_up(int n);
void terminal_set_alternate_screen(void);
void terminal_set_bg(int color);
void terminal_set_fg(int color);
void terminal_set_normal_screen(void);

static inline void
terminal_linewrap_enable() {
  terminal_command_number_char(7, 'h');
}
static inline void
terminal_linewrap_disable() {
  terminal_command_number_char(7, 'l');
}

#endif
