#include "../strarray.h"
#include "../str.h"
#include "../byte.h"
#include "../unix.h"
#include <stdlib.h>

static int
strarray_glob_error(const char* path, int err) {
  return 0;
}

int
strarray_glob(strarray* arr, const char* pattern) {
  int r;
  glob_t g;
  byte_zero(&g, sizeof(glob_t));

  if((r = glob(pattern, GLOB_ERR, &strarray_glob_error, &g)) == 0) {
    ssize_t i;

    for(i = 0; i < g.gl_pathc; ++i) {
      strarray_push(arr, g.gl_pathv[i]);
      ++r;
    }
    globfree(&g);
    return r;
  }
  return 0;
}

int
strarray_glob_b(strarray* arr, const char* x, size_t len) {
  int r;
  char* pattern = str_ndup(x, len);
  r = strarray_glob(arr, pattern);
  free(pattern);
  return r;
}
