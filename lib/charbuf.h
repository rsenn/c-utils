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
  unsigned char ch;
  unsigned p : 1;
  unsigned eof : 1;
  unsigned err : 1;
  read_fn* op;
  union {
    fd_t fd;
    void* ptr;
  };
  charloc loc;
} charbuf;

#define CHARBUF_INIT(op, fd)                                                                                           \
  { '\0', 0, 0, 0, (op), (fd) }

void charbuf_close(charbuf*);
void charbuf_froms(charbuf*, char** s);
ssize_t charbuf_get(charbuf*);
ssize_t charbuf_getc(charbuf*, unsigned char* ch);
void charbuf_init(charbuf*, read_fn* op, fd_t fd);
ssize_t charbuf_next(charbuf*);
ssize_t charbuf_nextc(charbuf*, unsigned char* ch);
ssize_t charbuf_peek(charbuf*);
ssize_t charbuf_peekc(charbuf*, unsigned char* ch);
ssize_t charbuf_skip(charbuf*);
ssize_t charbuf_skip_pred(charbuf*, int (*pred)(int));
ssize_t charbuf_skip_until(charbuf*, int c);
ssize_t charbuf_stubborn_read(charbuf*);

static inline ssize_t
charbuf_done(charbuf* b) {
  return b->eof || b->err;
}

static inline ssize_t
charbuf_skip_ifeq(charbuf* b, unsigned char c) {
  unsigned char ch;
  ssize_t ret;
  if((ret = charbuf_peekc(b, &ch)) <= 0)
    return ret;
  if((ret = (ch == c)))
    charbuf_skip(b);
  return ret;
}

static inline ssize_t
charbuf_skip_ifset(charbuf* b, const char* set, size_t setlen) {
  unsigned char c;
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
