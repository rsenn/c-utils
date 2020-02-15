#include "../alloc.h"
#include "../strlist.h"
#include <stdlib.h>

char**
strlist_to_argv(const strlist* sl) {
  size_t i = 0, n = strlist_count(sl);
  char** v = malloc((n + 1) * sizeof(char*) + sl->sa.len + 1);
  char *s, *p = (char*)&v[n + 1];

  strlist_foreach(sl, s, n) {
    byte_copy(p, n, s);
    p[n] = '\0';
    v[i++] = p;
    p += n + 1;
  }
  v[i] = NULL;
  return v;
}
