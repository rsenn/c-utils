#ifndef ITERATOR_H
#define ITERATOR_H

#define size(container) _Generic((container), \
  stralloc*: (container)->len, \
  array*: array_length((container)) \
  )

#endif // ITERATOR_H
