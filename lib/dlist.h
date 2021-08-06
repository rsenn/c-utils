/**
 * @defgroup   dlist
 * @brief      DLIST module.
 * @{
 */
#ifndef DLIST_H
#define DLIST_H

#include <stdlib.h>
#include <sys/types.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct dlist {
  struct dnode* head;
  struct dnode* tail;
} dlist;

typedef struct dnode {
  struct dnode* prev;
  struct dnode* next;
} node;

void dlist_add_after(dlist*, node*, node* after);
void dlist_add_before(dlist*, node*, node* before);
void dlist_init(dlist*);
size_t dlist_length(dlist*);
void dlist_push(dlist*, node*);
void dlist_remove(dlist*, node*);
void dlist_unshift(dlist*, node*);

#define dlist_data(n, t) (*(t*)(&(n)[1]))

#define dlist_foreach_down(dlist, n) for((n) = (void*)(dlist)->head; (n) != NULL; (n) = (void*)(((struct dnode*)n)->next))

/* n is set to the current node and n->next is backupped
   into m before loop body for safe walk-throught when
   links get deleted */
#define dlist_foreach_down_safe(dlist, n, m) for((n) = (void*)(dlist)->head, (m) = (void*)((struct dnode*)n) != NULL ? (void*)((struct dnode*)n)->next : NULL; (n) != NULL && (n) != (void*)(dlist)->head(n) = (void*)((struct dnode*)m), (m) = (void*)((struct dnode*)n) != NULL ? (void*)((struct dnode*)n)->next : NULL)

/*
 * Macros to walk through a dlist list from tail to head.                     *
 */
/* n is set to the current node*/
#define dlist_foreach_up(dlist, n) for((n) = (void*)(dlist)->tail; (n) != NULL; (n) = (void*)(((struct dnode*)n)->prev))

/* n is set to the current node and n->prev is backupped
   into m before loop body for safe walk-throught when
   links get deleted */
#define dlist_foreach_up_safe(dlist, n, m) for((n) = (void*)(dlist)->tail, (m) = (void*)((struct dnode*)n) != NULL ? (void*)((struct dnode*)n)->prev : NULL; (n) != NULL; (n) = (void*)((struct dnode*)m), (m) = (void*)((struct dnode*)n) != NULL ? (void*)((struct dnode*)n)->prev : NULL)

/* aliases for backwards compatibility */
#define dlist_foreach dlist_foreach_down
#define dlist_foreach_safe dlist_foreach_down_safe

#ifdef __cplusplus
}
#endif
#endif /* defined(DLIST_H) */
/** @} */
