#include "../cpp.h"
#include "../cpp_internal.h"
#include "../buffer.h"
#include <stdio.h>

/* Reports an error message in the following format.

 foo.c:10: x = y + 1;
               ^ <error message here> */
void
cpp_verror_at(char* filename, char* input, int line_no, char* loc, char* fmt, va_list ap) {
  char *line = loc, *end = loc;

  /* Find a line containing `loc`. */
  while(input < line && line[-1] != '\n')
    line--;

  while(*end && *end != '\n')
    end++;

  size_t indent = buffer_2->p;

  buffer_puts(buffer_2, filename);
  buffer_putc(buffer_2, ':');
  buffer_putulong(buffer_2, line_no);
  buffer_puts(buffer_2, ": ");
  indent = buffer_2->p - indent;
  buffer_put(buffer_2, line, end - line);
  buffer_putc(buffer_2, '\n');

  /* Show the error message. */
  buffer_putnspace(buffer_2, cpp_display_width(line, loc - line) + indent);
  buffer_puts(buffer_2, "^ ");

  {
    char buf[1024];
    size_t n = vsnprintf(buf, sizeof(buf), fmt, ap);

    buffer_put(buffer_2, buf, n);
  }

  buffer_putnlflush(buffer_2);
}
