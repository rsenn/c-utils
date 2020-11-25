#include "../fmt.h"

size_t
fmt_escapechardoublequotedshell(char* dest, int ch) {
    switch(ch) {
    case '"':
    case '$':
    case '`':
    case '\\':
      if(dest) {
         dest[0] = '\\';
        dest[1] = ch;
       }
      return 2;
    default: dest[0] = ch; return 1;
  }
}
