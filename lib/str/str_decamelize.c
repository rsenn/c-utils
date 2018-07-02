#include "../str.h"
#include "../stralloc.h"


char*
str_decamelize(const char* s) {
  stralloc in, out;
  stralloc_init(&out);
  in.s = (char*)s;
  in.len = str_len(s);
  in.a = 0;
  stralloc_decamelize(&in, &out);
  stralloc_0(&out);
  return out.s;
}
