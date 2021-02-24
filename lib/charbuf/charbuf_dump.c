#include "../charbuf_internal.h"

#ifdef DEBUG_CHARBUF
int charbuf_debug = 1;
int charbuf_colors = 1;

void
charbuf_dump(charbuf* b, buffer* out) {
  charbuf_dumplabel("p", out);
  buffer_putulong(out, b->p);
  charbuf_dumplabel("offset", out);
  buffer_putulong(out, b->offset);
  if(b->eof) {
    charbuf_dumplabel("eof", out);
    buffer_putulong(out, b->eof);
  }
  if(b->err) {
    charbuf_dumplabel("err", out);
    buffer_putulong(out, b->err);
  }
  if(b->loc.column == 0) {
    charbuf_dumplabel("loc", out);
    buffer_putulong(out, b->loc.line + 1);
    buffer_puts(out, ":");
    buffer_putulong(out, b->loc.column + 1);
  }
  if(charbuf_colors)
    buffer_puts(out, CHARBUF_NC);
}

#endif