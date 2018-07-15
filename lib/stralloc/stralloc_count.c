#include "../byte.h"
#include "../byte.h"
#include "../stralloc.h"

unsigned int stralloc_count(const stralloc *sa, char c)
{
  return byte_count(sa->s, sa->len, c);
}
