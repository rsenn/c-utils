/**
 * @defgroup   charbuf
 * @brief      CHARBUF module.
 * @{
 */
#ifndef CHARBUF_H
#define CHARBUF_H

#include "typedefs.h"
#include "byte.h"
#include "uint32.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef ssize_t read_fn(int fd, void* buf, size_t n, void*);

typedef struct {
  uint32 line;
  uint32 column;
} charloc;

typedef struct {
  char ch;
  int p : 1;
  int eof : 1;
  int err : 1;
  read_fn* op;
  union {
    fd_t fd;
    void* ptr;
  };
  charloc loc;
} charbuf;

#define CHARBUF_INIT(op, fd)                                                   \
  { '\0', 0, 0, 0, (op), (fd) }

void    charbuf_close(charbuf*);
void    charbuf_froms(charbuf*, char** s);
int     charbuf_get(charbuf*);
int     charbuf_getc(charbuf*, char* ch);
void    charbuf_init(charbuf*, read_fn* op, fd_t fd);
int     charbuf_next(charbuf*);
int     charbuf_nextc(charbuf*, char* ch);
int     charbuf_peek(charbuf*);
int     charbuf_peekc(charbuf*, char* ch);
int     charbuf_skip(charbuf*);
int     charbuf_skip_pred(charbuf*, int (*pred)(int));
int     charbuf_skip_until(charbuf*, char c);
ssize_t charbuf_stubborn_read(charbuf*);


static inline int
charbuf_done(charbuf* b) {
  return b->eof || b->err;
}
static inline int
charbuf_skip_ifeq(charbuf* b, int c) {
  return (charbuf_peek(b) == c) ? charbuf_skip(b) : 0;
}
static inline int
charbuf_skip_ifset(charbuf* b, const char* set, size_t setlen) {
  char c;
  if(!charbuf_peekc(b, &c))
    return 0;
  if(byte_chr(set, setlen, c) == setlen)
    return 0;
  return charbuf_skip(b);
}


#ifdef __cplusplus
}
#endif

#endif /* defined CHARBUF_H */
/** @} */
