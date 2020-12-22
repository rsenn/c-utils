#ifndef __LIST_H
#define __LIST_H

#include <stdlib.h>
#include <sys/types.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct list {
  struct node* head;
  struct node* tail;
} list;

typedef struct node {
  struct node* prev;
  struct node* next;
} node;

void dlist_add_after(list*, node*, node* after);
void dlist_add_before(list*, node*, node* before);
void list_init(list*);
size_t list_length(list*);
void list_push(list*, node*);
void list_remove(list*, node*);
void list_unshift(list*, node*);

#define dlist_foreach_down(list, n) for((n) = (void*)(list)->head; (n) != NULL; (n) = (void*)(((node*)n)->next))

/* n is set to the current node and n->next is backupped
   into m before loop body for safe walk-throught when
   links get deleted */
#define dlist_foreach_down_safe(list, n, m)                                                                                                                                                             \
  for((n) = (void*)(list)->head, (m) = (void*)((node*)n) != NULL ? (void*)((node*)n)->next : NULL; (n) != NULL;                                                                                        \
      (n) = (void*)((node*)m), (m) = (void*)((node*)n) != NULL ? (void*)((node*)n)->next : NULL)

/*
 * Macros to walk through a list list from tail to head.                     *
 */
/* n is set to the current node*/
#define list_foreach_up(list, n) for((n) = (void*)(list)->tail; (n) != NULL; (n) = (void*)(((node*)n)->prev))

/* n is set to the current node and n->prev is backupped
   into m before loop body for safe walk-throught when
   links get deleted */
#define list_foreach_up_safe(list, n, m)                                                                                                                                                               \
  for((n) = (void*)(list)->tail, (m) = (void*)((node*)n) != NULL ? (void*)((node*)n)->prev : NULL; (n) != NULL;                                                                                        \
      (n) = (void*)((node*)m), (m) = (void*)((node*)n) != NULL ? (void*)((node*)n)->prev : NULL)

/* aliases for backwards compatibility */
#define dlist_foreach dlist_foreach_down
#define list_foreach_safe dlist_foreach_down_safe

#ifdef __cplusplus
}
#endif
#endif /* defined(__LIST_H) */
