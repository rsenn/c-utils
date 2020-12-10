#include "../byte.h"
#include "../cb_internal.h"
int
cb_erase(critbit_tree* cb, const void* key, size_t keylen) {
  void** iter = NULL;
  void* ptr = cb->root;
  struct critbit_node* parent = 0;
  unsigned char* bytes = (unsigned char*)key;
  int branch = 0;

  if(!cb->root)
    return CB_NOTFOUND;

  for(;;) {
    int type;

    type = decode_pointer(&ptr);
    if(type == INTERNAL_NODE) {
      iter = parent ? &parent->child[branch] : &cb->root;
      parent = (struct critbit_node*)ptr;
      branch = (keylen <= parent->byte) ? 0 : ((1 + ((bytes[parent->byte] | parent->mask) & 0xFF)) >> 8);
      ptr = parent->child[branch];
    } else {
      void* str;
      size_t len;
      from_external_node(ptr, &str, &len);
      if(len == keylen && byte_diff(key, len, str) == 0) {
        free(ptr);
        if(iter) {
          *iter = parent->child[1 - branch];
          free(parent);
        } else {
          cb->root = 0;
        }
        return CB_SUCCESS;
      }
      return CB_NOTFOUND;
    }
  }
}
