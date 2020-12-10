#include "../cb_internal.h"

void*
cb_find_top_i(const critbit_tree* cb, const void* key, size_t keylen) {
  void *ptr, *top = 0;
  assert(keylen == 0 || key);

  if(!cb->root) {
    return 0;
  }
  for(ptr = cb->root, top = cb->root;;) {
    void* last = ptr;
    if(decode_pointer(&ptr) == INTERNAL_NODE) {
      struct critbit_node* node = (struct critbit_node*)ptr;
      int branch;
      if(keylen <= node->byte) {
        break;
      } else {
        unsigned char* bytes = (unsigned char*)key;
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

const void*
cb_find(critbit_tree* cb, const void* key, size_t keylen) {
  void* str;
  size_t len;
  unsigned char* bytes = (unsigned char*)key;
  void* ptr;

  assert(cb);
  assert(key);
  if(!cb->root)
    return 0;
  for(ptr = cb->root; decode_pointer(&ptr) == INTERNAL_NODE;) {
    struct critbit_node* node = (struct critbit_node*)ptr;
    int branch = (keylen <= node->byte) ? 0 : ((1 + ((bytes[node->byte] | node->mask) & 0xFF)) >> 8);
    ptr = node->child[branch];
  }
  from_external_node(ptr, &str, &len);
  if(len >= keylen && byte_diff(key, keylen, str) == 0) {
    return str;
  }
  return 0;
}
