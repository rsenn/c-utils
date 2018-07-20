#ifndef TEXTBUF_H
#define TEXTBUF_H 1

#include "buffer.h"
#include "uint64.h"

typedef struct {
  uint64 line;
  uint64 column;
  buffer* source;
  char buf[];
} textbuf;

void textbuf_destroy(buffer *b);
void textbuf_init(buffer *b, buffer *src, size_t bufsize);
ssize_t textbuf_read(int, char *, size_t, void*);
void textbuf_free(buffer* b);
uint64 textbuf_column(buffer *b);
uint64 textbuf_line(buffer *b);
textbuf* is_textbuf(buffer *b);

#endif /* defined TEXTBUF_H */
