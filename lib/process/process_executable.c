#include "../windoze.h"
#include "../process.h"
#include "../stralloc.h"
#include "../path.h"
#include <limits.h>

#if WINDOWS
#include <windows.h>
#else
#include <unistd.h>
#endif

char*
process_executable(void) {
  ssize_t len;
  stralloc prog = {0, 0, 0};

#if WINDOWS
  wchar_t w[PATH_MAX + 1];

  if((len = GetModuleFileNameW(0, w, PATH_MAX)) > 0) {
    w[len] = '\0';
    wcs_to_u8sa(w, &prog);
    return prog.s;
  }
#else
  stralloc_ready(&prog, PATH_MAX);

  if((len = readlink("/proc/self/exe", prog.s, prog.a)) > 0) {
    prog.len = len;
    return prog.s;
  }
#endif

  return 0;
}
