#include "../windoze.h"
#include "../io_internal.h"
#include "../socket_internal.h"
#include "../buffer.h"

#if WINDOWS_NATIVE
// #include <mswsock.h>
#else
#include <sys/select.h>
#endif

void
io_dump(buffer* p) {
  io_entry* e;
  iarray* fds = io_getfds();
  size_t nfds = iarray_length(fds);

  for(size_t i = 0; i < nfds; ++i) {
    if(!(e = (io_entry*)iarray_get(fds, i)))
      continue;

    buffer_puts(p, "fd #");
    buffer_putlong(p, i);

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
