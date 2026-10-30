#include "../uri.h"
#include "../byte.h"

/* uri_find returns len if x holds no "://", else the start of the scheme */
size_t
uri_find(const char* x, size_t len) {
  size_t i = byte_finds(x, len, "://");

  if(i < len)
    while(i > 0 && ((x[i - 1] >= '0' && x[i - 1] <= '9') || (x[i - 1] >= 'A' && x[i - 1] <= 'Z') ||
                    (x[i - 1] >= 'a' && x[i - 1] <= 'z') || x[i - 1] == '+'))
      i--;

  return i;
}
