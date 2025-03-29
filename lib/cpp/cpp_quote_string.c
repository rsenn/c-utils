#include "../cpp.h"
#include "../cpp_internal.h"
#include "../alloc.h"

/* Double-quote a given string and returns it. */
char*
cpp_quote_string(char* str) {
  size_t bufsize = 3;
  char *buf, *p;

  for(size_t i = 0; str[i]; i++) {
    if(str[i] == '\\' || str[i] == '"')
      bufsize++;

    bufsize++;
  }

  p = buf = alloc_zero(bufsize);
  *p++ = '"';

  for(size_t i = 0; str[i]; i++) {
    if(str[i] == '\\' || str[i] == '"')
      *p++ = '\\';

    *p++ = str[i];
  }

  *p++ = '"';
  *p++ = '\0';

  return buf;
}
