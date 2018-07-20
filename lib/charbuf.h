#ifndef CHARBUF_H
#define CHARBUF_H

#include <sys/types.h>

typedef int (getchar_fn)();
typedef ssize_t (read_fn)(int fd, void* buf, size_t n);

typedef struct {
  char ch;
  size_t p;
  //getchar_fn* op;
  read_fn* op;
  int fd;
} charbuf;


#define CHARBUF_INIT(op, fd) { '\0', 0, (op), (fd) }

int charbuf_get(charbuf *b);
int charbuf_peek(charbuf *b);
int charbuf_skip(charbuf *b);


#endif /* defined CHARBUF_H */
