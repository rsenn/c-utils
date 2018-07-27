#include "../cb_internal.h"

static void*
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

static int
cb_foreach_i(void* ptr, const void* key, size_t keylen, int (*match_cb)(const void* match, const void* key, size_t keylen, void*), void* data) {
  int result = 0;

  if(decode_pointer(&ptr) == INTERNAL_NODE) {
    struct critbit_node* node = (struct critbit_node*)ptr;
    result = cb_foreach_i(node->child[0], key, keylen, match_cb, data);
    if(!result) {
      result = cb_foreach_i(node->child[1], key, keylen, match_cb, data);
    }
  } else {
    /* reached an external node */
    void* match;
    size_t len;

    from_external_node(ptr, &match, &len);
    if(len >= keylen && memcmp(key, match, keylen) == 0) {
      return match_cb(match, key, keylen, data);
    }
  }
  return result;
}

int
cb_foreach(critbit_tree* cb, const void* key, size_t keylen, int (*match_cb)(const void* match, const void* key, size_t keylen, void*), void* data) {
  void* top = cb_find_top_i(cb, key, keylen);
  if(top) {
    /* recursively add all children except the ones from [0-offset) of top to the results */
    return cb_foreach_i(top, key, keylen, match_cb, data);
  }
  return 0;
}
