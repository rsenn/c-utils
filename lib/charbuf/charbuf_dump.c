#include "../buffer.h"
#include "../charbuf.h"

#ifdef DEBUG_CHARBUF
int charbuf_debug = 1;

void
charbuf_dump(charbuf* b, buffer* out) {

  buffer_puts(out, " p: ");
  buffer_putulong(out, b->p);
  buffer_puts(out, ", offset: ");
  buffer_putulong(out, b->offset);

  if(b->eof) {
    buffer_puts(out, ", EOF");
  }
  if(b->err) {
    buffer_puts(out, ", err: ");
    buffer_putulong(out, b->err);
  }

  if(b->loc.column == 0) {
    buffer_puts(out, ", loc ");
    buffer_putulong(out, b->loc.line + 1);
    buffer_puts(out, ":");
    buffer_putulong(out, b->loc.column + 1);
  }
  buffer_putnlflush(out);
}

#endif