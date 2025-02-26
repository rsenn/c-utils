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

static int
cb_find_prefix_i(void* ptr, const void* key, size_t keylen, void** results, int numresults, int* offset, int next) {
  struct critbit_node* node;
  assert(next <= numresults);

  if(next == numresults)
    return next;

  if(decode_pointer(&ptr) == INTERNAL_NODE) {
    node = (struct critbit_node*)ptr;

    if(numresults > (next = cb_find_prefix_i(node->child[0], key, keylen, results, numresults, offset, next)))
      next = cb_find_prefix_i(node->child[1], key, keylen, results, numresults, offset, next);

  } else {
    /* reached an external node */
    char* str;
    void* vptr;
    size_t len;

    from_external_node(ptr, &vptr, &len);
    str = vptr;

    if(len >= keylen && byte_diff(key, keylen, str) == 0) {
      if(*offset > 0)
        --*offset;
      else
        results[next++] = str;
    }
  }
  return next;
}

int
cb_find_prefix(const critbit_tree* cb, const void* key, size_t keylen, void** results, int numresults, int offset) {
  void* top;

  if(numresults > 0 && (top = cb_find_top_i(cb, key, keylen)))
    /* recursively add all children except the ones from [0-offset) of top
     * to the results */
    return cb_find_prefix_i(top, key, keylen, results, numresults, &offset, 0);

  return 0;
}
