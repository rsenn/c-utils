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
