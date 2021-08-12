#include "../ihex.h"

int
ihex_write(ihex_file* ihf, buffer* b) {
  ihex_record* r;
  int i = 0;
  for(r = ihf->records; r; r = r->next) {
    ihex_record_write(r, b);
    i++;
  }
  return i;
}
