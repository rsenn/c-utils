#include "../byte.h"
#include "../stralloc.h"

unsigned int stralloc_chr(const stralloc *sa, char c)
{
  return byte_chr(sa->s, sa->len, c);
}
