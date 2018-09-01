#ifndef ALGORITHM_H
#define ALGORITHM_H 1

#ifdef __cplusplus
extern "C" {
#endif

#define for_each(container) \
  for(iterator(container) it = begin(container); !is_end(container, it); iterator_increment(container))


#ifdef __cplusplus
}
#endif
#endif /* defined ALGORITHM_H */
