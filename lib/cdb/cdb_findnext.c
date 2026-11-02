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

static int match(struct cdb *c,const unsigned char *key,unsigned long int len,uint32 pos) {
  unsigned char buf[32];
  unsigned long n;

  while (len > 0) {
    n = sizeof buf;
    if (n > len) n = len;
    if (cdb_read(c,buf,n,pos) == -1) return -1;
    if (byte_diff(buf,n,key)) return 0;
    pos += n;
    key += n;
    len -= n;
  }
  return 1;
}

int cdb_findnext(struct cdb *c,const unsigned char *key,unsigned long int len) {
  unsigned char buf[8];
  uint32 pos;
  uint32 u;

  if (!c->loop) {
    u = cdb_hash(key,len);
    if (cdb_read(c,buf,8,(u << 3) & 2047) == -1) return -1;
    uint32_unpack((char*)buf + 4,&c->hslots);
    if (!c->hslots) return 0;
    uint32_unpack((char*)buf,&c->hpos);
    c->khash = u;
    u >>= 8;
    u %= c->hslots;
    u <<= 3;
    c->kpos = c->hpos + u;
  }

  while (c->loop < c->hslots) {
    if (cdb_read(c,buf,8,c->kpos) == -1) return -1;
    uint32_unpack((char*)buf + 4,&pos);
    if (!pos) return 0;
    c->loop += 1;
    c->kpos += 8;
    if (c->kpos == c->hpos + (c->hslots << 3)) c->kpos = c->hpos;
    uint32_unpack((char*)buf,&u);
    if (u == c->khash) {
      if (cdb_read(c,buf,8,pos) == -1) return -1;
      uint32_unpack((char*)buf,&u);
      if (u == len)
	switch(match(c,key,len,pos + 8)) {
	  case -1:
	    return -1;
	  case 1:
	    uint32_unpack((char*)buf + 4,&c->dlen);
	    c->dpos = pos + 8 + len;
	    return 1;
	}
    }
  }

  return 0;
}
