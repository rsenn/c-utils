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

int critbit0_delete(critbit0_tree* t,const char* u) {
  const uint8_t* ubytes = (void*)u;
  const size_t ulen = strlen(u);
  uint8_t* p = t->root;
  void** wherep = &t->root;
  void** whereq = 0;
  critbit0_node* q = 0;
  int direction = 0;

  if (!p) return 0;

  while ((intptr_t)p&1) {
    whereq = wherep;
    q = (void*)(p-1);
    uint8_t c = 0;
    if (q->byte<ulen)
      c = ubytes[q->byte];
    direction = (1+(q->otherbits|c))>>8;
    wherep = q->child+direction;
    p = *wherep;
  }

  if (0!=strcmp(u,(const char*)p))
    return 0;
  free(p);

  if (!whereq) {
    t->root = 0;
    return 1;
  }

  *whereq = q->child[1-direction];
  free(q);

  return 1;
}
