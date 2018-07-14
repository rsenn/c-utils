#include "stralloc.h"
/*
  return index to first occurance of data,
  otherwise return sa->len
*/
unsigned int stralloc_find(const stralloc *sa, register const stralloc *what)
{
  return stralloc_findb(sa, what->s, what->len);
}
