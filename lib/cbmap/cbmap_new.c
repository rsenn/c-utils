#include "../byte.h"
#include "../cbmap_internal.h"

/** mark - cbmap_new */

cbmap_t
cbmap_new_with_custom_allocation(cbmap_allocator_fn key_allocator,
                                 cbmap_deallocator_fn key_deallocator,
                                 cbmap_allocator_fn value_allocator,
                                 cbmap_deallocator_fn value_deallocator) {
  cbmap_t map = (cbmap_t)CBM_MALLOC(sizeof(struct cbmap));
  if(map == NULL) {
    return NULL;
  }
  map->root = NULL;
  map->count = 0;
  map->key_allocator.malloc = key_allocator;
  map->key_allocator.free = key_deallocator;
  map->value_allocator.malloc = value_allocator;
  map->value_allocator.free = value_deallocator;
  return map;
}

void*
cbmap_default_allocator(void* data, size_t data_len) {
  void* data_copy = CBM_MALLOC(data_len);
  if(data_copy == NULL) {
    return NULL;
  }
  byte_copy(data_copy, data_len, data);
  return data_copy;
}

void
cbmap_default_deallocator(void* data) {
  CBM_FREE(data);
}

void*
cbmap_nop_allocator(void* data, size_t data_len) {
  return data;
}

void
cbmap_nop_deallocator(void* data) {
  ; /* Empty */
}

struct cbmap_allocator CBMAP_DEFAULT_ALLOCATOR = {cbmap_default_allocator,
                                                  cbmap_default_deallocator};

struct cbmap_allocator CBMAP_NOP_ALLOCATOR = {cbmap_nop_allocator,
                                              cbmap_nop_deallocator};

cbmap_t
cbmap_new(void) {
  return cbmap_new_with_custom_allocation(CBMAP_DEFAULT_ALLOCATOR.malloc,
                                          CBMAP_DEFAULT_ALLOCATOR.free,
                                          CBMAP_DEFAULT_ALLOCATOR.malloc,
                                          CBMAP_DEFAULT_ALLOCATOR.free);
}

cbmap_t
cbmap_new_with_static_keys_and_values(void) {
  return cbmap_new_with_custom_allocation(CBMAP_NOP_ALLOCATOR.malloc,
                                          CBMAP_NOP_ALLOCATOR.free,
                                          CBMAP_NOP_ALLOCATOR.malloc,
                                          CBMAP_NOP_ALLOCATOR.free);
}
