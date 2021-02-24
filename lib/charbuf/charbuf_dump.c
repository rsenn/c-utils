#include "../charbuf_internal.h"

#ifdef DEBUG_CHARBUF
int charbuf_debug = 1;
int charbuf_colors = 1;

void
charbuf_dump(charbuf* b, buffer* out) {

  buffer_puts(out, charbuf_colors ? " " CHARBUF_GRAY "p" CHARBUF_EQ CHARBUF_CYAN : " p=");
  buffer_putulong(out, b->p);
  buffer_puts(out, charbuf_colors ? CHARBUF_SEP CHARBUF_GRAY "offset" CHARBUF_EQ CHARBUF_CYAN : " offset=");
  buffer_putulong(out, b->offset);

  if(b->eof) {
    buffer_puts(out, charbuf_colors ? CHARBUF_SEP CHARBUF_GREEN "EOF" : ", EOF");
  }
  if(b->err) {
    buffer_puts(out, charbuf_colors ? CHARBUF_SEP CHARBUF_GRAY "err" CHARBUF_EQ CHARBUF_YELLOW : ", err=");
    buffer_putulong(out, b->err);
  }

  if(b->loc.column == 0) {
    buffer_puts(out, ", loc ");
    buffer_putulong(out, b->loc.line + 1);
    buffer_puts(out, ":");
    buffer_putulong(out, b->loc.column + 1);
  }
  if(charbuf_colors)
    buffer_puts(out, "\x1b  [m");
}

#endif