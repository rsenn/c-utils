#include "../buffer.h"
#include "../charbuf.h"

 #ifdef DEBUG_CHARBUF
int charbuf_debug = 1;

void
charbuf_dump(charbuf* b, buffer* out) {
 
  buffer_puts(out, " p=");
  buffer_putulong(out, b->p);
  buffer_puts(out, " offset=");
  buffer_putulong(out, b->offset);

  if(b->eof) {
    buffer_puts(out, " EOF");
  } else if(b->err) {
    buffer_puts(out, " err=");
    buffer_putulong(out, b->err);
  }
  buffer_putnlflush(out);
}

#endif