#include "../cbmap_internal.h"

/** mark - cbmap_get */

int
cbmap_get(cbmap_t map, void* key, size_t key_len, void** value, size_t* value_len) {
  unsigned char* p = map->root;
  if(p == NULL || key == NULL) {
    return 0;
  }
  const unsigned char* key_bytes = (const unsigned char*)key;

  while(IS_INTERNAL_NODE(p)) {
    struct cbmap_internal_node* q = GET_INTERNAL_NODE(p);
    unsigned char c = 0;
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
