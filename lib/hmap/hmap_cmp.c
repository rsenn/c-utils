#include "../hmap_internal.h"

int
hmap_cmp(void* v1, int v1_len, void* v2, int v2_len) {
  if(v1_len != v2_len) {
    return -1;
  }
  return byte_diff(v1, v1_len, v2);
}
