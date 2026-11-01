#include "../omf.h"

void*
omf_record_end(omf_record* rec) {
  range data = omf_data(rec);

  /* the last byte of a record is its checksum, not part of its contents */
  return data.end > data.start ? data.end - 1 : data.end;
}
