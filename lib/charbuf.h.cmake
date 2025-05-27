#ifndef CHARBUF_H
#define CHARBUF_H

@SIZE_TYPES_INCLUDE_FILES @

#ifdef __cplusplus
    extern "C" {
#endif

#ifdef _MSC_VER
#include <windows.h>
#undef ssize_t
  //#define ssize_t SSIZE_T
  typedef SSIZE_T ssize_t;
#endif

  typedef ssize_t(read_fn)(fd_t fd, void* buf, size_t n);

  typedef struct {
    char ch;
    size_t p;
    read_fn* op;
    fd_t fd;
  } charbuf;

#define CHARBUF_INIT(op, fd) \
  { '\0', 0, (op), (fd) }

  int charbuf_get(charbuf * b);
  int charbuf_getc(charbuf * b, char* ch);

  int charbuf_peek(charbuf * b);
  int charbuf_peekc(charbuf * b, char* ch);

  int charbuf_skip(charbuf * b);

  int charbuf_nextc(charbuf * b, char* ch);

  int charbuf_skip_until(charbuf * b, char c);

#ifdef __cplusplus
}
#endif

#endif /* defined CHARBUF_H */
