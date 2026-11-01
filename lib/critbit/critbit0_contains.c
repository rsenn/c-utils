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

int critbit0_contains(critbit0_tree* t,const char* u) {
  const uint8_t* ubytes= (void*)u;
  const size_t ulen= strlen(u);
  uint8_t* p= t->root;

  if (!p) return 0;

  while ((uintptr_t)p & 1) {
    critbit0_node* q = (void*)(p-1);

    uint8_t c = 0;
    if (q->byte<ulen)
      c = ubytes[q->byte];

    const int direction = (1+(q->otherbits|c))>>8;

    p = q->child[direction];
  }

  return 0==strcmp(u,(const char*)p);
}
