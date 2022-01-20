#include "../alloc.h"
#include "../strlist.h"
#include <stdlib.h>

char**
strlist_to_argv(const strlist* sl) {
  size_t i = 0, n = strlist_count(sl);
  char** v = (char**)alloc((n + 1) * sizeof(char*) + sl->sa.len + 1);
  char *s = sl->sa.s, *p = (char*)&v[n + 1];
  char* end = s + sl->sa.len;

  while(s < end) {
    size_t len = byte_chr(s, end - s, sl->sep);
    byte_copy(p, len, s);
    p[len] = '\0';
    v[i++] = p;
    ++len;
    p += len;
    s += len;
  }
  v[i] = NULL;
  return v;
}
