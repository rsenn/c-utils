#include "../ihex.h"

ihex_record*
ihex_record_next(ihex_file* ihf, ihex_record* rec, int type) {
  ihex_recptr p = {rec};

  while((p.link = p.link->next)) {
    if(p.link == &ihf->records)
      break;
    if(p.rec->type == type)
      break;
  }
  return p.rec;
}
