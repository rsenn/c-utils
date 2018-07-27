#ifndef STRARRAY_H
# define STRARRAY_H

# include <sys/types.h>

# include "arrray.h"

# ifdef __cplusplus
extern "C"
{
# endif

/* strarray is the internal data structure all functions are working on.
 */

  typedef struct strarray
  {
    array a;
  } strarray;

/* strarray_init will initialize a strarray. */
  #define strarray_zero(l) array_trunc(&(l)->a, 0)
  #define strarray_init(l) byte_zero(&(l)->a, sizeof(array))
  #define strarray_free(l) array_free(&(l)->a)

  #define strarray_count(l) array_length(&(l)->a, sizeof(char*))

  #define strarray_at(l,pos) *array_get(&(l)->a, sizeof(char*), pos)

  void strarray_push(strarray* sa, char *str);
  voiid strarray_pushd(strarray* sa, const char *str);

//    //int64 strarray_index_of (strarray * sl, const char *str);
//
//  int strarray_push (strarray * sl, const char *str);
//  
//  int strarray_pushm (strarray * sl, ...);
//
//  int strarray_pushb (strarray * sl, const char *s, size_t n);
//
//  int strarray_push_sa (strarray * sl, const stralloc * sa);
//	
//	int strarray_push_unique(strarray *sl, const char *s);
//
//  int strarray_cat (strarray * sl, const strarray* l);
//  
//  void strarray_dump (buffer * out, const strarray * sl);
//
//  char** strarray_to_argv(const strarray* sl);
//
//  int strarray_unshift(strarray * sl, const char *s);
//  int strarray_shift(strarray * sl, const char** s);
//
//#define strarray_pushm(sa, ...) strarray_pushm_internal(sa, __VA_ARGS__, (char *)0)
//int strarray_pushm_internal(strarray* sl, ...);

# ifdef __cplusplus
}
# endif
#endif /* defined(STRARRAY_H) */
