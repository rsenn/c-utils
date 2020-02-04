#ifndef STRARRAY_H
#define STRARRAY_H

#include "uint64.h"
#include "array.h"
#include "stralloc.h"
#include "byte.h"

#ifdef __cplusplus
extern "C" {
#endif

/* strarray is the internal data structure all functions are working on.
 */

typedef array strarray;

/* strarray_init will initialize a strarray. */
#define strarray_zero(l) array_trunc((l), 0)
#define strarray_init(l) byte_zero((l), sizeof(strarray))
#define strarray_free(l) array_reset((l))

#define strarray_size(l) array_length((l), sizeof(char*))

#define strarray_begin(l) (char**)array_start((l))
#define strarray_end(l) (strarray_begin(l) + strarray_size(l))

#define strarray_at(l, pos) (*(char**)array_get((l), sizeof(char*), pos))

#define strarray_foreach(a, ptr) for((ptr) = (void*)strarray_begin(a); ((char**)(ptr)) < strarray_end(a); ++ptr)

char** strarray_to_argv(strarray*);
int strarray_from_argv(int argc, const char* const argv[], strarray* arr);

#define strarray_contains(a, s) (strarray_index_of((a), (s)) != -1)
#define strarray_push_unique(a, s)                                                                                     \
  do {                                                                                                                 \
    if(!strarray_contains((a), (s)))                                                                                   \
      strarray_push((a), (s));                                                                                         \
  } while(0)

int64 strarray_index_of(strarray*, const char* s);
char* strarray_pop(strarray*, char* s);
int strarray_pushb(strarray*, const char* b, size_t n);
int strarray_pushd(strarray*, const char* s);
int strarray_push(strarray*, const char* s);
int strarray_set(strarray*, size_t index, const char* s);
int strarray_setb(strarray*, size_t index, const char* s, size_t len);

#ifdef STRALLOC_H
#define strarray_push_sa_unique(a, sa)                                                                                 \
  do {                                                                                                                 \
    if(!strarray_contains_sa((a), (sa)))                                                                               \
      strarray_push_sa((a), (sa));                                                                                     \
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

int strarray_joins(strarray* arr, stralloc* sa, const char* sep);

int strarray_transform(strarray* in, strarray* out, char* (*pred)(const char*));
int strarray_each(strarray* arr, void (*pred)(char**));

int strarray_glob(strarray* arr, const char* pattern);

int64 strarray_splice(strarray*, uint64 start, uint64 del, uint64 insert, const char** x);

#ifdef __cplusplus
}
#endif
#endif /* defined(STRARRAY_H) */
