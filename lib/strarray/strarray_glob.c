#include "../strarray.h"
#include "../str.h"
#include "../byte.h"
#include "../unix.h"
#include "../glob.h"
#include <assert.h>
#include <stdlib.h>

static int
strarray_glob_error(const char* path, int err) {
  return 0;
}

/* appends the files that match pattern; returns how many (0 when nothing
 * matches), -1 on a real error. (It used to return glob()'s own error code on
 * failure, which looked like a match count: GLOB_NOMATCH is 3.) */
int
strarray_glob(strarray* arr, const char* pattern) {
  int r, count = 0;
  glob_t g;

  byte_zero(&g, sizeof(glob_t));
  r = glob(pattern, 0, &strarray_glob_error, &g);
  if(r == 0) {
    if(g.gl_pathv) {
      size_t i;

      for(i = 0; i < (size_t)g.gl_pathc; ++i)
        if(strarray_push(arr, g.gl_pathv[i]))
          ++count;
    }
  } else if(r != GLOB_NOMATCH) {
    count = -1;
  }
  if(r == 0 || r == GLOB_NOMATCH)
    globfree(&g);
  return count;
}

int
strarray_glob_b(strarray* arr, const char* x, size_t len) {
  char* pattern = str_ndup(x, len);
  int r = strarray_glob(arr, pattern);

  free(pattern);

  return r;
}
