#ifndef __ITERATOR_H
#define __ITERATOR_H 1

#ifdef HAVE_C11_GENERICS

#include "array.h"
#include "strarray.h"
#include "stralloc.h"

#ifdef __cplusplus
extern "C" {
#endif

#define size(container)                                                        \
  _Generic((container), stralloc *                                             \
           : stralloc_length, array *                                          \
           : array_length)(container)

#define begin(container)                                                       \
  _Generic((container), array *                                                \
           : array_start, strarray *                                           \
           : strarray_begin, slist *                                           \
           : slist_begin)(container)

#define end(container)                                                         \
  _Generic((container), array *                                                \
           : array_end, strarray *                                             \
           : strarray_end)(container)

#define iterator_increment(container, it)                                      \
  _Generic((container), slist * : slist_iterator_increment)(container, &it)

#define iterator_decrement(container, it)                                      \
  _Generic((container), array * : array_iterator_decrement, )(&it)

#define iterator_dereference(container, it)                                    \
  _Generic((container), array *                                                \
           : array_iterator_dereference, slist *                               \
           : slist_iterator_dereference)(&it)

#define iterator_distance(container, it1, it2)                                 \
  _Generic((container), array *                                                \
           : array_iterator_distance, stralloc *                               \
           : stralloc_iterator_distance, slist *                               \
           : slist_iterator_distance)(&it1, &it2)

#define iterator_equal(container, it1, it2)                                    \
  _Generic((container), array *                                                \
           : array_iterator_equal, stralloc *                                  \
           : stralloc_iterator_equal, slist *                                  \
           : slist_iterator_equal)(&it1, &it2)

#define is_first(container, it)                                                \
  _Generic((container), slist * : slist_iterator_first)(container, it)

#define is_last(container, it)                                                 \
  _Generic((container), slist * : slist_iterator_last)(container, it)

#define is_end(container, it)                                                  \
  _Generic((container), slist * : slist_iterator_end)(container, it)

#ifdef __cplusplus
}
#endif

#endif /* defined(HAVE_C11_GENERICS) */

#endif /* defined __ITERATOR_H */
