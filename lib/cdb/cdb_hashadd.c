
#include "../cdb.h"

uint32 cdb_hashadd(uint32 h,unsigned char c) {
  h += (h << 5);
  return h ^ c;
}
