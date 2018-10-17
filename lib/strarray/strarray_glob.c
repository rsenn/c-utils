#include "../strarray.h"
#include "../glob.h"

static int
strarray_glob_error(const char* path, int err) {
}

int
strarray_glob(strarray* arr, const char* pattern) {
  int r;
  glob_t g;

  if((r = glob(pattern, GLOB_ERR, &strarray_glob_error, &g)) == 0) {
    size_t i;
    
    for(i = 0; i < g.gl_pathc; ++i) {
      strarray_push(arr, g.gl_pathv[i]);
      ++r;
    }
    return r;
  }
  return 0;
}
