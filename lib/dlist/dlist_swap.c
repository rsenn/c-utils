#include "../dlist.h"

/* swaps the places of two nodes of the list */
void
dlist_swap(dlist* l, node* a, node* b) {
  node *ap, *an, *bp, *bn;

  if(a == b)
    return;

  /* neighbours: a is the one in front */
  if(b->next == a) {
    node* t = a;

    a = b;
    b = t;
  }

  ap = a->prev;
  an = a->next;
  bp = b->prev;
  bn = b->next;

  if(an == b) {
    b->prev = ap;
    b->next = a;
    a->prev = b;
    a->next = bn;
  } else {
    a->prev = bp;
    a->next = bn;
    b->prev = ap;
    b->next = an;

    if(an)
      an->prev = b;
    else
      l->tail = b;

    if(bp)
      bp->next = a;
    else
      l->head = a;
  }

  if(ap)
    ap->next = b;
  else
    l->head = b;

  if(bn)
    bn->prev = a;
  else
    l->tail = a;
}
