#include "../byte.h"
#include "../stralloc.h"

void
stralloc_trimr(stralloc* sa, const char* trimchars, unsigned int trimcharslen) {
  if(!sa->s)
    return;

  byte_trim(sa->s, &sa->len, trimchars, trimcharslen);
}
