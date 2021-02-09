#ifndef TERMINAL_H
#define TERMINAL_H

#define ESC 27

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

#endif
