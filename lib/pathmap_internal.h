#ifndef PATHMAP_INTERNAL_H
#define PATHMAP_INTERNAL_H

#include "pathmap.h"

/* pathmap_match returns the length of the part of p[0..n) rule r
 * consumes, or (size_t)-1 when r does not apply to p. */
size_t pathmap_match(const pathmap_rule* r, const char* p, size_t n);

#endif /* PATHMAP_INTERNAL_H */
