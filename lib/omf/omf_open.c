#include "../omf.h"
#include "../mmap.h"

int
omf_open(omf_file* omf, const char* filename) {
  if((omf->map = (void*)mmap_read(filename, &omf->size)) == 0)
    return 0;

  omf->record = (omf_record*)omf->map;

  return 1;
}
