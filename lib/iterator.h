#ifndef ITERATOR_H
#define ITERATOR_H 1

#include "array.h"
#include "stralloc.h"
#include "hmap.h"
#include "xml.h"

#define size(container) _Generic((container), \
    stralloc*: stralloc_length, \
    array*: array_length, \
    xmlnodeset*: xml_nodeset_length \
  )(container)

#define begin(container) _Generic((container), \
    HMAP_DB*: hmap_begin, \
    array*: array_start, \
    xmlnodeset*: xml_nodeset_begin \
  )(container)

#define end(container) _Generic((container), \
    array*: array_end, \
    xmlnodeset*: xml_nodeset_end \
  )(container)

#define iterator_increment(container, it...) _Generic((container), \
    HMAP_DB*: hmap_iterator_increment, \
    array*: array_iterator_increment, \
    xmlnodeset*: xml_nodeset_iterator_increment \
  )(&it)

#define iterator_decrement(container, it) _Generic((container), \
    HMAP_DB*: hmap_iterator_decrement, \
    array*: array_iterator_decrement, \
    xmlnodeset*: xml_nodeset_iterator_decrement \
  )(container, it)

#define iterator_dereference(container, it) _Generic((container), \
    HMAP_DB*: hmap_iterator_dereference, \
    array*: array_iterator_dereference, \
    xmlnodeset*: xml_nodeset_iterator_dereference \
  )(container, it)

#define iterator_distance(container,it1,it2) _Generic((container), \
    HMAP_DB*: hmap_iterator_distance, \
    array*: array_iterator_distance, \
    stralloc*: stralloc_iterator_distance, \
    xmlnodeset*: xml_nodeset_iterator_distance \
  )(container, it1, it2)

#define iterator_equal(container,it1,it2) _Generic((container), \
    HMAP_DB*: hmap_iterator_equal, \
    array*: array_iterator_equal, \
    stralloc*: stralloc_iterator_equal, \
    xmlnodeset*: xml_nodeset_iterator_equal \
  )(container, it1, it2)

#define is_last(container,it) _Generic((container), \
    HMAP_DB*: hmap_last \
  )(container,it)

#endif /* defined ITERATOR_H */
