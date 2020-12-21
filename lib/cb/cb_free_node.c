#include "../cb_internal.h"

void
cb_free_node(void* ptr) {
  if(decode_pointer(&ptr) == INTERNAL_NODE) {
    struct critbit_node* node = (struct critbit_node*)ptr;
    cb_free_node(node->child[0]);
    cb_free_node(node->child[1]);
    free(node);
  } else {
    free(ptr);
  }
}