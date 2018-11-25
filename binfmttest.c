#include "lib/byte.h"
#include "lib/binfmt.h"

int
main(int argc, char* argv[]) {
  binfmt_file bf;

  if(argc > 1) {
    binfmt_open(&bf, argv[1]);
  }

  return 0;
}
