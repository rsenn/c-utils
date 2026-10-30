#include "../http.h"
#include "../http_internal.h"

/* chunked_line reads a line from in without the end of line into x[n],
 * returns its length or -1 at the end of the input */
ssize_t
chunked_line(buffer* in, char* x, size_t n) {
  size_t i = 0;
  char c;

  for(;;) {
    if(buffer_getc(in, &c) <= 0)
      return -1;

    if(c == '\n')
      break;

    if(c != '\r' && i + 1 < n)
      x[i++] = c;
  }
  x[i] = '\0';
  return (ssize_t)i;
}
