#include "../http.h"
#include "../byte.h"

size_t
http_skip_header(const char* x, size_t len) {
  const char* s = x;
  const char* end = x + len;

  while(s < end) {
    s += byte_chr(s, end - s, '\n');

    if(s < end) {
      ++s;

      if(s < end && (*s == '\r' || *s == '\n')) {
        while(s < end && *s != '\n')
          ++s;

        if(s < end)
          ++s;

        break;
      }
    }
  }

  return s - x;
}
