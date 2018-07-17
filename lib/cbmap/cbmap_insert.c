#include "alloc.h"
#include "../cbmap_internal.h"

static void*
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

static struct cbmap_data_node*
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

/** mark - cbmap_insert */

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
