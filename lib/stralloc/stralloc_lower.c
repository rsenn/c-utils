#include "../byte.h"
#include "../byte.h"
#include "../stralloc.h"

void stralloc_lower(register stralloc *sa)
{
  byte_lower(sa->s, sa->len);
}
