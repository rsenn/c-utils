#include "../omf.h"

range
omf_get_name(omf_record* rec, size_t idx) {
  range r = omf_data(rec);
  if(rec->type != OMF_LNAMES) {
    r.end = r.start;
    r.elem_size = 0;
    return r;
  }
  while(idx > 0) {
    r.start += (unsigned char)r.start[0] + 1;
    --idx;
  }
  r.elem_size = (unsigned char)r.start[0];
  ++r.start;
  r.end = r.start + r.elem_size;
  return r;
}
