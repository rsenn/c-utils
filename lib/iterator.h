#ifndef ITERATOR_H
#define ITERATOR_H 1

#ifdef HAVE_C11_GENERICS

#include "array.h"
#include "stralloc.h"
#include "hmap.h"
#include "xml.h"

#ifdef __cplusplus
extern "C" {
#endif

#define iterator(container) _Generic((container), \
    slist*: slink_iterator \
    )()

#define size(container) _Generic((container), \
    stralloc*: stralloc_length, \
    array*: array_length, \
    xmlnodeset*: xmlnodeset_size \
  )(container)

#define begin(container) _Generic((container), \
    HMAP_DB*: hmap_begin, \
    array*: array_start, \
    xmlnodeset*: xmlnodeset_begin \
  )(container)

#define end(container) _Generic((container), \
    array*: array_end, \
    xmlnodeset*: xmlnodeset_end \
  )(container)

#define iterator_increment(container, it...) _Generic((container), \
    HMAP_DB*: hmap_iterator_increment, \
    array*: array_iterator_increment, \
    xmlnodeset*: xmlnodeset_iter_pp, \
    slist*: slist_iterator_increment \
  )(&it)

#define iterator_decrement(container, it) _Generic((container), \
    HMAP_DB*: hmap_iterator_decrement, \
    array*: array_iterator_decrement, \
    xmlnodeset*: xmlnodeset_iter_mm \
  )(&it)

#define iterator_dereference(container, it) _Generic((container), \
    HMAP_DB*: hmap_iterator_dereference, \
    array*: array_iterator_dereference, \
    xmlnodeset*: xmlnodeset_iter_ref, \
    slist*: slist_iterator_dereference \
  )(&it)

#define iterator_distance(container,it1,it2) _Generic((container), \
    HMAP_DB*: hmap_iterator_distance, \
    array*: array_iterator_distance, \
    stralloc*: stralloc_iterator_distance, \
    xmlnodeset*: xmlnodeset_iter_dist, \
    slist*: slist_iterator_distance
  )(&it1, &it2)

#define iterator_equal(container,it1,it2) _Generic((container), \
    HMAP_DB*: hmap_iterator_equal, \
    array*: array_iterator_equal, \
    stralloc*: stralloc_iterator_equal, \
    xmlnodeset*: xmlnodeset_iter_eq, \
    slist*: slist_iterator_equal
  )(&it1, &it2)

#define is_first(container,it) _Generic((container), \
    HMAP_DB*: hmap_first, \
    slist*: slist_first \
  )(container,it)


#define is_last(container,it) _Generic((container), \
    HMAP_DB*: hmap_last, \
    slist*: slist_last \
  )(container,it)

#ifdef __cplusplus
}
#endif

#endif /* defined(HAVE_C11_GENERICS) */

#endif /* defined ITERATOR_H */
