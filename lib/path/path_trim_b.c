#include "../path_internal.h"
#include "../utf8.h"

size_t
path_trim_b(const char* path, size_t size) {
  size_t j = 0;
  int prev = 0;

  for(size_t i = 0; i < size;) {
    size_t len = u8_len(&path[i], 1);

    int is_sep = len == 1 && str_chr(PATHSEP_S_MIXED, path[i]) < 2;

    if(is_sep && !prev) {
      j = i;
    }

    prev = is_sep;

    i += len;
  }

  return j;
}
