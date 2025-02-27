#include "lib/rdir.h"
#include "lib/dir.h"
#include "lib/buffer.h"
#include "lib/stralloc.h"
#include "lib/byte.h"
#include "lib/str.h"

int
main(int argc, char* argv[]) {
  struct rdir_s rd;
  char* s;
  int r = rdir_open(&rd, argc > 1 ? argv[1] : "build");

  while((s = rdir_read(&rd))) {
    buffer_putm_internal(buffer_1, s, "\n", NULL);
    buffer_flush(buffer_1);
  }
  rdir_close(&rd);
  return 0;
}
