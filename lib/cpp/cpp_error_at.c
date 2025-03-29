#include "../cpp_internal.h"

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
