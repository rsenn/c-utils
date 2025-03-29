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

void
cpp_error_at(char* loc, char* fmt, ...) {
  int line_no = 1;

  for(char* p = cpp_current_file->contents; p < loc; p++)
    if(*p == '\n')
      line_no++;

  va_list ap;
  va_start(ap, fmt);
  cpp_verror_at(cpp_current_file->name, cpp_current_file->contents, line_no, loc, fmt, ap);
  exit(1);
}

void
cpp_error_tok(cpp_token* tok, char* fmt, ...) {
  va_list ap;

  va_start(ap, fmt);
  cpp_verror_at(tok->file->name, tok->file->contents, tok->line_no, tok->loc, fmt, ap);
  exit(1);
}

void
cpp_warn_tok(cpp_token* tok, char* fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  cpp_verror_at(tok->file->name, tok->file->contents, tok->line_no, tok->loc, fmt, ap);
  va_end(ap);
}
