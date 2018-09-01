#ifndef SLIST_H
#define SLIST_H 1

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

void   slist_add_after(slist* l, slink* p, slink* after);
void   slist_add_before(slist* l, slink* p, slink* before);
slink* slist_find_delete(slist* l, int (*pred)(void* ));
slink* slist_find(slist* l, int (*pred)(void* ));
void   slist_init(slist* l);
size_t slist_length(slist* l);
void   slist_move_head(slist* from, slist* to);
void   slist_move_tail(slist* from, slist* to);
void   slist_push(slist* l, slink* p);
int    slist_pushs(slist* l, const char* s);
slink* slist_remove(slist* l, slink* p);
slink* slist_unshift(slist* l);

#define slist_foreach(slist, n) \
  for((n) = (void*)(slist)->root; \
      (n) != NULL; \
      (n) = (void*)(((slink*)n)->next))

/* n is set to the current slink and n->next is backupped
   into m before loop body for safe walk-throught when
   slinks get deleted */
#define slist_foreach_safe(slist, n, m) \
  for((n) = (void*)(slist)->root, \
      (m) = (void*)((slink*)n) != NULL ? (void*)((slink*)n)->next : NULL; \
      (n) != NULL; \
      (n) = (void*)((slink*)m), \
      (m) = (void*)((slink*)n) != NULL ? (void*)((slink*)n)->next : NULL)

#define slist_iterator() slink**

static inline slink** slist_begin(slist* l) { return &l->root; }
static inline slink** slist_end(slist* l) { slink** p = &l->root; while(*p) p = &(*p)->next; return p; }

static inline int slist_iterator_first(slist* l, slink** p) { return l->root == *p; }
static inline int slist_iterator_last(slist* l, slink** p) { return (*p)->next == NULL; }

static inline void slist_iterator_increment(slist* l, slink*** p) { *p = &(**p)->next; }
static inline slink* slist_iterator_dereference(slist* l, slink** p) { return *p; }
static inline size_t slist_iterator_distance(slist* l, slink** from, slink** to) { size_t i; while(*from) { from = &(*from)->next; ++i; }; return i; }
static inline int slist_iterator_equal(slist* l, slink** it1, slink** it2) { return *it1 == *it2; }

#ifdef __cplusplus
}
#endif
#endif /* defined(SLIST_H) */
