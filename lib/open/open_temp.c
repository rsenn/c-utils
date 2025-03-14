/* ported from dietlibcs mkstemp() */

#include "../windoze.h"
#include "../open.h"
#include "../str.h"
#include "../uint32.h"

#if WINDOWS_NATIVE
#ifdef _MSC_VER
#define _CRT_INTERNAL_NONSTDC_NAMES 1
#endif
#include <io.h>
#include <windows.h>
#if !defined(__LCC__) && !defined(__MINGW32__)
#define read _read
#define write _write
#define open _open
#define close _close
#endif
#else
#include <unistd.h>
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

static char default_tmpl[] = "temp-XXXXXX.txt";

int
open_temp(char** pt) {
  int i, res;
  unsigned int random;
  char* tmp;
  char* tmpl = pt && *pt ? *pt : default_tmpl;
  size_t pos;

  if(!tmpl)
    tmpl = (char*)default_tmpl;

  pos = str_chr(tmpl, 'X');

  tmp = (char*)tmpl + pos;

  if(tmp < tmpl)
    goto error;

  if(pt && tmpl)
    *pt = tmpl;

  for(i = 0; i < 6; ++i) {
    if(tmp[i] != 'X') {
    error:
      errno = EINVAL;
      return -1;
    }
  }

  *pt = str_dup(tmpl);
  tmp = *(char**)pt + pos;

  for(;;) {
    random = uint32_random();

    for(i = 0; i < 6; ++i) {
      int hexdigit = (random >> (i * 5)) & 0x1f;
      tmp[i] = hexdigit > 9 ? hexdigit + 'A' - 10 : hexdigit + '0';
    }

    unlink(*pt);
    res = open(*pt,
               O_RDWR | O_CREAT | O_TRUNC | O_BINARY
#if !WINDOWS_NATIVE
               ,
               0666
#endif
    );

    if(res >= 0 || errno != EEXIST)
      break;
  }

  return res;
}
