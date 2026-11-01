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

static int allprefixed_traverse(uint8_t* top,int(*handle)(const char*,void*),void* arg) {
  if ((uintptr_t)top&1) {
    critbit0_node* q = (void*)(top-1);
    int direction;
    for (direction=0; direction<2; ++direction)
      switch (allprefixed_traverse(q->child[direction],handle,arg)) {
      case 1: break;
      case 0: return 0;
      default: return-1;
      }
    return 1;
  }

  return handle((const char*)top,arg);
}

int critbit0_allprefixed(critbit0_tree* t,const char* prefix,int(*handle)(const char*,void*),void* arg) {
  const uint8_t* ubytes = (void*)prefix;
  const size_t ulen = strlen(prefix);
  uint8_t* p = t->root;
  uint8_t* top = p;

  if (!p) return 1;

  while ((uintptr_t)p&1) {
    critbit0_node* q = (void*)(p-1);
    uint8_t c = 0;
    if (q->byte<ulen)
      c=ubytes[q->byte];
    const int direction = (1+(q->otherbits|c))>>8;
    p = q->child[direction];
    if (q->byte<ulen)
      top = p;
  }

  size_t i;
  for (i=0; i<ulen; ++i) {
    if (p[i]!=ubytes[i])
      return 1;
  }

  return allprefixed_traverse(top,handle,arg);
}
