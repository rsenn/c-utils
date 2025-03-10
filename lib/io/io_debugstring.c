#include "../io_internal.h"
#include "../windoze.h"
#include "../socket_internal.h"
#include "../fmt.h"
#include "../iarray.h"

#include <time.h>

#if WINDOWS_NATIVE
#else
#include <sys/time.h>
#endif

unsigned int
io_debugstring(fd_type s, char* buf, unsigned int bufsize) {
  unsigned int i;
  io_entry* e;
  e = (io_entry*)iarray_get((iarray*)io_getfds(), s);

  if(!e)
    return 0;

  if(bufsize < 100)
    return 0;
  i = fmt_str(buf, "first_readable ");
  i += fmt_long(buf + i, first_readable);
  i += fmt_str(buf + i, ", first_writeable ");
  i += fmt_long(buf + i, first_writeable);
#ifdef HAVE_SIGIO
  i += fmt_str(buf + i, ", alt_firstread ");
  i += fmt_long(buf + i, alt_firstread);
  i += fmt_str(buf + i, ", alt_firstwrite ");
  i += fmt_long(buf + i, alt_firstwrite);
#endif
  i += fmt_str(buf + i, "\n");

  i += fmt_str(buf + i, "fd # ");
  i += fmt_ulong(buf + i, s);
  i += fmt_str(buf + i, ": ");

  if(bufsize - i < 100)
    return 0;
  i += fmt_str(buf + i, "timeout ");
  i += fmt_ulonglong(buf + i, e->timeout.sec.x - (uint64)4611686018427387914 - time(NULL));
  i += fmt_str(buf + i, ".");
  i += fmt_ulong(buf + i, e->timeout.nano);
  i += fmt_str(buf + i, " ");

  if(bufsize - i < 100)
    return 0;

  if(e->wantread)
    i += fmt_str(buf + i, "wr ");

  if(e->wantwrite)
    i += fmt_str(buf + i, "ww ");

  if(e->canread)
    i += fmt_str(buf + i, "cr ");

  if(e->canwrite)
    i += fmt_str(buf + i, "cw ");

  if(e->nonblock)
    i += fmt_str(buf + i, "nb ");

  if(!e->inuse)
    i += fmt_str(buf + i, "!inuse ");

  if(e->kernelwantread)
    i += fmt_str(buf + i, "kwr ");

  if(e->kernelwantwrite)
    i += fmt_str(buf + i, "kww ");

  if(e->epolladded)
    i += fmt_str(buf + i, "ea ");

  if(e->mmapped) {
    i += fmt_str(buf + i, "mmap(");
    i += fmt_xlonglong(buf + i, (ptrdiff_t)e->mmapped);
    i += fmt_str(buf + i, ",");
    i += fmt_xlong(buf + i, (unsigned long)e->maplen);
    i += fmt_str(buf + i, "@");
    i += fmt_xlong(buf + i, (unsigned long)e->mapofs);
  }

  if(bufsize - i < 100)
    return 0;
  i += fmt_str(buf + i, "next_read ");
  i += fmt_long(buf + i, e->next_read);
  i += fmt_str(buf + i, " next_write ");
  i += fmt_long(buf + i, e->next_write);
  i += fmt_str(buf + i, " cookie ");
  i += fmt_xlonglong(buf + i, (ptrdiff_t)e->cookie);
  buf[i] = 0;
  return i;
}
