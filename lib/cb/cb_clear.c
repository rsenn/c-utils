#include "../cb_internal.h"

void
cb_clear(critbit_tree* cb) {
  if(cb->root) {
    cb_free_node(cb->root);
    cb->root = 0;
  }
}
