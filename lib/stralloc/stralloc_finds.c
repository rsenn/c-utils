#include "../str.h"
#include "../stralloc.h"
/*
  return index to first occurance of data,
  otherwise return sa->len
*/
unsigned int stralloc_finds(const stralloc *sa, register const char *what) {
  return stralloc_findb(sa, what, str_len(what));
}
