#include "../io_internal.h"
#include "../iom.h"
#include <errno.h>
#ifdef HAVE_EPOLL
#include <sys/epoll.h>
#endif
#ifdef HAVE_KQUEUE
#include <sys/types.h>
#include <sys/event.h>
#include <sys/time.h>
#endif

int
iom_add(iomux_t* c, int64 s, unsigned int events) {
#ifdef HAVE_EPOLL
  struct epoll_event e = {.events = EPOLLONESHOT, .data.fd = s};
  if(events & IOM_READ)
    e.events |= EPOLLIN;
  if(events & IOM_WRITE)
    e.events |= EPOLLOUT;
  /* a one-shot descriptor stays registered after it fired: re-arm it */
  if(epoll_ctl(c->ctx, EPOLL_CTL_ADD, s, &e) == 0)
    return 0;
  if(errno != EEXIST)
    return -1;
  return epoll_ctl(c->ctx, EPOLL_CTL_MOD, s, &e);
#elif defined(HAVE_KQUEUE)
  struct kevent kev;
  struct timespec ts = {0};
  EV_SET(&kev,
         s,
         (events & IOM_READ ? EVFILT_READ : 0) + (events & IOM_WRITE ? EVFILT_WRITE : 0),
         EV_ADD | EV_ENABLE | EV_ONESHOT,
         0,
         0,
         (void*)s);
  return kevent(c->ctx, &kev, 1, 0, 0, &ts);
#else
#ifndef __BORLANDC__ /* #warning is not known to cpp32 */
#warning "only epoll and kqueue supported for now"
#endif
#endif
}
