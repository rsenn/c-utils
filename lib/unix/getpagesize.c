#include "../unix.h"

#if WINDOWS_NATIVE
#include <windows.h>

size_t
getpagesize() {
  static DWORD cachedPageSize = 0;

  if(cachedPageSize == 0) {
    SYSTEM_INFO si;
    GetNativeSystemInfo(&si);
    cachedPageSize = si.dwPageSize;
  }
  return cachedPageSize;
}
#endif /* WINDOWS_NATIVE */
