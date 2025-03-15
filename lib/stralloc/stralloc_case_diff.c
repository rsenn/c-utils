#include "../byte.h"
#include "../stralloc.h"

int
stralloc_case_diff(const stralloc* sa1, const stralloc* sa2) {
  int r;
  /* get shortest len */
  size_t len = sa1->len;

  if(len > sa2->len)
    len = sa2->len;

  /* compare common lengths */
  if((r = byte_case_diff(sa1->s, len, sa2->s)))
    return r;

  return (int)sa1->len - (int)sa2->len;
}
