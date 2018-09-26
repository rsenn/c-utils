#include "../windoze.h"
#include "../io.h"


int64
io_dup(int64 fd) {
#if WINDOWS_NATIVE
  HANDLE h;

  DuplicateHandle(GetCurrentProcess(), 
                    (HANDLE)fd, 
                    GetCurrentProcess(),
                    &h, 
                    0,
                    FALSE,
                    DUPLICATE_SAME_ACCESS);
#else
  return dup(fd);
#endif
}
