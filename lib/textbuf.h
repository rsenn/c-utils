#ifndef TEXTBUF_H
#define TEXTBUF_H 1

#include "buffer.h"

typedef struct {
  buffer b;
  unsigned line;
  unsigned col;
  buffer* source;
} textbuf;

void textbuf_init(textbuf *tb, buffer *src);
ssize_t textbuf_read(intptr_t fd, void *buf, size_t n, void *cookie);

#endif /* defined TEXTBUF_H */
