#include "../cbmap_internal.h"

/** mark - Visiting */

int
cbmap_visit(unsigned char* top, cbmap_visitor visitor_fn, void* user_data) {
  struct cbmap_data_node* data;
  if(IS_INTERNAL_NODE(top)) {
    int direction;
    struct cbmap_internal_node* q = GET_INTERNAL_NODE(top);
    for(direction = 0; direction < 2; ++direction) {
      if(cbmap_visit((unsigned char*)q->branch[direction], visitor_fn, user_data) != 1) {
        return 0;
      }
    }
    return 1;
  }
  data = GET_DATA_NODE(top);
  return visitor_fn(data->key, data->key_len, data->value, data->value_len, user_data);
}

int
cbmap_visit_prefix(cbmap_t map,
                   unsigned char* key_prefix,
                   size_t key_prefix_len,
                   cbmap_visitor visitor_fn,
                   void* user_data) {
  unsigned char* p = (unsigned char*)map->root;
  unsigned char* top;
  struct cbmap_data_node* data;
  size_t i;

  if(p == NULL) {
    return 1;
  }
  top = p;

  while(IS_INTERNAL_NODE(p)) {
    struct cbmap_internal_node* q = GET_INTERNAL_NODE(p);
    unsigned char c = 0;
    int direction;
    if(q->byte < key_prefix_len) {
      c = key_prefix[q->byte];
    }
    direction = (1 + (q->otherbits | c)) >> 8;
    p = (unsigned char*)q->branch[direction];
    if(q->byte < key_prefix_len) {
      top = p;
    }
  }
  data = GET_DATA_NODE(p);
  for(i = 0; i < key_prefix_len; i++) {
    if(data->key[i] != key_prefix[i]) {
      return 1;
    }
  }
  return cbmap_visit(top, visitor_fn, user_data);
}
