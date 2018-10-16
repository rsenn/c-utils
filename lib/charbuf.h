#ifndef CHARBUF_H
#define CHARBUF_H

#include <stddef.h>
#include <sys/types.h>

#include "io.h"

#ifdef __cplusplus
extern "C" {
#endif

#if WINDOWS && !defined(__ssize_t_defined) && !defined(_SSIZE_T_DECLARED) && !defined(_SSIZE_T_DEFINED) && !defined(__DEFINED_ssize_t) && !defined(__dietlibc__)
#define __ssize_t_defined 1
#define _SSIZE_T_DECLARED 1
#define _SSIZE_T_DEFINED 1
#ifdef _WIN32
typedef __int32 ssize_t;
#endif
#ifdef _WIN64
typedef __int64 ssize_t;
#endif
#endif

typedef ssize_t (read_fn)(fd_t fd, void* buf, size_t n);

typedef struct {
  char ch;
  unsigned char p:1;
  read_fn* op;
  fd_t fd;
} charbuf;

#define CHARBUF_INIT(op, fd) { '\0', 0, (op), (fd) }

void charbuf_init(charbuf*, read_fn*, fd_t);
void charbuf_close(charbuf*);
int  charbuf_getc(charbuf*, char*);
int  charbuf_get(charbuf*);
int  charbuf_nextc(charbuf*, char*);
int  charbuf_peekc(charbuf*, char*);
int  charbuf_peek(charbuf*);
int  charbuf_skip_pred(charbuf*, int (*pred)(int));
int  charbuf_skip_until(charbuf*, char);
int  charbuf_skip(charbuf*);

#ifdef __cplusplus
}
#endif

#endif /* defined CHARBUF_H */

