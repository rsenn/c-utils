#include "../set.h"
#include "../byte.h"

/* Makes the set empty without looking at what it held, so it is for sets
 * that were never used (memory from malloc, a struct in a struct). It is the
 * same as assigning SET(). Use set_free for a set that has values. */
void
set_init(set_t* set) {
  byte_zero(set, sizeof(set_t));
}
