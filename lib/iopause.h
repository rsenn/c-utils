/* this header file comes from libowfat, http://www.fefe.de/libowfat/ */
#ifndef IOPAUSE_H
#define IOPAUSE_H

#include "io.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef IOPAUSE_POLL

/* sysdep: +poll */
#define IOPAUSE_POLL

#include <sys/types.h>
#include <poll.h>

typedef struct pollfd iopause_fd;
#define IOPAUSE_READ POLLIN
#define IOPAUSE_WRITE POLLOUT

#include "taia.h"

extern void iopause(iopause_fd*, unsigned int, struct taia*, struct taia*);

#else

/* sysdep: -poll */

typedef struct {
  fd_t fd;
  short events;
  short revents;
} iopause_fd;

#define IOPAUSE_READ 1
#define IOPAUSE_WRITE 4

#include "taia.h"

extern void iopause(iopause_fd*, unsigned int, struct taia*, struct taia*);

#endif /* IOPAUSE_POLL */

#ifdef __cplusplus
}
#endif
#endif
