#define _FILE_OFFSET_BITS 64

#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include "../cdb.h"
#include "../cdb_make.h"
#ifdef __MINGW32__
#include "../windows.h"
#endif

int cdb_make_addbegin(struct cdb_make *c,unsigned long int keylen,unsigned long int datalen) {
  char buf[8];

  if (keylen > 0xffffffff) { errno = ENOMEM; return -1; }
  if (datalen > 0xffffffff) { errno = ENOMEM; return -1; }

  uint32_pack(buf,keylen);
  uint32_pack(buf + 4,datalen);
  if (buffer_putalign(&c->b,buf,8) == -1) return -1;
  return 0;
}
