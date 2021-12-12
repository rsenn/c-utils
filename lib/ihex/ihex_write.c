#include "../ihex.h"

int
ihex_write(ihex_file* ihf, buffer* b) {
  ihex_recptr p;
  int i = 0;
  list_for_each(p.link, &ihf->records) {
    ihex_record_write(p.rec, b);
    i++;
  }
  return i;
}
