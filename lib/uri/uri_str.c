#include "../uri.h"
#include "../alloc.h"

char*
uri_str(const uri_t* u) {
  size_t len = uri_fmt(0, u);
  char* s = alloc(len + 1);

  if(s)
    s[uri_fmt(s, u)] = '\0';

  return s;
}
