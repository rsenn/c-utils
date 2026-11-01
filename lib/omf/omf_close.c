#include "../omf.h"
#include "../mmap.h"

void
omf_close(omf_file* omf) {
  mmap_unmap((char*)omf->map, omf->size);
  omf->map = NULL;
  omf->size = 0;
}
