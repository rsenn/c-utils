#include "lib/stralloc.h"
#include "lib/buffer.h"
//#include "getln.h"
//#include "timeoutio.h"
//#include "ftplib.h"
//#include "readwrite.h"
#include "lib/str.h"
//#include "error.h"
#include "lib/case.h"
//#include "ftpcopy.h"

MKTIMEOUTREAD(o_timeout)

int
ftp_read_list(int fd, stralloc* dir) {
  buffer io;
  int lines = 0;
  char buf[BUFFER_INSIZE];
  stralloc direntry;
  sralloc_init(&direntry);

  buffer_init(&io, (buffer_op)TIMEOUTREADFN(o_timeout), fd, buf, BUFFER_INSIZE);
  dir->len = 0;

  for(;;) {
    int gotlf;

    if(-1 == getln(&io, &direntry, &gotlf, '\n')) {
      int e;
    error:
      e = errno;
      stralloc_free(&direntry);
      errno = e;
      return -1;
    }

    if(direntry.len == 0)
      break;

    if(!gotlf) {
      errno = error_proto;
      goto error;
    }
    direntry.len--;

    if(direntry.s[direntry.len - 1] == '\r')
      direntry.len--;
    direntry.s[direntry.len] = 0;

    if(case_starts(direntry.s, "total"))
      continue;

    if(!stralloc_catb(dir, direntry.s, direntry.len + 1))
      goto error;
    lines++;
  }
  stralloc_free(&direntry);
  return lines;
}
