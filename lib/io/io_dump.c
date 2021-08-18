#include "../windoze.h"
#include "../io_internal.h"
#include "../socket_internal.h"
#include "../buffer.h"

#if WINDOWS_NATIVE
//#include <mswsock.h>
#else
#include <sys/select.h>
#endif

void
io_dump(buffer* p) {
  fd_set rfds, wfds;
  size_t i;
  io_entry* e;
  iarray* fds = io_getfds();
  size_t nfds = iarray_length(fds);

  for(i = 0; i < nfds; ++i) {
    if(!(e = (io_entry*)iarray_get(fds, i)))
      continue;

    buffer_puts(p, "fd #");
    buffer_putlong(p, i);
    //   buffer_puts(p, "fd #");
    if(e->canread)
      buffer_puts(p, " canread");
    if(e->canwrite)
      buffer_puts(p, " canwrite");
    if(e->wantread)
      buffer_puts(p, " wantread");
    if(e->wantwrite)
      buffer_puts(p, " wantwrite");

    buffer_putnlflush(p);
  }
}
