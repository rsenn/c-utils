#include "../byte.h"
#include "alloc.h"
#include "../cbmap_internal.h"

static void*
cbmap_data_node_update_value(cbmap_allocator_t value_allocator, struct cbmap_data_node* data, void* value, size_t value_len) {
   void* new_value ;
   /* If the data is the same then do not free/alloc */
  if(data->value_len == value_len && VALUE_COMPARE(data->value, value, value_len) == 0) {
    return data->value;
  }

  /* Create a new value to replace the old one */
  new_value = value_allocator->malloc(value, value_len);
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
  unsigned char c, *p = map->root;
  const unsigned char* const key_bytes = (void*)key;
  unsigned int newbyte, newotherbits;
  struct cbmap_data_node* data , *newdata;
   int newdirection;
   struct cbmap_internal_node* newnode, *q;
   void** insertion_node;
   
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
    unsigned char c = 0;
    int direction;
    if(node->byte < key_len) {
      c = key_bytes[node->byte];
    }
    direction = (1 + (node->otherbits | c)) >> 8;
    p = node->branch[direction];
  }

data = GET_DATA_NODE(p);

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
c = data->key[newbyte];
  newdirection = (1 + (newotherbits | c)) >> 8;

  newnode = cbmap_internal_node_new();
  if(newnode == NULL) {
    return INSERT_OUT_OF_MEMORY;
  }

  newdata = cbmap_data_node_new(&map->key_allocator, &map->value_allocator, key, key_len, value, value_len);
  if(newdata == NULL) {
    cbmap_internal_node_destroy(newnode);
    return INSERT_OUT_OF_MEMORY;
  }

  newnode->byte = newbyte;
  newnode->otherbits = newotherbits;
  newnode->branch[1 - newdirection] = newdata;

    insertion_node = &map->root;
  for(;;) {
      unsigned char c = 0;
    unsigned char* p = *insertion_node;
    int direction;
    if(!IS_INTERNAL_NODE(p)) {
      break;
    }
    q = GET_INTERNAL_NODE(p);
    if(q->byte > newbyte) {
      break;
    }
    if(q->byte == newbyte && q->otherbits > newotherbits) {
      break;
    }
    if(q->byte < key_len) {
      c = key_bytes[q->byte];
    }
    direction = (1 + (q->otherbits | c)) >> 8;
    insertion_node = q->branch + direction;
  }

  newnode->branch[newdirection] = *insertion_node;
  *insertion_node = (void*)(1 + (unsigned char*)newnode);
  map->count++;

  return INSERT_OK;
}
