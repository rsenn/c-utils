/* this header file comes from libowfat, http://www.fefe.de/libowfat/ */
#ifndef IOM_H
#define IOM_H

/* http://cr.yp.to/lib/io.html */

#include "uint64.h"
#include "taia.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __MINGW32__
#include <mcfgthread/c11thread.h>
#elif defined(__dietlibc__)
#include <threads.h>
#else
#include <pthread.h>
#include <semaphore.h>
#endif

enum { SLOTS = 128 };
typedef struct iomux {
  int ctx;
  int working;       /* used to synchronize who is filling the queue */
  unsigned int h, l; /* high, low */
  struct {
    int fd, events;
  } q[SLOTS];
#if defined(__MINGW32__) || defined(__dietlibc__)
  mtx_t mtx;
  cnd_t sem;
#else
  sem_t sem;
#endif
} iomux_t;

/* Init master context */
int iom_init(iomux_t* c);

/* Add socket to iomux */
enum { IOM_READ = 1, IOM_WRITE = 2, IOM_ERROR = 4 };
/* return -1 if error, events can be IOM_READ or IOM_WRITE */
int iom_add(iomux_t* c, int64 s, unsigned int events);

/* Blocking wait for single event, timeout in milliseconds */
/* return -1 if error, 0 if ok; s set to fd, revents set to known events on that fd */
/* when done with the fd, call iom_add on it again! */
/* This can be called by multiple threads in parallel */
int iom_wait(iomux_t* c, int64* s, unsigned int* revents, unsigned long timeout);

/* Call this to terminate all threads waiting in iom_wait */
int iom_abort(iomux_t* c);

#ifdef __cplusplus
}
#endif

#endif
