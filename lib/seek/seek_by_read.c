#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#define _FILE_OFFSET_BITS 64
#include "../seek.h"
#include "../io.h"
#include "../alloc.h"
#include <fcntl.h>
#include <errno.h>
#if WINDOWS_NATIVE
#include <io.h>
#else
#include <unistd.h>
#include <sys/types.h>
#endif
#ifdef HAVE_ALLOCA
#include <alloca.h>
#endif

/*  If we are reading through a pipe, or from stdin then we can't io_seek,
 *  we must read and discard the data to skip over it.
 */
void
seek_by_read(fd_type fd, int64 amount) {
  char* buf = (char*)
#ifdef HAVE_ALLOCA
      alloca(amount);
#else
      alloc(amount);
#endif

  if(amount) {
    ssize_t n = 0, r;

    while(n < amount && (r = read(fd, &buf[n], amount - n)) > 0) {
      n += r;
    }
  }

#ifndef HAVE_ALLOCA
  alloc_free(buf);
#endif
}
