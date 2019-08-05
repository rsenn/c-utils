#include "../cbmap_internal.h"

void
cbmap_data_node_destroy(struct cbmap_data_node* data,
                        cbmap_allocator_t key_allocator,
                        cbmap_allocator_t value_allocator) {
  if(data != NULL) {
    if(data->value != NULL) {
      value_allocator->free(data->value);
      data->value = NULL;
    }
    if(data->key != NULL) {
      key_allocator->free(data->key);
      data->key = NULL;
    }
    CBM_FREE(data);
  }
}

