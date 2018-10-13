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
typedef struct {
  array a;
} strarray;

/* strarray_init will initialize a strarray. */
#define strarray_zero(l) array_trunc(&(l)->a, 0)
#define strarray_init(l) byte_zero(&(l)->a, sizeof(array))
#define strarray_free(l) array_free(&(l)->a)

#define strarray_size(l) array_length(&(l)->a, sizeof(char*))

#define strarray_begin(l) (char**)array_start(&(l)->a)
#define strarray_end(l) (strarray_begin(l)+strarray_size(l))

#define strarray_at(l, pos) (*(char**)array_get(&(l)->a, sizeof(char*), pos))

char* *strarray_to_argv(strarray*);
int strarray_from_argv(int argc, char* argv[], strarray* arr);

int64 strarray_index_of(strarray*, const char* s);
char* strarray_pop(strarray*, char* s);
int   strarray_pushb(strarray*, const char* b, size_t n);
int   strarray_pushd(strarray*, const char* s);
int   strarray_push(strarray*, char* s);
int   strarray_set(strarray*, size_t index, const char* s);

#ifdef STRALLOC_H
int strarray_push_sa (strarray * a, const stralloc * sa);
#endif

void strarray_sort(strarray*, int (*)(char* , char* ));

int strarray_appends(strarray* arr, const char* s);
int strarray_prepends(strarray* arr, const char* s);
int strarray_removeprefixs(strarray* arr, const char* s);
int strarray_removesuffixs(strarray* arr, const char* s);

#ifdef __cplusplus
}
#endif
#endif /* defined(STRARRAY_H) */
int strarray_joins(strarray* arr, stralloc* sa, const char* sep);
