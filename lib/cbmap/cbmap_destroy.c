#include "../cbmap_internal.h"
/** mark - cbmap_destroy */

void
cbmap_destroy(cbmap_t* cmap_ptr) {
  if(cmap_ptr != NULL) {
    cbmap_t map = *cmap_ptr;

    if(map != NULL) {
      if(map->root != NULL) {
        cbmap_recursive_delete(map->root, &map->key_allocator, &map->value_allocator);
      }
      map->root = NULL;
      map->count = 0;
      CBM_FREE(map);
    }
    *cmap_ptr = NULL;
  }
}
