#include "../charbuf_internal.h"

#ifdef DEBUG_CHARBUF
int charbuf_debug = 1;
int charbuf_colors = 1;

#define CHARBUF_PROPSEP ", ."

void
charbuf_dump(charbuf* b, buffer* out) {
  charbuf_dumplabel(CHARBUF_SEP "{ .p", out);
  buffer_putulong(out, b->p);
  charbuf_dumplabel(CHARBUF_PROPSEP "offset", out);
  buffer_putulong(out, b->offset);
  if(b->eof) {
    charbuf_dumplabel(CHARBUF_PROPSEP "eof", out);
    buffer_putulong(out, b->eof);
  }
  if(b->err) {
    charbuf_dumplabel(CHARBUF_PROPSEP "err", out);
    buffer_putulong(out, b->err);
  }
  if(b->loc.column == 0) {
    charbuf_dumplabel(CHARBUF_PROPSEP "loc", out);
    buffer_putulong(out, b->loc.line + 1);
    buffer_puts(out, ":");
    buffer_putulong(out, b->loc.column + 1);
  }
  buffer_puts(out, charbuf_colors ? CHARBUF_NC "}" : "}");
}

#endif