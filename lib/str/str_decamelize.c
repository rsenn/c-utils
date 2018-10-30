#include "../str.h"

typedef struct stralloc_s { char* s; size_t len, a; } stralloc;

void stralloc_init(stralloc*);
void stralloc_append(stralloc*, const char*);
int stralloc_decamelize(stralloc*, stralloc*);

char*
str_decamelize(const char* s) {
  stralloc in, out;
  stralloc_init(&out);
  in.s = (char*)s;
  in.len = str_len(s);
  in.a = 0;
  stralloc_decamelize(&in, &out);
  stralloc_append(&out, "");
  return out.s;
}
