#ifndef TEXTBUF_H
#define TEXTBUF_H 1

#include "buffer.h"
#include "uint64.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
  uint64 line;
  uint64 column;
  buffer* source;
  char buf[];
} textbuf;

textbuf* is_textbuf    (buffer*);
uint64   textbuf_column(buffer*);
void     textbuf_free  (buffer*);
void     textbuf_init  (buffer*, buffer*, size_t);
uint64   textbuf_line  (buffer*);
ssize_t  textbuf_read  (fd_t, char*, size_t, void*);

#ifdef __cplusplus
}
#endif
#endif /* defined TEXTBUF_H */

