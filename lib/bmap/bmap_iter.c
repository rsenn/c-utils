#include "../bmap_internal.h"

bmap_iter_t
bmap_iter_(void) {
  bmap_iter_t iter;
  iter.bucketidx = -1;
  iter.node = NULL;
  return iter;
}
