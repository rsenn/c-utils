#include "../ihex.h"

/* ihex_record_base sets *base from an address record and returns 1.
 * Extended linear address (4) gives hi16 << 16, extended segment address
 * (2) gives segment << 4. Every other record returns 0 and leaves *base. */
int
ihex_record_base(const ihex_record* rec, uint32* base) {
  uint16 v;

  if(rec->type != IHEX_EXTLINADDR && rec->type != IHEX_EXTSEGADDR)
    return 0;

  if(rec->length < 2)
    return 0;

  uint16_unpack_big((const char*)rec->data, &v);
  *base = rec->type == IHEX_EXTLINADDR ? (uint32)v << 16 : (uint32)v << 4;
  return 1;
}
