#include "../ihex.h"

/* ihex_verify returns the number of records whose stored checksum differs
 * from the checksum computed over the record, 0 if all are good. */
size_t
ihex_verify(ihex_file* ihf) {
  ihex_recptr p;
  size_t bad = 0;

  list_for_each(p.link, &ihf->records)
    if(p.rec->checksum != ihex_record_checksum(p.rec))
      bad++;

  return bad;
}
