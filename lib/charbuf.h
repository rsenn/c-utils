#ifndef CHARBUF_H
#define CHARBUF_H

#include <stddef.h>
#include <sys/types.h>

#include "windoze.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef ssize_t (read_fn)(int fd, void* buf, size_t n);

typedef struct {
  char ch;
  unsigned char p:1;
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

#ifdef __cplusplus
}
#endif

#endif /* defined CHARBUF_H */
int charbuf_skip_pred(charbuf*, int (*pred)(void));
