#include "byte.h"
#include "stralloc.h"

unsigned int stralloc_rchr(register const stralloc *sa, char c)
{
  return byte_rchr(sa->s, sa->len, c);
}
