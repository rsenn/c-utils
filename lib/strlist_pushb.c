#include "strlist.h"
#include "byte.h"

int
strlist_pushb(strlist* sl, const char* s, size_t n) {

  if(stralloc_readyplus(&sl->sa, n + 2)) {
	stralloc_catb(&sl->sa, s, n);
	stralloc_catb(&sl->sa, "\0", 1);
  }
  return 0;
}
