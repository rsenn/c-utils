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
