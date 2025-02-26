#include "../charbuf_internal.h"

ssize_t
charbuf_pred_skip(charbuf* b, int (*pred)(int, size_t, void*), void* ptr) {
  size_t n = 0;
  uint8 ch;
  ssize_t ret = 0;

#ifdef DEBUG_CHARBUF_
  int debug = charbuf_debug;
  charbuf_debug = 0;
#endif

  for(; (ret = charbuf_peekc(b, &ch)) > 0;) {

    if(pred((unsigned int)(unsigned char)ch, n, ptr)) {
      n++;

      if(charbuf_skip(b) > 0)
        continue;
    }
    ret = n;
    break;
  }

#ifdef DEBUG_CHARBUF_
  if(debug) {
    buffer_putspad(buffer_2, "charbuf_pred_skip ", CHARBUF_COLUMN_1);

    /*  charbuf_dumpuint32("p", b->p, buffer_2);
      buffer_puts(buffer_2, CHARBUF_SEP);
      charbuf_dumpuint32("offset", b->offset, buffer_2);
      buffer_puts(buffer_2, CHARBUF_SEP);

  */ charbuf_dumpint64("ret", ret, buffer_2);
    buffer_puts(buffer_2, CHARBUF_SEP);
    charbuf_dumplabel("c", buffer_2);
    buffer_puts(buffer_2, CHARBUF_SEP);

    if(charbuf_colors)
      buffer_puts(buffer_2, CHARBUF_CYAN);
    charbuf_peekc(b, &ch);
    charbuf_dumpchar(ch, buffer_2, -6);

    if(charbuf_colors)
      buffer_puts(buffer_2, CHARBUF_NC);

    charbuf_dump(b, buffer_2);

    buffer_putnlflush(buffer_2);
  }
  charbuf_debug = debug;
#endif
  return ret;
}
