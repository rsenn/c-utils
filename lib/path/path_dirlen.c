#include "../path_internal.h"
#include "../utf8.h"
#include "../str.h"

size_t
path_dirlen(const char* path) {
  const char ps[] = PATHSEP_S_MIXED;
  const size_t pslen = sizeof(ps) - 1;

  return u8b_rchrs(path, str_len(path), ps, pslen);
}
