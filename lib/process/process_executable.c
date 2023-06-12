#include "../process.h"
#include "../stralloc.h"
#include "../path.h"
#include <limits.h>

char*
process_executable(void) {
  ssize_t len;
  stralloc prog;

  stralloc_init(&prog);

#ifndef WINDOWS_NATIVE
  stralloc_ready(&prog, PATH_MAX);

  if((len = readlink("/proc/self/exe", prog.s, prog.a)) > 0) {
    prog.len = len;
    return prog.s;
  }
#endif

#if WINDOWS
  if((len = GetModuleFileNameA(0, prog.s, prog.a)) > 0) {
    prog.len = len;
    return prog.s;
  }
#endif

  return 0;
}
