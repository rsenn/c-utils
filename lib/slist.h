#ifndef SLIST_H
#define SLIST_H 1

#include <stdlib.h>
#include <sys/types.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct slink { struct slink* next; } slink;

static inline void
slist_add(slink** list, slink* link) {
  link->next = *list;
  *list = link;
}

slink**list_find(slink**, int (*pred)(slink*));
void   slist_add_after(slink**, slink* p, slink* after);
void   slist_add_before(slink**, slink* p, slink* before);
slink* slist_find_delete(slink**, int (*pred)(slink*));
void   slist_init(slink**);
size_t slist_length(slink**);
int    slist_pushs(slink**, const char* s);
void   slist_push(slink**, slink* link);
slink* slist_remove(slink**);
int    slist_shifts(slink**);
slink* slist_shift(slink**);
int    slist_unshifts(slink**, const char* s);

#define slist_foreach(slist, n) for((n) = (void*)(slist); (n); (n) = (void*)((slink*)(n))->next)
#define slink_foreach(slink, n) for((n) = (void*)slist_begin(slink); *(slink**)(n); (n) = (void*)slist_next((slink**)n))

inline static slink**
slist_begin(slink** list) {
  return list;
}
inline static slink**
slist_next(slink** link) {
  return &(*link)->next;
}

inline static slink**
slist_end(slink** list) {
  while(*list)
    list = &(*list)->next;
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

