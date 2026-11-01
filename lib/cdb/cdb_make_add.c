#define _FILE_OFFSET_BITS 64

#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include "../cdb.h"
#include "../cdb_make.h"
#ifdef __MINGW32__
#include "../windows.h"
#endif

int cdb_make_add(struct cdb_make *c,const unsigned char *key,unsigned long int keylen,const unsigned char *data,unsigned long int datalen)
{
  if (cdb_make_addbegin(c,keylen,datalen) == -1) return -1;
  if (buffer_putalign(&c->b,(char*)key,keylen) == -1) return -1;
  if (buffer_putalign(&c->b,(char*)data,datalen) == -1) return -1;
  return cdb_make_addend(c,keylen,datalen,cdb_hash(key,keylen));
}
