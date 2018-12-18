#ifndef STRLIST_H
#define STRLIST_H

#include <sys/types.h>

#include "stralloc.h"
#include "byte.h"
#include "uint64.h"
#include "buffer.h"

#ifdef __cplusplus
extern "C" {
#endif

/* strlist is the internal data structure all functions are working on.
 */
typedef struct strlist_s {
  stralloc sa;
  char sep;
} strlist;

/* strlist_init will initialize a strlist. */
#define strlist_zero(l) stralloc_zero(&(l)->sa)
#define strlist_init(l, s) stralloc_init(&(l)->sa), (l)->sep = (s);
#define strlist_free(l) stralloc_free(&(l)->sa)
#define strlist_copy(d, s) (d)->sep = (s)->sep, stralloc_copy(&(d)->sa, &(s)->sa)

#ifdef __BORLANDC__
#define strlist_pushm(sa, args) strlist_pushm_internal(sa, args, (char*)0)
#else
#define strlist_pushm(sa, ...) strlist_pushm_internal(sa, __VA_ARGS__, (char*)0)
#endif

char** strlist_to_argv(const strlist*);
char* strlist_at(const strlist*, size_t i);
char* strlist_at_n(const strlist* sl, size_t i, size_t* n);
stralloc strlist_at_sa(const strlist* sl, size_t i);
int strlist_cat(strlist*, const strlist* l);
int strlist_contains(strlist*, const char* s);
size_t strlist_count(const strlist*);
void strlist_dump(buffer*, const strlist* sl);
void strlist_froms(strlist*, const char* s, char delim);
void strlist_fromb(strlist*, const char* x, size_t n, const char* delim);
int64 strlist_index_of(strlist*, const char* str);
void strlist_join(const strlist*, stralloc* sa, char delim);
int strlist_pushb(strlist*, const char* s, size_t n);
int strlist_pushm_internal(strlist*, ...);
int strlist_pushsa(strlist*, const stralloc* sa);
int strlist_push_sa(strlist*, const stralloc* sa);
void strlist_push_tokens(strlist*, const char* s, const char* delim);
int strlist_push_unique(strlist*, const char* s);
int strlist_push(strlist*, const char* s);
strlist strlist_range(const strlist*, size_t from, size_t to);
int strlist_shift(strlist*, const char** strp);
int strlist_shift_n(strlist* sl, size_t i);
size_t strlist_sort(strlist*);
int strlist_unshift(strlist*, const char* s);

int strlist_prepends(strlist* sl, const char* s);
int strlist_prependb(strlist* sl, const char* b, size_t n);
int strlist_prepend_sa(strlist* sl, const stralloc* sa);
int strlist_append_sa(strlist* sl, const stralloc* sa);

#define strlist_end(sl) ((sl)->sa.s + (sl)->sa.len)

#define strlist_foreach(sl, str, n)                                                                                    \
  for((str) = (sl)->sa.s;                                                                                              \
      ((str) < strlist_end(sl) && ((n) = byte_chr((str), strlist_end(sl) - (str), (sl)->sep)) >= 0);                   \
      (str) += (n) + 1)

#define strlist_foreach_s(sl, str)                                                                                     \
  for(str = (sl)->sa.s; str < strlist_end(sl); str += byte_chr((str), strlist_end(sl) - str, (sl)->sep) + 1)

#define strlist_len(sl, ptr) (byte_chr(ptr, strlist_end((sl)) - (ptr), (sl)->sep))

static inline size_t
strlist_skip(const strlist* sl, char* ptr) {
  size_t ret = strlist_len(sl, ptr);
  if(ptr + ret < strlist_end(sl) && ptr[ret] == sl->sep)
    ++ret;
  return ret;
}
static inline char*
strlist_next(const strlist* sl, char* ptr) {
  ptr += strlist_len(sl, ptr);
  if(ptr < strlist_end(sl) && *ptr == sl->sep)
    ++ptr;
  return ptr;
}

#ifdef STRALLOC_H
int strlist_contains_sa(strlist*, const stralloc* sa);
int strlist_push_sa(strlist*, const stralloc* sa);
int strlist_push_unique_sa(strlist*, const stralloc* sa);
void strlist_joins(const strlist*, stralloc* sa, const char* delim);
#endif

int strlist_pushb_unique(strlist* sl, const char* x, size_t n);
int strlist_pop(strlist*);
int strlist_pop(strlist*);

int64 strlist_indexofb(strlist*, const char* x, size_t n);
int strlist_containsb(strlist*, const char* x, size_t len);
int strlist_removeb(strlist*, const char* x, size_t n);
int strlist_removes(strlist*, const char* s);

int strlist_trunc(strlist*, size_t items);
int strlist_sub(strlist*, const strlist* o);
void strlist_fromv(strlist*, const char** v, int c);

#ifdef __cplusplus
}
#endif
#endif /* defined(STRLIST_H) */
