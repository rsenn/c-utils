#include "../path_internal.h"
#include "../str.h"
#include "../byte.h"
#include <assert.h>

size_t path_normalize2(char* path, size_t nb);

size_t
path_normalize(char* path) {
  return path_normalize2(path, str_len(path));
}

size_t
path_normalize2(char* path, size_t nb) {
  ssize_t i = 0, j, len;
  len = nb;

again:
  i = path_separator2(path, len);

  if(i > 1) {
    byte_copy(&path[1], len - i, &path[i]);
    len -= i - 1;
    i = 1;
  }

  while(i < len) {
    size_t k;
    j = i + path_skip3(&path[i], &k, len - i);
    if(j == i) {
      len = j;
      break;
    }

    if(i > 0 && path_isdot(&path[i])) {
      ssize_t clen = len - j;
      assert(clen >= 0);

      if(clen > 0)
        byte_copy(&path[i], clen, &path[j]);

      path[i + clen] = '\0';
      len = i + clen;
      goto again;
    }

    if(!path_isdotdot(&path[i]) && (len - j) >= 2 && path_isdotdot(&path[j])) {
      j += (len - j) == 2 || path[j + 2] == '\0' ? 2 : 3;
      ssize_t clen = len - j;
      assert(clen >= 0);

      if(clen > 0)
        byte_copy(&path[i], clen, &path[j]);

      path[i + clen] = '\0';
      len = i + clen;
      goto again;
    }

    if((i += k + 1) < j) {
      if(j < len)
        byte_copy(&path[i], len - j, &path[j]);

      len -= j - i;
    }
  }

  return len;
}
