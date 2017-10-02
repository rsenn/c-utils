#include "rdir.h"

char* rdir_read(struct rdir_s* d) {
  return dir_read(&d->dir);
}
