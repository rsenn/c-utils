#include "../ihex.h"

ihex_record*
ihex_record_prev(ihex_record* rec, int type) {
  ihex_recptr p = {rec};

  while((p.link = p.link->prev)) {
    if(p.link->prev == p.link)
      break;

    if(p.rec->type == type)
      break;
  }
  return p.rec;
}
