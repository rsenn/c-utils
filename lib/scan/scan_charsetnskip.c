#include "../scan.h"
#include <ctype.h>

/* Return the number of leading chars in s that are in charset, but no
 * more than limit */
size_t
scan_charsetnskip(const char* s, const char* charset, size_t limit) {
  const char* t = s;
  const char* u = t + limit;
  const char* i;

  while(t < u) {
    for(i = charset; *i; ++i)

      if(*i == *t)
        break;

    if(*i != *t)
      break;
    ++t;
  }
  return (size_t)(t - s);
}
