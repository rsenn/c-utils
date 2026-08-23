#include "../windoze.h"

#if WINDOWS_NATIVE

#include "../str.h"
#include "../uint32.h"

#include <windows.h>
#include <errno.h>

/* mingw/MSVC don't provide mkdtemp(3) (BSD/glibc extension). Same
 * randomization approach as open_temp.c's mkstemp() port: replace the
 * template's trailing "XXXXXX" with random alphanumerics and retry on
 * collision. Modifies tmpl in place and returns it, like the real
 * mkdtemp(); returns 0 (setting errno) on error. */
char*
mkdtemp(char* tmpl) {
  size_t len = str_len(tmpl);
  char* x;
  int i;

  if(len < 6) {
    errno = EINVAL;
    return 0;
  }

  x = tmpl + (len - 6);

  for(i = 0; i < 6; ++i) {
    if(x[i] != 'X') {
      errno = EINVAL;
      return 0;
    }
  }

  for(;;) {
    uint32 random = uint32_random();

    for(i = 0; i < 6; ++i) {
      int c = (random >> (i * 5)) & 0x1f;
      x[i] = c > 9 ? c + 'A' - 10 : c + '0';
    }

    if(CreateDirectoryA(tmpl, 0))
      return tmpl;

    if(GetLastError() != ERROR_ALREADY_EXISTS) {
      errno = EACCES;
      return 0;
    }
  }
}

#endif
