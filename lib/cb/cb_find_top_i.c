#include "../byte.h"
#include "../cb_internal.h"

void*
cb_find_top_i(const critbit_tree* cb, const void* key, size_t keylen) {
  void *ptr, *top = 0, *last;
  struct critbit_node* node;
  unsigned char* bytes = (unsigned char*)key;
  assert(keylen == 0 || key);

  if(!cb->root)
    return 0;

  for(ptr = cb->root, top = cb->root;;) {
    last = ptr;

    if(decode_pointer(&ptr) == INTERNAL_NODE) {
      int branch;
      node = (struct critbit_node*)ptr;

      if(keylen <= node->byte) {
        break;
      } else {
        top = last;
        branch = (1 + ((bytes[node->byte] | node->mask) & 0xFF)) >> 8;
        ptr = node->child[branch];
      }
    } else {
      /* we reached an external node before exhausting the key length */
      top = last;
      break;
    }
  }

  return top;
}
