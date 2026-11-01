#include "cbmap_alloc.h"
#include "../cbmap_internal.h"

void
cbmap_internal_node_destroy(struct cbmap_internal_node* node) {
  /* Note: Children must have been previously freed */

  if(node != NULL) {
    CBM_FREE(node);
  }
}
