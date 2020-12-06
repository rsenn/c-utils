//#include "../io.h"
#define USE_WS2_32 1

#include "../socket_internal.h"
#include "../io_internal.h"
#include "../buffer.h"
#include "../alloc.h"
#include "../byte.h"

#if WINDOWS_NATIVE
#include <windows.h>
#endif

#if !WINDOWS_NATIVE
#include <poll.h>
#endif

#include <errno.h>

#ifdef __dietlibc__
#include "../fmt.h"
#include <write12.h>
#endif

#if 0
static void handleevent(fd_t fd, int readable, int writable, int error) {
  io_entry* e = array_get(io_getfds(), sizeof(io_entry), fd);
  if(e) {
    int curevents = 0, newevents;
    if(e->kernelwantread) curevents |= POLLIN;
    if(e->kernelwantwrite) curevents |= POLLOUT;

#ifdef DEBUG
    if(readable && !e->kernelwantread) {
      buffer_puts(buffer_2, "got unexpected read event on fd #");
      buffer_putlong(buffer_2, fd);
      buffer_putnlflush(buffer_2);
    }
    if(writable && !e->kernelwantwrite) {
      buffer_puts(buffer_2, "got unexpected write event on fd #");
      buffer_putlong(buffer_2, fd);
      buffer_putnlflush(buffer_2);
    }
#endif

    if(error) {
      /* signal whatever app is looking for */
      if(e->wantread) readable = 1;
      if(e->wantwrite) writable = 1;
    }

    if(readable && !e->canread) {
      e->canread = 1;
      if(e->wantread) {
        e->next_read = first_readable;
        first_readable = y[i].data.fd;
      }
    }
    if(writable && !e->canwrite) {
      e->canwrite = 1;
      if(e->wantwrite) {
        e->next_write = first_writeable;
        first_writeable = y[i].data.fd;
      }
    }

    /* TODO: wie kommuniziere ich nach außen, was der Caller tun soll?
     * Bitfeld-Integer? */

    newevents = 0;
    if(!e->canread || e->wantread) {
      newevents |= EPOLLIN;
      e->kernelwantread = 1;
    } else
      e->kernelwantread = 0;
    if(!e->canwrite || e->wantwrite) {
      newevents |= EPOLLOUT;
      e->kernelwantwrite = 1;
    } else
      e->kernelwantwrite = 0;
    if(newevents != curevents) {
#if 0
      printf("canread %d, wantread %d, kernelwantread %d, canwrite %d, wantwrite %d, kernelwantwrite %d\n",
             e->canread, e->wantread, e->kernelwantread, e->canwrite, e->wantwrite, e->kernelwantwrite);
      printf("newevents: read %d write %d\n", !!(newevents & EPOLLIN), !!(newevents & EPOLLOUT));
#endif
      y[i].events = newevents;
      if(newevents) {
        epoll_ctl(io_master, EPOLL_CTL_MOD, y[i].data.fd, y + i);
      } else {
        epoll_ctl(io_master, EPOLL_CTL_DEL, y[i].data.fd, y + i);
        --io_wanted_fds;
      }
    }
  } else {
    epoll_ctl(io_master, EPOLL_CTL_DEL, y[i].data.fd, y + i);
  }
}
#endif

static void
put_fdset(buffer* b, const char* name, const fd_set* fds, fd_t maxfd) {
  fd_t i;
  buffer_putm_internal(b, "fd_set ", name, "=[", NULL);
  for(i = 0; i <= maxfd; ++i) {
    if(FD_ISSET(i, fds)) {
      buffer_putspace(b);
      buffer_putlong(b, i);
    }
  }
  buffer_puts(b, " ]");
}

