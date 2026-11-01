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

int critbit0_insert(critbit0_tree* t,const char* u) {
  const uint8_t* const ubytes = (void*)u;
  const size_t ulen = strlen(u);
  uint8_t* p = t->root;

  if (!p) {
    char* x = malloc(ulen+1);
    if (!x) return 0;
    memcpy(x,u,ulen+1);
    t->root= x;
    return 2;
  }

  while (1&(intptr_t)p) {
    critbit0_node* q = (void*)(p-1);

    uint8_t c = 0;
    if (q->byte<ulen)
      c = ubytes[q->byte];
    const int direction = (1+(q->otherbits|c))>>8;

    p = q->child[direction];
  }

  uint32_t newbyte;
  uint32_t newotherbits;

  for (newbyte = 0; newbyte < ulen; ++newbyte) {
    if (p[newbyte] != ubytes[newbyte]) {
      newotherbits = p[newbyte]^ubytes[newbyte];
      goto different_byte_found;
    }
  }

  if (p[newbyte]!=0) {
    newotherbits = p[newbyte];
    goto different_byte_found;
  }
  return 1;

different_byte_found:

  newotherbits |= newotherbits>>1;
  newotherbits |= newotherbits>>2;
  newotherbits |= newotherbits>>4;
  newotherbits = (newotherbits&~(newotherbits>>1))^255;
  uint8_t c = p[newbyte];
  int newdirection = (1+(newotherbits|c))>>8;

  critbit0_node* newnode;
  if (!(newnode=malloc(sizeof(critbit0_node))))
    return 0;

  char* x;
  if (!(x = malloc(ulen+1))) {
    free(newnode);
    return 0;
  }
  memcpy(x,ubytes,ulen+1);

  newnode->byte= newbyte;
  newnode->otherbits= newotherbits;
  newnode->child[1-newdirection]= x;

  void** wherep= &t->root;
  for(;;) {
    uint8_t* p = *wherep;
    if (!((intptr_t)p&1))
      break;
    critbit0_node* q = (void*)(p-1);
    if (q->byte > newbyte)break;
    if (q->byte==newbyte && q->otherbits>newotherbits)break;
    uint8_t c = 0;
    if (q->byte<ulen)
      c = ubytes[q->byte];
    const int direction = (1+(q->otherbits|c))>>8;
    wherep = q->child+direction;
  }

  newnode->child[newdirection]= *wherep;
  *wherep= (void*)(1+(char*)newnode);

  return 2;
}
