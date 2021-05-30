#include "../stralloc.h"
#include "../set.h"

void
set_join(const set_t* set, const char* sep, stralloc* out) {
  set_iterator_t it;
  size_t i = 0;
  set_foreach_it(set, it) {
    size_t len;
    if(i > 0)
      stralloc_cats(out, sep);

    stralloc_catb(out, set_iterator_value(&it, &len), len);
    i++;
  }
}