
#include "../cdb.h"

uint32 cdb_hash(const unsigned char *buf,unsigned long int len) {
  uint32 h;

  h = CDB_HASHSTART;
  while (len) {
    h = cdb_hashadd(h,*buf);
    ++buf;
    --len;
  }
  return h;
}
