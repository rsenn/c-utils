#ifndef TERMINAL_INTERNAL_H
#define TERMINAL_INTERNAL_H

#include "terminal.h"

#if WINDOWS_NATIVE
#include <windows.h>

/* terminal_info reads the screen buffer info of the output, 0 on failure */
int terminal_info(struct terminal* t, CONSOLE_SCREEN_BUFFER_INFO* info);

/* terminal_wattr applies fg, bg and style to the console */
void terminal_wattr(struct terminal* t);
#else
/* terminal_seq writes ESC [ prefix p[0] ; p[1] ... final */
void terminal_seq(struct terminal* t, char prefix, const int* p, size_t n, char final);
#endif

/* terminal_rgb16 returns the nearest of the 16 ANSI colors */
int terminal_rgb16(unsigned char r, unsigned char g, unsigned char b);

#endif /* defined(TERMINAL_INTERNAL_H) */
