#include "../omf.h"

omf_record*
omf_begin(omf_file* omf) {
  return omf->map;
}

