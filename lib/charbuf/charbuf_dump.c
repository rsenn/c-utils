#include "../charbuf_internal.h"

#ifdef DEBUG_CHARBUF
int charbuf_debug = 1;
int charbuf_colors = 1;

static void
charbuf_dump_chrs(charbuf* b, buffer* out) {
  if(b->p > 0) {
    size_t i;
    charbuf_colorstr("[ ", CHARBUF_CYAN30, out);
    for(i = 0; i < b->p; i++) {
      if(i > 0)
        buffer_puts(out, " ");

      buffer_puts(out, "");
      buffer_putxlong0(out, b->chrs[i], 2);
      //      charbuf_dumpchar(b->chrs[i], out);
    }
    charbuf_colorstr(" ]", CHARBUF_CYAN30, out);
  } else {
    charbuf_colorstr("[]", CHARBUF_CYAN30, out);
  }
}

void
charbuf_dump(charbuf* b, buffer* out) {
  // charbuf_column(out, 50);
  charbuf_dump_setcolumn(CHARBUF_COLUMN_2, out);

  buffer_puts(out, "{ ");

  charbuf_dumpint64("offset", b->offset, out);
  buffer_puts(out, CHARBUF_PROPSEP);
  charbuf_dumpint64("p", b->p, out);

  if(b->eof) {
    buffer_puts(out, CHARBUF_PROPSEP);
    charbuf_dumpint64("eof", b->eof, out);
  }
  if(b->err) {
    buffer_puts(out, CHARBUF_PROPSEP);

    charbuf_dumpint64("err", b->err, out);
  }
  if(b->loc.column > 0 || b->loc.line > 0) {
    buffer_puts(out, CHARBUF_PROPSEP);
    charbuf_dumplabel("loc", out);

    buffer_putulonglong(out, b->loc.line + 1);
    buffer_puts(out, ":");
    buffer_putulonglong(out, b->loc.column + 1);
  }
  buffer_puts(out, ",");
  charbuf_dump_setcolumn(CHARBUF_COLUMN_3, out);
  charbuf_dumplabel("chrs", out);

  charbuf_dump_chrs(b, out);
  charbuf_dump_setcolumn(CHARBUF_COLUMN_4, out);
  buffer_puts(out, charbuf_colors ? CHARBUF_NC " }" : " }");
}

#endif