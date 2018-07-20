#ifndef ITERATOR_H
#define ITERATOR_H 1

#include "array.h"
#include "stralloc.h"
#include "hmap.h"

#define size(container) _Generic((container), \
  stralloc*: (container)->len, \
  array*: array_length((container)) \
  )


#define begin(container) _Generic((container), \
   HMAP_DB*: hmap_begin, \
   array*: array_begin \
             )(container)

#define end(container) _Generic((container), \
  HMAP_DB*: hmap_end, \
   array*: array_end \
             )(container)

#define iterator_increment(container,it) _Generic((container), \
  HMAP_DB*: hmap_iterator_increment, \
  stralloc*: stralloc_iterator_increment \
  )(it)

#define iterator_decrement(container,it) _Generic((container), \
  HMAP_DB*: hmap_iterator_decrement, \
  stralloc*: stralloc_iterator_decrement \
  )(it)

#define iterator_distance(container,it1,it2) _Generic((container), \
  HMAP_DB*: hmap_iterator_distance, \
  stralloc*: stralloc_iterator_distance \
  )(it1, it2)

#define iterator_dereference(container,it_ptr) _Generic((container), \
  HMAP_DB*: hmap_iterator_dereference, \
  stralloc*: stralloc_iterator_dereference \
  )(it_ptr)

#define iterator_equal(container,it1,it2) _Generic((container), \
  HMAP_DB*: hmap_iterator_equal, \
  stralloc*: stralloc_iterator_equal \
  )(it1,it2)

#define is_last(container,it) _Generic((container), \
  HMAP_DB*: hmap_last \
  )(container,it)

#endif /* defined ITERATOR_H */
