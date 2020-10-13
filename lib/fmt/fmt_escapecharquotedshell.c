#include "../fmt.h"
#include "../uint32.h"
#include <sys/types.h>

size_t
fmt_escapecharquotedshell(char* dest, uint32 ch) {
  char c;
  switch(ch) {
    case '\'':
      c = '\'';
      if(dest) {
        dest[0] = c;
        dest[1] = '\\';
        dest[2] = c;
        dest[3] = c;
      }
      return 4;
    default: dest[0] = ch; return 1;
  }
}
