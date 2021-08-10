/**
 * @defgroup   strlist
 * @brief      STRLIST module.
 * @{
 */
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

typedef int(strlist_cmpfn_t)(const void*, const void*);
typedef int(strlist_filterfn_t)(const void*, size_t);

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
//#define strlist_copy(d, s) (d)->sep = (s)->sep, stralloc_copy(&(d)->sa, &(s)->sa)

#if defined(__BORLANDC__) || defined(__LCC__)
#define strlist_pushm(sa, args) strlist_pushm_internal(sa, args, (char*)0)
#else
#define strlist_pushm(sa, ...) strlist_pushm_internal(sa, __VA_ARGS__, (char*)0)
#endif

#define strlist_end(sl) ((sl)->sa.s + (sl)->sa.len)

#define strlist_foreach(sl, str, n) for((str) = (sl)->sa.s; ((str) < strlist_end(sl) && ((n) = byte_chr((str), strlist_end(sl) - (str), (sl)->sep)) > 0); (str) += (n) + 1)

#define strlist_foreach_s(sl, str) for(str = (sl)->sa.s; str < strlist_end(sl); str += byte_chr((str), strlist_end(sl) - str, (sl)->sep) + 1)

#define strlist_len(sl, ptr) (byte_chr(ptr, strlist_end((sl)) - (ptr), (sl)->sep))

static inline size_t
strlist_skip(const strlist* sl, char* ptr) {
  const char* end = sl->sa.s + sl->sa.len;
  size_t ret = strlist_len(sl, ptr);
  if(ptr + ret < end && ptr[ret] == sl->sep)
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

static inline char*
strlist_next_b(const strlist* sl, const char* ptr, size_t* n) {
  const char* end = sl->sa.s + sl->sa.len;
  ptr += strlist_len(sl, ptr);
  if(ptr < end && *ptr == sl->sep) {
    if(++ptr < end && n) {
      *n = strlist_len(sl, ptr);
      return (char*)ptr;
    }
  }
  return 0;
}

char* strlist_at(const strlist*, size_t i);
char* strlist_at_n(const strlist*, size_t i, size_t* n);
int strlist_cat(strlist*, const strlist* l);
int strlist_cat_unique(strlist*, const strlist* l);
int strlist_containsb(const strlist*, const char* x, size_t len);
int strlist_contains(const strlist*, const char* s);
size_t strlist_count(const strlist*);
size_t strlist_diff(const strlist*, const strlist* b, strlist* out);
size_t strlist_filter(strlist*, strlist_filterfn_t* filter_fn, strlist* out);
void strlist_fromb(strlist*, const char* x, size_t n, const char* delim);
void strlist_froms(strlist*, const char* s, char delim);
void strlist_fromv(strlist*, const char** v, int c);
int64 strlist_indexofb(strlist*, const char* x, size_t n);
int64 strlist_index_of(strlist*, const char* str);
int strlist_pop(strlist*);
int64 strlist_pos(const strlist*, size_t index);
int strlist_prependb(strlist*, const char* b, size_t n);
int strlist_prepends(strlist*, const char* s);
int strlist_pushb(strlist*, const char* s, size_t n);
int strlist_pushb_unique(strlist*, const char* s, size_t n);
int strlist_pushm_internal(strlist*, ...);
int strlist_push(strlist*, const char* s);
void strlist_push_tokens(strlist*, const char* s, const char* delim);
int strlist_push_unique(strlist*, const char* s);
strlist strlist_range(const strlist*, size_t from, size_t to);
size_t strlist_remove_all(strlist*, const strlist* remove);
int strlist_remove_at(strlist*, size_t n);
int strlist_removeb(strlist*, const char* x, size_t n);
int strlist_removes(strlist*, const char* s);
int strlist_shift_n(strlist*, size_t i);
int strlist_shift(strlist*, const char** strp);
size_t strlist_sort(strlist*, strlist_cmpfn_t* cmp_fn);
int strlist_sub(strlist*, const strlist* o);
char** strlist_to_argv(const strlist*);
int strlist_trunc(strlist*, size_t items);
int strlist_unshiftb(strlist*, const char* x, size_t len);
int strlist_unshift(strlist*, const char* s);
int strlist_splice(strlist*, size_t pos, size_t ndelete, char* x, size_t n);

#ifdef STRALLOC_H
int strlist_append_sa(strlist*, const stralloc* sa);
stralloc strlist_at_sa(const strlist*, size_t i);
int strlist_contains_sa(strlist*, const stralloc* sa);
int strlist_copyat(const strlist*, size_t i, stralloc* out);
void strlist_join(const strlist*, stralloc* sa, char delim);
void strlist_joins(const strlist*, stralloc* sa, const char* delim);
int strlist_prepend_sa(strlist*, const stralloc* sa);
int strlist_push_sa(strlist*, const stralloc* sa);
int strlist_push_unique_sa(strlist*, const stralloc* sa);

#endif
#ifdef BUFFER_H
void buffer_putsl(buffer*, const strlist* sl, const char* separator);
void strlist_dump(buffer*, const strlist* sl);
void strlist_dump_named(buffer*, const strlist* sl, const char* names[]);
#endif

void strlist_intersection(const strlist* s1, const strlist* s2, strlist* out);
void strlist_sorted_intersection(const strlist* s1, const strlist* s2, strlist* out);
void strlist_difference(const strlist* s1, const strlist* s2, strlist* out1, strlist* out2);
void strlist_union(const strlist* s1, const strlist* s2, strlist* out);

void strlist_fromq(strlist* sl, const char* s, size_t len, const char* delim, const char* quote);
void strlist_fromsq(strlist* sl, const char* s, const char* delim, const char* quote);

#ifdef __cplusplus
}
#endif
#endif /* defined(STRLIST_H) */
/** @} */