int64
io_waituntil2(int64 milliseconds) {
#if !(defined(_WIN32) || defined(_WIN64))
  struct pollfd* p;
#endif
  long i, j, r;
  if(!io_wanted_fds)
    return 0;

#ifdef DEBUG_IO
  buffer_putspad(buffer_2, "io_waituntil2", 30);
  buffer_puts(buffer_2, "milliseconds=");
  buffer_putlonglong(buffer_2, milliseconds);
  buffer_putnlflush(buffer_2);
#endif
#ifdef USE_LINUX_AIO
#warning USE_LINUX_AIO
  {
    iarray* fds = io_getfds();
    size_t nfds = iarray_length(fds);
    aio_context_t ctx = 0;
    struct timespec ts;
    struct io_event* evlist = 0;
    struct iocb* cblist = alloc_zero(sizeof(struct iocb) * (nfds + 1));
    struct iocb** ptrlist = alloc_zero(sizeof(struct iocb*) * (nfds + 1));

    for(i = j = r = 0; i <= nfds; ++i) {
      io_entry* e;
      struct iocb* cb = 0;
      if(!(e = (io_entry*)iarray_get(fds, i)))
        continue;
      if(e->cb.aio_lio_opcode) {
        cb = &e->cb;
        cb->aio_fildes = i;
      } else if(e->wantread || e->wantwrite) {
        int events = 0;
        cb = &cblist[j++];
        ;
        if(e->wantread)
          events |= POLLIN;
        if(e->wantwrite)
          events |= POLLOUT;
        cb->aio_fildes = i;
        cb->aio_lio_opcode = IOCB_CMD_POLL;
        cb->aio_buf = events;
      } else {
        continue;
      }
      ptrlist[r++] = cb;
    }

    if((r = io_setup(j, &ctx)) == -1)
      goto fail;

    if((r = io_submit(ctx, j, ptrlist)) == -1)
      goto fail;

    evlist = alloc_zero(sizeof(struct io_event) * j);
    byte_zero(&ts, sizeof(ts));
    ts.tv_sec = milliseconds / 1000;
    ts.tv_nsec = (milliseconds % 1000ull) * 1000000ull;
    if((r = io_getevents(ctx, 1, j, evlist, milliseconds == -1 ? 0 : &ts)) == -1)
      goto fail;

    for(i = 0; i < r; ++i) {
      struct io_event* ev = &evlist[i];
      struct iocb* cb = (struct iocb*)(uintptr_t)ev->obj;
      io_entry* e = iarray_get(fds, cb->aio_fildes);
      int revents = ev->res;

      if(ev->res & (POLLERR | POLLHUP | POLLNVAL)) {
        /* error; signal whatever app is looking for */
        if(e->wantread)
          revents |= POLLIN;
        if(e->wantwrite)
          revents |= POLLOUT;
      }
      if(!e->canread && (ev->res & POLLIN)) {
        e->canread = 1;
        e->next_read = first_readable;
        first_readable = cb->aio_fildes;
      }
      if(!e->canwrite && (ev->res & POLLOUT)) {
        e->canwrite = 1;
        e->next_write = first_writeable;
        first_writeable = cb->aio_fildes;
      }
      p++;
    }
  fail:
    if(ctx)
      io_destroy(ctx);

    alloc_free(cblist);
    alloc_free(ptrlist);
    if(evlist)
      alloc_free(evlist);

    return r;
  }
#elif defined(USE_SELECT)
  {
    fd_set rfds, wfds;
    fd_t maxfd = -1;
    io_entry* e;
    struct timeval tv;
    iarray* fds = io_getfds();
    size_t nfds = iarray_length(fds);
    FD_ZERO(&rfds);
    FD_ZERO(&wfds);
    for(i = r = 0; i <= nfds; ++i) {
      if(!(e = (io_entry*)iarray_get(fds, i)))
        continue;
      e->canread = e->canwrite = 0;
      if(e->wantread || e->wantwrite) {
        if(e->wantread)
          FD_SET(i, &rfds);
        if(e->wantwrite)
          FD_SET(i, &wfds);
        if(i > maxfd)
          maxfd = i;
      }
      ++r;
    }

#ifdef DEBUG_IO
    put_fdset(buffer_2, "rfds", &rfds, maxfd);
    buffer_puts(buffer_2, ", ");
    put_fdset(buffer_2, "wfds", &wfds, maxfd);
    buffer_putnlflush(buffer_2);
#endif
    tv.tv_sec = milliseconds / 1000;
    tv.tv_usec = milliseconds % 1000 * 1000;

    if((i = select(maxfd + 1, &rfds, &wfds, NULL, milliseconds == -1 ? 0 : &tv)) == -1)
      return -1;
#ifdef DEBUG_IO
    put_fdset(buffer_2, "rfds2", &rfds, maxfd);
    buffer_puts(buffer_2, ", ");
    put_fdset(buffer_2, "wfds2", &wfds, maxfd);
    buffer_putnlflush(buffer_2);
#endif
    for(j = maxfd; j >= 0; --j) {
      if(!(e = (io_entry*)iarray_get((iarray*)io_getfds(), j)))
        continue;

      if(!e->canread && FD_ISSET(j, &rfds)) {
        e->canread = 1;
        e->next_read = first_readable;
        first_readable = j;
      }
      if(!e->canwrite && FD_ISSET(j, &wfds)) {
        e->canwrite = 1;
        e->next_write = first_writeable;
        first_writeable = j;
      }
    }
    return i;
  }
#elif WINDOWS_NATIVE
  {
    DWORD numberofbytes;
    DWORD x;
    LPOVERLAPPED o;
    if(first_readable != -1 || first_writeable != -1) {
      return 1;
    }
    if(GetQueuedCompletionStatus(
           io_comport, &numberofbytes, &x, &o, milliseconds == -1 ? INFINITE : milliseconds)) {
      io_entry* e = (io_entry*)iarray_get((iarray*)io_getfds(), x);
      if(!e)
        return 0;
      e->errorcode = 0;
      if(o == &e->or &&e->readqueued == 1) {
        e->readqueued = 2;
        e->canread = 1;
        e->bytes_read = numberofbytes;
        e->next_read = first_readable;
        first_readable = x;
      } else if(o == &e->ow && e->writequeued == 1) {
        e->writequeued = 2;
        e->canwrite = 1;
        e->bytes_written = numberofbytes;
        e->next_write = first_writeable;
        first_writeable = x;
      } else if(o == &e->or &&e->acceptqueued == 1) {
        e->acceptqueued = 2;
        e->canread = 1;
        e->next_read = first_readable;
        first_readable = x;
      } else if(o == &e->ow && e->connectqueued == 1) {
        e->connectqueued = 2;
        e->canwrite = 1;
        e->next_write = first_writeable;
        first_writeable = x;
      } else if(o == &e->os && e->sendfilequeued == 1) {
        e->sendfilequeued = 2;
        e->canwrite = 1;
        e->bytes_written = numberofbytes;
        e->next_write = first_writeable;
        first_writeable = x;
      }
      return 1;
    } else {
      /* either the overlapped I/O request failed or we timed out */
      DWORD err;
      io_entry* e;
      if(!o)
        return 0; /* timeout */
      /* we got a completion packet for a failed I/O operation */
      err = GetLastError();
      if(err == WAIT_TIMEOUT)
        return 0; /* or maybe not */
      e = (io_entry*)iarray_get((iarray*)io_getfds(), x);
      if(!e)
        return 0; /* WTF?! */
      e->errorcode = err;
      if(o == &e->or &&(e->readqueued || e->acceptqueued)) {
        if(e->readqueued)
          e->readqueued = 2;
        else if(e->acceptqueued)
          e->acceptqueued = 2;
        e->canread = 1;
        e->bytes_read = -1;
        e->next_read = first_readable;
        first_readable = x;
      } else if((o == &e->ow || o == &e->os) &&
                (e->writequeued || e->connectqueued || e->sendfilequeued)) {
        if(o == &e->ow) {
          if(e->writequeued)
            e->writequeued = 2;
          else if(e->connectqueued)
            e->connectqueued = 2;
        } else if(o == &e->os)
          e->sendfilequeued = 2;
        e->canwrite = 1;
        e->bytes_written = -1;
        e->next_write = first_writeable;
        first_writeable = x;
      }
      return 1;
    }
  }
#elif defined(HAVE_EPOLL)
  if(io_waitmode == EPOLL) {
    int n;
    struct epoll_event y[100];
    if((n = epoll_wait(io_master, y, 100, milliseconds)) == -1)
      return -1;
    for(i = 0; i < n; ++i) {
      io_entry* e = (io_entry*)iarray_get((iarray*)io_getfds(), y[i].data.fd);
      if(e) {
        int curevents = 0, newevents;
        if(e->kernelwantread)
          curevents |= EPOLLIN;
        if(e->kernelwantwrite)
          curevents |= EPOLLOUT;

#ifdef DEBUG
        if((y[i].events & (EPOLLIN | EPOLLPRI | EPOLLRDNORM | EPOLLRDBAND)) && !e->kernelwantread)
          DEBUG_MSG("got unexpected read event on fd #", y[i].data.fd);
        if((y[i].events & EPOLLOUT) && !e->kernelwantwrite)
          DEBUG_MSG("got unexpected write event on fd #", y[i].data.fd);
#endif

        if(y[i].events & (POLLERR | POLLHUP)) {
          /* error; signal whatever app is looking for */
          if(e->wantread)
            y[i].events |= POLLIN;
          if(e->wantwrite)
            y[i].events |= POLLOUT;
        }

        newevents = 0;
        if(!e->canread || e->wantread) {
          newevents |= EPOLLIN;
          e->kernelwantread = 1;
        } else
          e->kernelwantread = 0;
        if(!e->canwrite || e->wantwrite) {
          newevents |= EPOLLOUT;
          e->kernelwantwrite = 1;
        } else
          e->kernelwantwrite = 0;

        /* if we think we can not read, but the kernel tells us that we
         * can, put this fd in the relevant data structures */
        if(!e->canread && (y[i].events & (EPOLLIN | EPOLLPRI | EPOLLRDNORM | EPOLLRDBAND))) {
          if(e->canread) {
            newevents &= ~EPOLLIN;
          } else {
            e->canread = 1;
            if(e->wantread) {
              e->next_read = first_readable;
              first_readable = y[i].data.fd;
            }
          }
        }

        /* if the kernel says the fd is writable, ... */
        if(y[i].events & EPOLLOUT) {
          /* Usually, if the kernel says a descriptor is writable, we
           * note it and do not tell the kernel not to tell us again.
           * The idea is that once we notify the caller that the fd is
           * writable, and the caller handles the event, the caller will
           * just ask to be notified of future write events again.  We
           * are trying to save the superfluous epoll_ctl syscalls.
           * If e->canwrite is set, then this gamble did not work out.
           * We told the caller, yet after the caller is done we still
           * got another write event.  Clearly the user is implementing
           * some kind of throttling and we can tell the kernel to leave
           * us alone for now. */
          if(e->canwrite) {
            newevents &= ~EPOLLOUT;
            e->kernelwantwrite = 0;
          } else {
            /* If !e->wantwrite: The laziness optimization in
             * io_dontwantwrite hit.  We did not tell the kernel that we
             * are no longer interested in writing to save the syscall.
             * Now we know we could write if we wanted; remember that
             * and then go on. */
            e->canwrite = 1;
            if(e->wantwrite) {
              e->next_write = first_writeable;
              first_writeable = y[i].data.fd;
            }
          }
        }

        if(newevents != curevents) {
#if 0
          printf("canread %d, wantread %d, kernelwantread %d, canwrite %d, wantwrite %d, kernelwantwrite %d\n",
                 e->canread, e->wantread, e->kernelwantread, e->canwrite, e->wantwrite, e->kernelwantwrite);
          printf("newevents: read %d write %d\n", !!(newevents & EPOLLIN), !!(newevents & EPOLLOUT));
#endif
          y[i].events = newevents;
          if(newevents) {
            epoll_ctl(io_master, EPOLL_CTL_MOD, y[i].data.fd, y + i);
          } else {
            epoll_ctl(io_master, EPOLL_CTL_DEL, y[i].data.fd, y + i);
            --io_wanted_fds;
          }
        }
      } else {
#if 0
#ifdef __dietlibc__
        char buf[FMT_ULONG];
        buf[fmt_ulong(buf, y[i].data.fd)] = 0;
        __write2("got epoll event on invalid fd ");
        __write2(buf);
        __write2("!\n");
#endif

#endif
        epoll_ctl(io_master, EPOLL_CTL_DEL, y[i].data.fd, y + i);
      }
    }
    return n;
  }
#elif defined(HAVE_KQUEUE)
  if(io_waitmode == KQUEUE) {
    struct kevent y[100];
    int n;
    struct timespec ts;
    ts.tv_sec = milliseconds / 1000;
    ts.tv_nsec = (milliseconds % 1000) * 1000000;
    if((n = kevent(io_master, 0, 0, y, 100, milliseconds != -1 ? &ts : 0)) == -1)
      return -1;
    for(i = n - 1; i >= 0; --i) {
      io_entry* e = (io_entry*)iarray_get((iarray*)io_getfds(), y[--n].ident);
      if(e) {
        if(y[n].flags & EV_ERROR) {
          /* error; signal whatever app is looking for */
          if(e->wantread)
            y[n].filter = EVFILT_READ;
          else if(e->wantwrite)
            y[n].filter = EVFILT_WRITE;
        }
        if(!e->canread && (y[n].filter == EVFILT_READ)) {
          e->canread = 1;
          e->next_read = first_readable;
          first_readable = y[n].ident;
        }
        if(!e->canwrite && (y[n].filter == EVFILT_WRITE)) {
          e->canwrite = 1;
          e->next_write = first_writeable;
          first_writeable = y[i].ident;
        }
#ifdef DEBUG
      } else {
#endif
      }
    }
    return n;
  }
#elif defined(HAVE_DEVPOLL)
  if(io_waitmode == DEVPOLL) {
    dvpoll_t timeout;
    struct pollfd y[100];
    int n;
    timeout.dp_timeout = milliseconds;
    timeout.dp_nfds = 100;
    timeout.dp_fds = y;
    if((n = ioctl(io_master, DP_POLL, &timeout)) == -1)
      return -1;
    for(i = n - 1; i >= 0; --i) {
      io_entry* e = (io_entry*)iarray_get((iarray*)io_getfds(), y[--n].fd);
      if(e) {
        if(y[n].revents & (POLLERR | POLLHUP | POLLNVAL)) {
          /* error; signal whatever app is looking for */
          if(e->wantread)
            y[n].revents = POLLIN;
          if(e->wantwrite)
            y[n].revents = POLLOUT;
        }
        if(!e->canread && (y[n].revents & POLLIN)) {
          e->canread = 1;
          if(e->next_read == -1) {
            e->next_read = first_readable;
            first_readable = y[n].fd;
          }
        }
        if(!e->canwrite && (y[n].revents & POLLOUT)) {
          e->canwrite = 1;
          if(e->next_write == -1) {
            e->next_write = first_writeable;
            first_writeable = y[i].fd;
          }
        }
#ifdef DEBUG
      } else {
#endif
      }
    }
    return n;
  }
#elif defined(HAVE_SIGIO)
  if(io_waitmode == _SIGIO) {
    siginfo_t info;
    struct timespec ts;
    int r;
    io_entry* e;
    if(alt_firstread >= 0 && (e = (io_entry*)iarray_get((iarray*)io_getfds(), alt_firstread)) &&
       e->canread)
      return 1;
    if(alt_firstwrite >= 0 && (e = (io_entry*)iarray_get((iarray*)io_getfds(), alt_firstwrite)) &&
       e->canwrite)
      return 1;
    if(milliseconds == -1)
      r = sigwaitinfo(&io_ss, &info);
    else {
      ts.tv_sec = milliseconds / 1000;
      ts.tv_nsec = (milliseconds % 1000) * 1000000;
      r = sigtimedwait(&io_ss, &info, &ts);
    }
    switch(r) {
      case SIGIO:
        /* signal queue overflow */
        signal(io_signum, SIG_DFL);
        goto dopoll;
      default:
        if(r == io_signum) {
          io_entry* e = (io_entry*)iarray_get((iarray*)io_getfds(), info.si_fd);
          if(e) {
            if(info.si_band & (POLLERR | POLLHUP)) {
              /* error; signal whatever app is looking for */
              if(e->wantread)
                info.si_band |= POLLIN;
              if(e->wantwrite)
                info.si_band |= POLLOUT;
            }
            if(info.si_band & POLLIN && !e->canread) {
              debug_printf(("io_waituntil2: enqueueing %ld in normal read queue before %ld\n",
                            info.si_fd,
                            first_readable));
              e->canread = 1;
              e->next_read = first_readable;
              first_readable = info.si_fd;
            }
            if(info.si_band & POLLOUT && !e->canwrite) {
              debug_printf(("io_waituntil2: enqueueing %ld in normal write queue before %ld\n",
                            info.si_fd,
                            first_writeable));
              e->canwrite = 1;
              e->next_write = first_writeable;
              first_writeable = info.si_fd;
            }
#ifdef DEBUG
          } else {
#endif
          }
        }
    }
    return 1;
  }
#else
#if !defined(HAVE_POLL)
#warning Poll fallback
#endif
#endif

#if defined(HAVE_POLL) || defined(SUPPORT_POLL)
dopoll :

{
  struct pollfd* p;
  for(i = r = 0; (size_t)i <= iarray_length((iarray*)io_getfds()); ++i) {
    io_entry* e = (io_entry*)iarray_get((iarray*)io_getfds(), i);
    if(!e)
      continue;
    e->canread = e->canwrite = 0;
    if(e->wantread || e->wantwrite) {
      if((p = (struct pollfd*)array_allocate(&io_pollfds, sizeof(struct pollfd), r))) {
        p->fd = i;
        p->events = (e->wantread ? POLLIN : 0) + (e->wantwrite ? POLLOUT : 0);
        ++r;
      } else
        return -1;
    }
  }
#ifdef DEBUG_IO
  p = (struct pollfd*)array_start(&io_pollfds);
  buffer_puts(buffer_2, "io_waituntil2(");
  buffer_putulonglong(buffer_2, milliseconds);
  buffer_puts(buffer_2, ") ");
  buffer_putlong(buffer_2, r);
  buffer_putsflush(buffer_2, " fds\n");
  for(i = 0; i < r; ++i) {
    buffer_puts(buffer_2, "pollfd[");
    buffer_putlong(buffer_2, i);
    buffer_puts(buffer_2, "] { .fd=");
    buffer_putlong(buffer_2, p[i].fd);
    buffer_puts(buffer_2, ", events=");
    if(p[i].events & POLLIN)
      buffer_puts(buffer_2, "IN ");
    if(p[i].events & POLLOUT)
      buffer_puts(buffer_2, "OUT ");
    if(p[i].events & POLLERR)
      buffer_puts(buffer_2, "ERR ");
    buffer_puts(buffer_2, "}");
    buffer_putnlflush(buffer_2);
  }
#endif
  if((i = poll((struct pollfd*)array_start(&io_pollfds), r, milliseconds)) < 1)
    return -1;
#ifdef DEBUG_IO
  for(i = 0; i < r; ++i) {
    buffer_puts(buffer_2, "pollfd[");
    buffer_putlong(buffer_2, i);
    buffer_puts(buffer_2, "] { .fd=");
    buffer_putlong(buffer_2, p[i].fd);
    buffer_puts(buffer_2, ", revents=");
    if(p[i].revents & POLLIN)
      buffer_puts(buffer_2, "IN ");
    if(p[i].revents & POLLOUT)
      buffer_puts(buffer_2, "OUT ");
    if(p[i].revents & POLLERR)
      buffer_puts(buffer_2, "ERR ");
    buffer_puts(buffer_2, "}");
    buffer_putnlflush(buffer_2);
  }
#endif
  for(j = r - 1; j >= 0; --j) {
    p = array_get(&io_pollfds, sizeof(struct pollfd), j);
    io_entry* e = (io_entry*)iarray_get((iarray*)io_getfds(), p->fd);
    if(p->revents & (POLLERR | POLLHUP | POLLNVAL)) {
      /* error; signal whatever app is looking for */
      if(e->wantread)
        p->revents |= POLLIN;
      if(e->wantwrite)
        p->revents |= POLLOUT;
    }
    if(!e->canread && (p->revents & POLLIN)) {
      e->canread = 1;
      e->next_read = first_readable;
      first_readable = p->fd;
    }
    if(!e->canwrite && (p->revents & POLLOUT)) {
      e->canwrite = 1;
      e->next_write = first_writeable;
      first_writeable = p->fd;
    }
    p++;
  }
  return i;
}
#endif /* HAVE_POLL */
}
