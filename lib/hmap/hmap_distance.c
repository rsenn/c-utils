#include "../hmap_internal.h"

int
hmap_distance(TUPLE** it1, TUPLE** it2) {
  TUPLE* a = *it1;
  TUPLE* b = *it2;
  int n = 0;

  while(a && a != *it2 && a != a->next) {
    a = a->next;
    ++n;
  }
  if(a == *it2 || (!it2 && a == a->next))
    return n;

  n = 0;
  while(b && b != *it1 && b != b->next) {
    b = b->next;
    ++n;
  }
  if(b == *it1 || (!it1 && b == b->next))
    return n;

  return -1;
}
