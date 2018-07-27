#include "../cb_internal.h"

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
  if(len >= keylen && memcmp(key, str, keylen) == 0) {
    return str;
  }
  return 0;
}
