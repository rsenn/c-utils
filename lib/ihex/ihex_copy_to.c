#include "../ihex.h"

/* ihex_copy_to copies the data bytes with an address in [base, base+size)
 * to image[address-base] and sets map[address-base] to 1. Later records
 * win over earlier ones. Returns the number of bytes copied. */
size_t
ihex_copy_to(ihex_file* ihf, uint32 base, uint8* image, uint8* map, size_t size) {
  ihex_recptr p;
  uint32 ext = 0, a;
  size_t i, n = 0;

  list_for_each(p.link, &ihf->records) {
    if(ihex_record_base(p.rec, &ext) || p.rec->type != IHEX_DATA)
      continue;

    for(i = 0; i < p.rec->length; i++) {
      a = ext + p.rec->offset + (uint32)i;

      if(a < base || (size_t)(a - base) >= size)
        continue;

      image[a - base] = p.rec->data[i];
      map[a - base] = 1;
      n++;
    }
  }
  return n;
}
