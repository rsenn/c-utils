#include "../byte.h"
#include <ctype.h>

size_t
byte_camelize(char* x, size_t len) {
  size_t i,o = 0;

  for(i = 0; i < len; i++) {
    if(x[i] == '-') {
      ++i;
      x[o++] = toupper(x[i]);
    } else {
      x[o++] = x[i];
    }
  }
  return o;
}

