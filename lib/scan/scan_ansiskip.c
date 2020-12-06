#include "../scan.h"
#include <ctype.h>

size_t
scan_ansiskip(const char* s, size_t limit) {
  const char* t = s;
  const char* u = t + limit;

  if(*t == 0x1b) {
    ;
    if((++t < u) && *t == '[') {
      while(t < u && !isalpha(*t)) ++t;
      return (size_t)(t - s);
    }
  }
  return 0;
}
