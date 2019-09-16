#include "../str.h"
#include "../strlist.h"

void
strlist_fromv(strlist* sl, const char** v, int c) {
  int i;

  for(i = 0; i < c; ++i) strlist_push(sl, v[i]);
}
