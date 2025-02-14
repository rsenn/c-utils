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
#include "uint8.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef ssize_t read_fn(int fd, void* buf, size_t n, void*);

typedef struct {
  uint32 line;
  uint32 column;
} charloc;

#pragma pack(push, 1)
typedef struct /*__attribute__((packed))*/ {
  uint8* chrs;
  unsigned p : 12;
  unsigned a : 12;
  unsigned eof : 1;
  unsigned err : 6;
  read_fn* op;
  union {
    fd_type fd;
    void* ptr;
  };
  charloc loc;
  size_t offset;

} charbuf;
#pragma pack(pop)

#define CHARBUF_INIT(op, fd) \
  { '\0', 0, 0, 0, (op), (fd) }

void charbuf_close(charbuf*);
void charbuf_free(charbuf*);
void charbuf_froms(charbuf*, char* s, unsigned lookahead);
ssize_t charbuf_get(charbuf*);
ssize_t charbuf_getc(charbuf*, unsigned char* ch);
void
charbuf_init(charbuf*, read_fn* op, fd_type fd, unsigned int lookahead);
ssize_t charbuf_next(charbuf*);
ssize_t charbuf_nextc(charbuf*, unsigned char* ch);
ssize_t charbuf_peek(charbuf*);
ssize_t charbuf_peekc(charbuf*, unsigned char* ch);
uint8* charbuf_peekn(charbuf*, unsigned int n);

ssize_t charbuf_skip(charbuf*);

ssize_t charbuf_skip_until(charbuf*, int c);
ssize_t charbuf_skipc(charbuf*, uint8* ch);
ssize_t charbuf_skipn(charbuf*, size_t n);
ssize_t charbuf_skip_ws(charbuf*);

typedef int charbuf_ctypefn_t(int);
typedef charbuf_ctypefn_t* charbuf_ctypefn_ptr;

static inline int
charbuf_is_ctype(int c, size_t pos, void* ptr) {
  charbuf_ctypefn_ptr fn = ptr;
  return fn(c);
}

ssize_t charbuf_pred_skip(charbuf*,
                          int (*pred)(int, size_t, void*),
                          void* ptr);
ssize_t charbuf_pred_lookahead(charbuf*,
                               int (*pred)(int, size_t, void*),
                               void* ptr);
#ifdef STRALLOC_H
ssize_t charbuf_pred_get(charbuf*,
                         stralloc* sa,
                         int (*pred)(int, size_t, void*),
                         void* ptr);
#endif

static inline ssize_t
charbuf_done(charbuf* b) {
  return b->eof || b->err;
}

static inline int
charbuf_is_samechar(int c, size_t pos, void* ptr) {
  int other = (size_t)ptr;

  return pos == 0 && c == other;
}

static inline ssize_t
charbuf_skip_ifeq(charbuf* b, unsigned char c) {
  return charbuf_pred_skip(b, charbuf_is_samechar, (void*)(size_t)c);
  /* unsigned char ch;
   ssize_t ret;
   if((ret = charbuf_peekc(b, &ch)) <= 0)
     return ret;
   if((ret = (ch == c)))
     charbuf_skip(b);
   return ret;*/
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

#ifdef BUFFER_H
void charbuf_dump(charbuf*, buffer*);

#endif

#ifdef __cplusplus
}
#endif

#endif /* defined CHARBUF_H */
/** @} */
