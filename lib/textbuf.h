#ifndef TEXTBUF_H
#define TEXTBUF_H 1

#include "buffer.h"

typedef struct {
  unsigned line;
  unsigned column;
  buffer* source;
  char buf[];
} textbuf;

void textbuf_destroy(buffer *b);
void textbuf_init(buffer *b, buffer *src, size_t bufsize);
ssize_t textbuf_read(intptr_t fd, char *buf, size_t n);
void textbuf_free(buffer* b);
unsigned textbuf_column(buffer *b);
unsigned textbuf_line(buffer *b);
int is_textbuf(buffer *b);

#endif /* defined TEXTBUF_H */
