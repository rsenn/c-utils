#include "../ihex.h"
#include "../slist.h"

int
ihex_write(ihex_file* ihf, buffer* b) {
  ihex_record* r;
  int i = 0;
  slink_foreach(ihf->records, r) {
    ihex_write_record(r, b);
    i++;
  }
  return i;
}
