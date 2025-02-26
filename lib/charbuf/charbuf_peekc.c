#include "../charbuf_internal.h"

ssize_t
charbuf_peekc(charbuf* b, unsigned char* ch) {
  ssize_t ret;

  for(;;) {
    if(b->p) {
      *ch = (unsigned int)(unsigned char)b->chrs[0];
      ret = 1;
      return 1;
    } else if((ret = charbuf_stubborn_read(b, 1)) > 0) {
      b->p = ret;
      continue;
    }
    break;
  }

#ifdef DEBUG_CHARBUF
  if(charbuf_debug) {
    buffer_puts(buffer_2, "charbuf_peekc ");
    charbuf_dumplabel("c", buffer_2);

    if(charbuf_colors)
      buffer_puts(buffer_2, CHARBUF_CYAN);
    charbuf_dumpchar(*ch, buffer_2, 2);

    if(charbuf_colors)
      buffer_puts(buffer_2, CHARBUF_NC);
    charbuf_dump(b, buffer_2);
    buffer_putspace(buffer_2);
    charbuf_dumpint64("ret", ret, buffer_2);

    if(charbuf_colors)
      buffer_puts(buffer_2, CHARBUF_NC);
    buffer_putnlflush(buffer_2);
  }
#endif

  return ret;
}
