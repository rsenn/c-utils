#include "../errmsg_internal.h"
#include "../str.h"
#include "../uint64.h"

#if WINDOWS || !defined(HAVE_WRITEV) || defined(_DEBUG)
void
errmsg_puts(int64 fd, const char* s) {
  write(fd, s, str_len(s));
}

void
errmsg_flush(int64 fd) {
  return;
}

#else
#include <sys/uio.h>

enum { COUNT = 25 };
static struct iovec x[COUNT];
static int l;

void
errmsg_puts(int64 fd, const char* s) {
  x[l].iov_base = (char*)s;
  x[l].iov_len = str_len(s);

  if(++l == COUNT)
    errmsg_flush(fd);
}

void
errmsg_flush(int64 fd) {
  int n = l;
  l = 0;

  if(n)
    writev(fd, x, n);
}
#endif

void
errmsg_start(int64 fd) {
  if(errmsg_argv0) {
    errmsg_puts(fd, errmsg_argv0);
    errmsg_puts(fd, ": ");
  }
}
