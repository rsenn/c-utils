#include "../windoze.h"
#include "../uint64.h"
#include "../dir_internal.h"

int64
dir_size(struct dir_s* d) {
  int64 r = -1;
#if !USE_READDIR && WINDOWS
  r = ((uint64)(dir_INTERNAL(d)->dir_finddata.nFileSizeHigh) << 32) +
      dir_INTERNAL(d)->dir_finddata.nFileSizeLow;
#endif

  return r;
}
