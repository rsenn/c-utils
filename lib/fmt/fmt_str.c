#include "../fmt.h"

size_t
fmt_str(char* out, const char* in) {
  char* s = out;
  const char* t = in;
  for(;;) {
    if(!*t) {
      break;
    };
    if(s) {
      *s = *t;
      ++s;
    };
    ++t;
  }
  return (size_t)(t - in);
}
