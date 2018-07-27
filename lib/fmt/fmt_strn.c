#include "../fmt.h"

size_t
fmt_strn(char* out, const char* in, size_t limit) {
  char* s = out;
  const char* t = in;
  const char* u = in + limit;
  for(;;) {
    if(!*t || t == u) { break; };
    if(s) {
      *s = *t;
      ++s;
    };
    ++t;
  }
  return (size_t)(t - in);
}
