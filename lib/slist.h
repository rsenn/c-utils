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
slink** slist_find(slist* l, int (*pred)(void* ));
void   slist_init(slist* l);
size_t slist_length(slist* l);
void   slist_move_head(slist* from, slist* to);
void   slist_move_tail(slist* from, slist* to);
void   slist_push(slist* l, slink* p);
int    slist_pushs(slist* l, const char* s);
slink* slist_remove(slist* l, slink** p);
slink* slist_unshift(slist* l);

#define slist_foreach(slist, n) \
  slink** n; for(n = slist_begin(slist); n != slist_end(slist); slist_iterator_increment(slist, &n))

static slink** slist_begin(slist* l) { return &l->root; }
static slink** slist_end(slist* l) { slink** p = &l->root; while(*p) p = &(*p)->next; return p; }

static int slist_iterator_first(slist* l, slink** p) { return l->root == *p; }
static int slist_iterator_last(slist* l, slink** p) { (void)l; slink* ptr = (*p);  return ptr->next == NULL; }
static int slist_iterator_end(slist* l, slink** p) { (void)l; return (*p)->next == NULL; }

static void slist_iterator_increment(slist* l, slink*** p) { (void)l; *p = &(**p)->next; }
static slink* slist_iterator_dereference(slist* l, slink** p) { (void)l; return *p; }
static size_t slist_iterator_distance(slist* l, slink** from, slink** to) { (void)l; size_t i; while(*from && from != to) { from = &(*from)->next; ++i; }; return i; }
static int slist_iterator_equal(slist* l, slink** it1, slink** it2) { (void)l; return *it1 == *it2; }

#ifdef __cplusplus
}
#endif
#endif /* defined(SLIST_H) */
