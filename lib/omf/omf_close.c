#include "../omf.h"
#include "../mmap.h"

void
omf_close(omf_file* omf) {
  mmap_unmap(omf->map, omf->size);
}
