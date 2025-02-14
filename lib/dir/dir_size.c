#include "../windoze.h"
#include "../uint64.h"
#include "../dir_internal.h"

int64
dir_size(struct dir_s* d) {
  int64 r = -1;
#if !USE_READDIR && WINDOWS
  struct dir_internal_s* internal = dir_INTERNAL(d);

  r = ((uint64)(internal->dir_finddata.nFileSizeHigh) << 32) +
      internal->dir_finddata.nFileSizeLow;
#endif

  return r;
}
