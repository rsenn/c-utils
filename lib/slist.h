#ifndef SLIST_H
#define SLIST_H 1

#include <stdlib.h>
#include <sys/types.h>
#include "alloc.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct slink {
  struct slink* next;
} slink;

static inline void
slist_add(slink** list, slink* link) {
  link->next = *list;
  *list = link;
}

slink** list_find(slink**, int (*pred)(slink*));
void slist_add_after(slink**, slink* p, slink* after);
void slist_add_before(slink**, slink* p, slink* before);
slink* slist_find_delete(slink**, int (*pred)(slink*));
void slist_init(slink**);
size_t slist_length(slink**);
int slist_pushs(slink**, const char* s);
void slist_push(slink**, slink* link);
slink* slist_remove(slink**);
int slist_shifts(slink**);
slink* slist_shift(slink**);
int slist_unshifts(slink**, const char* s);
int slist_pushb(slink** list, const void* x, size_t len);
int slist_unshiftb(slink** list, const void* x, size_t len);

#define slink_foreach(link, n) for((n) = (void*)(link); *(void**)(n); (n) = (void*)((slink*)(n))->next)
#define slist_foreach(list, n) for((n) = (void*)slist_begin(list); (n); (n) = (void*)slist_next((slink*)n))
#define slist_foreach_safe(list, n, next)                                                                              \
  for((n) = (void*)slist_begin(list); (n) && ((next) = slist_next((n))); (n) = (next))

#define _slist_begin(st) ((struct slink**)&(st))
#define _slist_next(st) (((struct slink*)(st))->next)

static inline void*
slist_data(slink* list) {
  return *(void**)&list[1];
}

static inline void*
slink_data(slink** link) {
  return slist_data(*link);
}
static inline slink**
slink_next(slink** link) {
  return &((*link)->next);
}
static inline slink*
slist_next(slink* list) {
  return list->next;
}

static inline int
slink_last(slink** link) {
  return !(*link);
}
static inline int
slist_islast(slink* list) {
  return !list->next;
}

static inline size_t
slink_size(slink** link) {
  size_t i = 0;
  while(!slink_last(link)) {
    ++i;
    link = slink_next(link);
  };
  return i;
}
static inline size_t
slist_size(slink* list) {
  size_t i = 0;
  while(list) {
    ++i;
    list = list->next;
  };
  return i;
}

static inline slink**
slink_tail(slink** link) {
  while(*link) {
    link = slink_next(link);
  };
  return link;
}
static inline slink*
slist_end(slink* list) {
  while(list->next) {
    list = list->next;
  };
  return list;
}

#define slink_new(type) ((slink*)alloc_zero(sizeof(type) + sizeof(slink)))

static inline slink**
slink_insert(slink** at, slink* link) {
  link->next = *at;
  *at = link;
  return &link->next;
}

#define slist_insert(list, link) slink_insert(list, (slink*)(link))

inline static slink*
slist_begin(slink* list) {
  return list;
}

inline static slink**
slink_begin(slink** list) {
  return list;
}
inline static slink**
slink_end(slink** list) {
  while(*list) list = &(*list)->next;
  return list;
}

inline static int
slist_iterator_first(slink** list, slink** p) {
  return list == p;
}
inline static int
slist_iterator_last(slink** list, slink** p) {
  (void)list;
  return *p && (*p)->next == NULL;
}
inline static int
slist_iterator_end(slink** list, slink** p) {
  (void)list;
  return (*p) == NULL;
}

inline static void
slist_iterator_increment(slink** list, slink*** p) {
  (void)list;
  *p = &(**p)->next;
}
inline static slink*
slist_iterator_dereference(slink** list, slink** p) {
  (void)list;
  return *p;
}

inline static size_t
slist_iterator_distance(slink** list, slink** from, slink** to) {
  size_t i = 0;
  (void)list;
  while(*from && from != to) {
    from = &(*from)->next;
    ++i;
  };
  return i;
}

inline static int
slist_iterator_equal(slink** list, slink** it1, slink** it2) {
  (void)list;
  return it1 == it2;
}

#ifdef __cplusplus
}
#endif
#endif /* defined(SLIST_H) */
