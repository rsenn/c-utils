#ifndef LIST_H
#define LIST_H

#include <stdlib.h>
#include <sys/types.h>

#ifdef __cplusplus
extern "C"{
#endif

typedef struct list {
  struct link* head;
  struct link* tail;
} list;

typedef struct link {
  struct link* prev;
  struct link* next;
} link;

void list_add_after(list*, link*, link *after);
void list_add_before(list*, link*, link *before);
void list_init(list*);
size_t list_length(list*);
void list_push(list*, link*);
void list_remove(list*, link*);
void list_unshift(list*, link*);

#define list_foreach_down(list, n) \
  for((n) = (void*)(list)->head; \
      (n) != NULL; \
      (n) = (void*)(((link*)n)->next))

/* n is set to the current link and n->next is backupped
   into m before loop body for safe walk-throught when
   links get deleted */
#define list_foreach_down_safe(list, n, m) \
  for((n) = (void*)(list)->head, \
      (m) = (void*)((link*)n) != NULL ? (void*)((link*)n)->next : NULL; \
      (n) != NULL; \
      (n) = (void*)((link*)m), \
      (m) = (void*)((link*)n) != NULL ? (void*)((link*)n)->next : NULL)

/* ------------------------------------------------------------------------ *
 * Macros to walk through a list list from tail to head.                     *
 * ------------------------------------------------------------------------ */
/* n is set to the current link*/
#define list_foreach_up(list, n) \
  for((n) = (void*)(list)->tail; \
      (n) != NULL; \
      (n) = (void*)(((link*)n)->prev))

/* n is set to the current link and n->prev is backupped
   into m before loop body for safe walk-throught when
   links get deleted */
#define list_foreach_up_safe(list, n, m) \
  for((n) = (void*)(list)->tail, \
      (m) = (void*)((link*)n) != NULL ? (void*)((link*)n)->prev : NULL; \
      (n) != NULL; \
      (n) = (void*)((link*)m), \
      (m) = (void*)((link*)n) != NULL ? (void*)((link*)n)->prev : NULL)

/* aliases for backwards compatibility */
#define list_foreach           list_foreach_down
#define list_foreach_safe      list_foreach_down_safe

#ifdef __cplusplus
}
#endif
#endif /* defined(LIST_H) */
