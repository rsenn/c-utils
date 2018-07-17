#include "../byte.h"
#include "../byte.h"
#include "../stralloc.h"

unsigned int stralloc_endb(register const stralloc *sa, const void *suffix, register unsigned int len)
{
  return (sa->len >= len) && byte_equal(sa->s + sa->len - len, len, suffix);
}
