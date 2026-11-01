#include "../buffer.h"
#include "../stralloc.h"

/* The same as buffer_get_token_sa_pred, but sa is emptied first. */
int
buffer_get_new_token_sa_pred(buffer* b, stralloc* sa, sa_predicate p) {
  stralloc_zero(sa);
  return buffer_get_token_sa_pred(b, sa, p);
}
