#define USE_WS2_32 1

#include "../socket_internal.h"
//#include "../io.h"
#include "../select.h"
#include "../iopause.h"
#include "../taia.h"
#include "../alloc.h"
//#include <alloca.h>

void
iopause(iopause_fd* x, unsigned int len, struct taia* deadline, struct taia* stamp) {
  struct taia t;
  int millisecs;

  double d;
  unsigned int i;

  if(taia_less(deadline, stamp))
    millisecs = 0;
  else {
    t = *stamp;
    taia_sub(&t, deadline, &t);
    d = taia_approx(&t);

    if(d > 1000.0)
      d = 1000.0;
    millisecs = (int)(d * 1000.0 + 20.0);
  }

  for(i = 0; i < len; ++i)
    x[i].revents = 0;

#ifdef IOPAUSE_LINUX_AIO
  {
    struct iocb* cblist;
    struct iocb** ptrlist[len]; // = alloca(sizeof(struct iocb*) * len);
    struct io_event* evlist = 0;
    aio_context_t ctx = 0;
    struct timespec ts;
    int j, r;

    if((cblist = alloc_zero(sizeof(struct iocb) * len)) == 0)
      goto aio_fail;

    for(i = 0; i < len; ++i) {
      struct iocb* cb = &cblist[i];
      cb->aio_fildes = x[i].fd;
      cb->aio_lio_opcode = IOCB_CMD_POLL;
      cb->aio_buf = x[i].events;

      ptrlist[i] = cb;
    }

    if((r = io_setup(len, &ctx)) == -1)
      goto aio_fail;

    if((r = io_submit(ctx, len, ptrlist)) == -1)
      goto aio_fail;

    ts.tv_sec = millisecs / 1000;
    ts.tv_nsec = (millisecs % 1000ull) * 1000000ull;

    if((evlist = alloc_zero(sizeof(struct io_event) * len)) == 0)
      goto aio_fail;

    if((r = io_getevents(ctx, 1, len, evlist, millisecs == -1 ? 0 : &ts)) == -1)
      goto aio_fail;

    for(i = 0; i < len; ++i) {
      struct io_event* ev = &evlist[i];
      struct iocb* cb = (struct iocb*)(uintptr_t)ev->obj;

      x[i].revents = ev->res;
    }
  aio_fail:

    if(ctx)
      io_destroy(ctx);
    alloc_free(cblist);

    if(evlist)
      alloc_free(evlist);
  }

#elif defined(IOPAUSE_POLL)

  poll(x, len, millisecs);
  /* XXX: some kernels apparently need x[0] even if len is 0 */
  /* XXX: how to handle EAGAIN? are kernels really this dumb? */
  /* XXX: how to handle EINVAL? when exactly can this happen? */

#else
  {
    struct timeval tv;
    fd_set rfds;
    fd_set wfds;
    int nfds;
    fd_type fd;

    FD_ZERO(&rfds);
    FD_ZERO(&wfds);

    nfds = 1;

    for(i = 0; i < len; ++i) {
      fd = x[i].fd;

      if(fd != x[i].fd)
        continue;

      if(fd < 0)
        continue;

      if(fd >= (int)(8 * sizeof(fd_set)))
        continue; /*XXX*/

      if(fd >= nfds)
        nfds = fd + 1;

      if(x[i].events & IOPAUSE_READ)
        FD_SET(fd, &rfds);

      if(x[i].events & IOPAUSE_WRITE)
        FD_SET(fd, &wfds);
    }

    tv.tv_sec = millisecs / 1000;
    tv.tv_usec = 1000 * (millisecs % 1000);

    if(select(nfds, &rfds, &wfds, (fd_set*)0, &tv) <= 0)
      return;
    /* XXX: for EBADF, could seek out and destroy the bad descriptor */

    for(i = 0; i < len; ++i) {
      fd = x[i].fd;

      if(fd < 0)
        continue;

      if(fd >= (fd_type)(8 * sizeof(fd_set)))
        continue; /*XXX*/

      if(x[i].events & IOPAUSE_READ)

        if(FD_ISSET(fd, &rfds))
          x[i].revents |= IOPAUSE_READ;

      if(x[i].events & IOPAUSE_WRITE)

        if(FD_ISSET(fd, &wfds))
          x[i].revents |= IOPAUSE_WRITE;
    }
  }
#endif
}
