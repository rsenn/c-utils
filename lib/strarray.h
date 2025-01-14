/**
 * @defgroup   strarray
 * @brief      STRARRAY module.
 * @{
 */
#ifndef STRARRAY_H
#define STRARRAY_H

#include "uint64.h"
#include "array.h"
#include "stralloc.h"
#include "byte.h"

#ifdef __cplusplus
extern "C" {
#endif

/* strarray is the internfal data structure all functions are working on.
 */

typedef struct {
  union {
    struct {
      char** p;
      int64 allocated;
      uint64 initialized;
      size_t headroom;
    };
    struct array_s a;
  };
} strarray;

/* strarray_init will initialize a strarray. */
#define strarray_zero(l) array_trunc(&(l)->a)
#define strarray_init(l) byte_zero((l), sizeof(strarray))

//#define strarray_size(l) array_length(&(l)->a, sizeof(char*))

static inline size_t
strarray_size(const strarray* l) {
  return array_length(&l->a, sizeof(char*));
}

#define strarray_BEGIN(l) (char**)array_start(&(l)->a)
#define strarray_END(l) (strarray_BEGIN(l) + strarray_size(l))

#define strarray_AT(l, pos) (((char**)((l)->p))[(pos)])

#define strarray_foreach(a, ptr) for((ptr) = (char**)strarray_BEGIN(a); ((char**)(ptr)) != strarray_END(a) && *(char**)(ptr); ++ptr)

char** strarray_to_argv(strarray*);
int strarray_from_argv(int argc, const char* const argv[], strarray* arr);
int strarray_from_vec(strarray* arr, char* vec[]);

#define strarray_contains(a, s) (strarray_index_of((a), (s)) != -1)
/*#define strarray_push_unique(a, s) \
  do { \
    if(!strarray_contains((a), (s))) \
      strarray_push((a), (s)); \
  } while(0)
*/
int64 strarray_index_of(strarray*, const char* s);
int64 strarray_index_of_b(strarray*, const char* s, size_t n);
char* strarray_pop(strarray*);
int strarray_pushb(strarray*, const char* b, size_t n);
int strarray_pushd(strarray*, const char* s);
int strarray_push(strarray*, const char* s);
int strarray_set(strarray*, size_t index, const char* s);
int strarray_setb(strarray*, size_t index, const char* s, size_t len);

#ifdef STRALLOC_H
#define strarray_push_sa_unique(a, sa) \
  do { \
    if(!strarray_contains_sa((a), (sa))) \
      strarray_push_sa((a), (sa)); \
  } while(0)
#define strarray_contains_sa(a, sa) (strarray_index_of_sa((a), (sa)) != -1)

int strarray_push_sa(strarray* a, const stralloc* sa);
int64 strarray_index_of_sa(strarray*, const stralloc* sa);
#endif

void strarray_sort(strarray*, int (*)(const char**, const char**));

int strarray_appends(strarray* arr, const char* s);
int strarray_prepends(strarray* arr, const char* s);
int strarray_removeprefixs(strarray* arr, const char* s);
int strarray_removesuffixs(strarray* arr, const char* s);

int strarray_joins(const strarray* arr, stralloc* sa, const char* sep);

int strarray_transform(strarray* in, strarray* out, char* (*pred)(const char*));
int strarray_each(strarray* arr, void (*pred)(char**));

int strarray_glob(strarray* arr, const char* pattern);
int strarray_glob_b(strarray* arr, const char* x, size_t len);

int64 strarray_splice(strarray*, uint64 start, uint64 del, uint64 insert, const char** x);
void strarray_free(strarray*);

int strarray_push_unique(strarray* arr, const char* s);

int strarray_pushb_unique(strarray* arr, const char* b, size_t n);

void strarray_intersection(const strarray* s1, const strarray* s2, strarray* out);
void strarray_union(const strarray* s1, const strarray* s2, strarray* out);

#ifdef BUFFER_H
int buffer_putstra(buffer* b, const strarray* stra, const char* sep);
int buffer_putstra(buffer*, const strarray* stra, const char* sep);
void strarray_dump(buffer*, const strarray*);
#endif

int strarray_unshiftb(strarray*, const char* b, size_t n);
int strarray_unshift(strarray*, const char* s);
int strarray_unshiftm(strarray*, ...);
int strarray_unshiftn(strarray*, const char* const* v, size_t n);

#ifdef STRALLOC_H
int strarray_emplace_sa(strarray* arr, stralloc* sa);
#endif
#ifdef SET_H
void set_tostrarray(const set_t*, strarray*);
#endif

int strarray_copy(strarray*, const strarray*);

#ifdef __cplusplus
}
#endif
#endif /* defined(STRARRAY_H) */
/** @} */
