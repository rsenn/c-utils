#define _FILE_OFFSET_BITS 64

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include "../byte.h"
#include "../cdb.h"
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#ifdef __MINGW32__
#include "../windows.h"
#else
#include <sys/mman.h>
#endif

int cdb_read(struct cdb *c,unsigned char *buf,unsigned long len,uint32 pos) {
  if (c->map) {
    if ((pos > c->size) || (c->size - pos < len)) goto FORMAT;
    byte_copy(buf,len,c->map + pos);
  }
  else {
#ifndef HAVE_PREAD
    if (lseek(c->fd,pos,SEEK_SET) == -1) return -1;
#endif
    while (len > 0) {
      ssize_t r;
      do
#ifdef HAVE_PREAD
	r = pread(c->fd,buf,len,pos);
#else
        r = read(c->fd,buf,len);
#endif
      while ((r == -1) && (errno == EINTR));
      if (r == -1) return -1;
      if (r == 0) goto FORMAT;
      buf += r;
      len -= r;
#ifdef HAVE_PREAD
      pos += r;
#endif
    }
  }
  return 0;

  FORMAT:
#ifdef EPROTO
  errno = EPROTO;
#else
  errno = EINVAL;
#endif
  return -1;
}
