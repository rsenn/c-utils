#include "../stralloc.h"

void stralloc_replace(register stralloc *sa, char before, char after) {
  register unsigned int i;

  for(i = 0; i < sa->len; i++) {
    if(sa->s[i] == before) {
      sa->s[i] = after;
    }
  }
}
