#include <stddef.h>
#include <stdint.h>	/* size_t, uintptr_t */

#include <string.h>
#include <stdlib.h>

#include "../critbit.h"

typedef struct {
  void* child[2];
  uint32_t byte;
  uint8_t otherbits;
} critbit0_node;

#if 0
typedef struct{
  void* root;
} critbit0_tree;
#endif

static void traverse(void* top) {
  uint8_t* p = top;

  if ((intptr_t)p&1) {
    critbit0_node* q = (void*)(p-1);
    traverse(q->child[0]);
    traverse(q->child[1]);
    free(q);
  } else {
    free(p);
  }
}

void critbit0_clear(critbit0_tree* t) {
  if (t->root)
    traverse(t->root);
  t->root = NULL;
}
