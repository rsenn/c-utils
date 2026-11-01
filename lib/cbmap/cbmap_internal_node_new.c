#include "cbmap_alloc.h"
#include "../cbmap_internal.h"

struct cbmap_internal_node*
cbmap_internal_node_new(void) {
  struct cbmap_internal_node* node;
  int fail = CBM_MEMALIGN((void**)&node, sizeof(unsigned char*), sizeof(struct cbmap_internal_node));

  if(fail) {
    return NULL;
  }
  node->branch[0] = NULL;
  node->branch[1] = NULL;
  node->byte = 0;
  node->otherbits = 0;
  return node;
}
