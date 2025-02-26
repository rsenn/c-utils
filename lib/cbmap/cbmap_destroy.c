#include "../cbmap_internal.h"

/** mark - cbmap_destroy */

void
cbmap_recursive_delete(void* top, cbmap_allocator_t key_allocator, cbmap_allocator_t value_allocator) {
  if(IS_INTERNAL_NODE(top)) {
    struct cbmap_internal_node* node = GET_INTERNAL_NODE(top);
    cbmap_recursive_delete(node->branch[0], key_allocator, value_allocator);
    cbmap_recursive_delete(node->branch[1], key_allocator, value_allocator);
    cbmap_internal_node_destroy(node);
  } else {
    struct cbmap_data_node* data = GET_DATA_NODE(top);
    cbmap_data_node_destroy(data, key_allocator, value_allocator);
  }
}

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
