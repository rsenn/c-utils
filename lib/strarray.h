#ifndef STRARRAY_H
#define STRARRAY_H

#include "uint64.h"
#include "array.h"

#ifdef __cplusplus
extern "C" {
#endif

/* strarray is the internal data structure all functions are working on.
 */
typedef struct {
  array a;
} strarray;

/* strarray_init will initialize a strarray. */
#define strarray_zero(l) array_trunc(&(l)->a, 0)
#define strarray_init(l) byte_zero(&(l)->a, sizeof(array))
#define strarray_free(l) array_free(&(l)->a)

#define strarray_size(l) array_length(&(l)->a, sizeof(char*))

#define strarray_at(l, pos) (*(char**)array_get(&(l)->a, sizeof(char*), pos))

char* *strarray_to_argv(strarray*);
int64 strarray_index_of(strarray*, const char* s);
char* strarray_pop(strarray*, char* s);
int   strarray_pushb(strarray*, const char* b, size_t n);
int   strarray_pushd(strarray*, const char* s);
int   strarray_push(strarray*, char* s);

#ifdef STRALLOC_H
int strarray_push_sa (strarray * a, const stralloc * sa);
#endif

#ifdef __cplusplus
}
#endif
#endif /* defined(STRARRAY_H) */
