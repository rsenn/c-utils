#include "../byte.h"
#include "../cb_internal.h"

static int
cb_foreach_i(void* ptr,
             const void* key,
             size_t keylen,
             int (*match_cb)(const void* match, const void* key, size_t keylen, void*),
             void* data) {
  int result = 0;
  struct critbit_node* node;

  if(decode_pointer(&ptr) == INTERNAL_NODE) {
    node = ptr;

    if(!(result = cb_foreach_i(node->child[0], key, keylen, match_cb, data)))
      result = cb_foreach_i(node->child[1], key, keylen, match_cb, data);

  } else {
    /* reached an external node */
    void* match;
    size_t len;

    from_external_node(ptr, &match, &len);
    if(len >= keylen && byte_diff(key, keylen, match) == 0)
      return match_cb(match, key, keylen, data);
  }
  return result;
}

int
cb_foreach(critbit_tree* cb,
           const void* key,
           size_t keylen,
           int (*match_cb)(const void* match, const void* key, size_t keylen, void*),
           void* data) {
  void* top;

  if((top = cb_find_top_i(cb, key, keylen)))
    /* recursively add all children except the ones from [0-offset) of top to
     * the results */
    return cb_foreach_i(top, key, keylen, match_cb, data);

  return 0;
}
