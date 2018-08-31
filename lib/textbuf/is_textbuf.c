#include "../textbuf.h"

textbuf*
is_textbuf(buffer* b) {
  textbuf* tb = (void*)(b->x - (size_t)((char*)&(((textbuf*)0)->buf)));

  return (void*)tb == b->cookie ? tb : 0;
}

