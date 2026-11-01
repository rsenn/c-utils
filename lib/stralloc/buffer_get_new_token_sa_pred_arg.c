#include "../buffer.h"
#include "../stralloc.h"

int
buffer_get_new_token_sa_pred_arg(buffer* b, stralloc* sa, sa_predicate_arg p, void* arg) {
  stralloc_zero(sa);

  return buffer_get_token_sa_pred_arg(b, sa, p, arg);
}
