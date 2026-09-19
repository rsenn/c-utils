#define _POSIX_SOURCE 1
#define _GNU_SOURCE 1
#define _DEFAULT_SOURCE 1
#include "../windoze.h"
#include "../env.h"

#if WINDOWS_NATIVE
#include <windows.h>
#endif

int
env_unset(const char* name) {
#if WINDOWS_NATIVE
  return SetEnvironmentVariable(name, "");
#else
  /* env_put()/env_put2() etc. shadow `environ` with their own
   * reallocated array and track its used/allocated slot counts once
   * env_init() has run; plain unsetenv(3) would shrink that array out
   * from under them without updating those counts. Route through the
   * module's own env_unset_internal() (lib/env/env_put.c) instead, so
   * it stays in sync the same way every env_put*() call already does. */
  return (int)env_unset_internal(name);
#endif
}
