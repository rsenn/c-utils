#include "../ihex.h"

ihex_record*
ihex_record_prev(ihex_file* ihf, ihex_record* rec, int type) {
  ihex_recptr p = {NULL};

  p.rec = rec;

  while((p.link = p.link->prev)) {
    if(p.link == &ihf->records)
      break;

    if(p.rec->type == type)
      break;
  }
  return p.rec;
}
