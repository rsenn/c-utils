#include "../scan.h"
#include <ctype.h>

size_t
scan_nonwhitenskip(const char* s, size_t limit) {
  const char* t = s;
  const char* u = t + limit;
  while(t < u && !isspace(*t))
    ++t;
  return (size_t)(t - s);
}

size_t
scan_nonwhitenskip_escaped(const char* s, size_t limit) {
  const char* t = s;
  const char* u = t + limit;
  while(t < u) {
    if(*t == '\\' && isspace(t[1]))
      break;
    if(isspace(*t))
      break;
    ++t;
  }
  return (size_t)(t - s);
}
