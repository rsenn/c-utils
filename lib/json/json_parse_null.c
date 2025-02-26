#include "../stralloc.h"
#include "../json_internal.h"

int
json_parse_null(jsonval* j, charbuf* b) {
  ssize_t n;
  n = charbuf_pred_lookahead(b, predicate_identifier, "null");

  if(n >= 4) {
    charbuf_skipn(b, 4);
    j->type = JSON_NULL;
    return 1;
  }
  return 0;
}
