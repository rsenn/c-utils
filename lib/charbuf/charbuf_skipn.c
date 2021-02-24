#include "../charbuf_internal.h"
#include "../buffer.h"
#include "../alloc.h"

#define max(a, b) ((a) >= (b) ? (a) : (b))
#define min(a, b) ((a) <= (b) ? (a) : (b))

ssize_t
charbuf_skipn(charbuf* b, size_t n) {
  size_t pos;
  ssize_t ret;
  uint8 skipped[n];

  for(pos = 0; pos < n; pos++) {
    uint8 ch;
#ifdef DEBUG_CHARBUF
    charbuf_debug = 0;
#endif
    if((ret = charbuf_peekc(b, &ch)) > 0) {
      //      charbuf_dumpchar(ch, buffer_2, 2);
      skipped[pos] = ch;

#ifdef DEBUG_CHARBUF
      charbuf_debug = 1;
#endif
      ret = charbuf_skip(b);
#ifdef DEBUG_CHARBUF
      charbuf_debug = 0;
#endif

      if(ret > 0)
        continue;
    }
    break;
  }

#ifdef DEBUG_CHARBUF
  charbuf_debug = 1;
  if(charbuf_debug) {
    buffer_puts(buffer_2, "charbuf_skipn(");
    charbuf_dumplabel("n", buffer_2);
    buffer_putulonglong(buffer_2, n);
    buffer_puts(buffer_2, ") ");
     charbuf_dumpchars(skipped, pos, buffer_2, 1);
   }
#endif

#ifdef DEBUG_CHARBUF
  charbuf_debug = 1;
  if(charbuf_debug) {
    buffer_puts(buffer_2, CHARBUF_PROPSEP);
    charbuf_dump(b, buffer_2);
       buffer_puts(buffer_2, CHARBUF_PROPSEP);
  charbuf_dumpint64("ret", pos, buffer_2);
   buffer_putnlflush(buffer_2);
  }
#endif
  return pos;
}
