#include "../cpp.h"
#include "../cpp_internal.h"
#include "../alloc.h"

/* Double-quote a given string and returns it. */
char*
cpp_quote_string(char* str) {
  int bufsize = 3;

  for(int i = 0; str[i]; i++) {
    if(str[i] == '\\' || str[i] == '"')
      bufsize++;

    bufsize++;
  }

  char* buf = alloc_zero(bufsize);
  char* p = buf;
  *p++ = '"';

  for(int i = 0; str[i]; i++) {
    if(str[i] == '\\' || str[i] == '"')
      *p++ = '\\';
    *p++ = str[i];
  }

  *p++ = '"';
  *p++ = '\0';
  return buf;
}
