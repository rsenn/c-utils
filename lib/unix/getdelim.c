#include "../typedefs.h"
#include "../windoze.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

#if WINDOWS_NATIVE
#include <io.h>
#else
#include <unistd.h>
#endif

#if !defined(HAVE_GETDELIM) && !defined(__LCC__)

ssize_t
getdelim(char** lineptr, size_t* n, int delim, FILE* stream) {
  size_t i;
  if(!lineptr || !n) {
    errno = EINVAL;
    return -1;
  }
  if(!*lineptr)
    *n = 0;
  for(i = 0;;) {
    int x;
    if(i >= *n) {
      int tmp = *n + 100;
      char* new = alloc_re(*lineptr, tmp);
      if(!new)
        return -1;
      *lineptr = new;
      *n = tmp;
    }
    x = fgetc(stream);
    if(x == EOF) {
      if(!i)
        return -1;
      (*lineptr)[i] = 0;
      return i;
    }
    (*lineptr)[i] = x;
    ++i;
    if(x == delim || i >= *n)
      break;
  }
  (*lineptr)[i] = 0;
  return i;
}
#endif
