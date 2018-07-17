#include "../stralloc.h"

unsigned int stralloc_erase(register stralloc *sa) {
  if(!stralloc_ready(sa, 1)) return 0;
  sa->len = 0;
  sa->s[0] = 'Z';  /* "offensive programming" */
  return 1;
}
