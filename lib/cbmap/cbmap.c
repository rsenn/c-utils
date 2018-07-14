#include "cbmap.h"
#include "alloc.h"

#include <stdint.h>
#include <string.h>

#pragma mark - Readability macros

/* KEY_COMPARE and VALUE_COMPARE are here because
 I cannot distinguish very well between memcmp and memcpy
 even at high editor font sizes: presbyopia they call it.
*/
#define KEY_COMPARE(k1, k2, len) memcmp(k1, k2, len)
#define VALUE_COMPARE(k1, k2, len) memcmp(k1, k2, len)

/* Return values */
#define INSERT_OUT_OF_MEMORY 0
#define INSERT_OK 1
#define INSERT_UPDATED 2
#define NOT_FOUND 0
#define FOUND 1

#pragma mark - Allocation and deallocation utility functions and structures

/** A pair of allocation and deallocation functions */
typedef struct cbmap_allocator {
  cbmap_allocator_fn malloc;
  cbmap_deallocator_fn free;
} * cbmap_allocator_t;

void*
cbmap_default_allocator(void* data, size_t data_len) {
  void* data_copy = CBM_MALLOC(data_len);
  if(data_copy == NULL) {
    return NULL;
  }
  memcpy(data_copy, data, data_len);
  return data_copy;
}

void
cbmap_default_deallocator(void* data) {
  CBM_FREE(data);
}

struct cbmap_allocator CBMAP_DEFAULT_ALLOCATOR = {.malloc = cbmap_default_allocator, .free = cbmap_default_deallocator};

void*
cbmap_nop_allocator(void* data, size_t data_len) {
  return data;
}

void
cbmap_nop_deallocator(void* data) {
  ; /* Empty */
}

struct cbmap_allocator CBMAP_NOP_ALLOCATOR = {.malloc = cbmap_nop_allocator, .free = cbmap_nop_deallocator};

#pragma mark - cbmap_data_node : holds keys and values

struct cbmap_data_node {
  uint8_t* key;
  uint8_t* value;
  size_t key_len;
  size_t value_len;
};

struct cbmap_data_node*
cbmap_data_node_new(cbmap_allocator_t key_allocator, cbmap_allocator_t value_allocator, void* key, size_t key_len, void* value, size_t value_len) {
  struct cbmap_data_node* data;

  /* Data node (memaligned) */
  int fail = CBM_MEMALIGN((void**)&data, sizeof(void*), sizeof(struct cbmap_data_node));
  if(fail) {
    return NULL;
  }

  /* Allocate value */
  data->value_len = value_len;
  data->value = value_allocator->malloc(value, value_len);
  if(data->value == NULL) {
    CBM_FREE(data);
    return NULL;
  }
  /* Allocate key */
  data->key_len = key_len;
  data->key = key_allocator->malloc(key, key_len);
  if(data->key == NULL) {
    value_allocator->free(data->value);
    CBM_FREE(data);
    return NULL;
  }
  return data;
}

void*
cbmap_data_node_update_value(cbmap_allocator_t value_allocator, struct cbmap_data_node* data, void* value, size_t value_len) {
  /* If the data is the same then do not free/alloc */
  if(data->value_len == value_len && VALUE_COMPARE(data->value, value, value_len) == 0) {
    return data->value;
  }

  /* Create a new value to replace the old one */
  void* new_value = value_allocator->malloc(value, value_len);
  if(new_value == NULL) {
    return NULL;
  }

  /* Free the old value */
  value_allocator->free(data->value);

  /* Update data with the new value */
  data->value_len = value_len;
  data->value = new_value;

  return data->value;
}

