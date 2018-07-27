#ifndef CHARBUF_H
#define CHARBUF_H

#include <sys/types.h>

typedef ssize_t (read_fn)(int fd, void* buf, size_t n);

typedef struct {
  char ch;
  size_t p;
  read_fn* op;
  int fd;
} charbuf;


#define CHARBUF_INIT(op, fd) { '\0', 0, (op), (fd) }

int charbuf_get(charbuf *b);
int charbuf_getc(charbuf *b, char* ch);

int charbuf_peek(charbuf *b);
int charbuf_peekc(charbuf *b, char* ch);

int charbuf_skip(charbuf *b);

int charbuf_nextc(charbuf *b, char* ch);

int charbuf_skip_until(charbuf *b, char c);

#endif /* defined CHARBUF_H */
