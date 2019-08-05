#include "../binfmt.h"
#include "../mmap.h"
#include "../byte.h"

int
binfmt_open(binfmt_file* bf, const char* filename) {
  byte_zero(bf, sizeof(binfmt_file));

  bf->baseaddr = (uint8*)mmap_private(filename, &bf->filesize);

}

