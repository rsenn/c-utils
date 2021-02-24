#include "../charbuf_internal.h"

ssize_t
charbuf_skip_ws(charbuf* b) {
  ssize_t ret;
#ifdef DEBUG_CHARBUF_
  int debug = charbuf_debug;
  charbuf_debug = 0;
#endif
  ret = charbuf_pred_skip(b, charbuf_is_ctype, isspace);
#ifdef DEBUG_CHARBUF_
  if(debug) {
    buffer_putspad(buffer_2, "charbuf_skip_ws ", CHARBUF_COLUMN_1);
    charbuf_dumpint64("ret", ret, buffer_2);
    buffer_puts(buffer_2, CHARBUF_SEP);
    charbuf_dump(b, buffer_2);
    buffer_putnlflush(buffer_2);
  }
  charbuf_debug = debug;
#endif
  return ret;
}