void
cbmap_data_node_destroy(struct cbmap_data_node* data, cbmap_allocator_t key_allocator, cbmap_allocator_t value_allocator) {
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

#pragma mark - cbmap_internal_node : holds structural information of nodes

/**
 A cbmap_internal_node is a branching node on a given bit.
*/
struct cbmap_internal_node {
  /* The two branches */
  void* branch[2];
  /* The number of the byte where the bits differ */
  uint32_t byte;
  /* All ones but the branching bit, that is set to 0 */
  uint8_t otherbits;
};

struct cbmap_internal_node*
cbmap_internal_node_new(void) {
  struct cbmap_internal_node* node;
  int fail = CBM_MEMALIGN((void**)&node, sizeof(uint8_t*), sizeof(struct cbmap_internal_node));
  if(fail) {
    return NULL;
  }
  node->branch[0] = NULL;
  node->branch[1] = NULL;
  node->byte = 0;
  node->otherbits = 0;
  return node;
}

void
cbmap_internal_node_destroy(struct cbmap_internal_node* node) {
  /* Note: Children must have been previously freed */
  if(node != NULL) {
    CBM_FREE(node);
  }
}

#pragma mark - cbmap

#define IS_INTERNAL_NODE(p) (1 & (intptr_t)(p))
#define GET_INTERNAL_NODE(p) ((struct cbmap_internal_node*)(((intptr_t)(p)) - 1))

#define IS_DATA_NODE(p) (!(IS_INTERNAL_NODE(p)))
#define GET_DATA_NODE(p) ((struct cbmap_data_node*)(p))

struct cbmap {
  void* root;
  size_t count;
  struct cbmap_allocator key_allocator;
  struct cbmap_allocator value_allocator;
};

#pragma mark - cbmap_new

cbmap_t
cbmap_new_with_custom_allocation(cbmap_allocator_fn key_allocator, cbmap_deallocator_fn key_deallocator, cbmap_allocator_fn value_allocator,
                                 cbmap_deallocator_fn value_deallocator) {
  cbmap_t map = CBM_MALLOC(sizeof(struct cbmap));
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

cbmap_t
cbmap_new(void) {
  return cbmap_new_with_custom_allocation(CBMAP_DEFAULT_ALLOCATOR.malloc, CBMAP_DEFAULT_ALLOCATOR.free, CBMAP_DEFAULT_ALLOCATOR.malloc,
                                          CBMAP_DEFAULT_ALLOCATOR.free);
}

cbmap_t
cbmap_new_with_static_keys_and_values(void) {
  return cbmap_new_with_custom_allocation(CBMAP_NOP_ALLOCATOR.malloc, CBMAP_NOP_ALLOCATOR.free, CBMAP_NOP_ALLOCATOR.malloc, CBMAP_NOP_ALLOCATOR.free);
}

#pragma mark - cbmap_destroy

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

#pragma mark - cbmap_insert

int
cbmap_insert(cbmap_t map, void* key, size_t key_len, void* value, size_t value_len) {
  uint8_t* p = map->root;
  const uint8_t* const key_bytes = (void*)key;

  if(p == NULL) {
    struct cbmap_data_node* data = cbmap_data_node_new(&map->key_allocator, &map->value_allocator, key, key_len, value, value_len);
    if(data == NULL) {
      return INSERT_OUT_OF_MEMORY;
    }
    map->root = data;
    map->count++;
    return INSERT_OK;
  }

  while(IS_INTERNAL_NODE(p)) {
    struct cbmap_internal_node* node = GET_INTERNAL_NODE(p);
    uint8_t c = 0;
    if(node->byte < key_len) {
      c = key_bytes[node->byte];
    }
    const int direction = (1 + (node->otherbits | c)) >> 8;
    p = node->branch[direction];
  }

  uint32_t newbyte;
  uint32_t newotherbits;

  struct cbmap_data_node* data = GET_DATA_NODE(p);

  for(newbyte = 0; newbyte < key_len; ++newbyte) {
    if(data->key[newbyte] != key_bytes[newbyte]) {
      newotherbits = data->key[newbyte] ^ key_bytes[newbyte];
      goto different_byte_found;
    }
  }

  if(newbyte == key_len) {
    void* result = cbmap_data_node_update_value(&map->value_allocator, data, value, value_len);
    return result == NULL ? INSERT_OUT_OF_MEMORY : INSERT_UPDATED;
  } else {
    newotherbits = data->key[newbyte];
  }

different_byte_found:

  newotherbits |= newotherbits >> 1;
  newotherbits |= newotherbits >> 2;
  newotherbits |= newotherbits >> 4;
  newotherbits = (newotherbits & ~(newotherbits >> 1)) ^ 255;
  uint8_t c = data->key[newbyte];
  int newdirection = (1 + (newotherbits | c)) >> 8;

  struct cbmap_internal_node* newnode = cbmap_internal_node_new();
  if(newnode == NULL) {
    return INSERT_OUT_OF_MEMORY;
  }

  struct cbmap_data_node* newdata = cbmap_data_node_new(&map->key_allocator, &map->value_allocator, key, key_len, value, value_len);
  if(newdata == NULL) {
    cbmap_internal_node_destroy(newnode);
    return INSERT_OUT_OF_MEMORY;
  }

  newnode->byte = newbyte;
  newnode->otherbits = newotherbits;
  newnode->branch[1 - newdirection] = newdata;

  void** insertion_node = &map->root;
  for(;;) {
    uint8_t* p = *insertion_node;
    if(!IS_INTERNAL_NODE(p)) {
      break;
    }
    struct cbmap_internal_node* q = GET_INTERNAL_NODE(p);
    if(q->byte > newbyte) {
      break;
    }
    if(q->byte == newbyte && q->otherbits > newotherbits) {
      break;
    }
    uint8_t c = 0;
    if(q->byte < key_len) {
      c = key_bytes[q->byte];
    }
    const int direction = (1 + (q->otherbits | c)) >> 8;
    insertion_node = q->branch + direction;
  }

  newnode->branch[newdirection] = *insertion_node;
  *insertion_node = (void*)(1 + (uint8_t*)newnode);
  map->count++;

  return INSERT_OK;
}

#pragma mark - cbmap_get

int
cbmap_get(cbmap_t map, void* key, size_t key_len, void** value, size_t* value_len) {
  uint8_t* p = map->root;
  if(p == NULL || key == NULL) {
    return 0;
  }
  const uint8_t* key_bytes = (const uint8_t*)key;

  while(IS_INTERNAL_NODE(p)) {
    struct cbmap_internal_node* q = GET_INTERNAL_NODE(p);
    uint8_t c = 0;
    if(q->byte < key_len) {
      c = key_bytes[q->byte];
    }
    const int direction = (1 + (q->otherbits | c)) >> 8;
    p = q->branch[direction];
  }

  struct cbmap_data_node* data = (struct cbmap_data_node*)p;
  if(data->key_len == key_len) {
    if(KEY_COMPARE(data->key, key, key_len) == 0) {
      *value = (void*)data->value;
      *value_len = data->value_len;
      return FOUND;
    }
  }

  return NOT_FOUND;
}

#pragma mark - Removal

int
cbmap_delete(cbmap_t map, void* key, size_t key_len) {
  uint8_t* p = map->root;
  if(p == NULL || key == NULL) {
    return 0;
  }
  const uint8_t* key_bytes = (const uint8_t*)key;
  void** wherep = &map->root;
  void** whereq = NULL;
  struct cbmap_internal_node* q = NULL;
  int direction = 0;

  while(IS_INTERNAL_NODE(p)) {
    whereq = wherep;
    q = GET_INTERNAL_NODE(p);
    uint8_t c = 0;
    if(q->byte < key_len) {
      c = key_bytes[q->byte];
    }
    direction = (1 + (q->otherbits | c)) >> 8;
    wherep = &q->branch[direction];
    p = *wherep;
  }

  struct cbmap_data_node* data = (struct cbmap_data_node*)p;
  if(data->key_len != key_len || KEY_COMPARE(data->key, key, key_len) != 0) {
    return NOT_FOUND;
  }

  map->count--;
  cbmap_data_node_destroy(data, &map->key_allocator, &map->value_allocator);

  if(whereq == NULL) {
    map->root = NULL;
    return FOUND;
  }

  *whereq = q->branch[1 - direction];
  cbmap_internal_node_destroy(q);

  return FOUND;
}

#pragma mark - Visiting

int
cbmap_visit(uint8_t* top, cbmap_visitor visitor_fn, void* user_data) {
  if(IS_INTERNAL_NODE(top)) {
    struct cbmap_internal_node* q = GET_INTERNAL_NODE(top);
    for(int direction = 0; direction < 2; ++direction) {
      if(cbmap_visit(q->branch[direction], visitor_fn, user_data) != 1) {
        return 0;
      }
    }
    return 1;
  }
  struct cbmap_data_node* data = GET_DATA_NODE(top);
  return visitor_fn(data->key, data->key_len, data->value, data->value_len, user_data);
}

int
cbmap_visit_prefix(cbmap_t map, uint8_t* key_prefix, size_t key_prefix_len, cbmap_visitor visitor_fn, void* user_data) {
  uint8_t* p = map->root;
  if(p == NULL) {
    return 1;
  }
  uint8_t* top = p;

  while(IS_INTERNAL_NODE(p)) {
    struct cbmap_internal_node* q = GET_INTERNAL_NODE(p);
    uint8_t c = 0;
    if(q->byte < key_prefix_len) {
      c = key_prefix[q->byte];
    }
    const int direction = (1 + (q->otherbits | c)) >> 8;
    p = q->branch[direction];
    if(q->byte < key_prefix_len) {
      top = p;
    }
  }
  struct cbmap_data_node* data = GET_DATA_NODE(p);
  for(size_t i = 0; i < key_prefix_len; i++) {
    if(data->key[i] != key_prefix[i]) {
      return 1;
    }
  }
  return cbmap_visit(top, visitor_fn, user_data);
}

int
cbmap_visit_all(cbmap_t map, cbmap_visitor visitor_fn, void* user_data) {
  uint8_t empty_prefix[0];
  return cbmap_visit_prefix(map, empty_prefix, 0, visitor_fn, user_data);
}

#pragma mark - count

size_t
cbmap_count(cbmap_t map) {
  return map->count;
}
