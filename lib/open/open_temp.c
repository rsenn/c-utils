/* ported from dietlibcs mkstemp() */

#include "../windoze.h"
#include "../open.h"
#include "../str.h"
#include "../uint32.h"

#if WINDOWS_NATIVE
# include <io.h>
#else
# include <unistd.h>
# ifdef __linux__
#  include <linux/limits.h>
# endif
#endif

#include <errno.h>
#include <fcntl.h>
#include <limits.h>

#ifndef O_NOFOLLOW
#define O_NOFOLLOW 0
#endif

#ifndef O_BINARY
#define O_BINARY 0
#endif

static char default_tmpl[PATH_MAX] = "temp-XXXXXX.txt";

int
open_temp(char* tmpl) {
  int i, res;
  unsigned int random;
  char* tmp;

  if(!tmpl) {
    tmpl = default_tmpl;
    tmp = &tmpl[5];
  } else {
    str_copy(default_tmpl, tmpl);
    tmpl = default_tmpl;
  tmp = tmpl + str_len(tmpl) - 6;
  if(tmp < tmpl) goto error;
  }

  for(i = 0; i < 6; ++i) {
    if(tmp[i] != 'X') {
    error:
      errno = EINVAL;
      return -1;
    }

  }
   
  for(;;) {
    random = uint32_random();
    
    for(i = 0; i < 6; ++i) {
      int hexdigit = (random >> (i * 5)) & 0x1f;
      tmp[i] = hexdigit > 9 ? hexdigit + 'A' - 10 : hexdigit + '0';
    }

    unlink(tmpl);
    res = open(tmpl,  O_RDWR | O_CREAT | O_TRUNC | O_BINARY, 0666);
    
    if(res >= 0 || errno != EEXIST) break;
  }
  return res;
}
