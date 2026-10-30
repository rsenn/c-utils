#include "../ihex.h"

/* ihex_range finds the lowest address and one past the highest address
 * of the data records. Returns the number of data bytes, 0 if there are none
 * and *lo and *hi stay as they were. */
size_t
ihex_range(ihex_file* ihf, uint32* lo, uint32* hi) {
  ihex_recptr p;
  uint32 base = 0, a, b, bottom = 0xffffffffu, top = 0;
  size_t total = 0;

  list_for_each(p.link, &ihf->records) {
    if(ihex_record_base(p.rec, &base) || p.rec->type != IHEX_DATA || !p.rec->length)
      continue;

    a = base + p.rec->offset;
    b = a + p.rec->length;

    if(a < bottom)
      bottom = a;
    if(b > top)
      top = b;

    total += p.rec->length;
  }

  if(total) {
    *lo = bottom;
    *hi = top;
  }
  return total;
}
