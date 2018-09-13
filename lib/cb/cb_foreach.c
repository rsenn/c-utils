#include "../byte.h"
#include "../cb_internal.h"
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
    if(len >= keylen && byte_diff(key, keylen, match) == 0) {
      return match_cb(match, key, keylen, data);
    }
  }
  return result;
}
