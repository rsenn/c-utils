#include "../cbmap_internal.h"

int
cbmap_delete(cbmap_t map, void* key, size_t key_len) {
  unsigned char* p = map->root;
  const unsigned char* key_bytes = (const unsigned char*)key;
  void** wherep = &map->root;
  void** whereq = NULL;
  struct cbmap_internal_node* q = NULL;
  int direction = 0;
    struct cbmap_data_node* data;

  if(p == NULL || key == NULL) {
    return 0;
  }
  
  while(IS_INTERNAL_NODE(p)) {
    unsigned char c = 0;
    whereq = wherep;
    q = GET_INTERNAL_NODE(p);
    if(q->byte < key_len) {
      c = key_bytes[q->byte];
    }
    direction = (1 + (q->otherbits | c)) >> 8;
    wherep = &q->branch[direction];
    p = *wherep;
  }

data = (struct cbmap_data_node*)p;
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
