#include "../charbuf_internal.h"
#include "../buffer.h"
#include "../alloc.h"

#define max(a, b) ((a) >= (b) ? (a) : (b))
#define max(a, b) ((a) >= (b) ? (a) : (b))
#define min(a, b) ((a) <= (b) ? (a) : (b))

ssize_t
charbuf_skipn(charbuf* b, size_t n) {
  size_t pos;
  ssize_t ret;
  uint8 skipped[n];
#ifdef DEBUG_CHARBUF
  int debug = charbuf_debug;
  charbuf_debug = 0;
#endif
  for(pos = 0; pos < n; pos++) {
    uint8 ch;
    if((ret = charbuf_peekc(b, &ch)) > 0) {
      //      charbuf_dumpchar(ch, buffer_2, 2);
      skipped[pos] = ch;

      ret = charbuf_skip(b);

      if(ret > 0)
        continue;
    }
    break;
  }

#ifdef DEBUG_CHARBUF
  if(debug) {
    buffer_puts(buffer_2, "charbuf_skipn(");
    charbuf_dumplabel("n", buffer_2);
    buffer_putulonglong(buffer_2, n);
    buffer_puts(buffer_2, ")\x1b[" charbuf_stringify_helper(CHARBUF_COLUMN_1) "G");
    charbuf_dumpchars(skipped, pos, buffer_2, 1);
    buffer_puts(buffer_2, CHARBUF_PROPSEP);
    charbuf_dumpint64("ret", pos, buffer_2);
    buffer_puts(buffer_2, CHARBUF_PROPSEP);
    charbuf_dump(b, buffer_2);
    buffer_putnlflush(buffer_2);
  }
  charbuf_debug = debug;
#endif
  return pos;
}
