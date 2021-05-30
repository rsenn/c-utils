#include "../set.h"

char*
set_at(const set_t* set, const size_t pos) {
  return set_at_n(set, pos, 0);
}

char*
set_at_n(const set_t* set, const size_t pos, size_t* len) {

  set_iterator_t it;
  size_t i = 0;
  set_foreach_it(set, it) {
    if(i == pos)
      return set_iterator_value(&it, len);
    i++;
  }
  return 0;
}