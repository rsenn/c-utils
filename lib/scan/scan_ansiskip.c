#include "../scan.h"
#include <ctype.h>

size_t
scan_ansiskip(const char* s, size_t limit) {
  size_t i = 0;

  if(s[i] == 0x1b) {
    if(++i < limit && s[i] == '[') {
      while(++i < limit && !isalpha(s[i]))
        ;
      ++i;
      if(i < limit && s[i] == '~')
        ++i;
      return i;
    }
  }
  return 0;
}