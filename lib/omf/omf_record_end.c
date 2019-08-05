#include "../omf.h"

void*
omf_record_end(omf_record* rec) {
  range data = omf_data(rec);

  return data.end;
}

