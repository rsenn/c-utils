#include "../charbuf_internal.h"

ssize_t
charbuf_skip_ws(charbuf* b) {
     return  charbuf_pred_skip(b, predicate_ctype, isspace);
}