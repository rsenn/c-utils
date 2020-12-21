#include "../cb_internal.h"

int
cb_insert(critbit_tree* cb, const void* key, size_t keylen) {
  struct critbit_node* node;
  unsigned char* bytes = (unsigned char*)key;
  assert(cb);
  assert(key);
  if(!cb->root) {
    cb->root = make_external_node(key, keylen);
    return CB_SUCCESS;
  } else {
    void** iter = &cb->root;
    struct critbit_node* prev = 0;
    for(;;) {
      void* ptr = *iter;
      if(decode_pointer(&ptr) == INTERNAL_NODE) {
        node = (struct critbit_node*)ptr;
        int branch =
            (keylen <= node->byte)
                ? 0
                : ((1 + ((bytes[node->byte] | node->mask) & 0xFF)) >> 8);
        iter = &node->child[branch];
        prev = node;
      } else {
        unsigned char *iptr, *ikey = bytes;
        void* vptr;
        unsigned int mask, byte = 0;
        int branch;
        size_t len;
        struct critbit_node* node;

        from_external_node(ptr, &vptr, &len);

        for(iptr = vptr; byte < keylen && byte < len && *ikey == *iptr;) {
          ++ikey;
          ++iptr;
          ++byte;
        }

        if(byte == keylen && byte == len)
          return CB_EXISTS; /* duplicate entry */

        node = make_internal_node();
        node->byte = byte;
        mask = *ikey ^ *iptr; /* these are all the bits that differ */
        mask |= mask >> 1;
        mask |= mask >> 2;
        mask |= mask >> 4; /* now, every bit up to the MSB is set to 1 */
        mask = (mask & ~(mask >> 1)) ^ 0xFF;
        node->mask = (unsigned char)mask;

        /* find the right place to insert, iff prev's crit-bit is later in the
         * string than new crit-bit */
        if(prev && cb_less(node, prev)) {
          for(iter = &cb->root;;) {
            ptr = *iter;
            if(decode_pointer(&ptr) == INTERNAL_NODE) {
              struct critbit_node* next = (struct critbit_node*)ptr;
              if(cb_less(next, node)) {
                branch = ((1 + ((bytes[next->byte] | next->mask) & 0xFF)) >> 8);
                iter = &next->child[branch];
              } else {
                break;
              }
            } else {
              assert(!"should never get here");
            }
          }
        }

        branch = ((1 + ((*ikey | node->mask) & 0xFF)) >> 8);
        node->child[branch] = make_external_node(key, keylen);
        node->child[1 - branch] = *iter;
        *iter = (void*)node;

        return CB_SUCCESS;
      }
    }
  }
}
