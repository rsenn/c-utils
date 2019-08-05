#include <stdlib.h>
#include "../textbuf.h"

void
textbuf_free(buffer* b) {
  textbuf* tb = is_textbuf(b);
  if(tb) {
    if(tb->source)
      buffer_close(tb->source);
    free(tb);
  }
}

