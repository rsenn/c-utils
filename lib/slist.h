#ifndef SLIST_H
#define SLIST_H

#include <stdlib.h>
#include <sys/types.h>

#ifdef __cplusplus
extern "C"{
#endif

typedef struct slist {
  struct slink* root;
} slist;

typedef struct slink {
  struct slink* next;
} slink;

void slist_add_after(slist*, slink*, slink *after);
void slist_add_before(slist*, slink*, slink *before);
void slist_init(slist*);
size_t slist_length(slist*);
void slist_push(slist*, slink*);
void slist_remove(slist*, slink*);
void slist_unshift(slist*, slink*);

#define slist_foreach(slist, n) \
  for((n) = (void*)(slist)->head; \
      (n) != NULL; \
      (n) = (void*)(((slink*)n)->next))

/* n is set to the current slink and n->next is backupped
   into m before loop body for safe walk-throught when
   slinks get deleted */
#define slist_foreach_safe(slist, n, m) \
  for((n) = (void*)(slist)->head, \
      (m) = (void*)((slink*)n) != NULL ? (void*)((slink*)n)->next : NULL; \
      (n) != NULL; \
      (n) = (void*)((slink*)m), \
      (m) = (void*)((slink*)n) != NULL ? (void*)((slink*)n)->next : NULL)


/* aliases for backwards compatibility */
#define slist_foreach           slist_foreach
#define slist_foreach_safe      slist_foreach_safe

#ifdef __cplusplus
}
#endif
#endif /* defined(SLIST_H) */
