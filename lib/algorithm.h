#ifndef ALGORITHM_H
#define ALGORITHM_H 1

#include "iterator.h"

#ifdef __cplusplus
extern "C" {
#endif

#define for_each(container, it) \
  for(it = begin(container); !is_end(container, it); iterator_increment(container, it))


#ifdef __cplusplus
}
#endif
#endif /* defined ALGORITHM_H */
